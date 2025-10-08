#include "VFS.hpp"
#include "Win32File.hpp"
#include "PathSanitizer.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace tau {

VFS& VFS::Instance() noexcept
{
    static VFS instance(nullptr);
    return instance;
}

void VFS::Mount(
    const C8DynString& mountPoint,
    const C8DynString& path,
    const ComRef<IFileLoader>& loader,
    const bool canCreateFile,
    const bool canWriteFile
) noexcept
{
    m_MountPoints.insert(MountMap::value_type(mountPoint, VFS::Container(PathSanitizer::sanitizePath(path), loader, canCreateFile, canWriteFile)));
}

void VFS::Mount(
    C8DynString&& mountPoint,
    const C8DynString& path,
    const ComRef<IFileLoader>& loader,
    const bool canCreateFile,
    const bool canWriteFile
) noexcept
{
    m_MountPoints.insert(MountMap::value_type(::std::move(mountPoint), VFS::Container(PathSanitizer::sanitizePath(path), loader, canCreateFile, canWriteFile)));
}

void VFS::Unmount(const C8DynString& mountPoint) noexcept
{
    m_MountPoints.erase(mountPoint);
}

struct PathPair final
{
    C8DynStringView MountPoint;
    C8DynString Path;
};

static PathPair Split(const C8DynStringView& str, const char32_t separator, const bool keepSeparator) noexcept
{
    const auto end = str.End();
    for(auto iter = str.Begin(); iter != end; ++iter)
    {
        if(*iter == separator)
        {
            if(!keepSeparator)
            {
                ++iter;
                if(!(*str))
                {
                    return { C8DynStringView(str, iter.Start(), iter.Index()), C8DynString() };
                }
            }
            return { C8DynStringView(str, iter.Start(), iter.Index()), C8DynStringView(str, iter.Index(), str.Length()) };
        }
    }

    return { str, C8DynStringView(str, 0, 0) };
}

VFS::Container VFS::ResolvePath(const C8DynString& path) const noexcept
{
    if(!path)
    {
        return VFS::Container::Static({ }, nullptr);
    }

    if(!path.length())
    {
        return VFS::Container::Static({ }, nullptr);
    }

    if(path[0] != '|')
    {
        return VFS::Container::Dynamic(path, m_DefaultLoader);
    }

    const auto splitStr = Split(C8DynStringView(path, 1, path.Length()), U'/', true);

    if(splitStr.Path.length() == 0)
    {
        return VFS::Container::Static({ }, nullptr);
    }

    for(auto it = m_MountPoints.find(splitStr.MountPoint); it != m_MountPoints.end(); ++it)
    {
        const VFS::Container cont = it->second;
        C8DynString foundPath = cont.FilePath.Concat(u8"/");
        foundPath = foundPath + splitStr.Path;
        if(cont.FileLoader->Exists(foundPath))
        {
            return { foundPath, cont.FileLoader, cont.CanCreateFile, cont.CanWriteFile };
        }
        else if(cont.CanCreateFile)
        {
            return { foundPath, cont.FileLoader, cont.CanCreateFile, cont.CanWriteFile };
        }
    }

    return VFS::Container::Static({ }, nullptr);
}

VFS::Container VFS::ResolvePath(const C8DynString& path, const C8DynString& subPath0) const noexcept
{
    if(!path || !subPath0)
    {
        return VFS::Container::Static({ }, nullptr);
    }

    if(path.Length() == 0)
    {
        return VFS::Container::Static({ }, nullptr);
    }

    return ResolvePath(path.Concat(subPath0));
}

VFS::Container VFS::ResolvePath(const C8DynString& path, const C8DynString& subPath0, const C8DynString& subPath1) const noexcept
{
    if(!path || !subPath0 || !subPath1) 
    {
        return VFS::Container::Static({ }, nullptr);
    }

    if(path.Length() == 0)
    {
        return VFS::Container::Static({ }, nullptr);
    }

    return ResolvePath(path.Concat(subPath0).Concat(subPath1));
}

VFS::Container VFS::ResolvePath(const C8DynString& path, const C8DynString& subPath0, const C8DynString& subPath1, const C8DynString& subPath2) const noexcept
{
    if(!path || !subPath0 || !subPath1 || !subPath2) 
    {
        return VFS::Container::Static({ }, nullptr);
    }

    if(path.Length() == 0)
    {
        return VFS::Container::Static({ }, nullptr);
    }

    return ResolvePath(path.Concat(subPath0).Concat(subPath1).Concat(subPath2));
}

C8DynString VFS::win32Path(const C8DynString& path) noexcept
{
    c8* cPath = new(::std::nothrow) c8[path.length() + 1];
    ::std::memcpy(cPath, path.c_str(), (path.length() + 1) * sizeof(c8));

    for(uSys i = 0; i < path.length(); ++i)
    {
        if(cPath[i] == L'/')
        {
            cPath[i] = L'\\';
        }
    }

    return C8DynString::passControl(cPath);
}

C8DynString VFS::unixPath(const C8DynString& path) noexcept
{
    c8* cPath = new(::std::nothrow) c8[path.length() + 1];
    ::std::memcpy(cPath, path.c_str(), (path.length() + 1) * sizeof(c8));

    for(uSys i = 0; i < path.length(); ++i)
    {
        if(cPath[i] == L'\\')
        {
            cPath[i] = L'/';
        }
    }

    return C8DynString::passControl(cPath);
}

