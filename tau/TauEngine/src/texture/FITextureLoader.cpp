#pragma warning(push, 0)
#include <FreeImage.h>
#pragma warning(pop)

#include "texture/FITextureLoader.hpp"
#include "maths/Maths.hpp"
#include "RenderingMode.hpp"
#include "VFS.hpp"
#include "Timings.hpp"
#include "system/GraphicsInterface.hpp"
#include "system/RenderingContext.hpp"
#include <EnumBitFields.hpp>

NullableRef<IResource> TextureLoader::_missingTexture = null;

NullableRef<IResource> TextureLoader::generateMissingTexture(IGraphicsInterface& gi, IRenderingContext& context) noexcept
{
    PERF();
    u8 textureData[2 * 2 * 4 + 1 * 1 * 4];

    textureData[0 * 4 + 0] = 0xFF;
    textureData[0 * 4 + 1] = 0x00;
    textureData[0 * 4 + 2] = 0xFF;
    textureData[0 * 4 + 3] = 0xFF;

    textureData[1 * 4 + 0] = 0x00;
    textureData[1 * 4 + 1] = 0x00;
    textureData[1 * 4 + 2] = 0x00;
    textureData[1 * 4 + 3] = 0xFF;

    textureData[2 * 4 + 0] = 0x00;
    textureData[2 * 4 + 1] = 0x00;
    textureData[2 * 4 + 2] = 0x00;
    textureData[2 * 4 + 3] = 0xFF;

    textureData[3 * 4 + 0] = 0xFF;
    textureData[3 * 4 + 1] = 0x00;
    textureData[3 * 4 + 2] = 0xFF;
    textureData[3 * 4 + 3] = 0xFF;

    // MipLevel 1
    textureData[16 + 0] = 0xFF;
    textureData[16 + 1] = 0x00;
    textureData[16 + 2] = 0xFF;
    textureData[16 + 3] = 0xFF;

    ResourceTexture2DArgs args;
    args.width = 2;
    args.height = 2;
    args.arrayCount = 1;
    args.mipLevels = 2;
    args.dataFormat = ETexture::Format::RedGreenBlueAlpha8UnsignedInt;
    args.flags = ETexture::BindFlags::ShaderAccess;
    args.usageType = EResource::UsageType::Immutable;

    const void* initialBuffers[2] = { textureData, &textureData[16] };

    args.initialBuffers = initialBuffers;

    const NullableRef<IResource> ret = gi.createResource().buildTauRef(args, null);

    return ret;
}

NullableRef<IResource> TextureLoader::generateDebugTexture8(IGraphicsInterface& gi, IRenderingContext& context, const uSys power) noexcept
{
    if(power > 12)
    { return _missingTexture; }

    const uSys side = 1 << power;

    void** initialBuffers = new(::std::nothrow) void* [power + 1];

    const uSys fullSize = ETexture::computeSize(ETexture::Format::RedGreenBlueAlpha8UnsignedInt, side, side);
    u8* const textureData = new u8[fullSize];

    uSys currSide = side;
    uSys base = 0;
    for(uSys i = 0; i < power; ++i)
    {
        const uSys size = ETexture::computeSubResourceSize(ETexture::Format::RedGreenBlueAlpha8UnsignedInt, side, side);

        for(uSys j = 0; j < size; ++j)
        {
            const uSys x = j / currSide;
            const uSys y = j % currSide;
            const u8 r = static_cast<u8>((0xFF * x) / currSide);
            const u8 g = static_cast<u8>((0xFF * y) / currSide);
            const u8 b = static_cast<u8>((0xFF * j) / size);

            textureData[base + j * 4 + 0] = 0xFF - b;
            textureData[base + j * 4 + 1] = g;
            textureData[base + j * 4 + 2] = r;
            textureData[base + j * 4 + 3] = 0xFF;
        }

        initialBuffers[i] = &textureData[base];

        currSide = ETexture::computeMipSide(currSide);
        base += size;
    }

    ResourceTexture2DArgs args;
    args.width = side;
    args.height = side;
    args.arrayCount = 1;
    args.mipLevels = power + 1;
    args.dataFormat = ETexture::Format::RedGreenBlueAlpha8UnsignedInt;
    args.flags = ETexture::BindFlags::ShaderAccess;
    args.usageType = EResource::UsageType::Immutable;
    args.initialBuffers = initialBuffers;

    const NullableRef<IResource> ret = gi.createResource().buildTauRef(args, null);

    delete[] textureData;
    delete[] initialBuffers;

    return ret;
}

