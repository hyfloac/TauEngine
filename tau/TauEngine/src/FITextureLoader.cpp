#pragma warning(push, 0)
#include <FreeImage.h>
#pragma warning(pop)

#include <texture/FITextureLoader.hpp>
#include <maths/Maths.hpp>
#include <texture/Texture.hpp>
#include <RenderingMode.hpp>

ITexture* loadTexture(const char* RESTRICT filename, const bool smooth, TextureLoadError* RESTRICT const error, i32 mipmapLevel) noexcept
{
#define ERR_EXIT(__ERR, __CHECK) if((__CHECK)) { *error = __ERR; return nullptr; }

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename);

    if(format == FIF_UNKNOWN)
    {
        format = FreeImage_GetFIFFromFilename(filename);
        ERR_EXIT(TextureLoadError::UNKNOWN_FORMAT, format == FIF_UNKNOWN)
    }

    FIBITMAP* texture = null;

    if(FreeImage_FIFSupportsReading(format)) { texture = FreeImage_Load(format, filename); }

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

    // GLuint texID = 0;
    // glGenTextures(1, &texID);
    // glBindTexture(GL_TEXTURE_2D, texID);

    RenderingMode rm(RenderingMode::Mode::OpenGL4);
    ITexture* ret = ITexture::create(rm);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    if(smooth)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    bool genMipmaps = false;

    if(mipmapLevel < 0)
    {
        genMipmaps = true;
        mipmapLevel = 0;
    }

    glTexImage2D(GL_TEXTURE_2D, mipmapLevel, GL_RGBA8, width, height, 0, bitsPerPixel > 24 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureData);

    if(genMipmaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    FreeImage_Unload(texture);

    *error = TextureLoadError::NONE;

    return ret;
#undef ERR_EXIT
}

ITexture* loadTextureEx(const char* RESTRICT filename, TextureLoadError* RESTRICT const error, GPUTextureSettings&& settings) noexcept
{
#define ERR_EXIT(__ERR, __CHECK) if((__CHECK)) { *error = __ERR; return nullptr; }

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename);

    if(format == FIF_UNKNOWN)
    {
        format = FreeImage_GetFIFFromFilename(filename);
        ERR_EXIT(TextureLoadError::UNKNOWN_FORMAT, format == FIF_UNKNOWN)
    }

    FIBITMAP* texture = null;

    if(FreeImage_FIFSupportsReading(format)) { texture = FreeImage_Load(format, filename); }

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

    // GLuint texID = 0;
    // glGenTextures(1, &texID);
    // glBindTexture(textureTarget, texID);

    RenderingMode rm(RenderingMode::Mode::OpenGL4);
    ITexture* ret = ITexture::create(rm, settings.textureType);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, settings.magnificationFilter == FilterType::Linear ? GL_LINEAR : GL_NEAREST);
    glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, settings. minificationFilter == FilterType::Linear ? GL_LINEAR : GL_NEAREST);

    glTexImage2D(textureTarget, maxT(settings.mipmapLevel, 0), GL_RGBA8, width, height, 0, bitsPerPixel > 24 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, textureData);

    if(settings.mipmapLevel < 0)
    {
        glGenerateMipmap(textureTarget);
    }

    FreeImage_Unload(texture);

    *error = TextureLoadError::NONE;

    return ret;
#undef ERR_EXIT
}
