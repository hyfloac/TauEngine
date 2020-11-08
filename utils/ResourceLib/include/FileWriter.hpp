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
    [[nodiscard]] u64 index() const noexcept { return _fileIndex + _bufferIndex; }

    /**
     * Flushes the buffer into the file.
     *
     * @return
     *      The number of bytes written to the file.
     */
    i64 flush() noexcept;
    
    /**
     * Writes data into a buffered file.
     *
     * @param[in] buffer
     *      The buffer to write.
     * @param[in] bufferSize
     *      The size of the buffer in bytes.
     * @return
     *      The number of bytes written into the file.
     */
    i64 write(const void* buffer, uSys bufferSize) noexcept;
    
    /**
     * Writes data into a buffered file.
     *
     *   This is a special version intended to generate more
     * optimized memcpy code. In this case the buffer is larger
     * than a standard page, and thus it will fall back to the
     * standard method of writing data.
     *
     * @param[in] buffer
     *      The buffer to write.
     * @tparam[in] _BufferSize
     *      The size of the buffer in bytes.
     * @return
     *      The number of bytes written into the file.
     */
    template<uSys _BufferSize, ::std::enable_if_t<(_BufferSize > 4096), int>* = nullptr>
    i64 writeS(const void* const buffer) noexcept
    { return write(buffer, _BufferSize); }
    
    /**
     * Writes data into a buffered file.
    *
    *   This is a special version intended to generate more
    * optimized memcpy code. In this case the buffer is smaller
    * than a standard page, and thus will generate more optimized
    * assembly.
    *
     * @param[in] buffer
     *      The buffer to write.
    * @tparam _BufferSize
    *      The size of the buffer in bytes.
    * @return
     *      The number of bytes written into the file.
    */  
    template<uSys _BufferSize, ::std::enable_if_t<(_BufferSize <= 4096), int>* = nullptr>
    i64 writeS(const void* const buffer) noexcept
    { return writeCountSafe<_BufferSize>(buffer); }
    
    /**
     * Writes a type into a file.
     *
     *   This is intended to make it easier to write specific types
     * such as a primitive or a POD structure. To aid with this, it
     * also supports explicitly setting the target endianness of
     * the data.
     *
     *   This uses some indirection to handle the endianness. This
     * requires partial template specialization, but that is not
     * possible with a function, thus it needs to use a templated
     * structure.
     *
     * @tparam _T
     *      The type of data to write. This will be automatically
     *    inferred by the compiler.
     * @tparam _Endian
     *      The target endianness to write the data as. By default
     *    this will treat it as the compiling system endianness.
     * @param[in] t
     *      A reference to the variable to write into the file.
     * @return
     *      The number of bytes written.
     */
    template<typename _T, Endian _Endian = Endian::EndianUnknown>
    i64 writeT(const _T& t) noexcept
    { return _FWUtils::WriteVal<_T, _Endian>::write(t, *this); }
private:
    /**
     * Called from a specialization of @link writeS @endlink.
     *
     * Writes 1 byte.
     *
     * @return
     *      The number of bytes read in. 
     */
    [[nodiscard]] i64 write1(const void* buffer) noexcept;

    /**
     * Called from a specialization of @link writeS @endlink.
     *
     * Writes 2 bytes.
     *
     * @return
     *      The number of bytes read in. 
     */
    [[nodiscard]] i64 write2(const void* buffer) noexcept;
    
    /**
     * Called from @link writeCountSafe @endlink.
     *
     *   This is used when the buffer size is less than a page. in
     * that case it is able to always perform the copy with 1 or 2
     * memcpy's. This version does not take in the size as a
     * template argument. This is because templating generates a
     * lot code, and we don't want 4096 different versions of the
     * same thing.
     *
     * @param[in] buffer
     *      The buffer to write.
     * @param bufferSize
     *      The size of the buffer in bytes.
     * @return
     *      The number of bytes written into the file.
     */
    [[nodiscard]] i64 writeCountSafe(const void* buffer, uSys bufferSize) noexcept;
    
    /**
     * Called from @link writeCountSafe @endlink.
     *
     *   This is used when the buffer size is less than a page. in
     * that case it is able to always perform the copy with 1 or 2
     * memcpy's. This version redirects to the non templated
     * version to reduce code output size.
     *
     * @param[in] buffer
     *      The buffer to write.
     * @tparam _BufferSize
     *      The size of the buffer in bytes.
     * @return
     *      The number of bytes written into the file.
     */
    template<uSys _BufferSize, ::std::enable_if_t<(_BufferSize > 64), int>* = nullptr>
    [[nodiscard]] i64 writeCountSafe(const void* buffer) noexcept
    { return writeCountSafe(buffer, _BufferSize); }
    
    /**
     * Called from @link writeCountSafe @endlink.
     *
     *   This is used when the buffer size is less than a page. in
     * that case it is able to always perform the copy with 1 or 2
     * memcpy's. This version will have up to 64 unique versions.
     * These versions will be taking advantage of the intrinsic
     * version of memcpy.
     *
     * @param[in] buffer
     *      The buffer to write.
     * @tparam _BufferSize
     *      The size of the buffer in bytes.
     * @return
     *      The number of bytes written into the file.
     */
    template<uSys _BufferSize, ::std::enable_if_t<(_BufferSize <= 64), int>* = nullptr>
    [[nodiscard]] i64 writeCountSafe(const void* buffer) noexcept
    {
        if(!buffer || _BufferSize == 0)
        { return 0; }

        // Will we overflow the buffer?
        if(_bufferIndex + _BufferSize > _bufferSize)
        {
            const uSys writeCount = _bufferSize - _bufferIndex;
            (void) ::std::memcpy(reinterpret_cast<u8*>(_buffer) + _bufferIndex, buffer, writeCount);

            if(flush() <= 0)
            { return writeCount; }

            (void) ::std::memcpy(_buffer, reinterpret_cast<const u8*>(buffer) + writeCount, _BufferSize - writeCount);
            _bufferIndex = _BufferSize - writeCount;
        }
        else
        {
            (void) ::std::memcpy(reinterpret_cast<u8*>(_buffer) + _bufferIndex, buffer, _BufferSize);
            _bufferIndex += _BufferSize;
        }

        return _BufferSize;
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
    (void) ::std::memcpy(buf, &t, sizeof(_T));
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
    (void) ::std::memcpy(buf, &t, sizeof(_T));
    _FWUtils::reverseT<_T>(buf);
    const i64 writeCount = writer.writeS<sizeof(_T)>(buf);
    return writeCount;
}    
#endif
}
