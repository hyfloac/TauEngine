#include "TauTexture.hpp"
#include "IFile.hpp"
#include <EnumBitFields.hpp>
#include <String.hpp>
#include <Lzma2Dec.h>
#include <Alloc.h>

#pragma pack(push, 1)
namespace TT {
enum Flags : u16
{
    None         = 0,
    HasDebugData = 1 << 0,
    HasChecksum  = 1 << 1,
    Compressed   = 1 << 2
};

enum class TextureType : u8
{
    Unknown = 0,
    PhongDiffuse,
    PhongAlbedo = PhongDiffuse,
    PhongSpecular,
    Normal,
    PBRDiffuse,
    PBRAlbedo = PBRDiffuse,
    AmbientOcclusion,
    Glossy,
    PBRSpecular,
    Roughness,
    Metalness,
    Emissive
};

struct Header final
{
    u32 magic;
    u16 version;
    u8 alignmentExponent;
};

template<typename _T>
[[nodiscard]] constexpr inline _T _alignTo(const _T val, const _T alignment) noexcept
{
    if(alignment == 1)
    { return val; }
    return (val + alignment) & ~(alignment - 1);
}

namespace _0_1 {
struct Header final
{
    TauTextureFormat format;
    u64 width;
    u32 height;
    union
    {
        u16 depth;
        u16 arrayCount;
    };
    u16 mipLevels;
    Flags flags;
};

struct DebugHeader final
{
    TauDebugTextureType type;
    uSys filePathLen;
    uSys nameLen;
};

struct DebugData final
{
    WDynString _base;
    WDynStringView filePath;
    WDynStringView name;

    DebugData()
        : _base(L"")
        , filePath(_base, 0, 0)
        , name(_base, 0, 0)
    { }
};

struct SubResourceHeader final
{
    uSys offset;
    uSys compressedLength;
    uSys uncompressedLength;
};

struct MipHeader final
{
    u32 width;
    u32 height;
    uSys len;
    u32 checksum;
};
} }
#pragma pack(pop)

static CPPRef<TauTexture> load_0_1(const CPPRef<IFile>& file, uSys offset) noexcept;

#define CHECK(__TARGET_SIZE) \
    if(readSize < 0) \
    { ERROR_CODE(Error::FileTooSmall); } \
    if(static_cast<uSys>(readSize) != (__TARGET_SIZE)) \
    { ERROR_CODE(Error::FileTooSmall); } \
    offset += static_cast<uSys>(readSize)

#define CHECK_V(__TARGET_SIZE, __VAL) \
    if(readSize < 0) \
    { ERROR_CODE_V(Error::FileTooSmall, __VAL); } \
    if(static_cast<uSys>(readSize) != (__TARGET_SIZE)) \
    { ERROR_CODE_V(Error::FileTooSmall, __VAL); } \
    offset += static_cast<uSys>(readSize)

#define CHECK_N(__TARGET_SIZE) \
    if(readSize < 0) \
    { return null; } \
    if(static_cast<uSys>(readSize) != (__TARGET_SIZE)) \
    { return null; } \
    offset += static_cast<uSys>(readSize)

CPPRef<TauTexture> TauTexture::load(const CPPRef<IFile>& file) noexcept
{
    if(!file)
    { return null; }

    if(file->size() <= 0)
    { return null; }

    if(static_cast<uSys>(file->size()) < sizeof(TT::Header))
    { return null; }

    uSys offset = 0;

    TT::Header header;
    const i64 readSize = file->readType(&header);
    CHECK_N(sizeof(header));

    if(header.magic != TAU_TEXTURE_MAGIC)
    { return null; }

    if(header.version > TAU_TEXTURE_VERSION_CURRENT)
    { return null; }

    switch(header.version)
    {
        case TAU_TEXTURE_VERSION_0_1: return load_0_1(file, offset);
        default: return null;
    }
}

