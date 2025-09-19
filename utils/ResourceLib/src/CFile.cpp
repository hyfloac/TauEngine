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
class CFile final : public IFile, ICFile
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
            fclose(m_File);
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

        if(iid == iid_of<IUnknown> || iid == iid_of<IFile>)
        {
            *pInterface = static_cast<IFile*>(this);
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

    // IFile

    [[nodiscard]] i64 Size() noexcept override
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

    [[nodiscard]] bool Exists() noexcept override { return m_File != nullptr; }

    [[nodiscard]] C8DynString Name() noexcept override { return m_Name; }

    void SetPos(const i64 pos) noexcept override
    {
#ifdef _WIN32
        (void) _fseeki64(m_File, pos, SEEK_SET);
#else
        (void) fseek(m_File, pos, SEEK_SET);
#endif
    }

    void AdvancePos(const i64 phase) noexcept override
    {
#ifdef _WIN32
        (void) _fseeki64(m_File, pos, SEEK_CUR);
#else
        (void) fseek(m_File, phase, SEEK_CUR);
#endif
    }

    i64 ReadBytes(u8* const buffer, const uSys len) noexcept override
    {
        if(m_Props != FileProps::Read && m_Props != FileProps::ReadWrite)
        {
            return -1;
        }

        return fread(buffer, sizeof(u8), len, m_File);
    }

    i64 WriteBytes(const u8* buffer, uSys len) noexcept override
    {
        if(m_Props == FileProps::Read)
        {
            return -1;
        }

        return fwrite(buffer, sizeof(u8), len, m_File);
    }

    int ReadChar() noexcept override
    {
        if(m_Props == FileProps::Read)
        {
            return -1;
        }

        return fgetc(m_File);
    }

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
    DEFAULT_DESTRUCT(CFileLoader);
    DEFAULT_CM_PO(CFileLoader);
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
            fclose(file);
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

    [[nodiscard]] IFile* Load(const C8DynString& path, FileProps props) const noexcept override
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

        return new CFile(handle, path, props);
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
