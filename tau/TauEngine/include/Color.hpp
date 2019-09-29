#pragma once

#include <NumTypes.hpp>

struct RGBColor
{
    u8 r;
    u8 g;
    u8 b;
};

struct RGBAColor
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

struct HSVColor
{
    u32 h;
    float s;
    float v;
};

struct HSVAColor
{
    u32 h;
    float s;
    float v;
    u8 a;
};

struct HSLColor
{
    u32 h;
    float s;
    float l;
};

struct HSLAColor
{
    u32 h;
    float s;
    float l;
    u8 a;
};

struct YUVColor
{
    float y;
    float u;
    float v;
};

struct YUVAColor
{
    float y;
    float u;
    float v;
    u8 a;
};

struct CMYKColor
{
    float c;
    float m;
    float y;
    float k;
};

struct CMYKAColor
{
    float c;
    float m;
    float y;
    float k;
    u8 a;
};

struct YCbCrColor
{
    float y;
    float cb;
    float cr;
};

struct YCbCrAColor
{
    float y;
    float cb;
    float cr;
    u8 a;
};

typedef u32 hexXRGB;
typedef u32 hexRGBX;
typedef u32 hexRGBA;
typedef u32 hexARGB;

HSVColor rgbToHsv(RGBColor rgb) noexcept;

static inline HSVAColor rgbaToHsva(RGBAColor rgba) noexcept
{
    const HSVColor hsv = rgbToHsv({ rgba.r, rgba.g, rgba.b });
    return HSVAColor { hsv.h, hsv.s, hsv.v, rgba.a };
}

HSLColor rgbToHsl(RGBColor rgb) noexcept;

static inline HSLAColor rgbaToHsla(RGBAColor rgba) noexcept
{
    const HSLColor hsl = rgbToHsl({ rgba.r, rgba.g, rgba.b });
    return HSLAColor { hsl.h, hsl.s, hsl.l, rgba.a };
}

YUVColor rgbToYuv(RGBColor rgb) noexcept;

static inline YUVAColor rgbaToYuva(RGBAColor rgba) noexcept
{
    const YUVColor yuv = rgbToYuv({ rgba.r, rgba.g, rgba.b });
    return YUVAColor { yuv.y, yuv.u, yuv.v, rgba.a };
}

CMYKColor rgbToCmyk(RGBColor rgb) noexcept;

static inline CMYKAColor rgbaToCmyka(RGBAColor rgba) noexcept
{
    const CMYKColor cmyk = rgbToCmyk({ rgba.r, rgba.g, rgba.b });
    return CMYKAColor { cmyk.c, cmyk.m, cmyk.y, cmyk.k, rgba.a };
}

YCbCrColor rgbToYCbCr(RGBColor rgb) noexcept;

static inline YCbCrAColor rgbaToYCbCra(RGBAColor rgba) noexcept
{
    const YCbCrColor yCbCr = rgbToYCbCr({ rgba.r, rgba.g, rgba.b });
    return YCbCrAColor { yCbCr.y, yCbCr.cb, yCbCr.cr, rgba.a };
}

static inline hexXRGB rgbToXHex(RGBColor rgb) noexcept
{
    hexXRGB hex = 0;
    hex |= rgb.r << 0x10;
    hex |= rgb.g << 0x08;
    hex |= rgb.b << 0x00;
    return hex;
}

static inline hexRGBX rgbToHexX(RGBColor rgb) noexcept
{
    hexXRGB hex = 0;
    hex |= rgb.r << 0x18;
    hex |= rgb.g << 0x10;
    hex |= rgb.b << 0x08;
    return hex;
}

static inline hexRGBA rgbaToHexA(RGBAColor rgba) noexcept
{
    hexRGBA hex = 0;
    hex |= rgba.r << 0x18;
    hex |= rgba.g << 0x10;
    hex |= rgba.b << 0x08;
    hex |= rgba.a << 0x00;
    return hex;
}

static inline hexARGB rgbaToAHex(RGBAColor rgba) noexcept
{
    hexRGBA hex = 0;
    hex |= rgba.a << 0x18;
    hex |= rgba.r << 0x10;
    hex |= rgba.g << 0x08;
    hex |= rgba.b << 0x00;
    return hex;
}


RGBColor hsvToRgb(HSVColor hsv) noexcept;

static inline RGBAColor hsvaToRgba(HSVAColor hsva) noexcept
{
    const RGBColor rgb = hsvToRgb({ hsva.h, hsva.s, hsva.v });
    return RGBAColor { rgb.r, rgb.g, rgb.b, hsva.a };
}

RGBColor hslToRgb(HSLColor hsl) noexcept;

static inline RGBAColor hslaToRgba(HSLAColor hsla) noexcept
{
    const RGBColor rgb = hslToRgb({ hsla.h, hsla.s, hsla.l });
    return RGBAColor { rgb.r, rgb.g, rgb.b, hsla.a };
}

RGBColor yuvToRgb(YUVColor yuv) noexcept;

static inline RGBAColor yuvaToRgba(YUVAColor yuva) noexcept
{
    const RGBColor rgb = yuvToRgb({ yuva.y, yuva.u, yuva.v });
    return RGBAColor { rgb.r, rgb.g, rgb.b, yuva.a };
}

RGBColor cmykToRgb(CMYKColor cmyk) noexcept;

static inline RGBAColor cmykaToRgba(CMYKAColor cmyka) noexcept
{
    const RGBColor rgb = cmykToRgb({ cmyka.c, cmyka.m, cmyka.y, cmyka.k });
    return RGBAColor { rgb.r, rgb.g, rgb.b, cmyka.a };
}

RGBColor yCbCrToRgb(YCbCrColor yCbCr) noexcept;

static inline RGBAColor yCbCraToRgba(YCbCrAColor yCbCrA) noexcept
{
    const RGBColor rgb = yCbCrToRgb({ yCbCrA.y, yCbCrA.cb, yCbCrA.cr });
    return RGBAColor { rgb.r, rgb.g, rgb.b, yCbCrA.a };
}

static inline RGBColor xHexToRgb(hexXRGB hex) noexcept
{
    RGBColor rgb = { 0, 0, 0};

    rgb.r = (hex >> 0x10) & 0xFF;
    rgb.g = (hex >> 0x08) & 0xFF;
    rgb.b = (hex >> 0x00) & 0xFF;

    return rgb;
}

static inline RGBColor hexXToRgb(hexRGBX hex) noexcept
{
    RGBColor rgb = { 0, 0, 0 };

    rgb.r = (hex >> 0x18) & 0xFF;
    rgb.g = (hex >> 0x10) & 0xFF;
    rgb.b = (hex >> 0x08) & 0xFF;

    return rgb;
}

static inline RGBAColor hexAToRgba(hexRGBA hex) noexcept
{
    RGBAColor rgba = { 0, 0, 0, 0 };

    rgba.r = (hex >> 0x18) & 0xFF;
    rgba.g = (hex >> 0x10) & 0xFF;
    rgba.b = (hex >> 0x08) & 0xFF;
    rgba.a = (hex >> 0x00) & 0xFF;

    return rgba;
}

static inline RGBAColor aHexToRgba(hexARGB hex) noexcept
{
    RGBAColor rgba = { 0, 0, 0, 0 };

    rgba.a = (hex >> 0x18) & 0xFF;
    rgba.r = (hex >> 0x10) & 0xFF;
    rgba.g = (hex >> 0x08) & 0xFF;
    rgba.b = (hex >> 0x00) & 0xFF;

    return rgba;
}
