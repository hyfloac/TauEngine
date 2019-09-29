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
 * than using {@link CFile} due to removing the overhead
 * of the C abstractions.
 *
 *   On Windows it is recommended to use this instead of
 * {@link CFile}.
 */
class Win32File final : IFile
{
private:
    HANDLE _file;
private:
    Win32File(const Win32File& copy) noexcept = delete;
    Win32File(Win32File&& move) noexcept = delete;

    Win32File& operator=(const Win32File& copy) noexcept = delete;
    Win32File& operator=(Win32File&& move) noexcept = delete;
public:
    ~Win32File() noexcept override
    { CloseHandle(_file); }

    i64 size() noexcept override;

    i64 readBytes(u8* buffer, u64 len) noexcept override;

    void writeBytes(const u8* buffer, u64 len) noexcept override;
};

/**
 * A Win32 file loader.
 *
 *   A file loader for the Win32 API for the Windows
 * operating system. This is significantly faster than
 * using {@link CFileLoader} due to removing the overhead
 * of the C abstractions.
 *
 *   On Windows it is recommended to use this instead of
 * {@link CFileLoader}.
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
};

#endif