NullableRef<IResource> TextureLoader::generateDebugTexture16(IGraphicsInterface& gi, IRenderingContext& context, const uSys power) noexcept
{
    if(power > 12)
    { return  _missingTexture; }

    const uSys side = 1 << power;

    void** initialBuffers = new(::std::nothrow) void* [power + 1];

    const uSys fullSize = ETexture::computeSize(ETexture::Format::RedGreenBlueAlpha16UnsignedInt, side, side);
    u8* const textureDataU8 = new u8[fullSize];
    u16* const textureData = reinterpret_cast<u16*>(textureDataU8);

    uSys currSide = side;
    uSys base = 0;
    for(uSys i = 0; i < power; ++i)
    {
        const uSys size = ETexture::computeSubResourceSize(ETexture::Format::RedGreenBlueAlpha8UnsignedInt, side, side);

        for(uSys j = 0; j < size; ++j)
        {
            const uSys x = j / currSide;
            const uSys y = j % currSide;
            const u16 r = static_cast<u8>((0xFFFF * x) / currSide);
            const u16 g = static_cast<u8>((0xFFFF * y) / currSide);
            const u16 b = static_cast<u8>((0xFFFF * j) / size);

            textureData[base + j * 4 + 0] = 0xFFFF - b;
            textureData[base + j * 4 + 1] = g;
            textureData[base + j * 4 + 2] = r;
            textureData[base + j * 4 + 3] = 0xFFFF;
        }

        initialBuffers[i] = &textureData[base];

        currSide = ETexture::computeMipSide(currSide);
        base += size;
    }

    ResourceTexture2DArgs args;
    args.width = side;
    args.height = side;
    args.arrayCount = 1;
    args.mipLevels = power + 1;
    args.dataFormat = ETexture::Format::RedGreenBlueAlpha16UnsignedInt;
    args.flags = ETexture::BindFlags::ShaderAccess;
    args.usageType = EResource::UsageType::Immutable;
    args.initialBuffers = initialBuffers;

    const NullableRef<IResource> ret = gi.createResource().buildTauRef(args, null);

    delete[] textureData;
    delete[] initialBuffers;

    return ret;
}

NullableRef<IResource> TextureLoader::generateColorTexture(IGraphicsInterface& gi, IRenderingContext& context, RGBColor color) noexcept
{
    u8 textureData[1 * 1 * 4];

    textureData[0] = color.r;
    textureData[1] = color.g;
    textureData[2] = color.b;
    textureData[3] = 0xFF;

    ResourceTexture2DArgs args;
    args.width = 1;
    args.height = 1;
    args.arrayCount = 1;
    args.mipLevels = 1;
    args.dataFormat = ETexture::Format::RedGreenBlueAlpha8UnsignedInt;
    args.flags = ETexture::BindFlags::ShaderAccess;

    const void* initialBuffers[1] = { textureData };
    args.initialBuffers = initialBuffers;

    const NullableRef<IResource> ret = gi.createResource().buildTauRef(args, null);

    return ret;
}

NullableRef<IResource> TextureLoader::loadTexture(IGraphicsInterface& gi, IRenderingContext& context, const char* RESTRICT fileName, TextureLoadError* RESTRICT const error) noexcept
{
    PERF();
#define ERR_EXIT(__ERR, __CHECK) \
    if((__CHECK)) { \
        if(error) { *error = __ERR; } \
        if(texture) { FreeImage_Unload(texture); } \
        return _missingTexture; }

    const VFS::Container physPath = VFS::Instance().resolvePath(fileName);

    FIBITMAP* texture = null;

    ERR_EXIT(TextureLoadError::INVALID_PATH, physPath.path.length() == 0);

    fileName = physPath.path.c_str();

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName);

    if(format == FIF_UNKNOWN)
    {
        format = FreeImage_GetFIFFromFilename(fileName);
        ERR_EXIT(TextureLoadError::UNKNOWN_FORMAT, format == FIF_UNKNOWN)
    }

    if(FreeImage_FIFSupportsReading(format)) { texture = FreeImage_Load(format, fileName); }

    ERR_EXIT(TextureLoadError::TEXTURE_FAILED_TO_LOAD, !texture);

    FIBITMAP* tmp = FreeImage_ConvertTo32Bits(texture);
    FreeImage_Unload(texture);
    texture = tmp;

    BYTE*     textureData  = FreeImage_GetBits(texture);
    const u32 width        = FreeImage_GetWidth(texture);
    const u32 height       = FreeImage_GetHeight(texture);
    const u32 bitsPerPixel = FreeImage_GetBPP(texture);

    ERR_EXIT(TextureLoadError::NULL_TEXTURE_DATA, !textureData);
    ERR_EXIT(TextureLoadError::NULL_WIDTH, !width);
    ERR_EXIT(TextureLoadError::NULL_HEIGHT, !height);
    ERR_EXIT(TextureLoadError::BITS_PER_PIXEL_TOO_SMALL, bitsPerPixel < 8);
    ERR_EXIT(TextureLoadError::BITS_PER_PIXEL_TOO_LARGE, bitsPerPixel > 32);

    ResourceTexture2DArgs args;
    args.width = width;
    args.height = height;
    args.arrayCount = 1;
    args.mipLevels = 0;
    args.dataFormat = ETexture::Format::RedGreenBlueAlpha8UnsignedInt;
    args.flags = ETexture::BindFlags::ShaderAccess | ETexture::BindFlags::RenderTarget | ETexture::BindFlags::GenerateMipmaps;

    const void* initialBuffers[1] = { textureData };
    args.initialBuffers = initialBuffers;

    const NullableRef<IResource> ret = gi.createResource().buildTauRef(args, null);

    FreeImage_Unload(texture);

    if(error) { *error = TextureLoadError::NONE; }

    return ret;