static CPPRef<TauTexture> load_0_1(const CPPRef<IFile>& file, uSys offset) noexcept
{
    if(static_cast<uSys>(file->size()) < sizeof(TT::_0_1::Header) + offset)
    { return null; }

    TT::_0_1::Header header;
    i64 readSize = file->readType(&header);
    CHECK_N(sizeof(header));

    if(header.format > TauTextureFormat::MAX)
    { return null; }

    const bool hasDebugData = hasFlag(header.flags, TT::Flags::HasDebugData);

    TT::_0_1::DebugHeader debugHeader;
    TT::_0_1::DebugData debugData;

    if(hasDebugData)
    {
        if(static_cast<uSys>(file->size()) < sizeof(TT::_0_1::DebugHeader) + offset)
        { return null; }

        readSize = file->readType(&debugHeader);
        CHECK_N(sizeof(debugHeader));
        
        if(static_cast<uSys>(file->size()) < (debugHeader.filePathLen + debugHeader.nameLen) * sizeof(wchar_t) + offset)
        { return null; }

        const uSys totalLen = debugHeader.filePathLen + debugHeader.nameLen + 1;

        wchar_t* const fullData = new(::std::nothrow) wchar_t[totalLen];
        readSize = file->read(fullData, totalLen * sizeof(wchar_t));
        CHECK_N(totalLen * sizeof(wchar_t));

        debugData._base = WDynString::passControl(fullData);
        debugData.filePath = WDynStringView(debugData._base, 0, debugHeader.filePathLen);
        debugData.name = WDynStringView(debugData._base, debugHeader.filePathLen, debugHeader.nameLen);
    }

    RefDynArray<TauTextureMip> mipmaps(header.mipLevels);

    return null;
}

void TauTextureCodec::beginTextureLoad(ReadState& readState, const CPPRef<IFile>& file, Error* const error) noexcept
{
    ERROR_CODE_COND(!file, TauTextureCodec::NullFile);
    ERROR_CODE_COND(file->size() <= 0, TauTextureCodec::FileTooSmall);

    readState.file = file;
    readState.offset = 0;
    readState.version = 0;
    readState.flags = 0;
    readState.subResourceCount = 0;
    readState.subResourceHeaderOffset = 0;
    readState.dataAlignment = 0;
    readState.alignmentExponent = 0;

    ERROR_CODE(TauTextureCodec::NoError);
}

void TauTextureCodec::beginTextureWrite(WriteState& writeState, const CPPRef<IFile>& file, const u8 alignmentExponent, const bool clearPadSpace, Error* const error) noexcept
{
    ERROR_CODE_COND(!file, TauTextureCodec::NullFile);
    ERROR_CODE_COND(file->size() <= 0, TauTextureCodec::FileTooSmall);

    writeState.file = file;
    writeState.offset = 0;
    writeState.version = 0;
    writeState.flags = 0;
    writeState.subResourceCount = 0;
    writeState.subResourceHeaderOffset = 0;
    writeState.dataAlignment = 1 << alignmentExponent;
    writeState.alignmentExponent = alignmentExponent;
    writeState.curSubResWriteOffset = 0;
    writeState.clearPadSpace = clearPadSpace;

    ERROR_CODE(TauTextureCodec::NoError);
}

void TauTextureCodec::loadTextureInfo(ReadState& readState, TauTextureInfo& info, TauTextureDebugData* const debugData, Error* const error) noexcept
{
    const CPPRef<IFile>& file = readState.file;

    ERROR_CODE_COND(!file, Error::NullFile);
    ERROR_CODE_COND(file->size() <= 0, Error::FileTooSmall);
    ERROR_CODE_COND(static_cast<uSys>(file->size()) < sizeof(TT::Header), Error::FileTooSmall);

    uSys& offset = readState.offset;
    
    TT::Header header;
    const i64 readSize = file->readType(&header);
    CHECK(sizeof(header));

    ERROR_CODE_COND(header.magic != TAU_TEXTURE_MAGIC, Error::InvalidFileFormat);
    ERROR_CODE_COND(header.version > TAU_TEXTURE_VERSION_CURRENT, Error::UnsupportedVersion);

    readState.version = header.version;
    info.version = header.version;

    switch(header.version)
    {
        case TAU_TEXTURE_VERSION_0_1: return loadTextureInfo_0_1(readState, info, debugData, error);
        default: ERROR_CODE(Error::UnsupportedVersion);
    }
}

