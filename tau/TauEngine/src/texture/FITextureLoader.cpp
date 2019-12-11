#pragma warning(push, 0)
#include <FreeImage.h>
#include <GL/glew.h>
#pragma warning(pop)

#include <texture/FITextureLoader.hpp>
#include <maths/Maths.hpp>
#include <texture/Texture.hpp>
#include <RenderingMode.hpp>
#include "VFS.hpp"
#include "Timings.hpp"

static ITexture* _missingTexture = null;

void TextureLoader::setMissingTexture(ITexture* missingTexture) noexcept
{
    _missingTexture = missingTexture;
}

ITexture* TextureLoader::generateMissingTexture(IRenderingContext& context) noexcept
{
    PERF();
    u8* const textureData = new u8[2 * 2 * 3];

    textureData[0 * 3 + 0] = 0xFF;
    textureData[0 * 3 + 1] = 0x00;
    textureData[0 * 3 + 2] = 0xFF;

    textureData[1 * 3 + 0] = 0x00;
    textureData[1 * 3 + 1] = 0x00;
    textureData[1 * 3 + 2] = 0x00;

    textureData[2 * 3 + 0] = 0xFF;
    textureData[2 * 3 + 1] = 0x00;
    textureData[2 * 3 + 2] = 0xFF;

    textureData[3 * 3 + 0] = 0x00;
    textureData[3 * 3 + 1] = 0x00;
    textureData[3 * 3 + 2] = 0x00;

    ITexture* const ret = ITexture::create(context, 2, 2, ETexture::Format::RedGreenBlue8UnsignedInt, ETexture::Type::T2D);

    ret->setFilterMode(ETexture::Filter::Nearest, ETexture::Filter::Nearest);
    ret->set(textureData);

    // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 2, 2, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData);


    delete[] textureData;

    return ret;
}

ITexture* TextureLoader::loadTextureEx(IRenderingContext& context, const char* RESTRICT fileName, const GPUTextureSettings& settings, TextureLoadError* RESTRICT const error) noexcept
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

    ITexture* const ret = ITexture::create(context, width, height, ETexture::Format::RedGreenBlueAlpha8UnsignedInt, ETexture::Type::T2D);

    ret->setFilterMode(settings.minificationFilter, settings.magnificationFilter);
    ret->setWrapMode(settings.wrapS, settings.wrapT);
    ret->set(textureData);

    if(settings.mipmapLevel < 0)
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

ITextureCube* TextureLoader::loadTextureCubeEx(IRenderingContext& context, const char* folderPath, const char* fileExtension, const GPUTextureSettings& settings, TextureLoadError* error) noexcept
{
    PERF();
#define ERR_EXIT(__ERR, __CHECK) \
    if((__CHECK)) { \
        if(error) { *error = __ERR; } \
        if(texture) { FreeImage_Unload(texture); } \
        delete ret; \
        return null; }

    ITextureCube* ret = null;

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
            ret = ITextureCube::create(context, width, height, ETexture::Format::RedGreenBlueAlpha8UnsignedInt);
            ret->setFilterMode(settings.minificationFilter, settings.magnificationFilter);
            ret->setWrapModeCube(settings.wrapS, settings.wrapT, settings.wrapR);
        }

        ret->setCube(static_cast<ETexture::CubeSide>(i + 1), textureData);

        FreeImage_Unload(texture);
    }

    if(settings.mipmapLevel < 0)
    {
        ret->generateMipmaps();
    }

    if(error) { *error = TextureLoadError::NONE; }

    return ret;
#undef ERR_EXIT
}
