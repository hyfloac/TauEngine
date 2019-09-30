#include "pch.h"

#include "VFS.hpp"

VFS& VFS::Instance() noexcept
{
    static VFS instance;
    return instance;
}

void VFS::mount(String mountPoint, const char* path) noexcept
{
    _mountPoints.insert(MountMap::value_type(mountPoint, path));
}


void VFS::unmount(String mountPoint) noexcept
{
    _mountPoints.erase(mountPoint);
}

Ref<IFile> VFS::openFile(const char* path) noexcept
{
}
