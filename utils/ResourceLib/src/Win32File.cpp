#include "pch.h"
#include "Win32File.hpp"

i64 Win32File::size() noexcept
{
    return -1;
}

bool Win32File::exists() noexcept
{
    return false;
}

void Win32File::setPos(u64 pos) noexcept
{
}

i64 Win32File::readBytes(u8* buffer, u64 len) noexcept
{
    return -1;
}

void Win32File::writeBytes(const u8* buffer, u64 len) noexcept
{
}

Ref<IFile> Win32FileLoader::load(const char* path) noexcept
{
    return nullptr;
}