#undef ERR_EXIT
}

static const char* fileNames[6] = {
    "back",
    "front",
    "left",
    "right",
    "bottom",
    "top",
};

NullableRef<IResource> TextureLoader::loadTextureCube(IGraphicsInterface& gi, IRenderingContext& context, const char* RESTRICT const folderPath, const char* RESTRICT const fileExtension, TextureLoadError* RESTRICT const error) noexcept
{
    PERF();
#define ERR_EXIT(__ERR, __CHECK) \
    if((__CHECK)) { \
        if(error) { *error = __ERR; } \
        if(texture) { FreeImage_Unload(texture); } \
        return null; }

    ResourceTexture2DArgs args;
    args.arrayCount = 6;
    args.mipLevels = 0;
    args.dataFormat = ETexture::Format::RedGreenBlueAlpha8UnsignedInt;
    args.flags = ETexture::BindFlags::ShaderAccess | ETexture::BindFlags::RenderTarget | ETexture::BindFlags::GenerateMipmaps;

    const void* initialBuffers[6];
    args.initialBuffers = initialBuffers;

    u32 width = 0;
    u32 height = 0;
    u32 bitsPerPixel = 0;

    FIBITMAP* textures[6];

    for(uSys i = 0; i < 6; ++i)
    {
        const VFS::Container physPath = VFS::Instance().resolvePath(folderPath, fileNames[i],  fileExtension);

        FIBITMAP* texture = null;

        ERR_EXIT(TextureLoadError::INVALID_PATH, physPath.path.length() == 0);

        const char* fileName = physPath.path.c_str();

        FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName);

        if(format == FIF_UNKNOWN)
        {
            format = FreeImage_GetFIFFromFilename(fileName);
            ERR_EXIT(TextureLoadError::UNKNOWN_FORMAT, format == FIF_UNKNOWN)
        }

        if(FreeImage_FIFSupportsReading(format)) { texture = FreeImage_Load(format, fileName); }

        ERR_EXIT(TextureLoadError::TEXTURE_FAILED_TO_LOAD, !texture);

        FIBITMAP* tmp = FreeImage_ConvertTo32Bits(texture);
        FreeImage_Unload(texture);
        textures[i] = tmp;

        BYTE* textureData = FreeImage_GetBits(textures[i]);
        const u32 _width = FreeImage_GetWidth(textures[i]);
        const u32 _height = FreeImage_GetHeight(textures[i]);
        const u32 _bitsPerPixel = FreeImage_GetBPP(textures[i]);

        if(width == 0 && height == 0 && bitsPerPixel == 0)
        {
            width = _width;
            height = _height;
            bitsPerPixel = _bitsPerPixel;
        }
        else if(width != _width || height != _height || bitsPerPixel != _bitsPerPixel)
        {
            if(error)
            {
                *error = TextureLoadError::TEXTURE_SIZES_DONT_MATCH;
            }
            return null;
        }

        ERR_EXIT(TextureLoadError::NULL_TEXTURE_DATA, !textureData);
        ERR_EXIT(TextureLoadError::NULL_WIDTH, !width);
        ERR_EXIT(TextureLoadError::NULL_HEIGHT, !height);
        ERR_EXIT(TextureLoadError::BITS_PER_PIXEL_TOO_SMALL, bitsPerPixel < 8);
        ERR_EXIT(TextureLoadError::BITS_PER_PIXEL_TOO_LARGE, bitsPerPixel > 32);

        if(i == 0)
        {
            args.width = width;
            args.height = height;
        }

        initialBuffers[i] = textureData;
    }

    const NullableRef<IResource> ret = gi.createResource().buildTauRef(args, null);

    for(uSys i = 0; i < 6; ++i)
    {
        FreeImage_Unload(textures[i]);
    }

    if(error) { *error = TextureLoadError::NONE; }

    return ret;
#undef ERR_EXIT
}