uSys TauTextureCodec::loadTextureSubresource(ReadState& readState, void* const storage, const uSys length, const uSys subResource, Error* const error) noexcept
{
    ERROR_CODE_COND_V(!readState.file, Error::NullFile, 0);

    switch(readState.version)
    {
        case TAU_TEXTURE_VERSION_0_1: return loadTextureSubresource_0_1(readState, storage, length, subResource, error);
        default: ERROR_CODE_V(Error::UnsupportedVersion, 0);
    }
}

void TauTextureCodec::writeTextureHeader(WriteState& writeState, const TauTextureInfo& info, const TauTextureDebugData* const debugData, Error* error) noexcept
{
    const CPPRef<IFile>& file = writeState.file;

    ERROR_CODE_COND(!file, Error::NullFile);

    uSys& offset = writeState.offset;

    TT::Header header;
    header.magic = TAU_TEXTURE_MAGIC;
    header.version = TAU_TEXTURE_VERSION_CURRENT;
    header.alignmentExponent = writeState.alignmentExponent;

    offset += file->writeType(header);

    switch(writeState.version)
    {
        case TAU_TEXTURE_VERSION_0_1: writeTextureInfo_0_1(writeState, info, debugData, error); break;
        default: ERROR_CODE(Error::UnsupportedVersion);
    }
}

void TauTextureCodec::loadTextureInfo_0_1(ReadState& readState, TauTextureInfo& info, TauTextureDebugData* const debugData, Error* const error) noexcept
{
    const CPPRef<IFile>& file = readState.file;
    uSys& offset = readState.offset;

    ERROR_CODE_COND(static_cast<uSys>(file->size()) < sizeof(TT::_0_1::Header) + offset, Error::FileTooSmall);

    TT::_0_1::Header header;
    i64 readSize = file->readType(&header);
    CHECK(sizeof(header));
    
    ERROR_CODE_COND(header.format > TauTextureFormat::MAX, Error::InvalidTextureFormat);

    readState.flags = static_cast<uSys>(header.flags);
    readState.subResourceCount = header.mipLevels * header.arrayCount;

    info.hasDebugData = hasFlag(header.flags, TT::Flags::HasDebugData);
    info.compressed = hasFlag(header.flags, TT::Flags::Compressed);

    TT::_0_1::DebugHeader debugHeader;

    if(info.hasDebugData)
    {
        ERROR_CODE_COND(static_cast<uSys>(file->size()) < sizeof(TT::_0_1::DebugHeader) + offset, Error::FileTooSmall);

        readSize = file->readType(&debugHeader);
        CHECK(sizeof(debugHeader));
        
        const uSys totalLen = debugHeader.filePathLen + debugHeader.nameLen;
        ERROR_CODE_COND(static_cast<uSys>(file->size()) < totalLen * sizeof(wchar_t) + offset, Error::FileTooSmall);

        if(debugData)
        {
            wchar_t* const fullData = new(::std::nothrow) wchar_t[totalLen + 1];
            fullData[totalLen] = L'\0';
            readSize = file->read(fullData, totalLen * sizeof(wchar_t));
            CHECK(totalLen * sizeof(wchar_t));

            debugData->type = debugHeader.type;
            debugData->_base = WDynString::passControl(fullData);
            debugData->filePath = WDynStringView(debugData->_base, 0, debugHeader.filePathLen);
            debugData->name = WDynStringView(debugData->_base, debugHeader.filePathLen, debugHeader.nameLen);
        }
        else
        {
            file->advancePos(totalLen);
        }
    }

    readState.subResourceHeaderOffset = offset;

    ERROR_CODE(Error::NoError);
}

