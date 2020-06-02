/**
 * @file
 *
 * Describes a C standard IO file handle.
 */
#pragma once

// #include "pch.h"
#include "IFile.hpp"
#include <cstdio>
#include <Utils.hpp>

class Win32File;
class Win32FileLoader;

/**
 * The most basic file handle.
 *
 *   This is here as a fallback to the most basic file
 * handling system. In general there may be a much faster
 * and more optimized implementation like {@link Win32File @endlink}.
 */
class CFile final : public IFile
{
    DELETE_COPY(CFile);
private:
    FILE* _file;
    const char* _name;
    FileProps _props;
public:
    CFile(FILE* const file, const char* const name, const FileProps props) noexcept
        : _file(file)
        , _name(name)
        , _props(props)
    { }

    ~CFile() noexcept override
    {
        if(_file)
        { fclose(_file); }
    }

    [[nodiscard]] i64 size() noexcept override;

    [[nodiscard]] bool exists() noexcept override { return _file != null; }

    [[nodiscard]] const char* name() noexcept override { return _name; }

    void setPos(uSys pos) noexcept override;

    i64 readBytes(u8* buffer, uSys len) noexcept override;

    i64 writeBytes(const u8* buffer, uSys len) noexcept override;
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
    DELETE_COPY(CFileLoader);
public:
    static CPPRef<CFileLoader>& Instance() noexcept;
public:
    [[nodiscard]] bool fileExists(const char* path) const noexcept override;

    [[nodiscard]] CPPRef<IFile> load(const char* path, FileProps props) const noexcept override;

    [[nodiscard]] CPPRef<CFile> load2(const char* const path, const FileProps props) const noexcept
    { return RefCast<CFile>(load(path, props)); }

    [[nodiscard]] bool createFolder(const char* path) const noexcept override;

    [[nodiscard]] bool createFolders(const char* path) const noexcept override;

    [[nodiscard]] bool deleteFolder(const char* path) const noexcept override;

    [[nodiscard]] bool deleteFile(const char* path) const noexcept override;

    [[nodiscard]] u64 creationTime(const char* path) const noexcept override;

    [[nodiscard]] u64 modifyTime(const char* path) const noexcept override;
};
