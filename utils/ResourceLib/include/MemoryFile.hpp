#pragma once

#include "IFile.hpp"

#include <unordered_map>
#include <Safeties.hpp>
#include <String.hpp>

class MemoryFile final : public IFile
{
    DEFAULT_DESTRUCT(MemoryFile);
    DELETE_CM(MemoryFile);
    union InternalFileView final
    {
        void* raw;
        u8* buf;
    };
    struct InternalFile
    {
        InternalFileView view;
        uSys reservedPages;
        uSys committedPages;
        u64 fileSize;
        u64 modifyTime;
        u64 creationTime;

        InternalFile(void* rawPages, uSys _reservedPages) noexcept;
    };
private:
    Ref<InternalFile> _file;
    WDynString _name;
    FileProps _props;
    u64 _cursor;
public:
    MemoryFile(const Ref<InternalFile>& file, const WDynString& name, const FileProps props, const u64 cursor = 0) noexcept
        : _file(file)
        , _name(name)
        , _props(props)
        , _cursor(cursor)
    { }

    MemoryFile(const Ref<InternalFile>& file, WDynString&& name, const FileProps props, const u64 cursor = 0) noexcept
        : _file(file)
        , _name(::std::move(name))
        , _props(props)
        , _cursor(cursor)
    { }
    
    [[nodiscard]] i64 size() noexcept override { return _file->fileSize; }
    [[nodiscard]] bool exists() noexcept override { return true; }
    [[nodiscard]] const wchar_t* name() noexcept override { return _name; }

    void setPos(uSys pos) noexcept override;
    void advancePos(iSys phase) noexcept override;
    i64 readBytes(u8* buffer, uSys len) noexcept override;
    i64 writeBytes(const u8* buffer, uSys len) noexcept override;
private:
    [[nodiscard]] bool reserveData(uSys pages) noexcept;
    [[nodiscard]] bool assertSize(uSys targetSize) noexcept;
};

class MemoryFileLoader final : public IFileLoader
{
    DEFAULT_CONSTRUCT_PU(MemoryFileLoader);
    DEFAULT_DESTRUCT(MemoryFileLoader);
    DEFAULT_CM_PO(MemoryFileLoader);
public:
    using FileMap = ::std::unordered_map<WDynString, Ref<MemoryFile::InternalFile>>;
    static CPPRef<MemoryFileLoader>& Instance() noexcept;
private:
    mutable FileMap _fileMap;
public:
    [[nodiscard]] bool fileExists(const wchar_t* path) const noexcept override;
    [[nodiscard]] CPPRef<IFile> load(const wchar_t* path, FileProps props) const noexcept override;

    [[nodiscard]] bool createFolder(const wchar_t* path) const noexcept override { return true; }
    [[nodiscard]] bool createFolder(const char* path) const noexcept override { return true; }
    [[nodiscard]] bool createFolder(const wchar_t* basePath, const wchar_t* subPath) const noexcept override { return true; }
    [[nodiscard]] bool createFolder(const char* basePath, const char* subPath) const noexcept override { return true; }

    [[nodiscard]] bool createFolders(const wchar_t* path) const noexcept override { return true; }
    [[nodiscard]] bool createFolders(const char* path) const noexcept override { return true; }
    [[nodiscard]] bool createFolders(const wchar_t* basePath, const wchar_t* subPath) const noexcept override { return true; }
    [[nodiscard]] bool createFolders(const char* basePath, const char* subPath) const noexcept override { return true; }

    [[nodiscard]] bool deleteFolder(const wchar_t* path) const noexcept override { return true; }
    [[nodiscard]] bool deleteFolder(const char* path) const noexcept override { return true; }
    [[nodiscard]] bool deleteFolder(const wchar_t* basePath, const wchar_t* subPath) const noexcept override { return true; }
    [[nodiscard]] bool deleteFolder(const char* basePath, const char* subPath) const noexcept override { return true; }

    [[nodiscard]] bool deleteFile(const wchar_t* path) const noexcept override;
    [[nodiscard]] u64 creationTime(const wchar_t* path) const noexcept override;
    [[nodiscard]] u64 modifyTime(const wchar_t* path) const noexcept override;
};
