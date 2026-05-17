#include "PosixFile.hpp"

#if !defined(_WIN32)

#include "IFile.hpp"
#include <TauCOM.impl.hpp>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <filesystem>

namespace tau {

/**
 *   A file handle backed by raw POSIX syscalls.
 *
 *   Skips the stdio buffering layer that @link CFile @endlink pays
 * for and uses pread/pwrite so the kernel-side seek state isn't
 * touched per I/O. The current position is tracked in userspace.
 */
class PosixFile final : public IFileStream, IPosixFile
{
    DELETE_CM(PosixFile);
    TAU_COM_IMPL_REF_COUNT();
public:
    PosixFile(
        const int fd,
        const C8DynString& name,
        const FileProps props
    ) noexcept
        : m_Fd(fd)
        , m_Name(name)
        , m_Props(props)
        , m_Position(0)
    { }

    PosixFile(
        const int fd,
        C8DynString&& name,
        const FileProps props
    ) noexcept
        : m_Fd(fd)
        , m_Name(::std::move(name))
        , m_Props(props)
        , m_Position(0)
    { }

    ~PosixFile() noexcept override
    {
        if(m_Fd >= 0)
        {
            (void) ::close(m_Fd);
        }
    }

    // IUnknown

    com::EResultCode QueryInterface(const com::UUID& iid, void** const pInterface) noexcept override
    {
        using namespace tau::com;

        if(!pInterface)
        {
            return RC_NullParam;
        }

        if(iid == iid_of<IUnknown> || iid == iid_of<IStream> || iid == iid_of<IFileStream>)
        {
            *pInterface = static_cast<IFileStream*>(this);
        }
        else if(iid == iid_of<IPosixFile>)
        {
            *pInterface = static_cast<IPosixFile*>(this);
        }
        else
        {
            return RC_InterfaceNotFound;
        }

        AddReference();
        return RC_Success;
    }

    // IStream

    [[nodiscard]] bool CanRead() const noexcept override
    {
        return m_Props == FileProps::Read || m_Props == FileProps::ReadWrite;
    }

    [[nodiscard]] bool CanWrite() const noexcept override
    {
        switch(m_Props)
        {
            case FileProps::WriteNew:
            case FileProps::WriteOverwrite:
            case FileProps::WriteAppend:
            case FileProps::ReadWrite:
                return true;
            default: return false;
        }
    }

    [[nodiscard]] bool CanSeek() const noexcept override { return true; }

    [[nodiscard]] i64 Length() const noexcept override
    {
        struct stat st;
        if(::fstat(m_Fd, &st) != 0)
        {
            return -1;
        }
        return static_cast<i64>(st.st_size);
    }

    [[nodiscard]] i64 Position() const noexcept override { return m_Position; }

    i64 Position(const i64 pos, const ESeekOrigin origin) noexcept override
    {
        switch(origin)
        {
            case ESeekOrigin::Begin:
                m_Position = pos;
                break;
            case ESeekOrigin::Current:
                m_Position += pos;
                break;
            case ESeekOrigin::End:
                m_Position = Length() + pos;
                break;
            default: break;
        }
        return m_Position;
    }

    ::std::expected<uSys, ErrorCode> Read(void* const buffer, const uSys count) override
    {
        if(!CanRead())
        {
            return ::std::unexpected(ErrorCode::ReadOnly);
        }

        uSys totalRead = 0;
        u8* out = static_cast<u8*>(buffer);

        while(totalRead < count)
        {
            const ssize_t n = ::pread(m_Fd, out + totalRead, count - totalRead, m_Position + static_cast<off_t>(totalRead));
            if(n == 0)
            {
                break;
            }
            if(n < 0)
            {
                if(errno == EINTR)
                {
                    continue;
                }
                return ::std::unexpected(ErrorCode::Unexpected);
            }
            totalRead += static_cast<uSys>(n);
        }

        m_Position += static_cast<i64>(totalRead);
        return totalRead;
    }

    ::std::expected<u8, ErrorCode> ReadByte() override
    {
        u8 byte = 0;
        const auto result = Read(&byte, 1);
        if(!result)
        {
            return ::std::unexpected(result.error());
        }
        if(result.value() == 0)
        {
            return ::std::unexpected(ErrorCode::EndOfFile);
        }
        return byte;
    }

