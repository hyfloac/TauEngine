#pragma once

#pragma warning(push, 0)
#include <algorithm>
#pragma warning(pop)

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <DynArray.hpp>
#include <Safeties.hpp>
#include <String.hpp>


#ifndef TAU_MAKE_VERSION
  #define TAU_MAKE_VERSION(_MAJOR, _MINOR) (((_MAJOR) << 8) | (_MINOR))
#endif

static constexpr u32 TAU_TEXTURE_MAGIC = 0x5461754D; // TauT

static constexpr u16 TAU_TEXTURE_VERSION_0_1 = TAU_MAKE_VERSION(0, 1);

static constexpr u16 TAU_TEXTURE_VERSION_CURRENT = TAU_TEXTURE_VERSION_0_1;

class IFile;

enum class TauTextureFormat : u16
{
    R_u8 = 0,
    RG_u8,
    RGB_u8,
    RGBA_u8,
    R_u16,
    RG_u16,
    RGB_u16,
    RGBA_u16,
    R_u32,
    RG_u32,
    RGB_u32,
    RGBA_u32,
    R_f16,
    RG_f16,
    RGB_f16,
    RGBA_f16,
    R_f32,
    RG_f32,
    RGB_f32,
    RGBA_f32,
    MIN = R_u8,
    MAX = RGBA_f32
};

enum class TauDebugTextureType : u8
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

class TauTextureMip final
{
    DEFAULT_CONSTRUCT_PU(TauTextureMip);
    DEFAULT_DESTRUCT(TauTextureMip);
    DEFAULT_CM_PU(TauTextureMip);
private:
    u32 _width;
    u32 _height;
    RefDynArray<u8> _data;
public:
    TauTextureMip(const u32 width, const u32 height, const RefDynArray<u8>& data) noexcept
        : _width(width)
        , _height(height)
        , _data(data)
    { }

    TauTextureMip(const u32 width, const u32 height, RefDynArray<u8>&& data) noexcept
        : _width(width)
        , _height(height)
        , _data(::std::move(data))
    { }

    [[nodiscard]] u32&               width() noexcept { return _width;  }
    [[nodiscard]] u32&              height() noexcept { return _height; }
    [[nodiscard]] RefDynArray<u8>&    data() noexcept { return _data;   }

    [[nodiscard]] u32                   width() const noexcept { return _width;  }
    [[nodiscard]] u32                  height() const noexcept { return _height; }
    [[nodiscard]] const RefDynArray<u8>& data() const noexcept { return _data;   }
};

class TauTexture final
{
    DEFAULT_CONSTRUCT_PU(TauTexture);
    DEFAULT_DESTRUCT(TauTexture);
    DEFAULT_CM_PU(TauTexture);
public:
    CPPRef<TauTexture> load(const CPPRef<IFile>& file) noexcept;
private:
    TauTextureFormat _format;
    RefDynArray<TauTextureMip> _mipChain;
public:
    TauTexture(const TauTextureFormat format, const RefDynArray<TauTextureMip>& mipChain) noexcept
        : _format(format)
        , _mipChain(mipChain)
    { }

    TauTexture(const TauTextureFormat format, RefDynArray<TauTextureMip>&& mipChain) noexcept
        : _format(format)
        , _mipChain(::std::move(mipChain))
    { }

    [[nodiscard]] TauTextureFormat& format()       noexcept { return _format; }
    [[nodiscard]] TauTextureFormat  format() const noexcept { return _format; }

    [[nodiscard]]       RefDynArray<TauTextureMip>& mipChain()       noexcept { return _mipChain; }
    [[nodiscard]] const RefDynArray<TauTextureMip>& mipChain() const noexcept { return _mipChain; }
};

struct TauTextureInfo final
{
    u16 version;
    bool hasDebugData;
    bool compressed;
    TauTextureFormat format;
    u64 width;
    u32 height;
    union
    {
        u16 depth;
        u16 arrayCount;
    };
    u16 mipmapLevels;
};

struct TauTextureDebugData final
{
    TauDebugTextureType type;
    WDynString _base;
    WDynStringView filePath;
    WDynStringView name;

    TauTextureDebugData() noexcept
        : type(TauDebugTextureType::Unknown)
        , _base(L"")
        , filePath(_base, 0, 0)
        , name(_base, 0, 0)
    { }
};

class TauTextureCodec final
{
    DEFAULT_CONSTRUCT_PU(TauTextureCodec);
    DEFAULT_DESTRUCT(TauTextureCodec);
    DEFAULT_CM_PU(TauTextureCodec);
public:
    enum Error
    {
        NoError = 0,
        NullFile,
        FileTooSmall,
        InvalidFileFormat,
        UnsupportedVersion,
        InvalidTextureFormat,
        BufferTooSmall,
        SystemMemoryAllocationFailure,
        CompressedDataCorruption
    };

    struct ReadState final
    {
        CPPRef<IFile> file;
        uSys offset;
        u16 version;
        uSys flags;
        uSys subResourceCount;
        uSys subResourceHeaderOffset;
        uSys dataAlignment;
        u8 alignmentExponent;
    };

    struct WriteState final
    {
        CPPRef<IFile> file;
        uSys offset;
        u16 version;
        uSys flags;
        uSys subResourceCount;
        uSys subResourceHeaderOffset;
        uSys dataAlignment;
        u8 alignmentExponent;
        uSys curSubResWriteOffset;
        bool clearPadSpace;
    };
public:
    static void beginTextureLoad(ReadState& readState, const CPPRef<IFile>& file, [[tau::out]] Error* error) noexcept;
    static void beginTextureWrite(WriteState& writeState, const CPPRef<IFile>& file, u8 alignmentExponent, bool clearPadSpace, [[tau::out]] Error* error) noexcept;

    static void loadTextureInfo(ReadState& readState, [[tau::out]] TauTextureInfo& info, [[tau::out]] TauTextureDebugData* debugData, [[tau::out]] Error* error) noexcept;
    static uSys loadTextureSubresource(ReadState& readState, [[tau::out]] void* storage, uSys length, uSys subResource, [[tau::out]] Error* error) noexcept;

    static void writeTextureHeader(WriteState& writeState, const TauTextureInfo& info, const TauTextureDebugData* debugData, [[tau::out]] Error* error) noexcept;
    static void writeTextureSubresource(const CPPRef<IFile>& file, const void* textureData, uSys dataLength, uSys subResource) noexcept;
private:
    static void loadTextureInfo_0_1(ReadState& readState, [[tau::out]] TauTextureInfo& info, [[tau::out]] TauTextureDebugData* debugData, [[tau::out]] Error* error) noexcept;
    static uSys loadTextureSubresource_0_1(ReadState& readState, [[tau::out]] void* storage, uSys length, uSys subResource, [[tau::out]] Error* error) noexcept;

    static void writeTextureInfo_0_1(WriteState& writeState, const TauTextureInfo& info, const TauTextureDebugData* debugData, [[tau::out]] Error* error) noexcept;
};
