#pragma once

#include <NumTypes.hpp>
#include <Texture.hpp>
#include <Utils.hpp>
#include <memory>

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

std::shared_ptr<FITexture> loadTexture(const char* const RESTRICT path, TextureLoadError* const RESTRICT error) noexcept;

bool saveTexture(FIBITMAP* texture, const char* path, FREE_IMAGE_FORMAT format = FREE_IMAGE_FORMAT::FIF_PNG) noexcept;
