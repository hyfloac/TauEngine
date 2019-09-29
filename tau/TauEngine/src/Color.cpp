#include <Color.hpp>
#include <maths/Maths.hpp>

HSVColor rgbToHsv(RGBColor rgb) noexcept
{
    i32 h = 0;
    float s;

    const u8 minComponent = minT(rgb.r, rgb.g, rgb.b);
    const u8 maxComponent = maxT(rgb.r, rgb.g, rgb.b);
    const u8 delta = maxComponent - minComponent;

    if(maxComponent == 0)
    {
        s = 0.0f;
    }
    else
    {
        s = static_cast<float>(delta) / static_cast<float>(maxComponent);
    }

    if(s == 0.0f)
    {
        h = 0;
    }
    else
    {
        if(rgb.r == maxComponent)
        {
            h = (rgb.g - rgb.b) / delta;
        }
        else if(rgb.g == maxComponent)
        {
            h = 2 + (rgb.b - rgb.r) / delta;
        }
        else
        {
            h = 4 + (rgb.r - rgb.g) / delta;
        }

        h *= 60;

        if(h < 0)
        {
            h += 360;
        }
    }

    return HSVColor { reinterpret_cast<u32&>(h), s, (static_cast<float>(maxComponent) / 255.0f) };
}

HSLColor rgbToHsl(RGBColor rgb) noexcept
{
    i32 h = 0;
    float s;

    const u8 minComponent = minT(rgb.r, rgb.g, rgb.b);
    const u8 maxComponent = maxT(rgb.r, rgb.g, rgb.b);
    const u8 delta = maxComponent - minComponent;

    const float l = static_cast<float>(maxComponent + minComponent) / 2.0f;

    if(delta == 0)
    {
        return HSLColor { 0, 0.0f, l };
    }

    if(l < 0.5f)
    {
        s = static_cast<float>(delta) / static_cast<float>(maxComponent + minComponent);
    }
    else
    {
        s = static_cast<float>(delta) / static_cast<float>(2 - maxComponent - minComponent);
    }

    if(rgb.r == maxComponent)
    {
        h = (rgb.g - rgb.b) / delta;
    }
    else if(rgb.g == maxComponent)
    {
        h = 2 + (rgb.b - rgb.r) / delta;
    }
    else
    {
        h = 4 + (rgb.r - rgb.g) / delta;
    }

    h *= 60;

    if(h < 0)
    {
        h += 360;
    }

    return HSLColor { reinterpret_cast<u32&>(h), s, l };
}

YUVColor rgbToYuv(RGBColor rgb) noexcept
{
    const float y = static_cast<float>(rgb.r) *  0.299000f + static_cast<float>(rgb.g) *  0.587000f + static_cast<float>(rgb.b) *  0.114000f;
    const float u = static_cast<float>(rgb.r) * -0.168736f + static_cast<float>(rgb.g) * -0.331264f + static_cast<float>(rgb.b) *  0.500000f + 128.0f;
    const float v = static_cast<float>(rgb.r) *  0.500000f + static_cast<float>(rgb.g) * -0.418688f + static_cast<float>(rgb.b) * -0.081312f + 128.0f;

    return YUVColor { y, u, v };
}

CMYKColor rgbToCmyk(RGBColor rgb) noexcept
{
    const float fr = static_cast<float>(rgb.r) / 255.0f;
    const float fg = static_cast<float>(rgb.g) / 255.0f;
    const float fb = static_cast<float>(rgb.b) / 255.0f;

    const float maxComponent = static_cast<float>(maxT(rgb.r, rgb.g, rgb.b));

    const float k = 1.0f - (maxComponent / 255.0f);
    const float c = (1.0f - fr - k) / (1.0f - k);
    const float m = (1.0f - fg - k) / (1.0f - k);
    const float y = (1.0f - fb - k) / (1.0f - k);

    return CMYKColor { c, m, y, k };
}

YCbCrColor rgbToYCbCr(RGBColor rgb) noexcept
{
    const float fr = static_cast<float>(rgb.r) / 255.0f;
    const float fg = static_cast<float>(rgb.g) / 255.0f;
    const float fb = static_cast<float>(rgb.b) / 255.0f;

    const float y  =  0.2989f * fr + 0.5866f * fg + 0.1145f * fb;
    const float cb = -0.1687f * fr - 0.3313f * fg + 0.5000f * fb;
    const float cr =  0.5000f * fr - 0.4184f * fg - 0.0816f * fb;

    return YCbCrColor { y, cb, cr };
}


RGBColor hsvToRgb(HSVColor rgb) noexcept;

RGBColor hslToRgb(HSLColor rgb) noexcept;

RGBColor yuvToRgb(YUVColor rgb) noexcept;

RGBColor cmykToRgb(CMYKColor rgb) noexcept;

RGBColor yCbCrToRgb(YCbCrColor rgb) noexcept;

