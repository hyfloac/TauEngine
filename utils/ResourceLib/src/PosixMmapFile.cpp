#include "PosixMmapFile.hpp"

#if !defined(_WIN32)

#include "IFile.hpp"
#include "MmapFile.hpp"
#include <TauCOM.impl.hpp>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <filesystem>

namespace tau {

/**
 *   A read-only file handle whose contents are mapped into the
 * process address space via @c mmap.
 *
 *   The backing file descriptor is kept open for the lifetime of
 * the mapping so callers can grab it through
 * @link IPosixMmapFile @endlink and submit io_uring SQEs or call
 * @c madvise / @c fcntl against the same kernel object.
 */
class PosixMmapFile final : public IFileStream, IMmapFile, IPosixMmapFile
{
    DELETE_CM(PosixMmapFile);
    TAU_COM_IMPL_REF_COUNT();
public:
    PosixMmapFile(
        const int fd,
        void* const mapping,
        const uSys length,
        const C8DynString& name
    ) noexcept
        : m_Fd(fd)
        , m_Mapping(mapping)
        , m_Length(length)
        , m_Name(name)
        , m_Position(0)
    { }

    PosixMmapFile(
        const int fd,
        void* const mapping,
        const uSys length,
        C8DynString&& name
    ) noexcept
        : m_Fd(fd)
        , m_Mapping(mapping)
        , m_Length(length)
        , m_Name(::std::move(name))
        , m_Position(0)
    { }

    ~PosixMmapFile() noexcept override
    {
        if(m_Mapping)
        {
            (void) ::munmap(m_Mapping, m_Length);
        }
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
        else if(iid == iid_of<IMmapFile>)
        {
            *pInterface = static_cast<IMmapFile*>(this);
        }
        else if(iid == iid_of<IPosixMmapFile>)
        {
            *pInterface = static_cast<IPosixMmapFile*>(this);
        }
        else
        {
            return RC_InterfaceNotFound;
        }

        AddReference();
        return RC_Success;
    }

    // IStream

    [[nodiscard]] bool CanRead() const noexcept override { return true; }
    [[nodiscard]] bool CanWrite() const noexcept override { return false; }
    [[nodiscard]] bool CanSeek() const noexcept override { return true; }

    [[nodiscard]] i64 Length() const noexcept override { return static_cast<i64>(m_Length); }
    [[nodiscard]] i64 Position() const noexcept override { return m_Position; }

    i64 Position(const i64 pos, const ESeekOrigin origin) noexcept override
    {
        switch(origin)
        {
            case ESeekOrigin::Begin:   m_Position = pos; break;
            case ESeekOrigin::Current: m_Position += pos; break;
            case ESeekOrigin::End:     m_Position = static_cast<i64>(m_Length) + pos; break;
            default: break;
        }
        return m_Position;
    }

    ::std::expected<uSys, ErrorCode> Read(void* const buffer, const uSys count) override
    {
        if(m_Position < 0 || static_cast<uSys>(m_Position) >= m_Length)
        {
            return 0;
        }

        const uSys remaining = m_Length - static_cast<uSys>(m_Position);
        const uSys toRead = count < remaining ? count : remaining;

        (void) ::std::memcpy(buffer, static_cast<const u8*>(m_Mapping) + m_Position, toRead);
        m_Position += static_cast<i64>(toRead);
        return toRead;
    }

    ::std::expected<u8, ErrorCode> ReadByte() override
    {
        if(m_Position < 0 || static_cast<uSys>(m_Position) >= m_Length)
        {
            return ::std::unexpected(ErrorCode::EndOfFile);
        }

        const u8 byte = static_cast<const u8*>(m_Mapping)[m_Position];
        ++m_Position;
        return byte;
    }

    void Write(const void*, const uSys) override
    {
        assert(false);
    }

    // IFileStream

    [[nodiscard]] C8DynString Name() noexcept override { return m_Name; }

    // IMmapFile

    [[nodiscard]] const void* MappedData() const noexcept override { return m_Mapping; }
    [[nodiscard]] uSys MappedSize() const noexcept override { return m_Length; }

    // IPosixMmapFile

    [[nodiscard]] int GetFileDescriptor() noexcept override { return m_Fd; }
private:
    int m_Fd;
    void* m_Mapping;
    uSys m_Length;
    C8DynString m_Name;
    i64 m_Position;
};

/**
 *   A file loader producing read-only @link PosixMmapFile @endlink
 * handles.
 *
 *   Only @c FileProps::Read is supported; any other access mode
 * returns @c nullptr.
 */
class PosixMmapFileLoader final : public IFileLoader
{
    DEFAULT_CONSTRUCT_PU(PosixMmapFileLoader);
    DEFAULT_DESTRUCT_O(PosixMmapFileLoader);
    DELETE_CM(PosixMmapFileLoader);
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
        if(props != FileProps::Read)
        {
            return nullptr;
        }

        const char* const pathStr = reinterpret_cast<const char*>(path.String());
        const int fd = ::open(pathStr, O_RDONLY);
        if(fd < 0)
        {
            return nullptr;
        }

        struct stat st;
        if(::fstat(fd, &st) != 0 || st.st_size < 0)
        {
            (void) ::close(fd);
            return nullptr;
        }

        const uSys length = static_cast<uSys>(st.st_size);

        // Mapping a zero-length file is undefined; hand back an empty
        // stream that still owns the fd so IPosixMmapFile consumers
        // have something valid to inspect.
        if(length == 0)
        {
            return BasicTauAllocator<AllocationTracking::None>::Instance().AllocateT<PosixMmapFile>(fd, nullptr, 0u, path);
        }

        void* const mapping = ::mmap(nullptr, length, PROT_READ, MAP_PRIVATE, fd, 0);
        if(mapping == MAP_FAILED)
        {
            (void) ::close(fd);
            return nullptr;
        }

        return BasicTauAllocator<AllocationTracking::None>::Instance().AllocateT<PosixMmapFile>(fd, mapping, length, path);
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

    [[nodiscard]] ::std::chrono::time_point<::std::chrono::utc_clock> CreationTime(const C8DynString&) const noexcept override
    {
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

com::ComRef<IFileLoader> CreatePosixMmapFileLoader() noexcept
{
    return com::ComRef<IFileLoader>(BasicTauAllocator<AllocationTracking::None>::Instance().AllocateT<PosixMmapFileLoader>());
}

}

#endif
