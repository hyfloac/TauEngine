#include <file/BufferedBinaryReader.hpp>

static void reverseArr(char arr[], u32 start, u32 end)
{
    while(start < end)
    {
        const char temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

BufferedBinaryReader::BufferedBinaryReader(const char* name, bool littleEndian) noexcept
    : _file(null), _fileName(name), _littleEndian(littleEndian)
{
#ifdef _MSC_VER
    fopen_s(&this->_file, name, "rbe");
#else
    this->_file = fopen(name, "rbe");
#endif
}

int BufferedBinaryReader::close() const noexcept
{
    return fclose(this->_file);
}

size_t BufferedBinaryReader::readBytes(char* buffer, u32 bufferLen, size_t* readCount) const noexcept
{
    const size_t read = fread(buffer, 1, bufferLen, this->_file);
    if(readCount) { *readCount = read; }
    return read;
}

#pragma region Big Endian
#pragma region Signed
i8 BufferedBinaryReader::readInt8BE(size_t* readCount) const noexcept
{
    i8 ret;
    const size_t read = fread(&ret, 1, 1, this->_file);
    if(readCount) { *readCount = read; }
    return ret;
}

i16 BufferedBinaryReader::readInt16BE(size_t* readCount) const noexcept
{
    char buffer[2];
    const size_t read = fread(buffer, 1, 2, this->_file);
    if(readCount) { *readCount = read; }
    i16 ret;
#if !IS_BIG_ENDIAN
    const char tmp = buffer[0];
    buffer[0] = buffer[1];
    buffer[1] = tmp;
#endif
    ret = *((i16*) buffer);
    return ret;
}

i32 BufferedBinaryReader::readInt32BE(size_t* readCount) const noexcept
{
    char buffer[4];
    const size_t read = fread(buffer, 1, 4, this->_file);
    if(readCount) { *readCount = read; }
    i32 ret;
#if !IS_BIG_ENDIAN
    reverseArr(buffer, 0, 3);
#endif
    ret = *((i32*) buffer);
    return ret;
}

i64 BufferedBinaryReader::readInt64BE(size_t* readCount) const noexcept
{
    char buffer[8];
    const size_t read = fread(buffer, 1, 8, this->_file);
    if(readCount) { *readCount = read; }
    i64 ret;
#if !IS_BIG_ENDIAN
    reverseArr(buffer, 0, 7);
#endif
    ret = *((i64*) buffer);
    return ret;
}
#pragma endregion

#pragma region Unsigned
u8 BufferedBinaryReader::readUInt8BE(size_t* readCount) const noexcept
{
    u8 ret;
    const size_t read = fread(&ret, 1, 1, this->_file);
    if(readCount) { *readCount = read; }
    return ret;
}

u16 BufferedBinaryReader::readUInt16BE(size_t* readCount) const noexcept
{
    char buffer[2];
    const size_t read = fread(buffer, 1, 2, this->_file);
    if(readCount) { *readCount = read; }
    u16 ret;
#if !IS_BIG_ENDIAN
    const char tmp = buffer[0];
    buffer[0] = buffer[1];
    buffer[1] = tmp;
#endif
    ret = *((u16*) buffer);
    return ret;
}

u32 BufferedBinaryReader::readUInt32BE(size_t* readCount) const noexcept
{
    char buffer[4];
    const size_t read = fread(buffer, 1, 4, this->_file);
    if(readCount) { *readCount = read; }
    u32 ret;
#if !IS_BIG_ENDIAN
    reverseArr(buffer, 0, 3);
#endif
    ret = *((u32*) buffer);
    return ret;
}

u64 BufferedBinaryReader::readUInt64BE(size_t* readCount) const noexcept
{
    char buffer[8];
    const size_t read = fread(buffer, 1, 8, this->_file);
    if(readCount) { *readCount = read; }
    u64 ret;
#if !IS_BIG_ENDIAN
    reverseArr(buffer, 0, 7);
#endif
    ret = *((u64*) buffer);
    return ret;
}
#pragma endregion

#pragma region FP
f32 BufferedBinaryReader::readFloatBE(size_t* readCount) const noexcept
{
    const i32 intBits = readInt32BE(readCount);
    return *((float*) intBits);
}

f64 BufferedBinaryReader::readDoubleBE(size_t* readCount) const noexcept
{
    const i64 intBits = readInt64BE(readCount);
    return *((double*) intBits);
}
#pragma endregion
#pragma endregion

#pragma region Little Endian
#pragma region Signed
i8 BufferedBinaryReader::readInt8LE(size_t* readCount) const noexcept
{
    i8 ret;
    const size_t read = fread(&ret, 1, 1, this->_file);
    if(readCount) { *readCount = read; }
    return ret;
}

i16 BufferedBinaryReader::readInt16LE(size_t* readCount) const noexcept
{
    char buffer[2];
    const size_t read = fread(buffer, 1, 2, this->_file);
    if(readCount) { *readCount = read; }
    i16 ret;
#if !IS_LITTLE_ENDIAN
    const char tmp = buffer[0];
    buffer[0] = buffer[1];
    buffer[1] = tmp;
#endif
    ret = *((i16*) buffer);
    return ret;
}

i32 BufferedBinaryReader::readInt32LE(size_t* readCount) const noexcept
{
    char buffer[4];
    const size_t read = fread(buffer, 1, 4, this->_file);
    if(readCount) { *readCount = read; }
    i32 ret;
#if !IS_LITTLE_ENDIAN
    reverseArr(buffer, 0, 3);
#endif
    ret = *((i32*) buffer);
    return ret;
}

i64 BufferedBinaryReader::readInt64LE(size_t* readCount) const noexcept
{
    char buffer[8];
    const size_t read = fread(buffer, 1, 8, this->_file);
    if(readCount) { *readCount = read; }
    i64 ret;
#if !IS_LITTLE_ENDIAN
    reverseArr(buffer, 0, 7);
#endif
    ret = *((i64*) buffer);
    return ret;
}
#pragma endregion

#pragma region Unsigned
u8 BufferedBinaryReader::readUInt8LE(size_t* readCount) const noexcept
{
    u8 ret;
    const size_t read = fread(&ret, 1, 1, this->_file);
    if(readCount) { *readCount = read; }
    return ret;
}

u16 BufferedBinaryReader::readUInt16LE(size_t* readCount) const noexcept
{
    char buffer[2];
    const size_t read = fread(buffer, 1, 2, this->_file);
    if(readCount) { *readCount = read; }
    u16 ret;
#if !IS_LITTLE_ENDIAN
    const char tmp = buffer[0];
    buffer[0] = buffer[1];
    buffer[1] = tmp;
#endif
    ret = *((u16*) buffer);
    return ret;
}

u32 BufferedBinaryReader::readUInt32LE(size_t* readCount) const noexcept
{
    char buffer[4];
    const size_t read = fread(buffer, 1, 4, this->_file);
    if(readCount) { *readCount = read; }
    u32 ret;
#if !IS_LITTLE_ENDIAN
    reverseArr(buffer, 0, 3);
#endif
    ret = *((u32*) buffer);
    return ret;
}

u64 BufferedBinaryReader::readUInt64LE(size_t* readCount) const noexcept
{
    char buffer[8];
    const size_t read = fread(buffer, 1, 8, this->_file);
    if(readCount) { *readCount = read; }
    u64 ret;
#if !IS_LITTLE_ENDIAN
    reverseArr(buffer, 0, 7);
#endif
    ret = *((u64*) buffer);
    return ret;
}
#pragma endregion

#pragma region FP
f32 BufferedBinaryReader::readFloatLE(size_t* readCount) const noexcept
{
    const i32 intBits = readInt32LE(readCount);
    return *((float*) intBits);
}

f64 BufferedBinaryReader::readDoubleLE(size_t* readCount) const noexcept
{
    const i64 intBits = readInt64LE(readCount);
    return *((double*) intBits);
}
#pragma endregion
#pragma endregion

#pragma region Variable
#pragma region Signed
i8 BufferedBinaryReader::readInt8(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readInt8LE(readCount); }
    else { return readInt8BE(readCount); }
}

i16 BufferedBinaryReader::readInt16(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readInt16LE(readCount); }
    else { return readInt16BE(readCount); }
}

i32 BufferedBinaryReader::readInt32(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readInt32LE(readCount); }
    else { return readInt32BE(readCount); }
}

i64 BufferedBinaryReader::readInt64(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readInt64LE(readCount); }
    else { return readInt64BE(readCount); }
}
#pragma endregion

#pragma region Unsigned
u8 BufferedBinaryReader::readUInt8(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readUInt8LE(readCount); }
    else { return readUInt8BE(readCount); }
}

u16 BufferedBinaryReader::readUInt16(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readUInt16LE(readCount); }
    else { return readUInt16BE(readCount); }
}

u32 BufferedBinaryReader::readUInt32(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readUInt32LE(readCount); }
    else { return readUInt32BE(readCount); }
}

u64 BufferedBinaryReader::readUInt64(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readUInt64LE(readCount); }
    else { return readUInt64BE(readCount); }
}
#pragma endregion

#pragma region FP
f32 BufferedBinaryReader::readFloat(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readFloatLE(readCount); }
    else { return readFloatBE(readCount); }
}

f64 BufferedBinaryReader::readDouble(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readDoubleLE(readCount); }
    else { return readDoubleBE(readCount); }
}
#pragma endregion
#pragma endregion
