// #include "pch.h"
#include "Win32File.hpp"

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

static thread_local i64 _retBytesRead;

static void CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransferred, LPOVERLAPPED lpOverlapped) noexcept
{ _retBytesRead = dwNumberOfBytesTransferred; }

i64 Win32File::readBytes(u8* buffer, u64 len) noexcept
{
    OVERLAPPED ol;
    ZeroMemory(&ol, sizeof(ol));
    ReadFileEx(_file, buffer, len, &ol, FileIOCompletionRoutine);
    return _retBytesRead;
}

i64 Win32File::writeBytes(const u8* buffer, u64 len) noexcept
{
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

Ref<IFile> Win32FileLoader::load(const char* path) const noexcept
{
    HANDLE file = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(!file && GetLastError() == ERROR_FILE_NOT_FOUND)
    {
        file = CreateFileA(path, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    }

    return Ref<Win32File>(new Win32File(file, path));
}
