#include "CFile.hpp"
#ifdef _WIN32
#include "Win32File.hpp"
#endif

i64 CFile::size() noexcept
{
    const long curPos = ftell(_file);
    if(fseek(_file, 0, SEEK_END))
    { return -2; }

    const i64 size = ftell(_file);
    fseek(_file, curPos, SEEK_SET);

    return size;
}

void CFile::setPos(const uSys pos) noexcept
{
    fseek(_file, pos, SEEK_SET);
}

void CFile::advancePos(iSys phase) noexcept
{
    fseek(_file, phase, SEEK_CUR);
}

i64 CFile::readBytes(u8* const buffer, const uSys len) noexcept
{
    if(_props != FileProps::Read && _props != FileProps::ReadWrite)
    { return -1; }
    return fread(buffer, sizeof(u8), len, _file);
}

i64 CFile::writeBytes(const u8* const buffer, const uSys len) noexcept
{
    if(_props == FileProps::Read)
    { return -1; }
    return fwrite(buffer, sizeof(u8), len, _file);
}

int CFile::readChar() noexcept
{
    if(_props == FileProps::Read)
    { return -1; }
    return fgetc(_file);
}


CPPRef<CFileLoader>& CFileLoader::Instance() noexcept
{
    static CPPRef<CFileLoader> instance(new CFileLoader);
    return instance;
}

bool CFileLoader::fileExists(const wchar_t* const path) const noexcept
{
    FILE* file;
    if((file = _wfopen(path, L"r")))
    {
        fclose(file);
        return true;
    }
    return false;
}

bool CFileLoader::fileExists(const char* const path) const noexcept
{
    FILE* file;
    if((file = fopen(path, "r")))
    {
        fclose(file);
        return true;
    }
    return false;
}

CPPRef<IFile> CFileLoader::load(const wchar_t* path, FileProps props) const noexcept
{
    FILE* handle;
    switch(props)
    {
        case FileProps::Read:           handle = _wfopen(path, L"rb");  break;
        case FileProps::WriteNew:       handle = _wfopen(path, L"wb");  break;
        case FileProps::WriteOverwrite: handle = _wfopen(path, L"r+b"); break;
        case FileProps::WriteAppend:    handle = _wfopen(path, L"ab");  break;
        case FileProps::ReadWrite:      handle = _wfopen(path, L"r+b"); break;
        default: return nullptr;
    }

    if(!handle)
    { return nullptr; }

    return CPPRef<CFile>(new CFile(handle, path, props));
}

CPPRef<IFile> CFileLoader::load(const char* const path, const FileProps props) const noexcept
{
    FILE* handle;
    switch(props)
    {
        case FileProps::Read:           handle = fopen(path, "rb");  break;
        case FileProps::WriteNew:       handle = fopen(path, "wb");  break;
        case FileProps::WriteOverwrite: handle = fopen(path, "r+b"); break;
        case FileProps::WriteAppend:    handle = fopen(path, "ab");  break;
        case FileProps::ReadWrite:      handle = fopen(path, "r+b"); break;
        default: return nullptr;
    }

    if(!handle)
    { return nullptr; }

    const DynString strPath(path);

    return CPPRef<CFile>(new CFile(handle, StringCast<wchar_t>(strPath), props));
}

bool CFileLoader::createFolder(const wchar_t* const path) const noexcept
{
#ifdef _WIN32
    return Win32FileLoader::Instance()->createFolder(path);
#else
    return false;
#endif
}

bool CFileLoader::createFolder(const char* const path) const noexcept
{
#ifdef _WIN32
    return Win32FileLoader::Instance()->createFolder(path);
#else
    return false;
#endif
}

bool CFileLoader::createFolders(const wchar_t* const path) const noexcept
{
#ifdef _WIN32
    return Win32FileLoader::Instance()->createFolders(path);
#else
    return false;
#endif
}

bool CFileLoader::createFolders(const char* const path) const noexcept
{
#ifdef _WIN32
    return Win32FileLoader::Instance()->createFolders(path);
#else
    return false;
#endif
}

bool CFileLoader::deleteFolder(const wchar_t* const path) const noexcept
{
#ifdef _WIN32
    return Win32FileLoader::Instance()->deleteFolder(path);
#else
    return false;
#endif
}

bool CFileLoader::deleteFolder(const char* const path) const noexcept
{
#ifdef _WIN32
    return Win32FileLoader::Instance()->deleteFolder(path);
#else
    return false;
#endif
}

bool CFileLoader::deleteFile(const wchar_t* const path) const noexcept
{
    return !_wremove(path);
}

bool CFileLoader::deleteFile(const char* const path) const noexcept
{
    return !remove(path);
}

u64 CFileLoader::creationTime(const wchar_t* const path) const noexcept
{
#ifdef _WIN32
    return Win32FileLoader::Instance()->creationTime(path);
#else
    return 0;
#endif
}

u64 CFileLoader::creationTime(const char* const path) const noexcept
{
#ifdef _WIN32
    return Win32FileLoader::Instance()->creationTime(path);
#else
    return 0;
#endif
}

u64 CFileLoader::modifyTime(const wchar_t* const path) const noexcept
{
#ifdef _WIN32
    return Win32FileLoader::Instance()->modifyTime(path);
#else
    return 0;
#endif
}

u64 CFileLoader::modifyTime(const char* const path) const noexcept
{
#ifdef _WIN32
    return Win32FileLoader::Instance()->modifyTime(path);
#else
    return 0;
#endif
}
