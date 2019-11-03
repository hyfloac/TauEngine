/**
 * @file
 *
 * Describes an abstract file handle.
 */
#pragma once

// #include "pch.h"

#include <Safeties.hpp>
#include <NumTypes.hpp>
#include <DynArray.hpp>

enum class FileProps : u8
{
    Read = 0,
    WriteNew,
    WriteOverwrite,
    WriteAppend,
    ReadWrite
};

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
    IFile() noexcept = default;

    virtual ~IFile() noexcept = default;

    virtual i64 size() noexcept = 0;

    virtual bool exists() noexcept = 0;

    virtual const char* name() noexcept = 0;

    /**
     * Sets the current read/write index pointer of the file.
     *
     *   For resetting the read/write index use {@link IFile::resetPos() @endlink}
     * as a specific implementation may have changed how the
     * index pointer works.
     */
    virtual void setPos(u64 pos) noexcept = 0;

    virtual void resetPos() noexcept
    { setPos(0); }

    virtual i64 readBytes(u8* buffer, u64 len) noexcept = 0;

    virtual i64 readString(char* buffer, u64 len) noexcept
    { return readBytes(reinterpret_cast<u8*>(buffer), len); }

    virtual RefDynArray<u8> readFile() noexcept
    {
        const size_t size_ = size();
        RefDynArray<u8> arr(size_ + 1);
        readBytes(arr.arr(), size_);
        arr[size_] = '\0';
        return arr;
    }

    virtual i64 writeBytes(const u8* buffer, u64 len) noexcept = 0;

    virtual i64 writeString(const char* str) noexcept
    { return writeBytes(reinterpret_cast<const u8*>(str), std::strlen(str)); }

    template<typename _T>
    i64 readType(_T* t) noexcept
    { return readBytes(reinterpret_cast<u8*>(t), sizeof(_T)); }

    template<typename _T>
    i64 writeType(const _T& t) noexcept
    { return writeBytes(reinterpret_cast<u8*>(&t), sizeof(_T)); }
};

/**
 * An interface used to load {@link IFile @endlink}'s.
 *
 *   Any implementation of {@link IFile @endlink} should implement
 * a corresponding {@link IFileLoader @endlink}.
 */
class IFileLoader
{
private:
    IFileLoader(const IFileLoader& copy) noexcept = delete;
    IFileLoader(IFileLoader&& move) noexcept = delete;

    IFileLoader& operator=(const IFileLoader& copy) noexcept = delete;
    IFileLoader& operator=(IFileLoader&& move) noexcept = delete;
public:
    IFileLoader() noexcept = default;

    virtual ~IFileLoader() noexcept = default;

    virtual bool fileExists(const char* path) const noexcept = 0;

    virtual Ref<IFile> load(const char* path, FileProps props) const noexcept = 0;

    virtual bool createFolder(const char* path) const noexcept = 0;

    virtual bool deleteFolder(const char* path) const noexcept = 0;

    virtual bool deleteFile(const char* path) const noexcept = 0;
};
