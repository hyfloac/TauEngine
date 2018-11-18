#pragma once

#pragma warning(push, 0)
#include <cstdio>
#pragma warning(pop)
#include <NumTypes.hpp>
#include <Endian.hpp>
#include <Utils.hpp>
#include <DLL.hpp>

class TAU_DLL BinaryReader
{
private:
    FILE*       _file;
    const char* _fileName;
    bool        _littleEndian;
public:
    BinaryReader(const char* name, bool littleEndian = true) noexcept;

    inline operator FILE*()             const noexcept { return _file; }
    inline operator const FILE* const() const noexcept { return _file; }
    inline const char* fileName() const noexcept { return _fileName; }

    int close() const noexcept;

    size_t readBytes(char* buffer, u32 bufferLen, size_t* readCount) const noexcept;

    i8 readInt8BE(size_t* readCount = null) const noexcept;
    i16 readInt16BE(size_t* readCount = null) const noexcept;
    i32 readInt32BE(size_t* readCount = null) const noexcept;
    i64 readInt64BE(size_t* readCount = null) const noexcept;

    u8 readUInt8BE(size_t* readCount = null) const noexcept;
    u16 readUInt16BE(size_t* readCount = null) const noexcept;
    u32 readUInt32BE(size_t* readCount = null) const noexcept;
    u64 readUInt64BE(size_t* readCount = null) const noexcept;

    f32 readFloatBE(size_t* readCount = null) const noexcept;
    f64 readDoubleBE(size_t* readCount = null) const noexcept;

    inline f32 readSingleBE(size_t* readCount = null) const noexcept { return readFloatBE(readCount); }
    inline f32 readF32BE(size_t* readCount = null)    const noexcept { return readFloatBE(readCount); }
    inline f64 readF64BE(size_t* readCount = null)    const noexcept { return readDoubleBE(readCount); }

    i8 readInt8LE(size_t* readCount = null) const noexcept;
    i16 readInt16LE(size_t* readCount = null) const noexcept;
    i32 readInt32LE(size_t* readCount = null) const noexcept;
    i64 readInt64LE(size_t* readCount = null) const noexcept;

    u8 readUInt8LE(size_t* readCount = null) const noexcept;
    u16 readUInt16LE(size_t* readCount = null) const noexcept;
    u32 readUInt32LE(size_t* readCount = null) const noexcept;
    u64 readUInt64LE(size_t* readCount = null) const noexcept;

    f32 readFloatLE(size_t* readCount = null) const noexcept;
    f64 readDoubleLE(size_t* readCount = null) const noexcept;

    inline f32 readSingleLE(size_t* readCount = null) const noexcept { return readFloatLE(readCount); }
    inline f32 readF32LE(size_t* readCount = null)    const noexcept { return readFloatLE(readCount); }
    inline f64 readF64LE(size_t* readCount = null)    const noexcept { return readDoubleLE(readCount); }

    inline i8 readInt8Native(size_t* readCount = null) const noexcept
    {
#if IS_BIG_ENDIAN
        return readInt8BE(readCount);
#else
        return readInt8LE(readCount);
#endif
    }
    inline i16 readInt16Native(size_t* readCount = null) const noexcept
    {
#if IS_BIG_ENDIAN
        return readInt16BE(readCount);
#else
        return readInt16LE(readCount);
#endif
    }
    inline i32 readInt32Native(size_t* readCount = null) const noexcept
    {
#if IS_BIG_ENDIAN
        return readInt32BE(readCount);
#else
        return readInt32LE(readCount);
#endif
    }
    inline i64 readInt64Native(size_t* readCount = null) const noexcept
    {
#if IS_BIG_ENDIAN
        return readInt64BE(readCount);
#else
        return readInt64LE(readCount);
#endif
    }
     
    inline u8 readUInt8Native(size_t* readCount = null) const noexcept
    {
#if IS_BIG_ENDIAN
        return readUInt8BE(readCount);
#else
        return readUInt8LE(readCount);
#endif
    }
    inline u16 readUInt16Native(size_t* readCount = null) const noexcept
    {
#if IS_BIG_ENDIAN
        return readUInt16BE(readCount);
#else
        return readUInt16LE(readCount);
#endif
    }
    inline u32 readUInt32Native(size_t* readCount = null) const noexcept
    {
#if IS_BIG_ENDIAN
        return readUInt32BE(readCount);
#else
        return readUInt32LE(readCount);
#endif
    }
    inline u64 readUInt64Native(size_t* readCount = null) const noexcept
    {
#if IS_BIG_ENDIAN
        return readUInt64BE(readCount);
#else
        return readUInt64LE(readCount);
#endif
    }
     
    inline f32 readFloatNative(size_t* readCount = null) const noexcept
    {
#if IS_BIG_ENDIAN
        return readFloatE(readCount);
#else
        return readFloatLE(readCount);
#endif
    }
    inline f64 readDoubleNative(size_t* readCount = null) const noexcept
    {
#if IS_BIG_ENDIAN
        return readDoubleBE(readCount);
#else
        return readDoubleLE(readCount);
#endif
    }

    inline f32 readSingleNative(size_t* readCount = null) const noexcept { return readFloatNative(readCount); }
    inline f32 readF32Native(size_t* readCount = null)    const noexcept { return readFloatNative(readCount); }
    inline f64 readF64Native(size_t* readCount = null)    const noexcept { return readDoubleNative(readCount); }

    i8 readInt8(size_t* readCount = null) const noexcept;
    i16 readInt16(size_t* readCount = null) const noexcept;
    i32 readInt32(size_t* readCount = null) const noexcept;
    i64 readInt64(size_t* readCount = null) const noexcept;

    u8 readUInt8(size_t* readCount = null) const noexcept;
    u16 readUInt16(size_t* readCount = null) const noexcept;
    u32 readUInt32(size_t* readCount = null) const noexcept;
    u64 readUInt64(size_t* readCount = null) const noexcept;

    f32 readFloat(size_t* readCount = null) const noexcept;
    f64 readDouble(size_t* readCount = null) const noexcept;

    inline f32 readSingle(size_t* readCount = null) const noexcept { return readFloat(readCount); }
    inline f32 readF32(size_t* readCount = null)    const noexcept { return readFloat(readCount); }
    inline f64 readF64(size_t* readCount = null)    const noexcept { return readDouble(readCount); }
};
