// #include "pch.h"

#include "VFS.hpp"
#include "Win32File.hpp"

VFS& VFS::Instance() noexcept
{
    static VFS instance(Win32FileLoader::Instance());
    return instance;
}

void VFS::mount(const DynString& mountPoint, const DynString& path, const Ref<IFileLoader>& loader, bool canCreateFile, bool canWriteFile)
{
    _mountPoints.insert(MountMap::value_type(mountPoint, VFS::Container(path, loader, canCreateFile, canWriteFile)));
}

void VFS::unmount(const DynString& mountPoint) noexcept
{
    _mountPoints.erase(mountPoint);
}

static std::pair<DynString, DynString> split(const char* str, char separator, bool keepSeparator) noexcept
{
    const char* const begin = str;

    while(*str)
    {
        if(*str == separator)
        {
            const std::size_t len = str - begin + 1;
            char* const copy = new char[len];
            std::memcpy(copy, begin, len - 1);
            copy[len - 1] = 0;
            DynString first(copy);
            delete[] copy;

            if(!keepSeparator)
            {
                ++str;
                if(!(*str))
                {
                    return { first, DynString("") };
                }
            }
            return { first, DynString(str) };
        }

        ++str;
    }

    return { DynString(begin), DynString("") };
}

VFS::Container VFS::resolvePath(const char* path) const noexcept
{
    if(!path) { return VFS::Container::Static("", null); }

    const std::size_t len = std::strlen(path);
    if(!len) { return VFS::Container::Static("", null); }

    if(path[0] != '|')
    {
        // if(_defaultLoader->fileExists(path))
        // {
        //     return VFS::Container::Dynamic(path, _defaultLoader);
        // }
        return VFS::Container::Dynamic(path, _defaultLoader);
    }

    const auto splitStr = split(path + 1, '/', true);

    if(splitStr.second.length() == 0)
    {
        return VFS::Container::Static("", null);
    }

    for(auto it = _mountPoints.find(splitStr.first); it != _mountPoints.end(); ++it)
    {
        const VFS::Container cont = it->second;
        DynString compound = it->second.path.concat(splitStr.second);
        if(cont.fileLoader->fileExists(compound.c_str()))
        {
            return { compound, cont.fileLoader, cont.canCreateFile, cont.canWriteFile };
        }
        else if(cont.canCreateFile)
        {
            return { compound, cont.fileLoader, cont.canCreateFile, cont.canWriteFile };
        }
    }

    return VFS::Container::Static("", null);
}

VFS::Container VFS::resolvePath(const char* path, const char* subPath0) const noexcept
{
    if(!path) { return VFS::Container::Static("", null); }

    const std::size_t len = std::strlen(path);
    if(!len) { return VFS::Container::Static("", null); }

    DynString pathCompound(path);
    pathCompound = pathCompound.concat(subPath0);

    return resolvePath(pathCompound);
}

VFS::Container VFS::resolvePath(const char* path, const char* subPath0, const char* subPath1) const noexcept
{
    if(!path) { return VFS::Container::Static("", null); }

    const std::size_t len = std::strlen(path);
    if(!len) { return VFS::Container::Static("", null); }

    DynString pathCompound(path);
    pathCompound = pathCompound.concat(subPath0).concat(subPath1);

    return resolvePath(pathCompound);
}

VFS::Container VFS::resolvePath(const char* path, const char* subPath0, const char* subPath1, const char* subPath2) const noexcept
{
    if(!path) { return VFS::Container::Static("", null); }

    const std::size_t len = std::strlen(path);
    if(!len) { return VFS::Container::Static("", null); }

    DynString pathCompound(path);
    pathCompound = pathCompound.concat(subPath0).concat(subPath1).concat(subPath2);

    return resolvePath(pathCompound);
}

bool VFS::fileExists(const char* path) const noexcept
{
    const Container c = resolvePath(path);
    if(!c.fileLoader) { return false; }
    return c.fileLoader->fileExists(c.path);
}

Ref<IFile> VFS::openFile(const char* path, FileProps props) const noexcept
{
    const VFS::Container physPath = resolvePath(path);

    if(physPath.fileLoader == null || physPath.path.length() == 0)
    { return null; }

    if(props != FileProps::Read && !physPath.canCreateAndWriteFile())
    {
        return null;
    }

    return physPath.fileLoader->load(physPath.path, props);
}
