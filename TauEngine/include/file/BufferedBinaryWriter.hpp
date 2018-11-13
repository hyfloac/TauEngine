#pragma once

#include <cstdio>
#include <NumTypes.hpp>
#include <Endian.hpp>
#include <DLL.hpp>

#ifndef BUFFER_SIZE
  #define BUFFER_SIZE ((u32) 8192)
#endif

class TAU_DLL BufferedBinaryWriter
{
private:
    FILE* _file;
    bool _littleEndian;
    char buffer[BUFFER_SIZE];
public:
    BufferedBinaryWriter(FILE* file, bool littleEndian = true) noexcept;
    BufferedBinaryWriter(const char* name, bool littleEndian = true) noexcept;

    inline operator FILE*()             const noexcept { return _file; }
    inline operator const FILE* const() const noexcept { return _file; }

    int close() const noexcept;
    void flush() noexcept;

    size_t writeBytes(char* buffer, u32 bufferLen) const noexcept;

    i8  writeInt8BE() const noexcept;
    i16 writeInt16BE() const noexcept;
    i32 writeInt32BE() const noexcept;
    i64 writeInt64BE() const noexcept;

    u8  writeUInt8BE() const noexcept;
    u16 writeUInt16BE() const noexcept;
    u32 writeUInt32BE() const noexcept;
    u64 writeUInt64BE() const noexcept;

    f32 writeFloatBE() const noexcept;
    f64 writeDoubleBE() const noexcept;

    inline f32 writeSingleBE() const noexcept { return writeFloatBE(); }
    inline f32 writeF32BE()    const noexcept { return writeFloatBE(); }
    inline f64 writeF64BE()    const noexcept { return writeDoubleBE(); }

    i8  writeInt8LE() const noexcept;
    i16 writeInt16LE() const noexcept;
    i32 writeInt32LE() const noexcept;
    i64 writeInt64LE() const noexcept;

    u8  writeUInt8LE() const noexcept;
    u16 writeUInt16LE() const noexcept;
    u32 writeUInt32LE() const noexcept;
    u64 writeUInt64LE() const noexcept;

    f32 writeFloatLE() const noexcept;
    f64 writeDoubleLE() const noexcept;

    inline f32 writeSingleLE() const noexcept { return writeFloatLE(); }
    inline f32 writeF32LE()    const noexcept { return writeFloatLE(); }
    inline f64 writeF64LE()    const noexcept { return writeDoubleLE(); }

    inline i8 writeInt8Native() const noexcept
    {
#if IS_BIG_ENDIAN
        return writeInt8BE();
#else
        return writeInt8LE();
#endif
    }
    inline i16 writeInt16Native() const noexcept
    {
#if IS_BIG_ENDIAN
        return writeInt16BE();
#else
        return writeInt16LE();
#endif
    }
    inline i32 writeInt32Native() const noexcept
    {
#if IS_BIG_ENDIAN
        return writeInt32BE();
#else
        return writeInt32LE();
#endif
    }
    inline i64 writeInt64Native() const noexcept
    {
#if IS_BIG_ENDIAN
        return writeInt64BE();
#else
        return writeInt64LE();
#endif
    }

    inline u8 writeUInt8Native() const noexcept
    {
#if IS_BIG_ENDIAN
        return writeUInt8BE();
#else
        return writeUInt8LE();
#endif
    }
    inline u16 writeUInt16Native() const noexcept
    {
#if IS_BIG_ENDIAN
        return writeUInt16BE();
#else
        return writeUInt16LE();
#endif
    }
    inline u32 writeUInt32Native() const noexcept
    {
#if IS_BIG_ENDIAN
        return writeUInt32BE();
#else
        return writeUInt32LE();
#endif
    }
    inline u64 writeUInt64Native() const noexcept
    {
#if IS_BIG_ENDIAN
        return writeUInt64BE();
#else
        return writeUInt64LE();
#endif
    }

    inline f32 writeFloatNative() const noexcept
    {
#if IS_BIG_ENDIAN
        return writeFloatE();
#else
        return writeFloatLE();
#endif
    }
    inline f64 writeDoubleNative() const noexcept
    {
#if IS_BIG_ENDIAN
        return writeDoubleBE();
#else
        return writeDoubleLE();
#endif
    }

    inline f32 writeSingleNative() const noexcept { return writeFloatNative(); }
    inline f32 writeF32Native()    const noexcept { return writeFloatNative(); }
    inline f64 writeF64Native()    const noexcept { return writeDoubleNative(); }

    i8  writeInt8() const noexcept;
    i16 writeInt16() const noexcept;
    i32 writeInt32() const noexcept;
    i64 writeInt64() const noexcept;

    u8  writeUInt8() const noexcept;
    u16 writeUInt16() const noexcept;
    u32 writeUInt32() const noexcept;
    u64 writeUInt64() const noexcept;

    f32 writeFloat() const noexcept;
    f64 writeDouble() const noexcept;

    inline f32 writeSingle() const noexcept { return writeFloat(); }
    inline f32 writeF32()    const noexcept { return writeFloat(); }
    inline f64 writeF64()    const noexcept { return writeDouble(); }
};
