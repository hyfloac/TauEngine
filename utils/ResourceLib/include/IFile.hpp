/**
 * @file
 *
 * Describes an abstract file handle.
 */
#pragma once

#include "pch.h"

/**
 * An interface used to represent an abstract file handle.
 *
 *   This can be overloaded to support any filesystem, or
 * other medium of holding files such as compressed
 * archives.
 */
class IFile
{
private:
    IFile(const IFile& copy) noexcept = delete;
    IFile(IFile&& move) noexcept = delete;

    IFile& operator=(const IFile& copy) noexcept = delete;
    IFile& operator=(IFile&& move) noexcept = delete;
public:
    virtual ~IFile() noexcept = default;

    virtual i64 size() noexcept = 0;

    virtual i64 readBytes(u8* buffer, u64 len) noexcept = 0;

    virtual i64 readString(char* buffer, u64 len) noexcept
    { return readBytes(reinterpret_cast<u8*>(buffer), len); }

    virtual void writeBytes(const u8* buffer, u64 len) noexcept = 0;

    virtual void writeString(const char* str) noexcept
    { writeBytes(reinterpret_cast<const u8*>(str), std::strlen(str)); }
};

/**
 * An interface used to load {@link IFile}'s.
 *
 *   Any implementation of {@link IFile} should implement
 * a corresponding {@link IFileLoader}.
 */
class IFileLoader
{
private:
    IFileLoader(const IFileLoader& copy) noexcept = delete;
    IFileLoader(IFileLoader&& move) noexcept = delete;

    IFileLoader& operator=(const IFileLoader& copy) noexcept = delete;
    IFileLoader& operator=(IFileLoader&& move) noexcept = delete;
public:
    virtual ~IFileLoader() noexcept = default;

    virtual Ref<IFile> load(const char* path) noexcept = 0;
};
