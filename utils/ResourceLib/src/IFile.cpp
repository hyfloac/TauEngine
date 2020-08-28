#include "IFile.hpp"
#include <String.hpp>

bool IFileLoader::fileExists(const char* const path) const noexcept
{
    const DynString strPath(path);
    return fileExists(StringCast<wchar_t>(strPath));
}

bool IFileLoader::fileExists(const wchar_t* const basePath, const wchar_t* const subPath) const noexcept
{
    const WDynString strPath = WDynString(basePath) + subPath;
    return fileExists(strPath);
}

bool IFileLoader::fileExists(const char* const basePath, const char* const subPath) const noexcept
{
    const DynString strPath = DynString(basePath) + subPath;
    return fileExists(StringCast<wchar_t>(strPath));
}

CPPRef<IFile> IFileLoader::load(const char* const path, const FileProps props) const noexcept
{
    const DynString strPath(path);
    return load(StringCast<wchar_t>(strPath), props);
}

CPPRef<IFile> IFileLoader::load(const wchar_t* const basePath, const wchar_t* const subPath, const FileProps props) const noexcept
{
    const WDynString strPath = WDynString(basePath) + subPath;
    return load(strPath, props);
}

CPPRef<IFile> IFileLoader::load(const char* const basePath, const char* const subPath, const FileProps props) const noexcept
{
    const DynString strPath = DynString(basePath) + subPath;
    return load(StringCast<wchar_t>(strPath), props);
}

bool IFileLoader::createFolder(const char* const path) const noexcept
{
    const DynString strPath(path);
    return createFolder(StringCast<wchar_t>(strPath));
}

bool IFileLoader::createFolder(const wchar_t* const basePath, const wchar_t* const subPath) const noexcept
{
    const WDynString strPath = WDynString(basePath) + subPath;
    return createFolder(strPath);
}

bool IFileLoader::createFolder(const char* const basePath, const char* const subPath) const noexcept
{
    const DynString strPath = DynString(basePath) + subPath;
    return createFolder(StringCast<wchar_t>(strPath));
}

bool IFileLoader::createFolders(const char* const path) const noexcept
{
    const DynString strPath(path);
    return createFolders(StringCast<wchar_t>(strPath));
}

bool IFileLoader::createFolders(const wchar_t* const basePath, const wchar_t* const subPath) const noexcept
{
    const WDynString strPath = WDynString(basePath) + subPath;
    return createFolders(strPath);
}

bool IFileLoader::createFolders(const char* const basePath, const char* const subPath) const noexcept
{
    const DynString strPath = DynString(basePath) + subPath;
    return createFolders(StringCast<wchar_t>(strPath));
}

bool IFileLoader::deleteFolder(const char* const path) const noexcept
{
    const DynString strPath(path);
    return deleteFolder(StringCast<wchar_t>(strPath));
}

bool IFileLoader::deleteFolder(const wchar_t* const basePath, const wchar_t* const subPath) const noexcept
{
    const WDynString strPath = WDynString(basePath) + subPath;
    return deleteFolder(strPath);
}

bool IFileLoader::deleteFolder(const char* const basePath, const char* const subPath) const noexcept
{
    const DynString strPath = DynString(basePath) + subPath;
    return deleteFolder(StringCast<wchar_t>(strPath));
}

bool IFileLoader::deleteFile(const char* const path) const noexcept
{
    const DynString strPath(path);
    return deleteFile(StringCast<wchar_t>(strPath));
}

bool IFileLoader::deleteFile(const wchar_t* const basePath, const wchar_t* const subPath) const noexcept
{
    const WDynString strPath = WDynString(basePath) + subPath;
    return deleteFile(strPath);
}

bool IFileLoader::deleteFile(const char* const basePath, const char* const subPath) const noexcept
{
    const DynString strPath = DynString(basePath) + subPath;
    return deleteFile(StringCast<wchar_t>(strPath));
}

u64 IFileLoader::creationTime(const char* const path) const noexcept
{
    const DynString strPath(path);
    return creationTime(StringCast<wchar_t>(strPath));
}

u64 IFileLoader::creationTime(const wchar_t* const basePath, const wchar_t* const subPath) const noexcept
{
    const WDynString strPath = WDynString(basePath) + subPath;
    return creationTime(strPath);
}

u64 IFileLoader::creationTime(const char* const basePath, const char* const subPath) const noexcept
{
    const DynString strPath = DynString(basePath) + subPath;
    return creationTime(StringCast<wchar_t>(strPath));
}

u64 IFileLoader::modifyTime(const char* const path) const noexcept
{
    const DynString strPath(path);
    return modifyTime(StringCast<wchar_t>(strPath));
}

u64 IFileLoader::modifyTime(const wchar_t* const basePath, const wchar_t* const subPath) const noexcept
{
    const WDynString strPath = WDynString(basePath) + subPath;
    return modifyTime(strPath);
}

u64 IFileLoader::modifyTime(const char* const basePath, const char* const subPath) const noexcept
{
    const DynString strPath = DynString(basePath) + subPath;
    return modifyTime(StringCast<wchar_t>(strPath));
}
