// #include "pch.h"
#include "Win32File.hpp"
#include <Utils.hpp>

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

void Win32File::setPos(u64 pos) noexcept
{
    LONG high = pos >> 32;

    SetFilePointer(_file, pos, &high, FILE_BEGIN);
}

i64 Win32File::readBytes(u8* buffer, u64 len) noexcept
{
    if(_props != FileProps::Read && _props != FileProps::ReadWrite)
    { return -1; }
    DWORD read;
    if(!ReadFile(_file, buffer, len, &read, NULL))
    { return -1; }
    return read;
}

i64 Win32File::writeBytes(const u8* buffer, u64 len) noexcept
{
    if(_props == FileProps::Read)
    { return -1; }
    OVERLAPPED ol;
    ZeroMemory(&ol, sizeof(ol));
    DWORD written;
    WriteFile(_file, buffer, len, &written, &ol);
    return written;
}

Ref<Win32FileLoader>& Win32FileLoader::Instance() noexcept
{
    static Ref<Win32FileLoader> instance = Ref<Win32FileLoader>(new Win32FileLoader);
    return instance;
}


bool Win32FileLoader::fileExists(const char* path) const noexcept
{
    return !(GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES && GetLastError() == ERROR_FILE_NOT_FOUND);
}

Ref<IFile> Win32FileLoader::load(const char* path, FileProps props) const noexcept
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
        return nullptr;
    }

    return Ref<Win32File>(new Win32File(file, path, props));
}


bool Win32FileLoader::createFolder(const char* path) const noexcept
{
    if(!CreateDirectoryA(path, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
    {
        return false;
    }

    return true;
}

bool Win32FileLoader::deleteFolder(const char* path) const noexcept
{
    return RemoveDirectoryA(path);
}

bool Win32FileLoader::deleteFile(const char* path) const noexcept
{
    return DeleteFileA(path);
}
