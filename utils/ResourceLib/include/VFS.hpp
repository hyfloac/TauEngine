/**
 * @file
 */
#pragma once

// #include "pch.h"

#pragma warning(push, 0)
#include <unordered_map>
#pragma warning(pop)

#include <String.hpp>

#include "IFile.hpp"

/**
 * Implements a Virtual File System.
 *
 *   A VFS (Virtual File System) lets you open a file in an
 * arbitrary directory and have it open the proper file
 * somewhere on your disk. This is achieved by having mount
 * points which are mapped to physical paths. When you
 * attempt to open a file using one of these mount points
 * it redirects your request to the proper directory.
 *
 *   This is useful for being able to open a file such
 * as `|models/TeaPot.fbx`, where `|models` can point to any
 * number of various directories to search for the model in.
 * In the future this will also be able to support compressed
 * archives when searching for assets.
 */
class VFS final
{
    DEFAULT_DESTRUCT(VFS);
    DEFAULT_CM_PU(VFS);
public:
    static VFS& Instance() noexcept;

    struct Container final
    {
        DEFAULT_DESTRUCT(Container);
        DEFAULT_CM_PU(Container);
    public:
        WDynString basePath;
        WDynString subPath;
        CPPRef<IFileLoader> fileLoader;
        bool canCreateFile;
        bool canWriteFile;

        Container(const WDynString& _basePath, const WDynString& _subPath, const CPPRef<IFileLoader>& _fileLoader, const bool _canCreateFile, const bool _canWriteFile) noexcept
            : basePath(_basePath)
            , subPath(_subPath)
            , fileLoader(_fileLoader)
            , canCreateFile(_canCreateFile)
            , canWriteFile(_canWriteFile)
        { }

        Container(WDynString&& _basePath, const WDynString& _subPath, const CPPRef<IFileLoader> & _fileLoader, const bool _canCreateFile, const bool _canWriteFile) noexcept
            : basePath(::std::move(_basePath))
            , subPath(_subPath)
            , fileLoader(_fileLoader)
            , canCreateFile(_canCreateFile)
            , canWriteFile(_canWriteFile)
        { }

        Container(const WDynString& _basePath, WDynString&& _subPath, const CPPRef<IFileLoader>& _fileLoader, const bool _canCreateFile, const bool _canWriteFile) noexcept
            : basePath(_basePath)
            , subPath(::std::move(_subPath))
            , fileLoader(_fileLoader)
            , canCreateFile(_canCreateFile)
            , canWriteFile(_canWriteFile)
        { }

        Container(WDynString&& _basePath, WDynString&& _subPath, const CPPRef<IFileLoader>& _fileLoader, const bool _canCreateFile, const bool _canWriteFile) noexcept
            : basePath(::std::move(_basePath))
            , subPath(::std::move(_subPath))
            , fileLoader(_fileLoader)
            , canCreateFile(_canCreateFile)
            , canWriteFile(_canWriteFile)
        { }

        static Container Dynamic(const WDynString& basePath, const WDynString& subPath, const CPPRef<IFileLoader>& fileLoader) noexcept
        { return Container(basePath, subPath, fileLoader, true, true); }

        static Container Dynamic(WDynString&& basePath, const WDynString& subPath, const CPPRef<IFileLoader>& fileLoader) noexcept
        { return Container(::std::move(basePath), subPath, fileLoader, true, true); }

        static Container Dynamic(const WDynString& basePath, WDynString&& subPath, const CPPRef<IFileLoader>& fileLoader) noexcept
        { return Container(basePath, ::std::move(subPath), fileLoader, true, true); }

        static Container Dynamic(WDynString&& basePath, WDynString&& subPath, const CPPRef<IFileLoader>& fileLoader) noexcept
        { return Container(::std::move(basePath), ::std::move(subPath), fileLoader, true, true); }

        static Container Static(const WDynString& basePath, const WDynString& subPath, const  CPPRef<IFileLoader>& fileLoader) noexcept
        { return Container(basePath, subPath, fileLoader, false, false); }

        static Container Static(WDynString&& basePath, const WDynString& subPath, const  CPPRef<IFileLoader>& fileLoader) noexcept
        { return Container(::std::move(basePath), subPath, fileLoader, false, false); }

        static Container Static(const WDynString& basePath, WDynString&& subPath, const  CPPRef<IFileLoader>& fileLoader) noexcept
        { return Container(basePath, ::std::move(subPath), fileLoader, false, false); }

        static Container Static(WDynString&& basePath, WDynString&& subPath, const  CPPRef<IFileLoader>& fileLoader) noexcept
        { return Container(::std::move(basePath), ::std::move(subPath), fileLoader, false, false); }

        [[nodiscard]] bool canCreateAndWriteFile() const noexcept { return canCreateFile && canWriteFile; }
    };

    using MountMap = std::unordered_multimap<WDynString, Container>;
private:
    CPPRef<IFileLoader> _defaultLoader;
    MountMap _mountPoints;
public:
    explicit VFS(const CPPRef<IFileLoader>& defaultLoader) noexcept
        : _defaultLoader(defaultLoader)
    { }

