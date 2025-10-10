/**
 * @file
 *
 * Describes an abstract file handle.
 */
#pragma once

#include <NumTypes.hpp>
#include <DynArray.hpp>
#include <String.hpp>
#include <TauCOM.hpp>
#include <chrono>
#include <EnumBitFields.hpp>
#include "IStream.hpp"

namespace tau {

enum class FileProps
{
    Read = 0,
    WriteNew,
    WriteOverwrite,
    WriteAppend,
    ReadWrite
};

enum class FileShare
{
    None = 0,
    Read = 1 << 0,
    Write = 1 << 1,
    ReadWrite = 3,
    Delete = 1 << 2
};

}

ENUM_FLAGS(tau::FileShare);

namespace tau {

/**
 * An interface used to represent an abstract file handle.
 *
 *   This can be overloaded to support any filesystem, or
 * other medium of holding files such as compressed
 * archives.
 */
class IFileStream : public IStream
{
    DEFAULT_CONSTRUCT_PO(IFileStream);
    DEFAULT_DESTRUCT_VIO(IFileStream);
    DEFAULT_CM_PO(IFileStream);
public:
    /**
     *   This is safe to use across DLL boundaries, DynStringT uses
     * TauUtilsAllocateNonConst as its allocator, which is imported from
     * TauUtils.
     *
     * @return The name of the file.
     */
    [[nodiscard]] virtual C8DynString Name() noexcept = 0;
};

/**
 * An interface used to load {@link IFileStream @endlink}'s.
 *
 *   Any implementation of {@link IFileStream @endlink} should implement
 * a corresponding {@link IFileLoader @endlink}.
 */
class IFileLoader : public com::IUnknown
{
    DEFAULT_CONSTRUCT_PU(IFileLoader);
    DEFAULT_DESTRUCT_VI(IFileLoader);
    DEFAULT_CM_PO(IFileLoader);
public:
    [[nodiscard]] virtual bool Exists(const C8DynString& path) const noexcept = 0;
    [[nodiscard]] virtual bool IsFolder(const C8DynString& path) const noexcept = 0;
    [[nodiscard]] virtual bool IsSymlink(const C8DynString& path) const noexcept = 0;

    [[nodiscard]] virtual IFileStream* Load(const C8DynString& path, FileProps props) const noexcept = 0;

    [[nodiscard]] virtual bool CreateFolders(const C8DynString& path) const noexcept = 0;

    [[nodiscard]] virtual bool Delete(const C8DynString& path) const noexcept = 0;
    
    [[nodiscard]] virtual ::std::chrono::time_point<::std::chrono::utc_clock> CreationTime(const C8DynString& path) const noexcept = 0;

    [[nodiscard]] virtual ::std::chrono::time_point<::std::chrono::utc_clock> ModifyTime(const C8DynString& path) const noexcept = 0;
};

}

TAU_DECL_UUID(tau::IFileStream, 0x41C9E07BFF04414Cull, 0x93201E3C44C15661ull);
TAU_DECL_UUID(tau::IFileLoader, 0xDC11BD20E41544ABull, 0xA6F1E040DFEA5506ull);
