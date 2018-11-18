#include <file/BinaryReader.hpp>

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

BinaryReader::BinaryReader(const char* name, bool littleEndian) noexcept
    : _file(null), _fileName(name), _littleEndian(littleEndian)
{
#ifdef _MSC_VER
    fopen_s(&this->_file, name, "rbe");
#else
    this->_file = fopen(name, "rbe");
#endif
}

int BinaryReader::close() const noexcept
{
    return fclose(this->_file);
}

size_t BinaryReader::readBytes(char* buffer, u32 bufferLen, size_t* readCount) const noexcept
{
    const size_t read = fread(buffer, 1, bufferLen, this->_file);
    if(readCount) { *readCount = read; }
    return read;
}

#pragma region Big Endian
#pragma region Signed
i8 BinaryReader::readInt8BE(size_t* readCount) const noexcept
{
    i8 ret;
    const size_t read = fread(&ret, 1, 1, this->_file);
    if(readCount) { *readCount = read; }
    return ret;
}

i16 BinaryReader::readInt16BE(size_t* readCount) const noexcept
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

i32 BinaryReader::readInt32BE(size_t* readCount) const noexcept
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

i64 BinaryReader::readInt64BE(size_t* readCount) const noexcept
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
u8 BinaryReader::readUInt8BE(size_t* readCount) const noexcept
{
    u8 ret;
    const size_t read = fread(&ret, 1, 1, this->_file);
    if(readCount) { *readCount = read; }
    return ret;
}

u16 BinaryReader::readUInt16BE(size_t* readCount) const noexcept
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

u32 BinaryReader::readUInt32BE(size_t* readCount) const noexcept
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

u64 BinaryReader::readUInt64BE(size_t* readCount) const noexcept
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
f32 BinaryReader::readFloatBE(size_t* readCount) const noexcept
{
    const i32 intBits = readInt32BE(readCount);
    return *((float*) &intBits);
}

f64 BinaryReader::readDoubleBE(size_t* readCount) const noexcept
{
    const i64 intBits = readInt64BE(readCount);
    return *((double*) &intBits);
}
#pragma endregion
#pragma endregion

#pragma region Little Endian
#pragma region Signed
i8 BinaryReader::readInt8LE(size_t* readCount) const noexcept
{
    i8 ret;
    const size_t read = fread(&ret, 1, 1, this->_file);
    if(readCount) { *readCount = read; }
    return ret;
}

i16 BinaryReader::readInt16LE(size_t* readCount) const noexcept
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

i32 BinaryReader::readInt32LE(size_t* readCount) const noexcept
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

i64 BinaryReader::readInt64LE(size_t* readCount) const noexcept
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
u8 BinaryReader::readUInt8LE(size_t* readCount) const noexcept
{
    u8 ret;
    const size_t read = fread(&ret, 1, 1, this->_file);
    if(readCount) { *readCount = read; }
    return ret;
}

u16 BinaryReader::readUInt16LE(size_t* readCount) const noexcept
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

u32 BinaryReader::readUInt32LE(size_t* readCount) const noexcept
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

u64 BinaryReader::readUInt64LE(size_t* readCount) const noexcept
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
f32 BinaryReader::readFloatLE(size_t* readCount) const noexcept
{
    const i32 intBits = readInt32LE(readCount);
    return *((float*) &intBits);
}

f64 BinaryReader::readDoubleLE(size_t* readCount) const noexcept
{
    const i64 intBits = readInt64LE(readCount);
    return *((double*) &intBits);
}
#pragma endregion
#pragma endregion

#pragma region Variable
#pragma region Signed
i8 BinaryReader::readInt8(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readInt8LE(readCount); }
    else { return readInt8BE(readCount); }
}

i16 BinaryReader::readInt16(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readInt16LE(readCount); }
    else { return readInt16BE(readCount); }
}

i32 BinaryReader::readInt32(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readInt32LE(readCount); }
    else { return readInt32BE(readCount); }
}

i64 BinaryReader::readInt64(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readInt64LE(readCount); }
    else { return readInt64BE(readCount); }
}
#pragma endregion

#pragma region Unsigned
u8 BinaryReader::readUInt8(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readUInt8LE(readCount); }
    else { return readUInt8BE(readCount); }
}

u16 BinaryReader::readUInt16(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readUInt16LE(readCount); }
    else { return readUInt16BE(readCount); }
}

u32 BinaryReader::readUInt32(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readUInt32LE(readCount); }
    else { return readUInt32BE(readCount); }
}

u64 BinaryReader::readUInt64(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readUInt64LE(readCount); }
    else { return readUInt64BE(readCount); }
}
#pragma endregion

#pragma region FP
f32 BinaryReader::readFloat(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readFloatLE(readCount); }
    else { return readFloatBE(readCount); }
}

f64 BinaryReader::readDouble(size_t* readCount) const noexcept
{
    if(this->_littleEndian) { return readDoubleLE(readCount); }
    else { return readDoubleBE(readCount); }
}
#pragma endregion
#pragma endregion