    void mount(const WDynString& mountPoint, const WDynString& path, const CPPRef<IFileLoader>& loader, bool canCreateFile, bool canWriteFile) noexcept;
    void mount(WDynString&& mountPoint, const WDynString& path, const CPPRef<IFileLoader>& loader, bool canCreateFile, bool canWriteFile) noexcept;

    void mount(const DynString& mountPoint, const DynString& path, const CPPRef<IFileLoader>& loader, const bool canCreateFile, const bool canWriteFile) noexcept
    { mount(StringCast<wchar_t>(mountPoint), StringCast<wchar_t>(path), loader, canCreateFile, canWriteFile); }
    
    void mountDynamic(const WDynString& mountPoint, const WDynString& path, const CPPRef<IFileLoader>& loader) noexcept
    { mount(mountPoint, path, loader, true, true); }
    
    void mountDynamic(WDynString&& mountPoint, const WDynString& path, const CPPRef<IFileLoader>& loader) noexcept
    { mount(::std::move(mountPoint), path, loader, true, true); }

    void mountStatic(const WDynString& mountPoint, const WDynString& path, const CPPRef<IFileLoader>& loader) noexcept
    { mount(mountPoint, path, loader, false, false); }
    
    void mountStatic(WDynString&& mountPoint, const WDynString& path, const CPPRef<IFileLoader>& loader) noexcept
    { mount(::std::move(mountPoint), path, loader, false, false); }
    
    void mountDynamic(const DynString& mountPoint, const DynString& path, const CPPRef<IFileLoader>& loader) noexcept
    { mount(StringCast<wchar_t>(mountPoint), StringCast<wchar_t>(path), loader, true, true); }

    void mountStatic(const DynString& mountPoint, const DynString& path, const CPPRef<IFileLoader>& loader) noexcept
    { mount(StringCast<wchar_t>(mountPoint), StringCast<wchar_t>(path), loader, false, false); }

    void unmount(const WDynString& mountPoint) noexcept;

    void unmount(const DynString& mountPoint) noexcept
    { unmount(StringCast<wchar_t>(mountPoint)); }

    /**
     * Converts the relative path to a physical path.
     *
     *   If the path does not start with a pipe '|' then this
     * will simply fall back to returning the path. The pipe
     * was chosen as it is generally an illegal character to
     * show up in a path, especially at the beginning.
     */
    Container resolvePath(const wchar_t* path) const noexcept;
    Container resolvePath(const char* path) const noexcept;

    Container resolvePath(const wchar_t* path, const wchar_t* subPath0) const noexcept;
    Container resolvePath(const wchar_t* path, const wchar_t* subPath0, const wchar_t* subPath1) const noexcept;
    Container resolvePath(const wchar_t* path, const wchar_t* subPath0, const wchar_t* subPath1, const wchar_t* subPath2) const noexcept;

    Container resolvePath(const char* path, const char* subPath0) const noexcept;
    Container resolvePath(const char* path, const char* subPath0, const char* subPath1) const noexcept;
    Container resolvePath(const char* path, const char* subPath0, const char* subPath1, const char* subPath2) const noexcept;

    static DynString win32Path(const DynString& path) noexcept;
    static DynString unixPath(const DynString& path) noexcept;

    static WDynString win32PathSanitizer(const WDynString& path) noexcept;

    static WDynStringView getFileName(const WDynString& path) noexcept;
    static WDynStringView getFileName(const WDynStringView& path) noexcept;

    static DynStringView getFileName(const DynString& path) noexcept;
    static DynStringView getFileName(const DynStringView& path) noexcept;

    static WDynStringView getFileExt(const WDynString& path, bool includeDot = true) noexcept;
    static WDynStringView getFileExt(const WDynStringView& path, bool includeDot = true) noexcept;

    static DynStringView getFileExt(const DynString& path, bool includeDot = true) noexcept;
    static DynStringView getFileExt(const DynStringView& path, bool includeDot = true) noexcept;

    static WDynStringView getParentFolder(const WDynString& path, bool includePathSeparator = true) noexcept;
    static WDynStringView getParentFolder(const WDynStringView& path, bool includePathSeparator = true) noexcept;

    static DynStringView getParentFolder(const DynString& path, bool includePathSeparator = true) noexcept;
    static DynStringView getParentFolder(const DynStringView& path, bool includePathSeparator = true) noexcept;

    static WDynString getVFSMount(const WDynString& path, bool includePathSeparator = true) noexcept;
    static WDynString getVFSMount(const WDynStringView& path, bool includePathSeparator = true) noexcept;

    static DynString getVFSMount(const DynString& path, bool includePathSeparator = true) noexcept;
    static DynString getVFSMount(const DynStringView& path, bool includePathSeparator = true) noexcept;

    bool fileExists(const wchar_t* path) const noexcept;
    bool fileExists(const char* path) const noexcept;

    CPPRef<IFile> openFile(const wchar_t* path, FileProps props) const noexcept;
    CPPRef<IFile> openFile(const char* path, FileProps props) const noexcept;
};
