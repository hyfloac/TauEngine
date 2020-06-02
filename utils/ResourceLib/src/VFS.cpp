// #include "pch.h"

#include "VFS.hpp"
#include "Win32File.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

VFS& VFS::Instance() noexcept
{
    static VFS instance(Win32FileLoader::Instance());
    return instance;
}

void VFS::mount(const DynString& mountPoint, const DynString& path, const CPPRef<IFileLoader>& loader, bool canCreateFile, bool canWriteFile)
{
#ifdef _WIN32
    CHAR buffer[MAX_PATH];
    GetFullPathNameA(path.c_str(), MAX_PATH, buffer, NULL);
    const DynString absolutePath(buffer);
#endif

    _mountPoints.insert(MountMap::value_type(mountPoint, VFS::Container(absolutePath, loader, canCreateFile, canWriteFile)));
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

DynString VFS::win32Path(const DynString& path) noexcept
{
    char* cPath = new(::std::nothrow) char[path.length() + 1];
    ::std::memcpy(cPath, path.c_str(), path.length() + 1);

    for(uSys i = 0; i < path.length(); ++i)
    {
        if(cPath[i] == '/')
        {
            cPath[i] = '\\';
        }
    }

    return DynString::passControl(cPath);
}

DynString VFS::unixPath(const DynString& path) noexcept
{
    char* cPath = new(::std::nothrow) char[path.length() + 1];
    ::std::memcpy(cPath, path.c_str(), path.length() + 1);

    for(uSys i = 0; i < path.length(); ++i)
    {
        if(cPath[i] == '\\')
        {
            cPath[i] = '/';
        }
    }

    return DynString::passControl(cPath);
}

DynStringView VFS::getFileName(const DynString& path) noexcept
{
    uSys i = path.length() - 1;
    if(path[i] == '/' || path[i] == '\\')
    { return DynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[i] == '/' || path[i] == '\\')
        {
            --i;
            break;
        }
    }

    return DynStringView(path, i, path.length());
}

DynStringView VFS::getFileName(const DynStringView& path) noexcept
{
    uSys i = path.length() - 1;
    if(path[i] == '/' || path[i] == '\\')
    { return DynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[i] == '/' || path[i] == '\\')
        {
            --i;
            break;
        }
    }

    return DynStringView(path, i, path.length());
}

DynStringView VFS::getFileExt(const DynString& path, const bool includeDot) noexcept
{
    uSys i = path.length() - 1;
    if(path[i] == '/' || path[i] == '\\')
    { return DynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[i] == '/' || path[i] == '\\')
        {
            --i;
            break;
        }
        if(path[i] == '.')
        {
            if(!includeDot)
            {
                --i;
            }
            break;
        }
    }

    return DynStringView(path, i, path.length());
}

DynStringView VFS::getFileExt(const DynStringView& path, const bool includeDot) noexcept
{
    uSys i = path.length() - 1;
    if(path[i] == '/' || path[i] == '\\')
    { return DynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[i] == '/' || path[i] == '\\')
        {
            --i;
            break;
        }
        if(path[i] == '.')
        {
            if(!includeDot)
            {
                --i;
            }
            break;
        }
    }

    return DynStringView(path, i, path.length());
}

DynStringView VFS::getParentFolder(const DynString& path, bool includePathSeparator) noexcept
{
    uSys i = path.length() - 1;
    if(path[i] == '/' || path[i] == '\\')
    { --i; }

    for(; i >= 0; --i)
    {
        if(path[i] == '/' || path[i] == '\\')
        {
            if(!includePathSeparator)
            {
                --i;
            }
            break;
        }
    }

    return DynStringView(path, 0, i);
}

DynStringView VFS::getParentFolder(const DynStringView& path, bool includePathSeparator) noexcept
{
    uSys i = path.length() - 1;
    if(path[i] == '/' || path[i] == '\\')
    { --i; }

    for(; i >= 0; --i)
    {
        if(path[i] == '/' || path[i] == '\\')
        {
            if(!includePathSeparator)
            {
                --i;
            }
            break;
        }
    }

    return DynStringView(path, 0, i);
}

DynString VFS::getVFSMount(const DynString& path, bool includePathSeparator) noexcept
{
    if(path[0] != '|')
    {
        return "";
    }

    uSys i = 0;
    for(; i < path.length(); ++i)
    {
        if(path[i] == '/' || path[i] == '\\')
        {
            if(!includePathSeparator)
            {
                --i;
            }
            break;
        }
    }

    return path.subString(0, i + 1);
}

DynString VFS::getVFSMount(const DynStringView& path, bool includePathSeparator) noexcept
{
    if(path[0] != '|')
    {
        return "";
    }

    uSys i = 0;
    for(; i < path.length(); ++i)
    {
        if(path[i] == '/' || path[i] == '\\')
        {
            if(!includePathSeparator)
            {
                --i;
            }
            break;
        }
    }

    return path.subString(0, i + 1);
}

bool VFS::fileExists(const char* path) const noexcept
{
    const Container c = resolvePath(path);
    if(!c.fileLoader) { return false; }
    return c.fileLoader->fileExists(c.path);
}

CPPRef<IFile> VFS::openFile(const char* path, FileProps props) const noexcept
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
