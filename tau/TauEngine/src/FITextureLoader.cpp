#pragma warning(push, 0)
#include <FreeImage.h>
#pragma warning(pop)

#include <texture/FITextureLoader.hpp>

GLuint loadTexture(const char* RESTRICT filename, const bool smooth, TextureLoadError* RESTRICT const error, i32 mipmapLevel) noexcept
{
#define ERR_EXIT(__ERR, __CHECK) if((__CHECK)) { *error = __ERR; return static_cast<GLuint>(-1); }

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename);

    if(format == FIF_UNKNOWN)
    {
        format = FreeImage_GetFIFFromFilename(filename);
        ERR_EXIT(TextureLoadError::UNKNOWN_FORMAT, format == FIF_UNKNOWN)
    }

    FIBITMAP* texture = null;

    if(FreeImage_FIFSupportsReading(format)) { texture = FreeImage_Load(format, filename); }

    ERR_EXIT(TextureLoadError::TEXTURE_FAILED_TO_LOAD, !texture)

    BYTE*     textureData = FreeImage_GetBits(texture);
    const u32 width = FreeImage_GetWidth(texture);
    const u32 height = FreeImage_GetHeight(texture);
    const u32 bitsPerPixel = FreeImage_GetBPP(texture);

    ERR_EXIT(TextureLoadError::NULL_TEXTURE_DATA, !textureData);
    ERR_EXIT(TextureLoadError::NULL_WIDTH, !width);
    ERR_EXIT(TextureLoadError::NULL_HEIGHT, !height);
    ERR_EXIT(TextureLoadError::BITS_PER_PIXEL_TOO_SMALL, bitsPerPixel < 8);
    ERR_EXIT(TextureLoadError::BITS_PER_PIXEL_TOO_LARGE, bitsPerPixel > 32);

    GLuint texID = 0;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
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

    return texID;
#undef ERR_EXIT
}
