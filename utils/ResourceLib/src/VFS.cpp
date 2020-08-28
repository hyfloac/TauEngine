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

void VFS::mount(const WDynString& mountPoint, const WDynString& path, const CPPRef<IFileLoader>& loader, bool canCreateFile, bool canWriteFile) noexcept
{
    _mountPoints.insert(MountMap::value_type(mountPoint, VFS::Container(win32PathSanitizer(path), WDynString(), loader, canCreateFile, canWriteFile)));
}

void VFS::mount(WDynString&& mountPoint, const WDynString& path, const CPPRef<IFileLoader>& loader, bool canCreateFile, bool canWriteFile) noexcept
{
    _mountPoints.insert(MountMap::value_type(::std::move(mountPoint), VFS::Container(win32PathSanitizer(path), WDynString(), loader, canCreateFile, canWriteFile)));
}

void VFS::unmount(const WDynString& mountPoint) noexcept
{
    _mountPoints.erase(mountPoint);
}

struct PathPair final
{
    WDynString mountPoint;
    WDynString path;
};

static PathPair split(const wchar_t* str, const wchar_t separator, const bool keepSeparator) noexcept
{
    const wchar_t* const begin = str;

    while(*str)
    {
        if(*str == separator)
        {
            const ::std::size_t len = str - begin + 1;
            wchar_t* const copy = new wchar_t[len];
            ::std::memcpy(copy, begin, len - 1);
            copy[len - 1] = 0;
            WDynString first = WDynString::passControl(copy);

            if(!keepSeparator)
            {
                ++str;
                if(!(*str))
                {
                    return { ::std::move(first), WDynString() };
                }
            }
            return { ::std::move(first), WDynString(str) };
        }

        ++str;
    }

    return { begin, WDynString() };
}

VFS::Container VFS::resolvePath(const wchar_t* path) const noexcept
{
    if(!path) 
    { return VFS::Container::Static({ }, { }, null); }

    const ::std::size_t len = std::wcslen(path);
    if(!len) 
    { return VFS::Container::Static({ }, { }, null); }

    if(path[0] != '|')
    {
        return VFS::Container::Dynamic({ }, path, _defaultLoader);
    }

    const auto splitStr = split(path + 1, L'/', true);

    if(splitStr.path.length() == 0)
    {
        return VFS::Container::Static({ }, { }, null);
    }

    for(auto it = _mountPoints.find(splitStr.mountPoint); it != _mountPoints.end(); ++it)
    {
        const VFS::Container cont = it->second;
        if(cont.fileLoader->fileExists(cont.basePath, splitStr.path))
        {
            return { cont.basePath, splitStr.path, cont.fileLoader, cont.canCreateFile, cont.canWriteFile };
        }
        else if(cont.canCreateFile)
        {
            return { cont.basePath, splitStr.path, cont.fileLoader, cont.canCreateFile, cont.canWriteFile };
        }
    }

    return VFS::Container::Static({ }, { }, null);
}

VFS::Container VFS::resolvePath(const char* path) const noexcept
{
    if(!path)
    { return VFS::Container::Static({ }, { }, null); }

    const WDynString wPath = StringCast<wchar_t>(DynString(path));
    if(!wPath.length())
    { return VFS::Container::Static({ }, { }, null); }

    if(path[0] != '|')
    {
        return VFS::Container::Dynamic({ }, wPath, _defaultLoader);
    }

    const auto splitStr = split(wPath.c_str() + 1, L'/', true);

    if(splitStr.path.length() == 0)
    {
        return VFS::Container::Static({ }, { }, null);
    }

    for(auto it = _mountPoints.find(splitStr.mountPoint); it != _mountPoints.end(); ++it)
    {
        const VFS::Container cont = it->second;
        if(cont.fileLoader->fileExists(cont.basePath, splitStr.path))
        {
            return { cont.basePath, splitStr.path, cont.fileLoader, cont.canCreateFile, cont.canWriteFile };
        }
        else if(cont.canCreateFile)
        {
            return { cont.basePath, splitStr.path, cont.fileLoader, cont.canCreateFile, cont.canWriteFile };
        }
    }

    return VFS::Container::Static({ }, { }, null);
}

