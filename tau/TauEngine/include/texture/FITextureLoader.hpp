#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#include <NumTypes.hpp>
#include <Utils.hpp>
#include <DLL.hpp>
#pragma warning(pop)

enum class TextureLoadError : u8
{
    NONE = 0,
    UNKNOWN_FORMAT,
    TEXTURE_FAILED_TO_LOAD,
    NULL_TEXTURE_DATA,
    NULL_WIDTH,
    NULL_HEIGHT,
    BITS_PER_PIXEL_TOO_SMALL,
    BITS_PER_PIXEL_TOO_LARGE,
};

TAU_DLL GLuint loadTexture(const char* RESTRICT filename, const bool smooth = true, TextureLoadError* RESTRICT const error = null, i32 mipmapLevel = -1) noexcept;
