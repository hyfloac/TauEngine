/**
 * @file
 */
#pragma once

// #include "pch.h"

#pragma warning(push, 0)
#include <utility>
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
    DEFAULT_COPY(VFS);
public:
    static VFS& Instance() noexcept;

    struct Container final
    {
        DEFAULT_DESTRUCT(Container);
        DEFAULT_COPY(Container);
    public:
        DynString path;
        CPPRef<IFileLoader> fileLoader;
        bool canCreateFile;
        bool canWriteFile;

        Container(const DynString& path, const CPPRef<IFileLoader>& fileLoader, const bool canCreateFile, const bool canWriteFile)
            : path(path)
            , fileLoader(fileLoader)
            , canCreateFile(canCreateFile)
            , canWriteFile(canWriteFile)
        { }

        static Container Dynamic(const DynString& path, const CPPRef<IFileLoader>& fileLoader)
        { return Container(path, fileLoader, true, true); }

        static Container Static(const DynString& path, const  CPPRef<IFileLoader>& fileLoader)
        { return Container(path, fileLoader, false, false); }

        [[nodiscard]] bool canCreateAndWriteFile() const noexcept { return canCreateFile && canWriteFile; }
    };

    using MountMap = std::unordered_multimap<DynString, Container>;
private:
    CPPRef<IFileLoader> _defaultLoader;
    MountMap _mountPoints;
public:
    explicit VFS(const CPPRef<IFileLoader>& defaultLoader) noexcept
        : _defaultLoader(defaultLoader)
    { }

    void mount(const DynString& mountPoint, const DynString& path, const CPPRef<IFileLoader>& loader, bool canCreateFile, bool canWriteFile);
    
    void mountDynamic(const DynString& mountPoint, const DynString& path, const CPPRef<IFileLoader>& loader)
    { mount(mountPoint, path, loader, true, true); }

    void mountStatic(const DynString& mountPoint, const DynString& path, const CPPRef<IFileLoader>& loader)
    { mount(mountPoint, path, loader, false, false); }

    void unmount(const DynString& mountPoint) noexcept;

    /**
     * Converts the relative path to a physical path.
     *
     *   If the path does not start with a pipe '|' then this
     * will simply fall back to returning the path. The pipe
     * was chosen as it is generally an illegal character to
     * show up in a path, especially at the beginning.
     */
    Container resolvePath(const char* path) const noexcept;

    Container resolvePath(const char* path, const char* subPath0) const noexcept;
    Container resolvePath(const char* path, const char* subPath0, const char* subPath1) const noexcept;
    Container resolvePath(const char* path, const char* subPath0, const char* subPath1, const char* subPath2) const noexcept;

    static DynString win32Path(const DynString& path) noexcept;
    static DynString unixPath(const DynString& path) noexcept;

    static DynStringView getFileName(const DynString& path) noexcept;
    static DynStringView getFileName(const DynStringView& path) noexcept;

    static DynStringView getFileExt(const DynString& path, bool includeDot = true) noexcept;
    static DynStringView getFileExt(const DynStringView& path, bool includeDot = true) noexcept;

    static DynStringView getParentFolder(const DynString& path, bool includePathSeparator = true) noexcept;
    static DynStringView getParentFolder(const DynStringView& path, bool includePathSeparator = true) noexcept;

    static DynString getVFSMount(const DynString& path, bool includePathSeparator = true) noexcept;
    static DynString getVFSMount(const DynStringView& path, bool includePathSeparator = true) noexcept;

    bool fileExists(const char* path) const noexcept;

    CPPRef<IFile> openFile(const char* path, FileProps props) const noexcept;
};
