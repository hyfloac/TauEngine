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
    DELETE_COPY(IFile);
public:
    IFile() noexcept = default;

    virtual ~IFile() noexcept = default;

    [[nodiscard]] virtual i64 size() noexcept = 0;

    [[nodiscard]] virtual bool exists() noexcept = 0;

    [[nodiscard]] virtual const char* name() noexcept = 0;

    /**
     * Sets the current read/write index pointer of the file.
     *
     *   For resetting the read/write index use {@link IFile::resetPos() @endlink}
     * as a specific implementation may have changed how the
     * index pointer works.
     */
    virtual void setPos(uSys pos) noexcept = 0;

    virtual void resetPos() noexcept
    { setPos(0); }

    virtual i64 readBytes(u8* buffer, uSys len) noexcept = 0;

    virtual i64 read(void* buffer, uSys len) noexcept
    { return readBytes(reinterpret_cast<u8*>(buffer), len); }

    virtual i64 readString(char* buffer, uSys len) noexcept
    { return readBytes(reinterpret_cast<u8*>(buffer), len); }

    virtual RefDynArray<u8> readFile() noexcept
    {
        const size_t size_ = size();
        RefDynArray<u8> arr(size_ + 1);
        readBytes(arr.arr(), size_);
        arr[size_] = '\0';
        return arr;
    }

    virtual int readChar() noexcept
    {
        char ret;
        const int cnt = readBytes(reinterpret_cast<u8*>(&ret), 1);
        return cnt ? ret : -1;
    }

    virtual i64 writeBytes(const u8* buffer, uSys len) noexcept = 0;

    virtual i64 write(const void* buffer, uSys len) noexcept
    { return writeBytes(reinterpret_cast<const u8*>(buffer), len); }

    virtual i64 writeString(const char* str) noexcept
    { return writeBytes(reinterpret_cast<const u8*>(str), std::strlen(str)); }

    template<typename _T>
    i64 readType(_T* t) noexcept
    { return readBytes(reinterpret_cast<u8*>(t), sizeof(_T)); }

    template<typename _T>
    i64 writeType(const _T& t) noexcept
    { return writeBytes(reinterpret_cast<const u8*>(&t), sizeof(_T)); }
};

/**
 * An interface used to load {@link IFile @endlink}'s.
 *
 *   Any implementation of {@link IFile @endlink} should implement
 * a corresponding {@link IFileLoader @endlink}.
 */
class IFileLoader
{
    DELETE_COPY(IFileLoader);
public:
    IFileLoader() noexcept = default;

    virtual ~IFileLoader() noexcept = default;

    [[nodiscard]] virtual bool fileExists(const char* path) const noexcept = 0;

    [[nodiscard]] virtual CPPRef<IFile> load(const char* path, FileProps props) const noexcept = 0;

    [[nodiscard]] virtual bool createFolder(const char* path) const noexcept = 0;

    [[nodiscard]] virtual bool createFolders(const char* path) const noexcept = 0;

    [[nodiscard]] virtual bool deleteFolder(const char* path) const noexcept = 0;

    [[nodiscard]] virtual bool deleteFile(const char* path) const noexcept = 0;

    [[nodiscard]] virtual u64 creationTime(const char* path) const noexcept = 0;

    [[nodiscard]] virtual u64 modifyTime(const char* path) const noexcept = 0;
};
