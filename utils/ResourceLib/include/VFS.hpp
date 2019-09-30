/**
 * @file
 */
#pragma once

#include "pch.h"
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
public:
    static VFS& Instance() noexcept;
private:
    using MountMap = std::unordered_multimap<String, const char*>;

    MountMap _mountPoints;
public:
    VFS() noexcept
    { }

    ~VFS() noexcept = default;

    VFS(const VFS& copy) noexcept = default;
    VFS(VFS&& move) noexcept = default;

    VFS& operator=(const VFS& copy) noexcept = default;
    VFS& operator=(VFS&& move) noexcept = default;

    void mount(String mountPoint, const char* path) noexcept;

    void unmount(String mountPoint) noexcept;

    Ref<IFile> openFile(const char* path) noexcept;
};
