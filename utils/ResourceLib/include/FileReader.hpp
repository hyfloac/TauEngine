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

#ifndef TAU_FR_BUFFER_PAGE_CNT
  #define TAU_FR_BUFFER_PAGE_CNT (16)
#endif

class FileReader;

/**
 * Special utilities only intended for use by FileReader.
 */
namespace _FRUtils {
/**
 * Reverses the bytes of a type.
 *
 * This is intended for changing the endianness of a type.
 */
template<typename _T>
void reverseT(_T* const arr) noexcept
{
    u8* bytes = reinterpret_cast<u8*>(arr);

    constexpr uSys halfLen = sizeof(_T) / 2;
    for(uSys i = 0; i < halfLen; ++i)
    {
        const u8 tmp = bytes[i];
        bytes[i] = bytes[sizeof(_T) - i - 1];
        bytes[sizeof(_T) - i - 1] = tmp;
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
struct ReadVal final
{
    [[nodiscard]] static i64 read(_T* t, FileReader& reader) noexcept = delete;
};

template<typename _T>
struct ReadVal<_T, Endian::EndianUnknown> final
{
    [[nodiscard]] static i64 read(_T* t, FileReader& reader) noexcept;
};

template<typename _T>
struct ReadVal<_T, Endian::EndianLittle> final
{
    [[nodiscard]] static i64 read(_T* t, FileReader& reader) noexcept;
};

template<typename _T>
struct ReadVal<_T, Endian::EndianBig> final
{
    [[nodiscard]] static i64 read(_T* t, FileReader& reader) noexcept;
};
}

class FileReader final
{
    DELETE_CM(FileReader);
private:
    CPPRef<IFile> _file;
    u64 _fileIndex;
    uSys _bufferIndex;
    uSys _bufferSize;
    uSys _bufferFillCount;
    void* _buffer;
public:
    FileReader(const CPPRef<IFile>& file) noexcept;

    ~FileReader() noexcept;

    [[nodiscard]] const CPPRef<IFile>& file() const noexcept { return _file; }
    [[nodiscard]] u64 index() const noexcept { return _fileIndex; }

    /**
     * Reads data into a buffer.
     *
     * @param[out] buffer
     *      The buffer where the data will get stored.
     * @param[in] bufferSize
     *      The size of the buffer in bytes.
     * @return
     *      The number of bytes read into the buffer.
     */
    i64 read(void* buffer, uSys bufferSize) noexcept;

    /**
     * Reads data into a buffer.
     *
     *   This is a special version intended to generate more
     * optimized memcpy code. In this case the buffer is larger
     * than a standard page, and thus it will fall back to the
     * standard method of reading data in.
     *
     * @param[out] buffer
     *      The buffer where the data will get stored.
     * @tparam[in] _BufferSize
     *      The size of the buffer in bytes.
     * @return
     *      The number of bytes read into the buffer.
     */
    template<uSys _BufferSize, ::std::enable_if_t<(_BufferSize > 4096), int>* = nullptr>
    i64 readS(void* buffer) noexcept
    { return read(buffer, _BufferSize); }

    /**
    * Reads data into a buffer.
    *
    *   This is a special version intended to generate more
    * optimized memcpy code. In this case the buffer is smaller
    * than a standard page, and thus will generate optimized
    * assembly.
    *
    * @param[out] buffer
    *      The buffer where the data will get stored.
    * @tparam _BufferSize
    *      The size of the buffer in bytes.
    * @return
    *      The number of bytes read into the buffer.
    */    
    template<uSys _BufferSize, ::std::enable_if_t<(_BufferSize <= 4096), int>* = nullptr>
    i64 readS(void* buffer) noexcept
    { return readCountSafe<_BufferSize>(buffer); }

    /**
     * Reads in a type from a file.
     *
     *   This is intended to make it easier to read specific types
     * such as a primitive or a structure. To aid with this, it
     * also supports explicitly setting the target endianness of
     * the data.
     *
     *   This uses some indirection to handle the endianness. This
     * requires partial template specialization, but that is not
     * possible with a function, thus it needs to use a templated
     * structure.
     *
     * @tparam _T
     *      The type of data to read. This will be automatically
     *    inferred by the compiler.
     * @tparam _Endian
     *      The target endianness to parse the data as. By default
     *    this will treat it as the compiling system endianness.
     * @param[out] t
     *      A pointer to variable to read in from the file.
     * @return
     *      The number of bytes read in.
     */
    template<typename _T, Endian _Endian = Endian::EndianUnknown>
    i64 readT(_T* t) noexcept
    { return _FRUtils::ReadVal<_T, _Endian>::read(t, *this); }
private:
    /**
     * Called from a specialization of @link readS @endlink.
     *
     * Reads in 1 byte.
     *
     * @return
     *      The number of bytes read in. 
     */
    [[nodiscard]] i64 read1(void* buffer) noexcept;

    /**
     * Called from a specialization of @link readS @endlink.
     *
     * Reads in 2 bytes.
     *
     * @return
     *      The number of bytes read in. 
     */
    [[nodiscard]] i64 read2(void* buffer) noexcept;

    /**
     * Buffers in a block of data from the file.
     *
     * @return
     *      The number of bytes read in.
     */    
    [[nodiscard]] i64 bufferData() noexcept;

    /**
     * Called from @link readS @endlink.
     *
     *   This is used when the buffer size is less than a page. in
     * that case it is able to always perform the copy with 1 or 2
     * memcpy's.
     *
     * @param[out] buffer
     *      The buffer where the data will get stored.
     * @tparam _BufferSize
     *      The size of the buffer in bytes.
     * @return
     *      The number of bytes read in. 
     */
    template<uSys _BufferSize>
    [[nodiscard]] i64 readCountSafe(void* buffer) noexcept
    {
        // Are we at the end of the buffer.
        if(_bufferIndex == _bufferFillCount)
        {
            // Refill the buffer.
            const iSys fillCount = bufferData();
            if(fillCount <= 0)
            { return fillCount; }
        }

        if(!buffer)
        { return 0; }

        // Will this read past the buffer.
        if(_bufferIndex + _BufferSize > _bufferFillCount)
        {
            // Copy the maximum number of bytes that can be read.
            const uSys readSize = _bufferFillCount - _bufferIndex;
            (void) ::std::memcpy(buffer, reinterpret_cast<u8*>(_buffer) + _bufferIndex, readSize);

            // Refill the buffer.
            const iSys fillCount = bufferData();
            if(fillCount <= 0)
            { return readSize; }

            // Copy in the remaining data.            
            (void) ::std::memcpy(reinterpret_cast<u8*>(buffer) + readSize, _buffer, _BufferSize - readSize);
            _bufferIndex += _BufferSize - readSize;
        }
        else
        {
            // Copy the data.
            (void) ::std::memcpy(buffer, reinterpret_cast<u8*>(_buffer) + _bufferIndex, _BufferSize);
            _bufferIndex += _BufferSize;
        }

        return _BufferSize;
    }
};

template<>
inline i64 FileReader::readS<1>(void* buffer) noexcept
{ return read1(buffer); }

template<>
inline i64 FileReader::readS<2>(void* buffer) noexcept
{ return read2(buffer); }

namespace _FRUtils {
template<typename _T>
i64 ReadVal<_T, Endian::EndianUnknown>::read(_T* const t, FileReader& reader) noexcept
{ return reader.readS<sizeof(_T)>(t); }

#if defined(LITTLE_ENDIAN)
template<typename _T>
i64 ReadVal<_T, Endian::EndianLittle>::read(_T* const t, FileReader& reader) noexcept
{ return reader.readS<sizeof(_T)>(t); }

template<typename _T>
i64 ReadVal<_T, Endian::EndianBig>::read(_T* const t, FileReader& reader) noexcept
{
    const i64 readCount = reader.readS<sizeof(_T)>(t);
    _FRUtils::reverseT(t);
    return readCount;
}
#else
template<typename _T>
i64 ReadVal<_T, Endian::EndianBig>::read(_T* const t, FileReader& reader) noexcept
{ return reader.readS<sizeof(_T)>(t); }

template<typename _T>
i64 ReadVal<_T, Endian::EndianLittle>::read(_T* const t, FileReader& reader) noexcept
{
    const i64 readCount = reader.readS<sizeof(_T)>(t);
    _FRUtils::reverseTArr(t);
    return readCount;
}    
#endif
}
