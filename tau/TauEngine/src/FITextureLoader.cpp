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

ITexture* TextureLoader::generateMissingTexture() noexcept
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

    const RenderingMode rm(RenderingMode::Mode::OpenGL4);
    ITexture* const ret = ITexture::create(rm, TextureType::TEXTURE_2D);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, 2, 2, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData);

    delete[] textureData;

    return ret;
}

ITexture* TextureLoader::loadTextureEx(const char* RESTRICT fileName, GPUTextureSettings&& settings, TextureLoadError* RESTRICT const error) noexcept
{
    PERF();
#define ERR_EXIT(__ERR, __CHECK) if((__CHECK)) { if(error) { *error = __ERR; } return _missingTexture; }

    const VFS::Container physPath = VFS::Instance().resolvePath(fileName);

    ERR_EXIT(TextureLoadError::INVALID_PATH, physPath.path.length() == 0);

    fileName = physPath.path.c_str();

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(fileName);

    if(format == FIF_UNKNOWN)
    {
        format = FreeImage_GetFIFFromFilename(fileName);
        ERR_EXIT(TextureLoadError::UNKNOWN_FORMAT, format == FIF_UNKNOWN)
    }

    FIBITMAP* texture = null;

    if(FreeImage_FIFSupportsReading(format)) { texture = FreeImage_Load(format, fileName); }

    ERR_EXIT(TextureLoadError::TEXTURE_FAILED_TO_LOAD, !texture);

    BYTE*     textureData  = FreeImage_GetBits(texture);
    const u32 width        = FreeImage_GetWidth(texture);
    const u32 height       = FreeImage_GetHeight(texture);
    const u32 bitsPerPixel = FreeImage_GetBPP(texture);

    ERR_EXIT(TextureLoadError::NULL_TEXTURE_DATA, !textureData);
    ERR_EXIT(TextureLoadError::NULL_WIDTH, !width);
    ERR_EXIT(TextureLoadError::NULL_HEIGHT, !height);
    ERR_EXIT(TextureLoadError::BITS_PER_PIXEL_TOO_SMALL, bitsPerPixel < 8);
    ERR_EXIT(TextureLoadError::BITS_PER_PIXEL_TOO_LARGE, bitsPerPixel > 32);

    GLenum textureTarget = 0;

    switch(settings.textureType)
    {
        case TextureType::TEXTURE_2D:
            textureTarget = GL_TEXTURE_2D;
            break;
        case TextureType::TEXTURE_3D:
            textureTarget = GL_TEXTURE_3D;
            break;
        case TextureType::TEXTURE_CUBE:
            textureTarget = GL_TEXTURE_CUBE_MAP;
            break;
    }

    const RenderingMode rm(RenderingMode::Mode::OpenGL4);
    ITexture* const ret = ITexture::create(rm, settings.textureType);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, settings.magnificationFilter == FilterType::Linear ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, settings. minificationFilter == FilterType::Linear ? GL_LINEAR : GL_NEAREST);

    glTexImage2D(textureTarget, maxT(settings.mipmapLevel, 0), GL_RGBA8, width, height, 0, bitsPerPixel > 24 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureData);

    if(settings.mipmapLevel < 0)
    {
        glGenerateMipmap(textureTarget);
    }

    FreeImage_Unload(texture);

    if(error) { *error = TextureLoadError::NONE; }

    return ret;
#undef ERR_EXIT
}
