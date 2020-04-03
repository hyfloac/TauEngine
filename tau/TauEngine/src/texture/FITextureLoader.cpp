#pragma warning(push, 0)
#include <FreeImage.h>
#pragma warning(pop)

#include <texture/FITextureLoader.hpp>
#include <maths/Maths.hpp>
#include <texture/Texture.hpp>
#include <RenderingMode.hpp>
#include "VFS.hpp"
#include "Timings.hpp"
#include "system/GraphicsInterface.hpp"
#include "system/RenderingContext.hpp"

static CPPRef<ITexture> _missingTexture = null;

void TextureLoader::setMissingTexture(const CPPRef<ITexture>& missingTexture) noexcept
{
    _missingTexture = missingTexture;
}

CPPRef<ITexture> TextureLoader::getMissingTexture() noexcept
{
    return _missingTexture;
}

CPPRef<ITexture> TextureLoader::generateMissingTexture(IGraphicsInterface& gi, IRenderingContext& context) noexcept
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

    TextureArgs args;
    args.width = 2;
    args.height = 2;
    args.dataFormat = ETexture::Format::RedGreenBlueAlpha8UnsignedInt;
    args.mipmapLevels = 0;
    args.initialBuffer = textureData;

    const CPPRef<ITexture> ret = gi.createTexture2D().buildCPPRef(args, null);

    ret->generateMipmaps(context);

    delete[] textureData;

    return ret;
}

CPPRef<ITexture> TextureLoader::generateDebugTexture8(IGraphicsInterface& gi, IRenderingContext& context, const uSys power) noexcept
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

    TextureArgs args;
    args.width = side;
    args.height = side;
    args.dataFormat = ETexture::Format::RedGreenBlueAlpha8UnsignedInt;
    args.mipmapLevels = 0;
    args.initialBuffer = textureData;

    const CPPRef<ITexture> ret = gi.createTexture2D().buildCPPRef(args, null);

    ret->generateMipmaps(context);

    delete[] textureData;

    return ret;
}

CPPRef<ITexture> TextureLoader::generateDebugTexture16(IGraphicsInterface& gi, IRenderingContext& context, const uSys power) noexcept
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

    TextureArgs args;
    args.width = side;
    args.height = side;
    args.dataFormat = ETexture::Format::RedGreenBlueAlpha16UnsignedInt;
    args.mipmapLevels = 0;
    args.initialBuffer = textureData;

    const CPPRef<ITexture> ret = gi.createTexture2D().buildCPPRef(args, null);

    ret->generateMipmaps(context);

    delete[] textureData;

    return ret;
}

CPPRef<ITexture> TextureLoader::generateColorTexture(IGraphicsInterface& gi, IRenderingContext& context, RGBColor color) noexcept
{
    u8* const textureData = new u8[1 * 1 * 4];

    textureData[0] = color.r;
    textureData[1] = color.g;
    textureData[2] = color.b;
    textureData[3] = 0xFF;

    TextureArgs args;
    args.width = 1;
    args.height = 1;
    args.dataFormat = ETexture::Format::RedGreenBlueAlpha8UnsignedInt;
    args.mipmapLevels = 0;
    args.initialBuffer = textureData;

    const CPPRef<ITexture> ret = gi.createTexture2D().buildCPPRef(args, null);

    ret->generateMipmaps(context);

    delete[] textureData;

    return ret;
}

CPPRef<ITexture> TextureLoader::loadTextureEx(IGraphicsInterface& gi, IRenderingContext& context, const char* RESTRICT fileName, const i32 mipmapLevel, TextureLoadError* RESTRICT const error) noexcept
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

    TextureArgs args;
    args.width = width;
    args.height = height;
    args.dataFormat = ETexture::Format::RedGreenBlueAlpha8UnsignedInt;
    args.mipmapLevels = 0;
    args.initialBuffer = textureData;

    const CPPRef<ITexture> ret = gi.createTexture2D().buildCPPRef(args, null);

    if(mipmapLevel < 0)
    {
        ret->generateMipmaps(context);
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

CPPRef<ITextureCube> TextureLoader::loadTextureCubeEx(IGraphicsInterface& gi, IRenderingContext& context, const char* RESTRICT const folderPath, const char* RESTRICT const fileExtension, const i32 mipmapLevel, TextureLoadError* RESTRICT const error) noexcept
{
    PERF();
#define ERR_EXIT(__ERR, __CHECK) \
    if((__CHECK)) { \
        if(error) { *error = __ERR; } \
        if(texture) { FreeImage_Unload(texture); } \
        return null; }

    TextureCubeArgs args;
    args.dataFormat = ETexture::Format::RedGreenBlueAlpha8UnsignedInt;
    args.mipmapLevels = 0;

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

        args.initialBuffer[i] = textureData;
    }

    CPPRef<ITextureCube> ret = gi.createTextureCube().buildCPPRef(args, null);

    for(uSys i = 0; i < 6; ++i)
    {
        FreeImage_Unload(textures[i]);
    }

    if(mipmapLevel < 0)
    {
        ret->generateMipmaps(context);
    }

    if(error) { *error = TextureLoadError::NONE; }

    return ret;
#undef ERR_EXIT
}
