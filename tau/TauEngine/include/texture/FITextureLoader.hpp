#pragma once

#pragma warning(push, 0)
#include <NumTypes.hpp>
#include <Utils.hpp>
#include <DLL.hpp>
#include <Objects.hpp>
#pragma warning(pop)

#include <texture/Texture.hpp>


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
};

class ITexture;

TAU_DLL ITexture* loadTexture(const char* RESTRICT filename, bool smooth = true, TextureLoadError* RESTRICT error = null, i32 mipmapLevel = -1) noexcept;

enum class FilterType : u8
{
    Nearest,
    Linear
};

struct GPUTextureSettings final
{
    DEFAULT_DESTRUCT(GPUTextureSettings);
    DEFAULT_COPY(GPUTextureSettings);

    TextureType textureType;
    i32 mipmapLevel;
    FilterType magnificationFilter;
    FilterType  minificationFilter;

    GPUTextureSettings() noexcept = default;

    GPUTextureSettings(const TextureType _textureType, const FilterType _magFilter, const FilterType _minFilter, const i32 _mipmapLevel) noexcept
        : textureType(_textureType), mipmapLevel(_mipmapLevel),
          magnificationFilter(_magFilter), minificationFilter(_minFilter)
    { }
};

TAU_DLL ITexture* loadTextureEx(const char* RESTRICT filename, TextureLoadError* RESTRICT error, GPUTextureSettings&& settings) noexcept;
