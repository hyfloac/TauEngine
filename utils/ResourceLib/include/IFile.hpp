/**
 * @file
 *
 * Describes an abstract file handle.
 */
#pragma once

#include <Safeties.hpp>
#include <NumTypes.hpp>
#include <DynArray.hpp>

enum class FileProps
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
    DEFAULT_CONSTRUCT_PO(IFile);
    DEFAULT_DESTRUCT_VI(IFile);
    DEFAULT_CM_PO(IFile);
public:
    [[nodiscard]] virtual i64 size() noexcept = 0;

    [[nodiscard]] virtual bool exists() noexcept = 0;

    [[nodiscard]] virtual const wchar_t* name() noexcept = 0;

    /**
     * Sets the current read/write index pointer of the file.
     *
     *   For resetting the read/write index use {@link IFile::resetPos() @endlink}
     * as a specific implementation may have changed how the
     * index pointer works.
     */
    virtual void setPos(uSys pos) noexcept = 0;

    virtual void advancePos(iSys phase) noexcept = 0;

    virtual void resetPos() noexcept
    { setPos(0); }

    virtual i64 readBytes(u8* buffer, uSys len) noexcept = 0;

    virtual i64 read(void* const buffer, const uSys len) noexcept
    { return readBytes(reinterpret_cast<u8*>(buffer), len); }

    virtual i64 readString(char* const buffer, const uSys len) noexcept
    { return readBytes(reinterpret_cast<u8*>(buffer), len * sizeof(char)); }

    virtual i64 readString(wchar_t* const buffer, const uSys len) noexcept
    { return readBytes(reinterpret_cast<u8*>(buffer), len * sizeof(wchar_t)); }

    virtual RefDynArray<u8> readFile() noexcept
    {
        const uSys size_ = size();
        RefDynArray<u8> arr(size_ + 1);
        readBytes(arr.arr(), size_);
        arr[size_] = '\0';
        return arr;
    }

    virtual int readChar() noexcept
    {
        char ret;
        const i64 cnt = readBytes(reinterpret_cast<u8*>(&ret), sizeof(char));
        return cnt ? ret : -1;
    }

    virtual int readWChar() noexcept
    {
        wchar_t ret;
        const i64 cnt = readBytes(reinterpret_cast<u8*>(&ret), sizeof(wchar_t));
        return cnt ? ret : -1;
    }

    virtual i64 writeBytes(const u8* buffer, uSys len) noexcept = 0;

    virtual i64 write(const void* const buffer, const uSys len) noexcept
    { return writeBytes(reinterpret_cast<const u8*>(buffer), len); }

    virtual i64 writeString(const char* const str) noexcept
    { return writeBytes(reinterpret_cast<const u8*>(str), ::std::strlen(str) * sizeof(char)); }

    virtual i64 writeString(const wchar_t* const str) noexcept
    { return writeBytes(reinterpret_cast<const u8*>(str), ::std::wcslen(str) * sizeof(wchar_t)); }

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
    DEFAULT_CONSTRUCT_PU(IFileLoader);
    DEFAULT_DESTRUCT_VI(IFileLoader);
    DEFAULT_CM_PO(IFileLoader);
public:
    [[nodiscard]] virtual bool fileExists(const wchar_t* path) const noexcept = 0;
    [[nodiscard]] virtual bool fileExists(const char* path) const noexcept;

    [[nodiscard]] virtual bool fileExists(const wchar_t* basePath, const wchar_t* subPath) const noexcept;
    [[nodiscard]] virtual bool fileExists(const char* basePath, const char* subPath) const noexcept;

    [[nodiscard]] virtual CPPRef<IFile> load(const wchar_t* path, FileProps props) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IFile> load(const char* path, FileProps props) const noexcept;

    [[nodiscard]] virtual CPPRef<IFile> load(const wchar_t* basePath, const wchar_t* subPath, FileProps props) const noexcept;
    [[nodiscard]] virtual CPPRef<IFile> load(const char* basePath, const char* subPath, FileProps props) const noexcept;
    
    [[nodiscard]] virtual bool createFolder(const wchar_t* path) const noexcept = 0;
    [[nodiscard]] virtual bool createFolder(const char* path) const noexcept;

    [[nodiscard]] virtual bool createFolder(const wchar_t* basePath, const wchar_t* subPath) const noexcept;
    [[nodiscard]] virtual bool createFolder(const char* basePath, const char* subPath) const noexcept;

    [[nodiscard]] virtual bool createFolders(const wchar_t* path) const noexcept = 0;
    [[nodiscard]] virtual bool createFolders(const char* path) const noexcept;

    [[nodiscard]] virtual bool createFolders(const wchar_t* basePath, const wchar_t* subPath) const noexcept;
    [[nodiscard]] virtual bool createFolders(const char* basePath, const char* subPath) const noexcept;

    [[nodiscard]] virtual bool deleteFolder(const wchar_t* path) const noexcept = 0;
    [[nodiscard]] virtual bool deleteFolder(const char* path) const noexcept;

    [[nodiscard]] virtual bool deleteFolder(const wchar_t* basePath, const wchar_t* subPath) const noexcept;
    [[nodiscard]] virtual bool deleteFolder(const char* basePath, const char* subPath) const noexcept;

    [[nodiscard]] virtual bool deleteFile(const wchar_t* path) const noexcept = 0;
    [[nodiscard]] virtual bool deleteFile(const char* path) const noexcept;

    [[nodiscard]] virtual bool deleteFile(const wchar_t* basePath, const wchar_t* subPath) const noexcept;
    [[nodiscard]] virtual bool deleteFile(const char* basePath, const char* subPath) const noexcept;
    
    [[nodiscard]] virtual u64 creationTime(const wchar_t* path) const noexcept = 0;
    [[nodiscard]] virtual u64 creationTime(const char* path) const noexcept;
    
    [[nodiscard]] virtual u64 creationTime(const wchar_t* basePath, const wchar_t* subPath) const noexcept;
    [[nodiscard]] virtual u64 creationTime(const char* basePath, const char* subPath) const noexcept;

    [[nodiscard]] virtual u64 modifyTime(const wchar_t* path) const noexcept = 0;
    [[nodiscard]] virtual u64 modifyTime(const char* path) const noexcept;

    [[nodiscard]] virtual u64 modifyTime(const wchar_t* basePath, const wchar_t* subPath) const noexcept;
    [[nodiscard]] virtual u64 modifyTime(const char* basePath, const char* subPath) const noexcept;
};
