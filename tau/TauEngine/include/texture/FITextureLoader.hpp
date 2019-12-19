#pragma once

#pragma warning(push, 0)
#include <NumTypes.hpp>
#include <Utils.hpp>
#include <DLL.hpp>
#include <Objects.hpp>
#pragma warning(pop)

#include "texture/Texture.hpp"
#include <array>

class TAU_DLL TextureLoader final
{
    DEFAULT_CONSTRUCT_PI(TextureLoader);
    DEFAULT_DESTRUCT(TextureLoader);
    DELETE_COPY(TextureLoader);
public:
    enum class TextureLoadError : u8
    {
        NONE = 0,
        UNKNOWN_FORMAT,
        INVALID_PATH,
        TEXTURE_FAILED_TO_LOAD,
        NULL_TEXTURE_DATA,
        NULL_WIDTH,
        NULL_HEIGHT,
        BITS_PER_PIXEL_TOO_SMALL,
        BITS_PER_PIXEL_TOO_LARGE,
        TEXTURE_SIZES_DONT_MATCH
    };

    struct GPUTextureSettings final
    {
        i32 mipmapLevel;
        ETexture::Filter magnificationFilter;
        ETexture::Filter  minificationFilter;
        ETexture::WrapMode wrapS;
        ETexture::WrapMode wrapT;
        ETexture::WrapMode wrapR;
    };
public:
    static void setMissingTexture(ITexture* missingTexture) noexcept;

    static ITexture* generateMissingTexture(IRenderingContext& context) noexcept;

    static ITexture* generateWhiteTexture(IRenderingContext& context) noexcept;

    static ITexture* generateBlackTexture(IRenderingContext& context) noexcept;

    static ITexture* loadTextureEx(IRenderingContext& context, const char* RESTRICT fileName, const GPUTextureSettings& settings, TextureLoadError* RESTRICT error = null) noexcept;

    static ITexture* loadTexture(IRenderingContext& context, const char* RESTRICT fileName, ETexture::Filter smooth = ETexture::Filter::Linear, i32 mipmapLevel = -1, TextureLoadError* RESTRICT error = null) noexcept
    {
        return TextureLoader::loadTextureEx(context, fileName, GPUTextureSettings { 
            mipmapLevel,
            smooth,
            smooth,
            ETexture::WrapMode::Repeat,
            ETexture::WrapMode::Repeat,
            ETexture::WrapMode::Repeat
        }, error);
    }

    static ITextureCube* loadTextureCubeEx(IRenderingContext & context, const char* folderPath, const char* fileExtension, const GPUTextureSettings& settings, TextureLoadError* error = null) noexcept;

    static ITextureCube* loadTextureCube(IRenderingContext & context, const char* folderPath, const char* fileExtension, ETexture::Filter smooth = ETexture::Filter::Linear, i32 mipmapLevel = -1, TextureLoadError* error = null) noexcept
    {
        return TextureLoader::loadTextureCubeEx(context, folderPath, fileExtension, GPUTextureSettings {
            mipmapLevel,
            smooth,
            smooth,
            ETexture::WrapMode::Repeat,
            ETexture::WrapMode::Repeat,
            ETexture::WrapMode::Repeat
        }, error);
    }
};
