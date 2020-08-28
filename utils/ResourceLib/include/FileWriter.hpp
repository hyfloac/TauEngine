/**
 * @file
 */
#pragma once

// ReSharper disable once CppUnusedIncludeDirective
#include <type_traits>
#include <Safeties.hpp>
#include <NumTypes.hpp>
#include <Endian.hpp>

class IFile;

#ifndef TAU_FW_BUFFER_PAGE_CNT
  #define TAU_FW_BUFFER_PAGE_CNT (16)
#endif

class FileWriter;

/**
 * Special utilities only intended for use by FileWriter.
 */
namespace _FWUtils {
/**
 * Reverses the bytes of a type.
 *
 * This is intended for changing the endianness of a type.
 */
template<typename _T>
void reverseT(u8* const arr) noexcept
{
    constexpr uSys halfLen = sizeof(_T) / 2;
    for(uSys i = 0; i < halfLen; ++i)
    {
        const u8 tmp = arr[i];
        arr[i] = arr[sizeof(_T) - i - 1];
        arr[sizeof(_T) - i - 1] = tmp;
    }
}

/**
 *   An indirection for reading the type in with a specific
 * endian.
 *
 *   Depending on the endianness of the system this will
 * require a different implementation. When the endianness is
 * different the bytes must be reversed. To do this would
 * require partial template specialization. Because functions
 * don't support partial template specialization in C++ this
 * has to be done with structures.
 */
template<typename _T, Endian _Endian>
struct WriteVal final
{
    [[nodiscard]] static i64 write(const _T& t, FileWriter& reader) noexcept = delete;
};

template<typename _T>
struct WriteVal<_T, Endian::EndianUnknown> final
{
    [[nodiscard]] static i64 write(const _T& t, FileWriter& reader) noexcept;
};

template<typename _T>
struct WriteVal<_T, Endian::EndianLittle> final
{
    [[nodiscard]] static i64 write(const _T& t, FileWriter& reader) noexcept;
};

template<typename _T>
struct WriteVal<_T, Endian::EndianBig> final
{
    [[nodiscard]] static i64 write(const _T& t, FileWriter& reader) noexcept;
};
}

class FileWriter final
{
    DELETE_CM(FileWriter);
private:
    CPPRef<IFile> _file;
    u64 _fileIndex;
    uSys _bufferIndex;
    uSys _bufferSize;
    void* _buffer;
public:
    FileWriter(const CPPRef<IFile>& file) noexcept;

    ~FileWriter() noexcept;

    [[nodiscard]] const CPPRef<IFile>& file() const noexcept { return _file; }
    [[nodiscard]] u64 index() const noexcept { return _fileIndex; }

    i64 flush() noexcept;

    i64 write(const void* buffer, uSys bufferSize) noexcept;

    template<uSys _BufferSize, ::std::enable_if_t<(_BufferSize > 4096), int>* = nullptr>
    i64 writeS(const void* const buffer) noexcept
    { return write(buffer, _BufferSize); }

    template<uSys _BufferSize, ::std::enable_if_t<(_BufferSize <= 4096), int>* = nullptr>
    i64 writeS(const void* const buffer) noexcept
    { return writeCountSafe<_BufferSize>(buffer); }

    template<typename _T, Endian _Endian = Endian::EndianUnknown>
    i64 writeT(const _T& t) noexcept
    { return _FWUtils::WriteVal<_T, _Endian>::write(t, *this); }
private:
    [[nodiscard]] i64 write1(const void* buffer) noexcept;
    [[nodiscard]] i64 write2(const void* buffer) noexcept;

    template<uSys _BufferSize>
    [[nodiscard]] i64 writeCountSafe(const void* buffer) noexcept
    {
        return 0;
    }
};

template<>
inline i64 FileWriter::writeS<1>(const void* const buffer) noexcept
{ return write1(buffer); }

template<>
inline i64 FileWriter::writeS<2>(const void* const buffer) noexcept
{ return write2(buffer); }

namespace _FWUtils {
template<typename _T>
i64 WriteVal<_T, Endian::EndianUnknown>::write(const _T& t, FileWriter& writer) noexcept
{ return writer.writeS<sizeof(_T)>(t); }

#if defined(LITTLE_ENDIAN)
template<typename _T>
i64 WriteVal<_T, Endian::EndianLittle>::write(const _T& t, FileWriter& writer) noexcept
{ return writer.writeS<sizeof(_T)>(t); }

template<typename _T>
i64 WriteVal<_T, Endian::EndianBig>::write(const _T& t, FileWriter& writer) noexcept
{
    u8 buf[sizeof(_T)];
    ::std::memcpy(buf, &t, sizeof(_T));
    _FWUtils::reverseT<_T>(buf);
    const i64 writeCount = writer.writeS<sizeof(_T)>(buf);
    return writeCount;
}
#else
template<typename _T>
i64 WriteVal<_T, Endian::EndianBig>::write(const _T& t, FileWriter& writer) noexcept
{ return writer.writeS<sizeof(_T)>(t); }

template<typename _T>
i64 WriteVal<_T, Endian::EndianLittle>::write(const _T& t, FileWriter& writer) noexcept
{
    u8 buf[sizeof(_T)];
    ::std::memcpy(buf, &t, sizeof(_T));
    _FWUtils::reverseT<_T>(buf);
    const i64 writeCount = writer.writeS<sizeof(_T)>(buf);
    return writeCount;
}    
#endif
}
