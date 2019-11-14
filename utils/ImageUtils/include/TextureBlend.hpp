#pragma once

#include <Texture.hpp>
#include <Utils.hpp>
#include <ratio>

struct TextureBlend final
{
public:
    Texture* texture;
public:
    TextureBlend(Texture* _texture) noexcept
        : texture(_texture)
    { }

    ~TextureBlend() noexcept
    {
        delete texture->pixels;
        delete texture;
    }

    TextureBlend(const TextureBlend& copy) noexcept = delete;
    TextureBlend(TextureBlend&& move) noexcept = delete;

    TextureBlend& operator =(const TextureBlend& copy) noexcept = delete;
    TextureBlend& operator =(TextureBlend&& move) noexcept = delete;
};

template<typename _In, typename _Out>
static inline _Out fastRound(const _In x)
{
    return static_cast<_Out>(x + 0.5);
}

template<typename _Img0, typename _Img1>
TextureBlend* mix(const _Img0* const img0, const _Img1* const img1, const double ratio) noexcept
{
    if(img0->width != img1->width || img0->height != img1->height || img0->bitsPerPixel != img1->bitsPerPixel)
    { return null; }

    const u8* const pix0 = img0->pixels;
    const u8* const pix1 = img1->pixels;
    const u32 len = img0->width * img0->height * 4;
    u8* const ret = new u8[len];

    const double invRatio = 1.0 - ratio;

    for(u32 i = 0; i < len; ++i)
    {
        const double a = pix0[i] * invRatio;
        const double b = pix1[i] * ratio;
        const double mixed = a + b;
        ret[i] = fastRound<double, u8>(mixed);
    }

    return new TextureBlend(new Texture(img0->width, img0->height, ret, 32));
}

template<typename _Img0, typename _Img1, u32 _RatioNum, u32 _RatioDen>
TextureBlend* mix(const _Img0* const img0, const _Img1* const img1) noexcept
{
    if(img0->width != img1->width || img0->height != img1->height || img0->bitsPerPixel != img1->bitsPerPixel)
    { return null; }

    const u8* const pix0 = img0->pixels;
    const u8* const pix1 = img1->pixels;
    const u32 len = img0->width * img0->height * 4;
    u8* const ret = new u8[len];

    const double ratio = static_cast<double>(_RatioNum) / static_cast<double>(_RatioDen);
    const double invRatio = 1.0 - ratio;

    for(u32 i = 0; i < len; ++i)
    {
        const double a = pix0[i] * invRatio;
        const double b = pix1[i] * ratio;
        const double mixed = a + b;
        ret[i] = fastRound<double, u8>(mixed);
    }

    return new TextureBlend(new Texture(img0->width, img0->height, ret, 32));
}

template<typename _Img0, typename _Img1>
TextureBlend* avg(const _Img0* const img0, const _Img1* const img1) noexcept
{
    if(img0->width != img1->width || img0->height != img1->height || img0->bitsPerPixel != img1->bitsPerPixel)
    { return null; }

    const u8* const pix0 = img0->pixels;
    const u8* const pix1 = img1->pixels;
    const u32 len = img0->width * img0->height * 4;
    u8* const ret = new u8[len];

    for(u32 i = 0; i < len; ++i)
    {
        const double mixed = (pix0[i] + pix1[i]) * 0.5;
        ret[i] = fastRound<double, u8>(mixed);
    }

    return new TextureBlend(new Texture(img0->width, img0->height, ret, 32));
}

template<typename _Img0, typename _Img1>
TextureBlend* mulImages(const _Img0* const img0, const _Img1* const img1) noexcept
{
    if(img0->width != img1->width || img0->height != img1->height || img0->bitsPerPixel != img1->bitsPerPixel)
    { return null; }

    const u8* const pix0 = img0->pixels;
    const u8* const pix1 = img1->pixels;
    const u32 len = img0->width * img0->height * 4;
    u8* const ret = new u8[len];

    for(u32 i = 0; i < len; ++i)
    {
        const u32 multiplied = static_cast<u32>(pix0[i]) * static_cast<u32>(pix1[i]);
        ret[i] = fastRound<double, u8>(multiplied / 255.0);
    }

    return new TextureBlend(new Texture(img0->width, img0->height, ret, 32));
}

template<typename _Img0, typename _Img1>
TextureBlend* xorImages(const _Img0* const img0, const _Img1* const img1) noexcept
{
    if(img0->width != img1->width || img0->height != img1->height || img0->bitsPerPixel != img1->bitsPerPixel)
    {
        return null;
    }

    const u8* const pix0 = img0->pixels;
    const u8* const pix1 = img1->pixels;
    const u32 len = img0->width * img0->height * 4;
    u8* const ret = new u8[len];

    for(u32 i = 0; i < len; ++i)
    {
        if(i % 4 == 3) continue;
        ret[i] = pix0[i] ^ pix1[i];
    }

    return new TextureBlend(new Texture(img0->width, img0->height, ret, 32));
}

template<typename _Img0, typename _Img1>
TextureBlend* xorImagesSIMD(const _Img0* const img0, const _Img1* const img1) noexcept
{
    if(img0->width != img1->width || img0->height != img1->height || img0->bitsPerPixel != img1->bitsPerPixel)
    {
        return null;
    }

    const u32* const pix0 = reinterpret_cast<u32*>(img0->pixels);
    const u32* const pix1 = reinterpret_cast<u32*>(img1->pixels);
    const u32 len = img0->width * img0->height;
    u8* const ret = new u8[len * 4];

    u32* const map = reinterpret_cast<u32*>(ret);

    for(u32 i = 0; i < len; ++i)
    {
        map[i] = pix0[i] ^ pix1[i];
        map[i] |= 0xFF000000;
    }

    return new TextureBlend(new Texture(img0->width, img0->height, ret, 32));
}

template<typename _Img0>
TextureBlend* filterImage(const _Img0* const img0) noexcept
{
    const u32* const pix0 = reinterpret_cast<u32*>(img0->pixels);
    const u32 len = img0->width * img0->height;
    u8* const ret = new u8[len * 4];

    u32* ret0 = reinterpret_cast<u32*>(ret);

    for(u32 i = 0; i < len; ++i)
    {
        ret0[i] = pix0[i];
        if((pix0[i] & 0x00FFFFFF) == 0x00FFFFFF || (pix0[i] & 0x00FFFFFF) == 0x00000000)
        {
            ret0[i] = 0x00000000;
        }
    }

    return new TextureBlend(new Texture(img0->width, img0->height, ret, 32));
}

template<int _Size>
TextureBlend* createDebug() noexcept
{
    const u32 len = _Size * _Size;
    u8* const ret = new u8[len * 4];
    // u32* ret0 = reinterpret_cast<u32*>(ret);

    for(u32 i = 0; i < len; ++i)
    {
        const u32 x = i / _Size;
        const u32 y = i % _Size;
        const char r = static_cast<char>((255 * x) / _Size);
        const char g = static_cast<char>((255 * y) / _Size);
        const char b = static_cast<char>((255 * i) / len);

        ret[i * 4 + 0] = 255 - b;
        ret[i * 4 + 1] = g;
        ret[i * 4 + 2] = r;
        ret[i * 4 + 3] = 0xFF;
    }

    return new TextureBlend(new Texture(_Size, _Size, ret, 32));
}
