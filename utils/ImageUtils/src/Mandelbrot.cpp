#include "Mandelbrot.hpp"
#include <complex>
#include <cmath>
#include <NumTypes.hpp>

static u8 compute(const float x, const float y, const float width, const float height, const u32 iters) noexcept
{
    const float scaleForm = 2.25f;
    const std::complex<float> point(((x / width) - 0.75f) * scaleForm, ((y / height) - 0.5f) * scaleForm);

    std::complex<float> z(0.0f, 0.0f);
    u32 nbIter;
    for(nbIter = 0; abs(z) < 2 && nbIter <= iters; ++nbIter)
    {
        z = z * z + point;
    }
    if(nbIter < iters)
    {
        return (255 * nbIter) / (iters - 1);
    }
    else
    {
        return 0;
    }
}

TextureBlend* mandelbrot(const u32 width, const u32 height, const u32 iters) noexcept
{
    const u32 len = width * height * 4;
    u8* const ret = new u8[len];
    const float fWidth = static_cast<float>(width);
    const float fHeight = static_cast<float>(height);

    u32 index = 0;

    for(u32 i = 0; i < width; ++i)
    {
        for(u32 j = 0; j < height; ++j)
        {
            // const u32 index = j * height + width;

            const u8 redComp = compute(static_cast<float>(j), static_cast<float>(i), fWidth, fHeight, iters);
            const u8 b = 0;
            const u8 g = 0;
            const u8 a = 0xFF;

            // ret[index * 4 + 0] = b;
            // ret[index * 4 + 1] = g;
            // ret[index * 4 + 2] = redComp;
            // ret[index * 4 + 3] = a;
            ret[index++] = b;
            ret[index++] = g;
            ret[index++] = redComp;
            ret[index++] = a;
        }
    }

    return new TextureBlend(new Texture(width, height, ret, 32));
}