C8DynStringView VFS::getFileName(const C8DynString& path) noexcept
{
    iSys i = static_cast<iSys>(path.length() - 1);
    if(path[static_cast<uSys>(i)] == L'/' || path[static_cast<uSys>(i)] == L'\\')
    { return C8DynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[static_cast<uSys>(i)] == L'/' || path[static_cast<uSys>(i)] == L'\\')
        {
            --i;
            break;
        }
    }

    return C8DynStringView(path, i, path.length());
}

C8DynStringView VFS::getFileName(const C8DynStringView& path) noexcept
{
    iSys i = static_cast<iSys>(path.length() - 1);
    if(path[static_cast<uSys>(i)] == L'/' || path[static_cast<uSys>(i)] == L'\\')
    { return C8DynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[static_cast<uSys>(i)] == L'/' || path[static_cast<uSys>(i)] == L'\\')
        {
            --i;
            break;
        }
    }

    return C8DynStringView(path, i, path.length());
}

DynStringView VFS::getFileName(const DynString& path) noexcept
{
    iSys i = static_cast<iSys>(path.length() - 1);
    if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
    { return DynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
        {
            --i;
            break;
        }
    }

    return DynStringView(path, i, path.length());
}

DynStringView VFS::getFileName(const DynStringView& path) noexcept
{
    iSys i = static_cast<iSys>(path.length() - 1);
    if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
    { return DynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
        {
            --i;
            break;
        }
    }

    return DynStringView(path, i, path.length());
}

C8DynStringView VFS::getFileExt(const C8DynString& path, const bool includeDot) noexcept
{
    iSys i = static_cast<iSys>(path.length() - 1);
    if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
    { return C8DynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
        {
            --i;
            break;
        }
        if(path[static_cast<uSys>(i)] == L'.')
        {
            if(!includeDot)
            {
                --i;
            }
            break;
        }
    }

    return C8DynStringView(path, i, path.length());
}

C8DynStringView VFS::getFileExt(const C8DynStringView& path, const bool includeDot) noexcept
{
    iSys i = static_cast<iSys>(path.length() - 1);
    if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
    { return C8DynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
        {
            --i;
            break;
        }
        if(path[static_cast<uSys>(i)] == L'.')
        {
            if(!includeDot)
            {
                --i;
            }
            break;
        }
    }

    return C8DynStringView(path, i, path.length());
}

DynStringView VFS::getFileExt(const DynString& path, const bool includeDot) noexcept
{
    iSys i = static_cast<iSys>(path.length() - 1);
    if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
    { return DynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
        {
            --i;
            break;
        }
        if(path[static_cast<uSys>(i)] == '.')
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
    iSys i = static_cast<iSys>(path.length() - 1);
    if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
    { return DynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
        {
            --i;
            break;
        }
        if(path[static_cast<uSys>(i)] == '.')
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

C8DynStringView VFS::getParentFolder(const C8DynString& path, bool includePathSeparator) noexcept
{
    iSys i = static_cast<iSys>(path.length() - 1);
    if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
    { --i; }

    for(; i >= 0; --i)
    {
        if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
        {
            if(!includePathSeparator)
            {
                --i;
            }
            break;
        }
    }

    return C8DynStringView(path, 0, i);
}

C8DynStringView VFS::getParentFolder(const C8DynStringView& path, bool includePathSeparator) noexcept
{
    iSys i = static_cast<iSys>(path.length() - 1);
    if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
    { --i; }

    for(; i >= 0; --i)
    {
        if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
        {
            if(!includePathSeparator)
            {
                --i;
            }
            break;
        }
    }

    return C8DynStringView(path, 0, i);
}

DynStringView VFS::getParentFolder(const DynString& path, bool includePathSeparator) noexcept
{
    iSys i = static_cast<iSys>(path.length() - 1);
    if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
    { --i; }

    for(; i >= 0; --i)
    {
        if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
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
    iSys i = static_cast<iSys>(path.length() - 1);
    if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
    { --i; }

    for(; i >= 0; --i)
    {
        if(path[static_cast<uSys>(i)] == '/' || path[static_cast<uSys>(i)] == '\\')
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

C8DynString VFS::getVFSMount(const C8DynString& path, bool includePathSeparator) noexcept
{
    if(path[0] != '|')
    {
        return { };
    }

    uSys i = 0;
    for(; i < path.length(); ++i)
    {
        if(path[i] == L'/' || path[i] == L'\\')
        {
            if(!includePathSeparator)
            {
                --i;
            }
            break;
        }
    }

    return path.SubString(0, i + 1);
}

C8DynString VFS::getVFSMount(const C8DynStringView& path, bool includePathSeparator) noexcept
{
    if(path[0] != '|')
    {
        return { };
    }

    uSys i = 0;
    for(; i < path.length(); ++i)
    {
        if(path[i] == L'/' || path[i] == L'\\')
        {
            if(!includePathSeparator)
            {
                --i;
            }
            break;
        }
    }

    return path.SubString(0, i + 1);
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

    return path.SubString(0, i + 1);
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

    return path.SubString(0, i + 1);
}

bool VFS::Exists(const C8DynString& path) const noexcept
{
    const Container c = ResolvePath(path);

    if(!c.FileLoader)
    {
        return false;
    }

    return c.FileLoader->Exists(c.FilePath);
}

IFileStream* VFS::Load(
    const C8DynString& path,
    const FileProps props
) const noexcept
{
    const VFS::Container physPath = ResolvePath(path);

    if(!physPath.FileLoader || physPath.FilePath.length() == 0)
    {
        return nullptr;
    }

    if(props != FileProps::Read && !physPath.CanCreateAndWriteFile())
    {
        return nullptr;
    }

    return physPath.FileLoader->Load(physPath.FilePath, props);
}

}
