/**
 * @file
 *
 * Describes a C standard IO file handle.
 */
#pragma once

#include "IFile.hpp"

#include <cstdio>
#include <Utils.hpp>
#include <String.hpp>

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
    DELETE_CM(CFile);
private:
    FILE* _file;
    WDynString _name;
    FileProps _props;
public:
    CFile(FILE* const file, const WDynString& name, const FileProps props) noexcept
        : _file(file)
        , _name(name)
        , _props(props)
    { }

    CFile(FILE* const file, WDynString&& name, const FileProps props) noexcept
        : _file(file)
        , _name(::std::move(name))
        , _props(props)
    { }

    ~CFile() noexcept override
    {
        if(_file)
        { fclose(_file); }
    }

    [[nodiscard]] i64 size() noexcept override;

    [[nodiscard]] bool exists() noexcept override { return _file != null; }

    [[nodiscard]] const wchar_t* name() noexcept override { return _name; }

    void setPos(uSys pos) noexcept override;
    void advancePos(iSys phase) noexcept override;

    i64 readBytes(u8* buffer, uSys len) noexcept override;

    i64 writeBytes(const u8* buffer, uSys len) noexcept override;

    int readChar() noexcept override;
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
    DEFAULT_CM_PO(CFileLoader);
public:
    static CPPRef<CFileLoader>& Instance() noexcept;
public:
    [[nodiscard]] bool fileExists(const wchar_t* path) const noexcept override;
    [[nodiscard]] bool fileExists(const char* path) const noexcept override;
    
    [[nodiscard]] CPPRef<IFile> load(const char* path, FileProps props) const noexcept override;
    [[nodiscard]] CPPRef<IFile> load(const wchar_t* path, FileProps props) const noexcept override;
    
    [[nodiscard]] CPPRef<CFile> load2(const wchar_t* const path, const FileProps props) const noexcept
    { return RefCast<CFile>(load(path, props)); }
    [[nodiscard]] CPPRef<CFile> load2(const char* const path, const FileProps props) const noexcept
    { return RefCast<CFile>(load(path, props)); }

    [[nodiscard]] bool createFolder(const wchar_t* path) const noexcept override;
    [[nodiscard]] bool createFolder(const char* path) const noexcept override;

    [[nodiscard]] bool createFolders(const wchar_t* path) const noexcept override;
    [[nodiscard]] bool createFolders(const char* path) const noexcept override;

    [[nodiscard]] bool deleteFolder(const wchar_t* path) const noexcept override;
    [[nodiscard]] bool deleteFolder(const char* path) const noexcept override;

    [[nodiscard]] bool deleteFile(const wchar_t* path) const noexcept override;
    [[nodiscard]] bool deleteFile(const char* path) const noexcept override;

    [[nodiscard]] u64 creationTime(const wchar_t* path) const noexcept override;
    [[nodiscard]] u64 creationTime(const char* path) const noexcept override;

    [[nodiscard]] u64 modifyTime(const wchar_t* path) const noexcept override;
    [[nodiscard]] u64 modifyTime(const char* path) const noexcept override;
};
