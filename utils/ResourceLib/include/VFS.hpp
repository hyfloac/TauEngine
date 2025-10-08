/**
 * @file
 */
#pragma once

#pragma warning(push, 0)
#include <unordered_map>
#pragma warning(pop)

#include <String.hpp>

#include "IFile.hpp"

namespace tau {

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
    template<typename T>
    using ComRef = com::ComRef<T>;

    static VFS& Instance() noexcept;

    struct Container final
    {
        DEFAULT_DESTRUCT(Container);
        DEFAULT_CM_PU(Container);
    public:
        C8DynString FilePath;
        ComRef<IFileLoader> FileLoader;
        bool CanCreateFile : 1;
        bool CanWriteFile : 1;

        Container(
            const C8DynString& basePath,
            const ComRef<IFileLoader>& fileLoader,
            const bool canCreateFile,
            const bool canWriteFile
        ) noexcept
            : FilePath(basePath)
            , FileLoader(fileLoader)
            , CanCreateFile(canCreateFile)
            , CanWriteFile(canWriteFile)
        { }

        Container(
            C8DynString&& basePath,
            const ComRef<IFileLoader>& fileLoader,
            const bool canCreateFile,
            const bool canWriteFile
        ) noexcept
            : FilePath(::std::move(basePath))
            , FileLoader(fileLoader)
            , CanCreateFile(canCreateFile)
            , CanWriteFile(canWriteFile)
        { }

        static Container Dynamic(const C8DynString& basePath, const ComRef<IFileLoader>& fileLoader) noexcept
        { return Container(basePath, fileLoader, true, true); }

        static Container Dynamic(C8DynString&& basePath, const ComRef<IFileLoader>& fileLoader) noexcept
        { return Container(::std::move(basePath), fileLoader, true, true); }

        static Container Static(const C8DynString& basePath, const ComRef<IFileLoader>& fileLoader) noexcept
        { return Container(basePath, fileLoader, false, false); }

        static Container Static(C8DynString&& basePath, const ComRef<IFileLoader>& fileLoader) noexcept
        { return Container(::std::move(basePath), fileLoader, false, false); }

        [[nodiscard]] bool CanCreateAndWriteFile() const noexcept { return CanCreateFile && CanWriteFile; }
    };

    using MountMap = std::unordered_multimap<C8DynString, Container>;
public:
    VFS(const tau::com::ComRef<IFileLoader>& defaultLoader) noexcept
        : m_DefaultLoader(defaultLoader)
    { }

    void Mount(const C8DynString& mountPoint, const C8DynString& path, const ComRef<IFileLoader>& loader, bool canCreateFile, bool canWriteFile) noexcept;
    void Mount(C8DynString&& mountPoint, const C8DynString& path, const ComRef<IFileLoader>& loader, bool canCreateFile, bool canWriteFile) noexcept;

    void MountDynamic(const C8DynString& mountPoint, const C8DynString& path, const ComRef<IFileLoader>& loader) noexcept
    { Mount(mountPoint, path, loader, true, true); }
    
    void MountDynamic(C8DynString&& mountPoint, const C8DynString& path, const ComRef<IFileLoader>& loader) noexcept
    { Mount(::std::move(mountPoint), path, loader, true, true); }

    void MountStatic(const C8DynString& mountPoint, const C8DynString& path, const ComRef<IFileLoader>& loader) noexcept
    { Mount(mountPoint, path, loader, false, false); }
    
    void MountStatic(C8DynString&& mountPoint, const C8DynString& path, const ComRef<IFileLoader>& loader) noexcept
    { Mount(::std::move(mountPoint), path, loader, false, false); }

    void Unmount(const C8DynString& mountPoint) noexcept;

    /**
     * Converts the relative path to a physical path.
     *
     *   If the path does not start with a pipe '|' then this
     * will simply fall back to returning the path. The pipe
     * was chosen as it is generally an illegal character to
     * show up in a path, especially at the beginning.
     */
    Container ResolvePath(const C8DynString& path) const noexcept;

    Container ResolvePath(const C8DynString& path, const C8DynString& subPath0) const noexcept;
    Container ResolvePath(const C8DynString& path, const C8DynString& subPath0, const C8DynString& subPath1) const noexcept;
    Container ResolvePath(const C8DynString& path, const C8DynString& subPath0, const C8DynString& subPath1, const C8DynString& subPath2) const noexcept;

    static C8DynString win32Path(const C8DynString& path) noexcept;

    static C8DynString unixPath(const C8DynString& path) noexcept;

    static C8DynStringView getFileName(const C8DynString& path) noexcept;
    static C8DynStringView getFileName(const C8DynStringView& path) noexcept;

    static DynStringView getFileName(const DynString& path) noexcept;
    static DynStringView getFileName(const DynStringView& path) noexcept;

    static C8DynStringView getFileExt(const C8DynString& path, bool includeDot = true) noexcept;
    static C8DynStringView getFileExt(const C8DynStringView& path, bool includeDot = true) noexcept;

    static DynStringView getFileExt(const DynString& path, bool includeDot = true) noexcept;
    static DynStringView getFileExt(const DynStringView& path, bool includeDot = true) noexcept;

    static C8DynStringView getParentFolder(const C8DynString& path, bool includePathSeparator = true) noexcept;
    static C8DynStringView getParentFolder(const C8DynStringView& path, bool includePathSeparator = true) noexcept;

    static DynStringView getParentFolder(const DynString& path, bool includePathSeparator = true) noexcept;
    static DynStringView getParentFolder(const DynStringView& path, bool includePathSeparator = true) noexcept;

    static C8DynString getVFSMount(const C8DynString& path, bool includePathSeparator = true) noexcept;
    static C8DynString getVFSMount(const C8DynStringView& path, bool includePathSeparator = true) noexcept;

    static DynString getVFSMount(const DynString& path, bool includePathSeparator = true) noexcept;
    static DynString getVFSMount(const DynStringView& path, bool includePathSeparator = true) noexcept;

    [[nodiscard]] bool Exists(const C8DynString& path) const noexcept;

    tau::IFileStream* Load(const C8DynString& path, tau::FileProps props) const noexcept;
private:
    ComRef<IFileLoader> m_DefaultLoader;
    MountMap m_MountPoints;
};

}
