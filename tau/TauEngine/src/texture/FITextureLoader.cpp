#pragma warning(push, 0)
#include <FreeImage.h>
#pragma warning(pop)

#include <texture/FITextureLoader.hpp>
#include <maths/Maths.hpp>
#include <texture/Texture.hpp>
#include <RenderingMode.hpp>
#include "VFS.hpp"
#include "Timings.hpp"
#include "system/RenderingContext.hpp"

static Ref<ITexture> _missingTexture = null;

void TextureLoader::setMissingTexture(const Ref<ITexture>& missingTexture) noexcept
{
    _missingTexture = missingTexture;
}

Ref<ITexture> TextureLoader::getMissingTexture() noexcept
{
    return _missingTexture;
}

Ref<ITexture> TextureLoader::generateMissingTexture(IRenderingContext& context) noexcept
{
    PERF();
    u8* const textureData = new u8[2 * 2 * 4];

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

    Ref<ITextureBuilder> builder = context.createTexture2D();
    builder->width(2);
    builder->height(2);
    builder->dataFormat(ETexture::Format::RedGreenBlueAlpha8UnsignedInt);
    builder->mipmapLevels(0);
    builder->initialBuffer(textureData);

    const Ref<ITexture> ret = Ref<ITexture>(builder->build(null));
    
    delete[] textureData;

    return ret;
}

Ref<ITexture> TextureLoader::generateDebugTexture8(IRenderingContext& context, const uSys power) noexcept
{
    if(power > 12)
    { return  _missingTexture; }

    const uSys side = 1 << power;
    const uSys size = side * side;

    u8* const textureData = new u8[size * 4];

    for(uSys i = 0; i < size; ++i)
    {
        const uSys x = i / side;
        const uSys y = i % side;
        const u16 r = static_cast<u8>((0xFF * x) / side);
        const u16 g = static_cast<u8>((0xFF * y) / side);
        const u16 b = static_cast<u8>((0xFF * i) / size);

        textureData[i * 4 + 0] = 0xFF - b;
        textureData[i * 4 + 1] = g;
        textureData[i * 4 + 2] = r;
        textureData[i * 4 + 3] = 0xFF;
    }

    Ref<ITextureBuilder> builder = context.createTexture2D();
    builder->width(side);
    builder->height(side);
    builder->dataFormat(ETexture::Format::RedGreenBlueAlpha8UnsignedInt);
    builder->mipmapLevels(0);
    builder->initialBuffer(textureData);

    const Ref<ITexture> ret = Ref<ITexture>(builder->build(null));
    
    delete[] textureData;

    return ret;
}

Ref<ITexture> TextureLoader::generateDebugTexture16(IRenderingContext& context, const uSys power) noexcept
{
    if(power > 12)
    {
        return  _missingTexture;
    }

    const uSys side = 1 << power;
    const uSys size = side * side;

    u16* const textureData = new u16[size * 4];

    for(uSys i = 0; i < size; ++i)
    {
        const uSys x = i / side;
        const uSys y = i % side;
        const u16 r = static_cast<u16>((0xFFFF * x) / side);
        const u16 g = static_cast<u16>((0xFFFF * y) / side);
        const u16 b = static_cast<u16>((0xFFFF * i) / size);

        textureData[i * 4 + 0] = 0xFFFF - b;
        textureData[i * 4 + 1] = g;
        textureData[i * 4 + 2] = r;
        textureData[i * 4 + 3] = 0xFFFF;
    }

    Ref<ITextureBuilder> builder = context.createTexture2D();
    builder->width(side);
    builder->height(side);
    builder->dataFormat(ETexture::Format::RedGreenBlueAlpha16UnsignedInt);
    builder->mipmapLevels(0);
    builder->initialBuffer(textureData);

    const Ref<ITexture> ret = Ref<ITexture>(builder->build(null));

    delete[] textureData;

    return ret;
}

Ref<ITexture> TextureLoader::generateColorTexture(IRenderingContext& context, RGBColor color) noexcept
{
    u8* const textureData = new u8[1 * 1 * 4];

    textureData[0] = color.r;
    textureData[1] = color.g;
    textureData[2] = color.b;
    textureData[3] = 0xFF;

    Ref<ITextureBuilder> builder = context.createTexture2D();
    builder->width(1);
    builder->height(1);
    builder->dataFormat(ETexture::Format::RedGreenBlueAlpha8UnsignedInt);
    builder->mipmapLevels(0);
    builder->initialBuffer(textureData);

    const Ref<ITexture> ret = Ref<ITexture>(builder->build(null));
    
    delete[] textureData;

    return ret;
}

Ref<ITexture> TextureLoader::loadTextureEx(IRenderingContext& context, const char* RESTRICT fileName, const i32 mipmapLevel, TextureLoadError* RESTRICT const error) noexcept
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

    Ref<ITextureBuilder> builder = context.createTexture2D();
    builder->width(width);
    builder->height(height);
    builder->dataFormat(ETexture::Format::RedGreenBlueAlpha8UnsignedInt);
    builder->mipmapLevels(0);
    builder->initialBuffer(textureData);

    Ref<ITexture> const ret = Ref<ITexture>(builder->build(null));

    if(mipmapLevel < 0)
    {
        ret->generateMipmaps();
    }

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

Ref<ITextureCube> TextureLoader::loadTextureCubeEx(IRenderingContext& context, const char* RESTRICT const folderPath, const char* RESTRICT const fileExtension, const i32 mipmapLevel, TextureLoadError* RESTRICT const error) noexcept
{
    PERF();
#define ERR_EXIT(__ERR, __CHECK) \
    if((__CHECK)) { \
        if(error) { *error = __ERR; } \
        if(texture) { FreeImage_Unload(texture); } \
        return null; }

    Ref<ITextureCubeBuilder> builder = context.createTextureCube();
    builder->dataFormat(ETexture::Format::RedGreenBlueAlpha8UnsignedInt);
    builder->mipmapLevels(0);

    Ref<ITextureCube> ret = null;

    u32 width = 0;
    u32 height = 0;
    u32 bitsPerPixel = 0;

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
        texture = tmp;

        BYTE* textureData = FreeImage_GetBits(texture);
        const u32 _width = FreeImage_GetWidth(texture);
        const u32 _height = FreeImage_GetHeight(texture);
        const u32 _bitsPerPixel = FreeImage_GetBPP(texture);

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
            builder->width(width);
            builder->height(height);

            ret = Ref<ITextureCube>(builder->build(null));
        }

        ret->setCube(0, static_cast<ETexture::CubeSide>(i + 1), textureData);

        FreeImage_Unload(texture);
    }

    if(mipmapLevel < 0)
    {
        ret->generateMipmaps();
    }

    if(error) { *error = TextureLoadError::NONE; }

    return ret;
#undef ERR_EXIT
}
