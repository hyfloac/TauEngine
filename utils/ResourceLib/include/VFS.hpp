/**
 * @file
 */
#pragma once

// #include "pch.h"

#include <utility>
#include <unordered_map>
#include "IFile.hpp"
#include <String.hpp>

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
public:
    static VFS& Instance() noexcept;
private:
    using Container = std::pair<DynString, Ref<IFileLoader>>;
    using MountMap = std::unordered_multimap<DynString, Container>;

    Ref<IFileLoader> _defaultLoader;
    MountMap _mountPoints;
public:
    explicit VFS(Ref<IFileLoader> defaultLoader) noexcept
        : _defaultLoader(std::move(defaultLoader)), _mountPoints()
    { }

    ~VFS() noexcept = default;

    VFS(const VFS& copy) noexcept = default;
    VFS(VFS&& move) noexcept = default;

    VFS& operator=(const VFS& copy) noexcept = default;
    VFS& operator=(VFS&& move) noexcept = default;

    void mount(const DynString& mountPoint, const DynString& path, const Ref<IFileLoader>& loader) noexcept;

    void unmount(const DynString& mountPoint) noexcept;

    VFS::Container resolvePath(const char* path) const noexcept;

    Ref<IFile> openFile(const char* path) const noexcept;
};
