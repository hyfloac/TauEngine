// #include "pch.h"
#include "Win32File.hpp"
#include <Utils.hpp>
#include <ShlObj.h>

#include "String.hpp"
#include "VFS.hpp"

i64 Win32File::size() noexcept
{
    if(_file == INVALID_HANDLE_VALUE)
    { return -1; }

    LARGE_INTEGER size;
    if(!GetFileSizeEx(_file, &size))
    { return -1; }

    return size.QuadPart;
}

bool Win32File::exists() noexcept
{
    return _file != INVALID_HANDLE_VALUE;
}

void Win32File::setPos(const uSys pos) noexcept
{
    LONG high = static_cast<u64>(pos) >> 32;

    SetFilePointer(_file, pos, &high, FILE_BEGIN);
}

i64 Win32File::readBytes(u8* buffer, const uSys len) noexcept
{
    if(_props != FileProps::Read && _props != FileProps::ReadWrite)
    { return -1; }
    DWORD read;
    if(!ReadFile(_file, buffer, len, &read, NULL))
    { return -1; }
    return read;
}

i64 Win32File::writeBytes(const u8* buffer, const uSys len) noexcept
{
    if(_props == FileProps::Read)
    { return -1; }
    // OVERLAPPED ol;
    // ZeroMemory(&ol, sizeof(ol));
    DWORD written;
    WriteFile(_file, buffer, len, &written, NULL);
    return written;
}

CPPRef<Win32FileLoader>& Win32FileLoader::Instance() noexcept
{
    static CPPRef<Win32FileLoader> instance = CPPRef<Win32FileLoader>(new Win32FileLoader);
    return instance;
}


bool Win32FileLoader::fileExists(const char* const path) const noexcept
{
    return !(GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND);
}

CPPRef<IFile> Win32FileLoader::load(const char* const path, const FileProps props) const noexcept
{
    HANDLE file;
    if(props == FileProps::Read)
    {
        file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    else if(props == FileProps::WriteNew)
    {
        file = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    else if(props == FileProps::WriteOverwrite)
    {
        file = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if(file == INVALID_HANDLE_VALUE && GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            file = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        }
    }
    else if(props == FileProps::WriteAppend)
    {
        file = CreateFileA(path, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    else if(props == FileProps::ReadWrite)
    {
        file = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if(!file && GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            file = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
        }
    }
    else
    {
        file = INVALID_HANDLE_VALUE;
    }

    if(file == INVALID_HANDLE_VALUE)
    {
        HRESULT error = GetLastError();
        UNUSED(error);
        return nullptr;
    }

    return CPPRef<Win32File>(new Win32File(file, path, props));
}

bool Win32FileLoader::createFolder(const char* const path) const noexcept
{
    if(!CreateDirectoryA(path, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
    {
        return false;
    }

    return true;
}

bool Win32FileLoader::createFolders(const char* path) const noexcept
{
    const DynString win32Path = VFS::win32Path(DynString(path));
    const HRESULT res = SHCreateDirectoryExA(NULL, win32Path.c_str(), NULL);
    return res == ERROR_SUCCESS || res == ERROR_ALREADY_EXISTS;
}

bool Win32FileLoader::deleteFolder(const char* const path) const noexcept
{
    return RemoveDirectoryA(path);
}

bool Win32FileLoader::deleteFile(const char* const path) const noexcept
{
    return DeleteFileA(path);
}

u64 Win32FileLoader::creationTime(const char* const path) const noexcept
{
    HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(file == INVALID_HANDLE_VALUE)
    { return 0; }

    FILETIME cTime;
    if(!GetFileTime(file, &cTime, NULL, NULL))
    { return 0; }

    return (static_cast<u64>(cTime.dwHighDateTime) << 32) | static_cast<u64>(cTime.dwLowDateTime);
}

u64 Win32FileLoader::modifyTime(const char* const path) const noexcept
{
    HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if(file == INVALID_HANDLE_VALUE)
    {
        return 0;
    }

    FILETIME mTime;
    if(!GetFileTime(file, NULL, NULL, &mTime))
    { return 0; }

    return (static_cast<u64>(mTime.dwHighDateTime) << 32) | static_cast<u64>(mTime.dwLowDateTime);
}
