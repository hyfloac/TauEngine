// #include "pch.h"
#include "CFile.hpp"

i64 CFile::size() noexcept
{
    if(_file == null)
    { return -1; }

    long curPos = ftell(_file);
    if(fseek(_file, 0, SEEK_END))
    { return -2; }

    const i64 size = ftell(_file);
    fseek(_file, curPos, SEEK_SET);

    return size;
}

void CFile::setPos(u64 pos) noexcept
{
    if(_file == null)
    { return; }

    fseek(_file, pos, SEEK_SET);
}

i64 CFile::readBytes(u8* buffer, u64 len) noexcept
{
    if(_file == null)
    { return -1; }
    return fread(buffer, sizeof(u8), len, _file);
}

i64 CFile::writeBytes(const u8* buffer, u64 len) noexcept
{
    if(_file == null)
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

Ref<IFile> CFileLoader::load(const char* path) const noexcept
{
    FILE* handle = fopen(path, "r+");
    return Ref<CFile>(new CFile(handle, path));
}
