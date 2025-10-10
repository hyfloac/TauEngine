#include "CFile.hpp"
#include <TauCOM.impl.hpp>
#include <filesystem>

#ifdef _WIN32
#include "Win32File.hpp"
#endif

namespace tau {

/**
 * The most basic file handle.
 *
 *   This is here as a fallback to the most basic file
 * handling system. In general there may be a much faster
 * and more optimized implementation like {@link Win32File @endlink}.
 */
class CFile final : public IFileStream, ICFile
{
    DELETE_CM(CFile);
    TAU_COM_IMPL_REF_COUNT();
public:
    CFile(
        FILE* const file,
        const C8DynString& name,
        const FileProps props
    ) noexcept
        : m_File(file)
        , m_Name(name)
        , m_Props(props)
    { }

    CFile(
        FILE* const file,
        C8DynString&& name,
        const FileProps props
    ) noexcept
        : m_File(file)
        , m_Name(::std::move(name))
        , m_Props(props)
    { }

    ~CFile() noexcept override
    {
        if(m_File)
        {
            (void) fclose(m_File);
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
        else if(iid == iid_of<ICFile>)
        {
            *pInterface = static_cast<ICFile*>(this);
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

    [[nodiscard]] bool CanSeek() const noexcept override
    {
        return true;
    }

    [[nodiscard]] i64 Length() const noexcept override
    {
#ifdef _WIN32
        const i64 curPos = _ftelli64(m_File);
        if(_fseeki64(m_File, 0, SEEK_END))
        {
            return -2;
        }

        const i64 size = _ftelli64(m_File);
        (void) _fseeki64(m_File, curPos, SEEK_SET);
#else
        const i64 curPos = ftell(m_File);
        if(fseek(m_File, 0, SEEK_END))
        {
            return -2;
        }

        const i64 size = ftell(m_File);
        (void) fseek(m_File, curPos, SEEK_SET);
#endif

        return size;
    }

    [[nodiscard]] i64 Position() const noexcept override
    {
#ifdef _WIN32
        return _ftelli64(m_File);
#else
        return ftell(m_File);
#endif
    }

    i64 Position(const i64 pos, const ESeekOrigin origin) noexcept override
    {
        int whence;
        switch(origin)
        {
            case ESeekOrigin::Begin:
                whence = SEEK_SET;
                break;
            case ESeekOrigin::Current:
                whence = SEEK_CUR;
                break;
            case ESeekOrigin::End:
                whence = SEEK_END;
                break;
            default: return Position();
        }

#ifdef _WIN32
        (void) _fseeki64(m_File, pos, whence);
#else
        (void) fseek(m_File, pos, whence);
#endif
        return Position();
    }

    ::std::expected<uSys, ErrorCode> Read(void* const buffer, const uSys len) override
    {
        if(!CanRead())
        {
            return ::std::unexpected(ErrorCode::ReadOnly);
        }

        return fread(buffer, 1, len, m_File);
    }

    ::std::expected<u8, ErrorCode> ReadByte() override
    {
        if(!CanRead())
        {
            return ::std::unexpected(ErrorCode::ReadOnly);
        }

        u8 ret;
        (void) fread(&ret, 1, 1, m_File);
        return ret;
    }

    void Write(const void* const buffer, const uSys len) override
    {
        if(!CanWrite())
        {
            assert(false);
        }

        (void) fwrite(buffer, 1, len, m_File);
    }

    // IFileStream

    [[nodiscard]] C8DynString Name() noexcept override { return m_Name; }

    // ICFile

    [[nodiscard]] FILE* GetFileHandle() noexcept override { return m_File; }
private:
    FILE* m_File;
    C8DynString m_Name;
    FileProps m_Props;
};

/**
 * The most basic file loader.
 *
 *   This is here as a fallback to the most basic file
 * handling system. In general there may be a much faster
 * and more optimized implementation like {@link Win32FileLoader @endlink}.
 */
class CFileLoader final : public IFileLoader
{
    DEFAULT_CONSTRUCT_PU(CFileLoader);
    DEFAULT_DESTRUCT_O(CFileLoader);
    DELETE_CM(CFileLoader);
    TAU_COM_IMPL_REF_COUNT();
public:
    // IUnknown

    com::EResultCode QueryInterface(const com::UUID& iid, void** const pInterface) noexcept override
    {
        using namespace tau::com;

        return RC_Success;
    }

    // IFileLoader

    [[nodiscard]] bool Exists(const C8DynString& path) const noexcept override
    {
        FILE* file;

#ifdef _WIN32
        const WDynString widePath = StringCast<wchar_t>(path);

        if((file = _wfopen(widePath, "r")))
        {
            (void) fclose(file);
            return true;
        }
#else
        ::std::error_code ec;
        return ::std::filesystem::exists(path.String(), ec);
#endif
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
        FILE* handle;
#ifdef _WIN32
        const WDynString widePath = StringCast<wchar_t>(path);

        switch(props)
        {
            case FileProps::Read:           handle = _wfopen(widePath, L"rb");  break;
            case FileProps::WriteNew:       handle = _wfopen(widePath, L"wb");  break;
            case FileProps::WriteOverwrite: handle = _wfopen(widePath, L"r+b"); break;
            case FileProps::WriteAppend:    handle = _wfopen(widePath, L"ab");  break;
            case FileProps::ReadWrite:      handle = _wfopen(widePath, L"r+b"); break;
            default: return nullptr;
        }
#else
        switch(props)
        {
            case FileProps::Read:           handle = fopen(reinterpret_cast<const char*>(path.String()), "rb");  break;
            case FileProps::WriteNew:       handle = fopen(reinterpret_cast<const char*>(path.String()), "wb");  break;
            case FileProps::WriteOverwrite: handle = fopen(reinterpret_cast<const char*>(path.String()), "r+b"); break;
            case FileProps::WriteAppend:    handle = fopen(reinterpret_cast<const char*>(path.String()), "ab");  break;
            case FileProps::ReadWrite:      handle = fopen(reinterpret_cast<const char*>(path.String()), "r+b"); break;
            default: return nullptr;
        }
#endif

        if(!handle)
        {
            return nullptr;
        }

        // ReSharper disable once CppRedundantTemplateArguments
        return BasicTauAllocator<AllocationTracking::None>::Instance().AllocateT<CFile>(handle, path, props);
    }

    [[nodiscard]] bool CreateFolders(const C8DynString& path) const noexcept override
    {
        return ::std::filesystem::create_directories(path.String());
    }

    [[nodiscard]] bool Delete(const C8DynString& path) const noexcept override
    {
        return ::std::filesystem::remove(path.String());
    }

    [[nodiscard]] ::std::chrono::time_point<::std::chrono::utc_clock> CreationTime(const C8DynString& path) const noexcept override
    {
        return ::std::chrono::time_point<::std::chrono::utc_clock>{};
    }

    [[nodiscard]] ::std::chrono::time_point<::std::chrono::utc_clock> ModifyTime(const C8DynString& path) const noexcept override
    {
        ::std::error_code ec;
        const auto writeTime = ::std::filesystem::last_write_time(path.String(), ec);
        return ::std::chrono::clock_cast<::std::chrono::utc_clock>(writeTime);
    }
};

}
