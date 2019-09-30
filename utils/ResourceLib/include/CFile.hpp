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
private:
    FILE* _file;
    const char* _name;
private:
    CFile(const CFile& copy) noexcept = delete;
    CFile(CFile&& move) noexcept = delete;

    CFile& operator=(const CFile& copy) noexcept = delete;
    CFile& operator=(CFile&& move) noexcept = delete;
public:
    CFile(FILE* file, const char* name) noexcept
        : _file(file), _name(name)
    { }

    ~CFile() noexcept override
    {
        if(_file)
        { fclose(_file); }
    }

    i64 size() noexcept override;

    bool exists() noexcept override { return _file != null; }

    const char* name() noexcept override { return _name; }

    void setPos(u64 pos) noexcept override;

    i64 readBytes(u8* buffer, u64 len) noexcept override;

    i64 writeBytes(const u8* buffer, u64 len) noexcept override;
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
public:
    static Ref<CFileLoader>& Instance() noexcept;
private:
    CFileLoader(const CFileLoader& copy) noexcept = delete;
    CFileLoader(CFileLoader&& move) noexcept = delete;

    CFileLoader& operator=(const CFileLoader& copy) noexcept = delete;
    CFileLoader& operator=(CFileLoader&& move) noexcept = delete;
public:
    CFileLoader() noexcept = default;

    ~CFileLoader() noexcept override = default;

    bool fileExists(const char* path) const noexcept override;

    Ref<IFile> load(const char* path) const noexcept override;

    Ref<CFile> load2(const char* path) const noexcept
    { return RefCast<CFile>(load(path)); }
};
