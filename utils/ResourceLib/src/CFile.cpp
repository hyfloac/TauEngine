// #include "pch.h"
#include "CFile.hpp"
#include "Win32File.hpp"

i64 CFile::size() noexcept
{
    long curPos = ftell(_file);
    if(fseek(_file, 0, SEEK_END))
    { return -2; }

    const i64 size = ftell(_file);
    fseek(_file, curPos, SEEK_SET);

    return size;
}

void CFile::setPos(u64 pos) noexcept
{
    fseek(_file, pos, SEEK_SET);
}

i64 CFile::readBytes(u8* buffer, u64 len) noexcept
{
    if(_props != FileProps::Read || _props != FileProps::ReadWrite)
    { return -1; }
    return fread(buffer, sizeof(u8), len, _file);
}

i64 CFile::writeBytes(const u8* buffer, u64 len) noexcept
{
    if(_props == FileProps::Read)
    { return -1; }
    return fwrite(buffer, sizeof(u8), len, _file);
}

Ref<CFileLoader>& CFileLoader::Instance() noexcept
{
    static Ref<CFileLoader> instance = Ref<CFileLoader>(new CFileLoader);
    return instance;
}


bool CFileLoader::fileExists(const char* path) const noexcept
{
    FILE* file;
    if((file = fopen(path, "r")))
    {
        fclose(file);
        return true;
    }
    return false;
}

Ref<IFile> CFileLoader::load(const char* path, FileProps props) const noexcept
{
    FILE* handle;
    switch(props)
    {
        case FileProps::Read:           handle = fopen(path, "rb"); break;
        case FileProps::WriteNew:       handle = fopen(path, "wb"); break;
        case FileProps::WriteOverwrite: handle = fopen(path, "r+b"); break;
        case FileProps::WriteAppend:    handle = fopen(path, "ab"); break;
        case FileProps::ReadWrite:      handle = fopen(path, "r+b"); break;
        default: return nullptr;
    }

    if(!handle)
    { return nullptr; }

    return Ref<CFile>(new CFile(handle, path, props));
}


bool CFileLoader::createFolder(const char* path) const noexcept
{
    return Win32FileLoader::Instance()->createFolder(path);
}

bool CFileLoader::deleteFolder(const char* path) const noexcept
{
    return Win32FileLoader::Instance()->deleteFolder(path);
}

bool CFileLoader::deleteFile(const char* path) const noexcept
{
    return !remove(path);
}
