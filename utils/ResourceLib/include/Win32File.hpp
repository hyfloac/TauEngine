/**
 * @file
 *
 * Describes a Win32 standard IO file handle.
 */
#pragma once

#ifdef _WIN32

#include "pch.h"
#include "IFile.hpp"

class CFile;
class CFileLoader;

/**
 * A Win32 file handle.
 *
 *   Represents a file handle for the Win32 API for the
 * Windows operating system. This is significantly faster
 * than using {@link CFile @endlink} due to removing the overhead
 * of the C abstractions.
 *
 *   On Windows it is recommended to use this instead of
 * {@link CFile @endlink}.
 */
class Win32File final : public IFile
{
private:
    HANDLE _file;
    const char* _name;
private:
    Win32File(const Win32File& copy) noexcept = delete;
    Win32File(Win32File&& move) noexcept = delete;

    Win32File& operator=(const Win32File& copy) noexcept = delete;
    Win32File& operator=(Win32File&& move) noexcept = delete;
public:
    Win32File(HANDLE file, const char* name) noexcept
        : _file(file), _name(name)
    { }

    ~Win32File() noexcept override
    { CloseHandle(_file); }

    i64 size() noexcept override;

    bool exists() noexcept override;

    const char* name() noexcept override { return _name; }

    void setPos(u64 pos) noexcept override;

    i64 readBytes(u8* buffer, u64 len) noexcept override;

    i64 writeBytes(const u8* buffer, u64 len) noexcept override;
};

/**
 * A Win32 file loader.
 *
 *   A file loader for the Win32 API for the Windows
 * operating system. This is significantly faster than
 * using {@link CFileLoader @endlink} due to removing the overhead
 * of the C abstractions.
 *
 *   On Windows it is recommended to use this instead of
 * {@link CFileLoader @endlink}.
 */
class Win32FileLoader final : IFileLoader
{
private:
    Win32FileLoader(const Win32FileLoader& copy) noexcept = delete;
    Win32FileLoader(Win32FileLoader&& move) noexcept = delete;

    Win32FileLoader& operator=(const Win32FileLoader& copy) noexcept = delete;
    Win32FileLoader& operator=(Win32FileLoader&& move) noexcept = delete;
public:
    ~Win32FileLoader() noexcept override = default;

    Ref<IFile> load(const char* path) noexcept override;

    Ref<Win32File> load2(const char* path) noexcept
    { return RefCast<Win32File>(load(path)); }
};

#endif
