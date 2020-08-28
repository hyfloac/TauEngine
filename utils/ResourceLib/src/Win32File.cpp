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
{ return _file != INVALID_HANDLE_VALUE; }

void Win32File::setPos(const uSys pos) noexcept
{
    const LONG low = static_cast<LONG>(pos);
    LONG high = static_cast<u64>(pos) >> 32;

    SetFilePointer(_file, low, &high, FILE_BEGIN);
}

void Win32File::advancePos(iSys phase) noexcept
{
    const LONG low = static_cast<LONG>(phase);
    LONG high = static_cast<u64>(phase) >> 32;

    SetFilePointer(_file, low, &high, FILE_CURRENT);
}

i64 Win32File::readBytes(u8* buffer, const uSys len) noexcept
{
    if(_props != FileProps::Read && _props != FileProps::ReadWrite)
    { return -1; }

    const DWORD dLen = static_cast<DWORD>(len);

    DWORD read;
    if(!ReadFile(_file, buffer, dLen, &read, nullptr))
    { return -1; }
    return read;
}

i64 Win32File::writeBytes(const u8* buffer, const uSys len) noexcept
{
    if(_props == FileProps::Read)
    { return -1; }
    const DWORD dLen = static_cast<DWORD>(len);

    DWORD written;
    WriteFile(_file, buffer, dLen, &written, nullptr);
    return written;
}

const CPPRef<Win32FileLoader>& Win32FileLoader::Instance() noexcept
{
    static CPPRef<Win32FileLoader> instance(new(::std::nothrow) Win32FileLoader);
    return instance;
}

bool Win32FileLoader::fileExists(const wchar_t* const path) const noexcept
{
    return !(GetFileAttributesW(path) == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND);
}

bool Win32FileLoader::fileExists(const char* const path) const noexcept
{
    return !(GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND);
}

CPPRef<IFile> Win32FileLoader::load(const wchar_t* const path, const FileProps props) const noexcept
{
    HANDLE file;
    if(props == FileProps::Read)
    {
        file = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    }
    else if(props == FileProps::WriteNew)
    {
        file = CreateFileW(path, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    }
    else if(props == FileProps::WriteOverwrite)
    {
        file = CreateFileW(path, GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if(file == INVALID_HANDLE_VALUE && GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            file = CreateFileW(path, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
        }
    }
    else if(props == FileProps::WriteAppend)
    {
        file = CreateFileW(path, FILE_APPEND_DATA, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    }
    else if(props == FileProps::ReadWrite)
    {
        file = CreateFileW(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if(!file && GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            file = CreateFileW(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
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

CPPRef<IFile> Win32FileLoader::load(const char* const path, const FileProps props) const noexcept
{
    HANDLE file;
    if(props == FileProps::Read)
    {
        file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    }
    else if(props == FileProps::WriteNew)
    {
        file = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    }
    else if(props == FileProps::WriteOverwrite)
    {
        file = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if(file == INVALID_HANDLE_VALUE && GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            file = CreateFileA(path, GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
        }
    }
    else if(props == FileProps::WriteAppend)
    {
        file = CreateFileA(path, FILE_APPEND_DATA, FILE_SHARE_READ, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    }
    else if(props == FileProps::ReadWrite)
    {
        file = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if(!file && GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            file = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
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

    const DynString strPath(path);

    return CPPRef<Win32File>(new Win32File(file, StringCast<wchar_t>(strPath), props));
}

bool Win32FileLoader::createFolder(const wchar_t* const path) const noexcept
{
    if(!CreateDirectoryW(path, nullptr) && GetLastError() != ERROR_ALREADY_EXISTS)
    { return false; }

    return true;
}

bool Win32FileLoader::createFolder(const char* const path) const noexcept
{
    if(!CreateDirectoryA(path, nullptr) && GetLastError() != ERROR_ALREADY_EXISTS)
    { return false; }

    return true;
}

bool Win32FileLoader::createFolders(const wchar_t* const path) const noexcept
{
    const WDynString win32Path = VFS::win32Path(WDynString(path));
    const HRESULT res = SHCreateDirectoryExW(nullptr, win32Path.c_str(), nullptr);
    return res == ERROR_SUCCESS || res == ERROR_ALREADY_EXISTS;
}

bool Win32FileLoader::createFolders(const char* const path) const noexcept
{
    const DynString win32Path = VFS::win32Path(DynString(path));
    const HRESULT res = SHCreateDirectoryExA(nullptr, win32Path.c_str(), nullptr);
    return res == ERROR_SUCCESS || res == ERROR_ALREADY_EXISTS;
}

bool Win32FileLoader::deleteFolder(const wchar_t* const path) const noexcept
{ return RemoveDirectoryW(path); }

bool Win32FileLoader::deleteFolder(const char* const path) const noexcept
{ return RemoveDirectoryA(path); }

bool Win32FileLoader::deleteFile(const wchar_t* const path) const noexcept
{ return DeleteFileW(path); }

bool Win32FileLoader::deleteFile(const char* const path) const noexcept
{ return DeleteFileA(path); }

u64 Win32FileLoader::creationTime(const wchar_t* const path) const noexcept
{
    const HANDLE file = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if(file == INVALID_HANDLE_VALUE)
    { return 0; }

    FILETIME cTime;
    if(!GetFileTime(file, &cTime, nullptr, nullptr))
    { return 0; }

    return (static_cast<u64>(cTime.dwHighDateTime) << 32) | static_cast<u64>(cTime.dwLowDateTime);
}

u64 Win32FileLoader::creationTime(const char* const path) const noexcept
{
    const HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if(file == INVALID_HANDLE_VALUE)
    { return 0; }

    FILETIME cTime;
    if(!GetFileTime(file, &cTime, nullptr, nullptr))
    { return 0; }

    return (static_cast<u64>(cTime.dwHighDateTime) << 32) | static_cast<u64>(cTime.dwLowDateTime);
}

u64 Win32FileLoader::modifyTime(const wchar_t* const path) const noexcept
{
    const HANDLE file = CreateFileW(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if(file == INVALID_HANDLE_VALUE)
    { return 0; }

    FILETIME mTime;
    if(!GetFileTime(file, nullptr, nullptr, &mTime))
    { return 0; }

    return (static_cast<u64>(mTime.dwHighDateTime) << 32) | static_cast<u64>(mTime.dwLowDateTime);
}

u64 Win32FileLoader::modifyTime(const char* const path) const noexcept
{
    const HANDLE file = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

    if(file == INVALID_HANDLE_VALUE)
    { return 0; }

    FILETIME mTime;
    if(!GetFileTime(file, nullptr, nullptr, &mTime))
    { return 0; }

    return (static_cast<u64>(mTime.dwHighDateTime) << 32) | static_cast<u64>(mTime.dwLowDateTime);
}
