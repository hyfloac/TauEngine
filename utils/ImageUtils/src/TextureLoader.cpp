#include <TextureLoader.h>
#include <FreeImage.h>

std::shared_ptr<FITexture> loadTexture(const char* const RESTRICT path, TextureLoadError* const RESTRICT error) noexcept
{
#define ERR_EXIT(__ERR, __CHECK) if((__CHECK)) { *error = TextureLoadError::__ERR; return nullptr; }

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path);

    if(format == FIF_UNKNOWN)
    {
        format = FreeImage_GetFIFFromFilename(path);
        ERR_EXIT(UNKNOWN_FORMAT, format == FIF_UNKNOWN)
    }

    FIBITMAP* texture = null;

    if(FreeImage_FIFSupportsReading(format)) { texture = FreeImage_Load(format, path); }

    ERR_EXIT(TEXTURE_FAILED_TO_LOAD, !texture);

    texture = FreeImage_ConvertTo32Bits(texture);

    BYTE*     textureData  = FreeImage_GetBits(texture);
    const u32 width        = FreeImage_GetWidth(texture);
    const u32 height       = FreeImage_GetHeight(texture);
    const u32 bitsPerPixel = FreeImage_GetBPP(texture);

    ERR_EXIT(NULL_TEXTURE_DATA, !textureData);
    ERR_EXIT(NULL_WIDTH, !width);
    ERR_EXIT(NULL_HEIGHT, !height);
    ERR_EXIT(BITS_PER_PIXEL_TOO_SMALL, bitsPerPixel < 8);
    ERR_EXIT(BITS_PER_PIXEL_TOO_LARGE, bitsPerPixel > 32);

    *error = TextureLoadError::NONE;


    return std::make_shared<FITexture>(width, height, textureData, bitsPerPixel, texture);
#undef ERR_EXIT
}

bool saveTexture(FIBITMAP* texture, const char* path, FREE_IMAGE_FORMAT format) noexcept
{
    return FreeImage_Save(format, texture, path);
}