uSys TauTextureCodec::loadTextureSubresource_0_1(ReadState& readState, void* const storage, uSys length, const uSys subResource, Error* const error) noexcept
{
    const CPPRef<IFile>& file = readState.file;
    uSys& offset = readState.offset;

    const uSys subResourceOffset = readState.subResourceHeaderOffset + sizeof(TT::_0_1::SubResourceHeader) * subResource;

    ERROR_CODE_COND_V(static_cast<uSys>(file->size()) < sizeof(TT::_0_1::SubResourceHeader) + subResourceOffset, Error::FileTooSmall, 0);

    offset = subResourceOffset;

    TT::_0_1::SubResourceHeader header;
    i64 readSize = file->readType(&header);
    CHECK_V(sizeof(header), 0);

    if(storage)
    {
        ERROR_CODE_COND_V(length < header.uncompressedLength, Error::BufferTooSmall, 0);
    }
    else
    {
        ERROR_CODE_V(Error::NoError, header.uncompressedLength);
    }
    
    if(hasFlag(readState.flags, TT::Flags::Compressed))
    {
        ERROR_CODE_COND_V(static_cast<uSys>(file->size()) < header.offset + header.compressedLength + 1, Error::FileTooSmall, 0);

        file->setPos(header.offset);
        offset = header.offset;

        Byte props;
        readSize = file->readType(&props);
        CHECK_V(sizeof(props), 0);

        void* srcBuffer = ::std::malloc(header.compressedLength);
        ERROR_CODE_COND_V(!srcBuffer, Error::SystemMemoryAllocationFailure, 0);
        readSize = file->read(srcBuffer, header.compressedLength);
        CHECK_V(header.compressedLength, 0);

        uSys srcLength = header.compressedLength;
        uSys destLength = length;

        ELzmaStatus status;
        const SRes res = Lzma2Decode(reinterpret_cast<Byte*>(storage), &destLength, reinterpret_cast<Byte*>(srcBuffer), &srcLength, props, LZMA_FINISH_ANY, &status, &g_Alloc);

        if(res == SZ_OK)
        {
            ERROR_CODE_COND_V(status == LZMA_STATUS_NOT_FINISHED, Error::CompressedDataCorruption, 0);
        }
        else
        {
            ERROR_CODE_COND_V(res == SZ_ERROR_MEM, Error::SystemMemoryAllocationFailure, 0);
            ERROR_CODE_V(Error::CompressedDataCorruption, 0);
        }

        ERROR_CODE_V(Error::NoError, destLength);
    }
    else
    {
        ERROR_CODE_COND_V(static_cast<uSys>(file->size()) < header.offset + header.uncompressedLength, Error::FileTooSmall, 0);

        file->setPos(header.offset);
        offset = header.offset;

        readSize = file->read(storage, header.uncompressedLength);
        CHECK_V(header.uncompressedLength, 0);

        ERROR_CODE_V(Error::NoError, header.uncompressedLength);
    }
}

void TauTextureCodec::writeTextureInfo_0_1(WriteState& writeState, const TauTextureInfo& info, const TauTextureDebugData* const debugData, Error* error) noexcept
{
    const CPPRef<IFile>& file = writeState.file;
    uSys& offset = writeState.offset;

    TT::_0_1::Header header;
    header.format = info.format;
    header.width = info.width;
    header.height = info.height;
    header.arrayCount = info.arrayCount;
    header.mipLevels = info.arrayCount;
    header.flags = setFlag(TT::Flags::None, TT::Flags::HasDebugData, static_cast<bool>(debugData));
    setFlag(header.flags, TT::Flags::Compressed, info.compressed);

    offset += file->writeType(header);

    writeState.flags = header.flags;

    if(debugData)
    {
        TT::_0_1::DebugHeader debugHeader;
        debugHeader.type = debugData->type;
        debugHeader.filePathLen = debugData->filePath.length();
        debugHeader.nameLen = debugData->name.length();

        offset += file->writeType(debugHeader);

        offset += file->write(debugData->_base.c_str(), debugData->_base.length() * sizeof(wchar_t));
    }

    if(writeState.dataAlignment)
    {
        writeState.subResourceCount = header.arrayCount * header.mipLevels;
        writeState.subResourceHeaderOffset = TT::_alignTo(offset, writeState.dataAlignment);

        if(writeState.clearPadSpace)
        {
            file->setPos(offset);
        }
    }

    

}