VFS::Container VFS::resolvePath(const wchar_t* path, const wchar_t* subPath0) const noexcept
{
    if(!path || !subPath0)
    { return VFS::Container::Static({ }, { }, null); }

    const ::std::size_t len = ::std::wcslen(path);
    if(!len)
    { return VFS::Container::Static({ }, { }, null); }

    WDynString pathCompound(path);
    pathCompound = pathCompound.concat(subPath0);

    return resolvePath(pathCompound);
}

VFS::Container VFS::resolvePath(const wchar_t* path, const wchar_t* subPath0, const wchar_t* subPath1) const noexcept
{
    if(!path || !subPath0 || !subPath1)
    { return VFS::Container::Static({ }, { }, null); }

    const ::std::size_t len = ::std::wcslen(path);
    if(!len)
    { return VFS::Container::Static({ }, { }, null); }

    WDynString pathCompound(path);
    pathCompound = pathCompound.concat(subPath0).concat(subPath1);

    return resolvePath(pathCompound);
}

VFS::Container VFS::resolvePath(const wchar_t* path, const wchar_t* subPath0, const wchar_t* subPath1, const wchar_t* subPath2) const noexcept
{
    if(!path || !subPath0 || !subPath1 || !subPath2)
    { return VFS::Container::Static({ }, { }, null); }

    const ::std::size_t len = ::std::wcslen(path);
    if(!len)
    { return VFS::Container::Static({ }, { }, null); }

    WDynString pathCompound(path);
    pathCompound = pathCompound.concat(subPath0).concat(subPath1).concat(subPath2);

    return resolvePath(pathCompound);
}

VFS::Container VFS::resolvePath(const char* path, const char* subPath0) const noexcept
{
    if(!path || !subPath0) 
    { return VFS::Container::Static({ }, { }, null); }

    const ::std::size_t len = ::std::strlen(path);
    if(!len) 
    { return VFS::Container::Static({ }, { }, null); }

    DynString pathCompound(path);
    pathCompound = pathCompound.concat(subPath0);

    return resolvePath(StringCast<wchar_t>(pathCompound));
}

VFS::Container VFS::resolvePath(const char* path, const char* subPath0, const char* subPath1) const noexcept
{
    if(!path || !subPath0 || !subPath1) 
    { return VFS::Container::Static({ }, { }, null); }

    const ::std::size_t len = ::std::strlen(path);
    if(!len) 
    { return VFS::Container::Static({ }, { }, null); }

    DynString pathCompound(path);
    pathCompound = pathCompound.concat(subPath0).concat(subPath1);

    return resolvePath(StringCast<wchar_t>(pathCompound));
}

VFS::Container VFS::resolvePath(const char* path, const char* subPath0, const char* subPath1, const char* subPath2) const noexcept
{
    if(!path || !subPath0 || !subPath1 || !subPath2) 
    { return VFS::Container::Static({ }, { }, null); }

    const ::std::size_t len = ::std::strlen(path);
    if(!len) 
    { return VFS::Container::Static({ }, { }, null); }

    DynString pathCompound(path);
    pathCompound = pathCompound.concat(subPath0).concat(subPath1).concat(subPath2);

    return resolvePath(StringCast<wchar_t>(pathCompound));
}

