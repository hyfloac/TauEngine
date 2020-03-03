/**
 * @file
 *
 * Describes a Win32 standard IO file handle.
 */
#pragma once

#ifdef _WIN32

// #include "pch.h"
#include "IFile.hpp"
#include <Windows.h>
#include <NumTypes.hpp>

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
    FileProps _props;
private:
    Win32File(const Win32File& copy) noexcept = delete;
    Win32File(Win32File&& move) noexcept = delete;

    Win32File& operator=(const Win32File& copy) noexcept = delete;
    Win32File& operator=(Win32File&& move) noexcept = delete;
public:
    Win32File(HANDLE file, const char* name, FileProps props) noexcept
        : _file(file), _name(name), _props(props)
    { }

    ~Win32File() noexcept override
    { CloseHandle(_file); }

    [[nodiscard]] i64 size() noexcept override;

    [[nodiscard]] bool exists() noexcept override;

    [[nodiscard]] const char* name() noexcept override { return _name; }

    void setPos(uSys pos) noexcept override;

    i64 readBytes(u8* buffer, uSys len) noexcept override;

    i64 writeBytes(const u8* buffer, uSys len) noexcept override;
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
class Win32FileLoader final : public IFileLoader
{
public:
    static CPPRef<Win32FileLoader>& Instance() noexcept;
private:
    Win32FileLoader(const Win32FileLoader& copy) noexcept = delete;
    Win32FileLoader(Win32FileLoader&& move) noexcept = delete;

    Win32FileLoader& operator=(const Win32FileLoader& copy) noexcept = delete;
    Win32FileLoader& operator=(Win32FileLoader&& move) noexcept = delete;
public:
    Win32FileLoader() noexcept = default;

    ~Win32FileLoader() noexcept override = default;

    [[nodiscard]] bool fileExists(const char* path) const noexcept override;

    [[nodiscard]] CPPRef<IFile> load(const char* path, FileProps props) const noexcept override;

    [[nodiscard]] CPPRef<Win32File> load2(const char* path, FileProps props) const noexcept
    { return RefCast<Win32File>(load(path, props)); }

    [[nodiscard]] bool createFolder(const char* path) const noexcept override;

    [[nodiscard]] bool createFolders(const char* path) const noexcept override;

    [[nodiscard]] bool deleteFolder(const char* path) const noexcept override;

    [[nodiscard]] bool deleteFile(const char* path) const noexcept override;

    [[nodiscard]] u64 creationTime(const char* path) const noexcept override;

    [[nodiscard]] u64 modifyTime(const char* path) const noexcept override;
};

#endif
