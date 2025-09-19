/**
 * @file
 *
 * Describes an abstract file handle.
 */
#pragma once

#include <Safeties.hpp>
#include <NumTypes.hpp>
#include <DynArray.hpp>
#include <String.hpp>
#include <TauCOM.hpp>
#include <chrono>

namespace tau {

enum class FileProps
{
    Read = 0,
    WriteNew,
    WriteOverwrite,
    WriteAppend,
    ReadWrite
};

/**
 * An interface used to represent an abstract file handle.
 *
 *   This can be overloaded to support any filesystem, or
 * other medium of holding files such as compressed
 * archives.
 */
class IFile : public com::IUnknown
{
    DEFAULT_CONSTRUCT_PO(IFile);
    DEFAULT_DESTRUCT_VIO(IFile);
    DEFAULT_CM_PO(IFile);
public:
    [[nodiscard]] virtual i64 Size() noexcept = 0;

    [[nodiscard]] virtual bool Exists() noexcept = 0;

    /**
     *   This is safe to use across DLL boundaries, DynStringT uses
     * TauUtilsAllocateNonConst as its allocator, which is imported from
     * TauUtils.
     *
     * @return The name of the file.
     */
    [[nodiscard]] virtual C8DynString Name() noexcept = 0;

    /**
     * Sets the current read/write index pointer of the file.
     *
     *   For resetting the read/write index use {@link IFile::ResetPos() @endlink}
     * as a specific implementation may have changed how the
     * index pointer works.
     */
    virtual void SetPos(const i64 pos) noexcept = 0;

    virtual void AdvancePos(const i64 phase) noexcept = 0;

    virtual i64 ReadBytes(u8* const buffer, const uSys len) noexcept = 0;

    virtual i64 WriteBytes(const u8* buffer, uSys len) noexcept = 0;

    virtual void ResetPos() noexcept
    {
        SetPos(0);
    }

    virtual i64 Read(void* const buffer, const uSys len) noexcept
    {
        return ReadBytes(static_cast<u8*>(buffer), len);
    }

    virtual i64 ReadString(char* const buffer, const uSys len) noexcept
    {
        return ReadBytes(reinterpret_cast<u8*>(buffer), len * sizeof(*buffer));
    }

    virtual i64 ReadString(wchar_t* const buffer, const uSys len) noexcept
    {
        return ReadBytes(reinterpret_cast<u8*>(buffer), len * sizeof(*buffer));
    }

    virtual i64 ReadString(char8_t* const buffer, const uSys len) noexcept
    {
        return ReadBytes(reinterpret_cast<u8*>(buffer), len * sizeof(*buffer));
    }

    virtual i64 ReadString(char16_t* const buffer, const uSys len) noexcept
    {
        return ReadBytes(reinterpret_cast<u8*>(buffer), len * sizeof(*buffer));
    }

    virtual i64 ReadString(char32_t* const buffer, const uSys len) noexcept
    {
        return ReadBytes(reinterpret_cast<u8*>(buffer), len * sizeof(*buffer));
    }

    virtual RefDynArray<u8> ReadFile() noexcept
    {
        const i64 size = Size();
        RefDynArray<u8> arr(static_cast<uSys>(size));
        ReadBytes(arr.arr(), size);
        return arr;
    }

    virtual int ReadChar() noexcept
    {
        char ret;
        const i64 cnt = ReadBytes(reinterpret_cast<u8*>(&ret), sizeof(ret));
        return cnt ? static_cast<int>(ret) : -1;
    }

    virtual int ReadWChar() noexcept
    {
        wchar_t ret;
        const i64 cnt = ReadBytes(reinterpret_cast<u8*>(&ret), sizeof(ret));
        return cnt ? static_cast<int>(ret) : -1;
    }

    virtual int ReadChar8() noexcept
    {
        char8_t ret;
        const i64 cnt = ReadBytes(reinterpret_cast<u8*>(&ret), sizeof(ret));
        return cnt ? static_cast<int>(ret) : -1;
    }

    virtual int ReadChar16() noexcept
    {
        char16_t ret;
        const i64 cnt = ReadBytes(reinterpret_cast<u8*>(&ret), sizeof(ret));
        return cnt ? static_cast<int>(ret) : -1;
    }

    virtual int ReadChar32() noexcept
    {
        char32_t ret;
        const i64 cnt = ReadBytes(reinterpret_cast<u8*>(&ret), sizeof(ret));
        return cnt ? static_cast<int>(ret) : -1;
    }

    virtual i64 Write(const void* const buffer, const uSys len) noexcept
    {
        return WriteBytes(static_cast<const u8*>(buffer), len);
    }

    virtual i64 WriteString(const char* const str) noexcept
    {
        return WriteBytes(reinterpret_cast<const u8*>(str), ::std::strlen(str) * sizeof(*str));
    }

    virtual i64 WriteString(const wchar_t* const str) noexcept
    {
        return WriteBytes(reinterpret_cast<const u8*>(str), ::std::wcslen(str) * sizeof(*str));
    }

    virtual i64 WriteString(const char8_t* const str) noexcept
    {
        return WriteBytes(reinterpret_cast<const u8*>(str), strLength(str) * sizeof(*str));
    }

    virtual i64 WriteString(const char16_t* const str) noexcept
    {
        return WriteBytes(reinterpret_cast<const u8*>(str), strLength(str) * sizeof(*str));
    }

    virtual i64 WriteString(const char32_t* const str) noexcept
    {
        return WriteBytes(reinterpret_cast<const u8*>(str), strLength(str) * sizeof(*str));
    }

    template<typename _T>
    i64 ReadType(_T* const t) noexcept
    {
        return ReadBytes(reinterpret_cast<u8*>(t), sizeof(*t));
    }

    template<typename _T>
    i64 WriteType(const _T& t) noexcept
    {
        return WriteBytes(reinterpret_cast<const u8*>(&t), sizeof(*t));
    }
};

/**
 * An interface used to load {@link IFile @endlink}'s.
 *
 *   Any implementation of {@link IFile @endlink} should implement
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

    [[nodiscard]] virtual IFile* Load(const C8DynString& path, FileProps props) const noexcept = 0;

    [[nodiscard]] virtual bool CreateFolders(const C8DynString& path) const noexcept = 0;

    [[nodiscard]] virtual bool Delete(const C8DynString& path) const noexcept = 0;
    
    [[nodiscard]] virtual ::std::chrono::time_point<::std::chrono::utc_clock> CreationTime(const C8DynString& path) const noexcept = 0;

    [[nodiscard]] virtual ::std::chrono::time_point<::std::chrono::utc_clock> ModifyTime(const C8DynString& path) const noexcept = 0;
};

}

TAU_DECL_UUID(tau::IFile, 0x41C9E07BFF04414Cull, 0x93201E3C44C15661ull);
TAU_DECL_UUID(tau::IFileLoader, 0xDC11BD20E41544ABull, 0xA6F1E040DFEA5506ull);
// TAU_DECL_UUID(, 0x38F76EBDCD9A4030ull, 0xB36630CCBC9CFB2Full);

// using tau::FileProps;
// using tau::IFile;
// using tau::IFileLoader;
