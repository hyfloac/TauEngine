#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#include <NumTypes.hpp>
#include <Utils.hpp>
#include <DLL.hpp>
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

TAU_DLL ITexture* loadTexture(const char* RESTRICT filename, const bool smooth = true, TextureLoadError* RESTRICT const error = null, i32 mipmapLevel = -1) noexcept;

enum class FilterType : u8
{
    Nearest,
    Linear
};

struct GPUTextureSettings final
{
    TextureType textureType;
    FilterType magnificationFilter;
    FilterType  minificationFilter;
    i32 mipmapLevel;

    GPUTextureSettings() noexcept = default;

    GPUTextureSettings(TextureType _textureType, FilterType _magFilter, FilterType _minFilter, i32 _mipmapLevel) noexcept
        : textureType(_textureType), 
          magnificationFilter(_magFilter),
          minificationFilter(_minFilter),
          mipmapLevel(_mipmapLevel)
    { }

    GPUTextureSettings(const GPUTextureSettings& copy) noexcept = default;
    GPUTextureSettings(GPUTextureSettings&& move) noexcept = default;

    ~GPUTextureSettings() noexcept = default;

    GPUTextureSettings& operator =(const GPUTextureSettings& copy) noexcept = default;
    GPUTextureSettings& operator =(GPUTextureSettings&& move) noexcept = default;
};

TAU_DLL ITexture* loadTextureEx(const char* RESTRICT filename, TextureLoadError* RESTRICT const error, GPUTextureSettings&& settings) noexcept;