WDynString VFS::win32Path(const WDynString& path) noexcept
{
    wchar_t* cPath = new(::std::nothrow) wchar_t[path.length() + 1];
    ::std::memcpy(cPath, path.c_str(), (path.length() + 1) * sizeof(wchar_t));

    for(uSys i = 0; i < path.length(); ++i)
    {
        if(cPath[i] == L'/')
        {
            cPath[i] = L'\\';
        }
    }

    return WDynString::passControl(cPath);
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

WDynString VFS::unixPath(const WDynString& path) noexcept
{
    wchar_t* cPath = new(::std::nothrow) wchar_t[path.length() + 1];
    ::std::memcpy(cPath, path.c_str(), (path.length() + 1) * sizeof(wchar_t));

    for(uSys i = 0; i < path.length(); ++i)
    {
        if(cPath[i] == L'\\')
        {
            cPath[i] = L'/';
        }
    }

    return WDynString::passControl(cPath);
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

static bool containsWin32Device(const uSys length, const wchar_t* const path) noexcept
{
    bool possibleDeviceEnd = true;

    for(iSys i = length - 1; i >= 0; --i)
    {
        // Devices are valid only if they are at the end of a path.
        if(path[i] == L'\\')
        {
            return false;
        }

        if(possibleDeviceEnd)
        {
            /*
             *   Technically devices don't have to end with [1-9], they can
             * end in an wide character digit, including U+00B2, U+00B3,
             * U+00B9, or Superscript 2, Superscript 3, Superscript 1
             * respectively.
             *
             * Source: https://googleprojectzero.blogspot.com/2016/02/the-definitive-guide-on-win32-to-nt.html
             *
             *   Devices can also technically be multiple characters if you
             * have an expansion card. Should this be the case you should
             * prefix the path with `\\.\`, but we'll just reject it either
             * way to be safe. 
             */ 
            while(i >= 0 && iswdigit(path[i])) // LPT[1-9], COM[1-9]
            {
                --i;
            }

            if(i < 3) // Not enough characters for a device.
            {
                return false;
            }

            switch(path[i])
            {
                case L'n':
                case L'N': // PRN, CON
                {
                    --i;
                    switch(path[i])
                    {
                        case L'r':
                        case L'R': // PRN
                        {
                            --i;
                            if(path[i] == L'P' || path[i] == L'p')
                            {
                                if(i == 0 || path[i - 1] == L'\\')
                                {
                                    return true;
                                }
                                --i;
                            }
                            break;
                        }
                        case L'o':
                        case L'O': // CON
                        {
                            --i;
                            if(path[i] == L'C' || path[i] == L'c')
                            {
                                if(i == 0 || path[i - 1] == L'\\')
                                {
                                    return true;
                                }
                                --i;
                            }
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                case L'x':
                case L'X': // AUX
                {
                    --i;
                    if(path[i] == L'U' || path[i] == L'u')
                    {
                        --i;
                        if(path[i] == L'A' || path[i] == L'a')
                        {
                            if(i == 0 || path[i - 1] == L'\\')
                            {
                                return true;
                            }
                            --i;
                        }
                    }
                    break;
                }
                case L'l':
                case L'L': // NUL
                {
                    --i;
                    if(path[i] == L'U' || path[i] == L'u')
                    {
                        --i;
                        if(path[i] == L'N' || path[i] == L'n')
                        {
                            if(i == 0 || path[i - 1] == L'\\')
                            {
                                return true;
                            }
                            --i;
                        }
                    }
                    break;
                }
                case L'$': // CONIN$, CONOUT$
                {
                    if(i < 5)  // Not enough characters for a device.
                    {
                        return false;
                    }

                    --i;
                    switch(path[i])
                    {
                        case L'n':
                        case L'N': // CONIN$
                        {
                            --i;
                            if(path[i] == L'I' || path[i] == L'i')
                            {
                                --i;
                                if(path[i] == L'N' || path[i] == L'n')
                                {
                                    --i;
                                    if(path[i] == L'O' || path[i] == L'o')
                                    {
                                        --i;
                                        if(path[i] == L'C' || path[i] == L'c')
                                        {
                                            if(i == 0 || path[i - 1] == L'\\')
                                            {
                                                return true;
                                            }
                                            --i;
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        case L't':
                        case L'T': // CONOUT$
                        {
                            if(i < 5)  // Not enough characters for a device.
                            {
                                return false;
                            }

                            --i;
                            if(path[i] == L'U' || path[i] == L'u')
                            {
                                --i;
                                if(path[i] == L'O' || path[i] == L'o')
                                {
                                    --i;
                                    if(path[i] == L'N' || path[i] == L'n')
                                    {
                                        --i;
                                        if(path[i] == L'O' || path[i] == L'o')
                                        {
                                            --i;
                                            if(path[i] == L'C' || path[i] == L'c')
                                            {
                                                if(i == 0 || path[i - 1] == L'\\')
                                                {
                                                    return true;
                                                }
                                                --i;
                                            }
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                }
                default:
                    break;
            }
        }

        /**
         *   Technically a device does not have to at the end of a
         * path, they are allowed to have arbitrary characters
         * (except '\') after the device, as long as the these
         * characters are prefixed by a '.' or ':', this delimiter is
         * allowed to be prefixed by a space. For safety I'm choosing
         * to consider all whitespace (' ', '\t', '\r', '\n) a valid
         * space, even if that isn't actually true.
         *
         * Example:
         *   COM1       - Valid
         *   COM1a      - Invalid
         *   COM1\      - Invalid
         *   COM1 a     - Invalid
         *   COM1:a     - valid
         *   COM1  :  a - valid
         *   COM1.  a   - valid
         */
        switch(path[i])
        {
            case L' ':
            case L'\t':
            case L'\r':
            case L'\n':
            case L'.':
            case L':':
                possibleDeviceEnd = true;
                break;
            default:
                possibleDeviceEnd = false;
                break;
        }
    }

    return false;
}

static bool ensureWin32DriveIsLetter(const uSys length, const wchar_t* const path) noexcept
{
    if(length < 2) // To short to be a Drive Absolute or Drive Relative path
    {
        return true;
    }

    if(path[1] == L':') // Is a Drive Absolute or Drive Relative path
    {
        /**
         *   Drive letter is not a drive letter, probably not an issue,
         * but it's better to be safe than sorry. There is some minor
         * potential that this could alias to DosDevices object
         * directory.
         */
        if(iswalpha(path[0]))
        {
            return true;
        }

        return false;
    }

    return true;
}

WDynString VFS::win32PathSanitizer(const WDynString& path) noexcept
{
    wchar_t* const cPath = new(::std::nothrow) wchar_t[path.length() + 1];
    ::std::memcpy(cPath, path.c_str(), path.length() + 1);

    for(uSys i = 0; i < path.length(); ++i)
    {
        if(cPath[i] == '/')
        {
            cPath[i] = '\\';
        }
    }

    if(path.length() >= 2)
    {
        if(cPath[0] == '\\' && cPath[1] == '\\') // UNC Path, Local Device Path, or Root Local Device Path
        {
            delete[] cPath;
            return WDynString();
        }

        if(path.length() >= 4)
        {
            if(cPath[0] == '\\' && cPath[1] == '?' && cPath[2] == '?' && cPath[3] == '\\') // Root local device bypass
            {
                delete[] cPath;
                return WDynString();
            }
        }
    }

    if(containsWin32Device(path.length(), cPath))
    {
        delete[] cPath;
        return WDynString();
    }

    if(!ensureWin32DriveIsLetter(path.length(), cPath))
    {
        delete[] cPath;
        return WDynString();
    }

    const DWORD pathLen = GetFullPathNameW(cPath, 0, NULL, NULL);
    if(pathLen == 0)
    {
        delete[] cPath;
        return WDynString();
    }

    wchar_t* const fullPath = new(::std::nothrow) wchar_t[pathLen + 4];
    fullPath[0] = L'\\';
    fullPath[1] = L'\\';
    fullPath[2] = L'?';
    fullPath[3] = L'\\';

    if(GetFullPathNameW(cPath, pathLen, fullPath + 4, NULL) != pathLen - 1)
    {
        delete[] cPath;
        delete[] fullPath;
        return WDynString();
    }

    delete[] cPath;

    return WDynString::passControl(fullPath);
}

WDynStringView VFS::getFileName(const WDynString& path) noexcept
{
    uSys i = path.length() - 1;
    if(path[i] == L'/' || path[i] == L'\\')
    { return WDynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[i] == L'/' || path[i] == L'\\')
        {
            --i;
            break;
        }
    }

    return WDynStringView(path, i, path.length());
}

WDynStringView VFS::getFileName(const WDynStringView& path) noexcept
{
    uSys i = path.length() - 1;
    if(path[i] == L'/' || path[i] == L'\\')
    { return WDynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[i] == L'/' || path[i] == L'\\')
        {
            --i;
            break;
        }
    }

    return WDynStringView(path, i, path.length());
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

WDynStringView VFS::getFileExt(const WDynString& path, const bool includeDot) noexcept
{
    uSys i = path.length() - 1;
    if(path[i] == L'/' || path[i] == L'\\')
    { return WDynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[i] == L'/' || path[i] == L'\\')
        {
            --i;
            break;
        }
        if(path[i] == L'.')
        {
            if(!includeDot)
            {
                --i;
            }
            break;
        }
    }

    return WDynStringView(path, i, path.length());
}

WDynStringView VFS::getFileExt(const WDynStringView& path, const bool includeDot) noexcept
{
    uSys i = path.length() - 1;
    if(path[i] == L'/' || path[i] == L'\\')
    { return WDynStringView(path, 0, 0); }

    for(; i >= 0; --i)
    {
        if(path[i] == L'/' || path[i] == L'\\')
        {
            --i;
            break;
        }
        if(path[i] == L'.')
        {
            if(!includeDot)
            {
                --i;
            }
            break;
        }
    }

    return WDynStringView(path, i, path.length());
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

WDynStringView VFS::getParentFolder(const WDynString& path, bool includePathSeparator) noexcept
{
    uSys i = path.length() - 1;
    if(path[i] == L'/' || path[i] == L'\\')
    { --i; }

    for(; i >= 0; --i)
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

    return WDynStringView(path, 0, i);
}

WDynStringView VFS::getParentFolder(const WDynStringView& path, bool includePathSeparator) noexcept
{
    uSys i = path.length() - 1;
    if(path[i] == L'/' || path[i] == L'\\')
    { --i; }

    for(; i >= 0; --i)
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

    return WDynStringView(path, 0, i);
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

WDynString VFS::getVFSMount(const WDynString& path, bool includePathSeparator) noexcept
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

    return path.subString(0, i + 1);
}

WDynString VFS::getVFSMount(const WDynStringView& path, bool includePathSeparator) noexcept
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

    return path.subString(0, i + 1);
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

bool VFS::fileExists(const wchar_t* path) const noexcept
{
    const Container c = resolvePath(path);
    if(!c.fileLoader) { return false; }
    return c.fileLoader->fileExists(c.basePath, c.subPath);
}

bool VFS::fileExists(const char* path) const noexcept
{
    const Container c = resolvePath(path);
    if(!c.fileLoader) { return false; }
    return c.fileLoader->fileExists(c.basePath, c.subPath);
}

CPPRef<IFile> VFS::openFile(const wchar_t* const path, const FileProps props) const noexcept
{
    const VFS::Container physPath = resolvePath(path);

    if(physPath.fileLoader == null || physPath.basePath.length() == 0 || physPath.subPath.length() == 0)
    { return null; }

    if(props != FileProps::Read && !physPath.canCreateAndWriteFile())
    { return null; }

    return physPath.fileLoader->load(physPath.basePath, physPath.subPath, props);
}

CPPRef<IFile> VFS::openFile(const char* const path, const FileProps props) const noexcept
{
    const VFS::Container physPath = resolvePath(path);

    if(physPath.fileLoader == null || physPath.basePath.length() == 0 || physPath.subPath.length() == 0)
    { return null; }

    if(props != FileProps::Read && !physPath.canCreateAndWriteFile())
    { return null; }

    return physPath.fileLoader->load(physPath.basePath, physPath.subPath, props);
}
