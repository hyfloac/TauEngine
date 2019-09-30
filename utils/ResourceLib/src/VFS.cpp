// #include "pch.h"

#include "VFS.hpp"
#include "Win32File.hpp"

VFS& VFS::Instance() noexcept
{
    static VFS instance(Win32FileLoader::Instance());
    return instance;
}

void VFS::mount(const DynString& mountPoint, const DynString& path, const Ref<IFileLoader>& loader) noexcept
{
    _mountPoints.insert(MountMap::value_type(mountPoint, VFS::Container(path, loader)));
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

Ref<IFile> VFS::openFile(const char* path) noexcept
{
    if(!path) { return null; }

    const std::size_t len = std::strlen(path);
    if(!len) { return null; }

    if(path[0] != '|')
    {
        return _defaultLoader->load(path);
    }

    const auto splitStr = split(path + 1, '/', true);

    if(splitStr.second.length() == 0)
    { return null; }

    for(auto it = _mountPoints.find(splitStr.first); it != _mountPoints.end(); ++it)
    {
        const VFS::Container cont = it->second;
        DynString compound = it->second.first.concat(splitStr.second);
        if(cont.second->fileExists(compound.c_str()))
        {
            return cont.second->load(compound.c_str());
        }
    }

    return null;
}
