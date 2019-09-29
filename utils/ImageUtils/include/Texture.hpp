#pragma once

#include <NumTypes.hpp>
#include <FreeImage.h>

struct Texture final
{
public:
    u32 width;
    u32 height;
    u8* pixels;
    u8  bitsPerPixel;
public:
    Texture(u32 _width, u32 _height, u8* _pixels, u8 _bitsPerPixel) noexcept
        : width(_width), height(_height), pixels(_pixels), bitsPerPixel(_bitsPerPixel)
    { }

    ~Texture() noexcept = default;

    Texture(const Texture& copy) noexcept = default;
    Texture(Texture&& move) noexcept = default;

    Texture& operator =(const Texture& copy) noexcept = default;
    Texture& operator =(Texture&& move) noexcept = default;
};

struct FITexture final
{
public:
    u32 width;
    u32 height;
    u8* pixels;
    u8  bitsPerPixel;
    FIBITMAP* fiBitmap;
public:
    FITexture(u32 _width, u32 _height, u8* _pixels, u8 _bitsPerPixel, FIBITMAP* _fiBitmap) noexcept
        : width(_width), height(_height), pixels(_pixels), bitsPerPixel(_bitsPerPixel), fiBitmap(_fiBitmap)
    { }

    ~FITexture() noexcept
    { FreeImage_Unload(fiBitmap); }

    FITexture(const FITexture& copy) noexcept = delete;
    FITexture(FITexture&& move) noexcept = delete;

    FITexture& operator =(const FITexture& copy) noexcept = delete;
    FITexture& operator =(FITexture&& move) noexcept = delete;
};

