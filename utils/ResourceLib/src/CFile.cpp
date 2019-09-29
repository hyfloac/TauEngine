#include "pch.h"
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

void CFile::writeBytes(const u8* buffer, u64 len) noexcept
{
    if(_file == null)
    { return; }
    fwrite(buffer, sizeof(u8), len, _file);
}

Ref<IFile> CFileLoader::load(const char* path) noexcept
{
    FILE* handle = fopen(path, "r+");
    return Ref<CFile>(new CFile(handle, path));
}