    void Write(const void* const buffer, const uSys count) override
    {
        if(!CanWrite())
        {
            assert(false);
            return;
        }

        const u8* const in = static_cast<const u8*>(buffer);
        uSys totalWritten = 0;

        while(totalWritten < count)
        {
            const ssize_t n = ::pwrite(m_Fd, in + totalWritten, count - totalWritten, m_Position + static_cast<off_t>(totalWritten));
            if(n < 0)
            {
                if(errno == EINTR)
                {
                    continue;
                }
                break;
            }
            if(n == 0)
            {
                break;
            }
            totalWritten += static_cast<uSys>(n);
        }

        m_Position += static_cast<i64>(totalWritten);
    }

    // IFileStream

    [[nodiscard]] C8DynString Name() noexcept override { return m_Name; }

    // IPosixFile

    [[nodiscard]] int GetFileDescriptor() noexcept override { return m_Fd; }
private:
    int m_Fd;
    C8DynString m_Name;
    FileProps m_Props;
    i64 m_Position;
};

/**
 *   A file loader that produces @link PosixFile @endlink handles.
 */
class PosixFileLoader final : public IFileLoader
{
    DEFAULT_CONSTRUCT_PU(PosixFileLoader);
    DEFAULT_DESTRUCT_O(PosixFileLoader);
    DELETE_CM(PosixFileLoader);
    TAU_COM_IMPL_REF_COUNT();
public:
    // IUnknown

    com::EResultCode QueryInterface(const com::UUID& iid, void** const pInterface) noexcept override
    {
        using namespace tau::com;

        if(!pInterface)
        {
            return RC_NullParam;
        }

        if(iid == iid_of<IUnknown> || iid == iid_of<IFileLoader>)
        {
            *pInterface = static_cast<IFileLoader*>(this);
            AddReference();
            return RC_Success;
        }

        return RC_InterfaceNotFound;
    }

    // IFileLoader

    [[nodiscard]] bool Exists(const C8DynString& path) const noexcept override
    {
        ::std::error_code ec;
        return ::std::filesystem::exists(path.String(), ec);
    }

    [[nodiscard]] bool IsFolder(const C8DynString& path) const noexcept override
    {
        ::std::error_code ec;
        return ::std::filesystem::is_directory(path.String(), ec);
    }

    [[nodiscard]] bool IsSymlink(const C8DynString& path) const noexcept override
    {
        ::std::error_code ec;
        return ::std::filesystem::is_symlink(path.String(), ec);
    }

    [[nodiscard]] IFileStream* Load(const C8DynString& path, FileProps props) const noexcept override
    {
        int flags = 0;
        switch(props)
        {
            case FileProps::Read:           flags = O_RDONLY; break;
            case FileProps::WriteNew:       flags = O_WRONLY | O_CREAT | O_TRUNC; break;
            case FileProps::WriteOverwrite: flags = O_RDWR | O_CREAT; break;
            case FileProps::WriteAppend:    flags = O_WRONLY | O_CREAT | O_APPEND; break;
            case FileProps::ReadWrite:      flags = O_RDWR | O_CREAT; break;
            default: return nullptr;
        }

        const char* const pathStr = reinterpret_cast<const char*>(path.String());
        const mode_t mode = 0644;
        const int fd = ::open(pathStr, flags, mode);
        if(fd < 0)
        {
            return nullptr;
        }

        return BasicTauAllocator<AllocationTracking::None>::Instance().AllocateT<PosixFile>(fd, path, props);
    }

    [[nodiscard]] bool CreateFolders(const C8DynString& path) const noexcept override
    {
        ::std::error_code ec;
        ::std::filesystem::create_directories(path.String(), ec);
        return !ec;
    }

    [[nodiscard]] bool Delete(const C8DynString& path) const noexcept override
    {
        ::std::error_code ec;
        return ::std::filesystem::remove(path.String(), ec);
    }

    [[nodiscard]] ::std::chrono::time_point<::std::chrono::utc_clock> CreationTime(const C8DynString& path) const noexcept override
    {
        (void) path;
        return ::std::chrono::time_point<::std::chrono::utc_clock>{};
    }

    [[nodiscard]] ::std::chrono::time_point<::std::chrono::utc_clock> ModifyTime(const C8DynString& path) const noexcept override
    {
        ::std::error_code ec;
        const auto writeTime = ::std::filesystem::last_write_time(path.String(), ec);
        if(ec)
        {
            return ::std::chrono::time_point<::std::chrono::utc_clock>{};
        }
        return ::std::chrono::clock_cast<::std::chrono::utc_clock>(writeTime);
    }
};

com::ComRef<IFileLoader> CreatePosixFileLoader() noexcept
{
    return com::ComRef<IFileLoader>(BasicTauAllocator<AllocationTracking::None>::Instance().AllocateT<PosixFileLoader>());
}

}

#endif
