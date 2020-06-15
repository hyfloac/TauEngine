#pragma once

#include <NumTypes.hpp>
#include <TUMaths.hpp>

namespace ETexture {

enum class Type
{
    Texture1D = 1,
    Texture1DArray,
    Texture2D,
    Texture2DArray,
    Texture3D,
    TextureCube,
    TextureCubeArray
};

enum class Filter
{
    Nearest = 1,
    Point = Nearest,
    Linear
};

enum class WrapMode
{
    ClampToEdge = 1,
    ClampToBorder,
    MirroredRepeat,
    Repeat,
    MirrorClampToEdge
};

enum class CompareFunc
{
    LessThanOrEqual = 1,
    GreaterThanOrEqual,
    LessThan,
    GreaterThan,
    Equal,
    NotEqual,
    Always,
    Never
};

enum class Format
{
    Red8UnsignedInt = 1,
    Red16UnsignedInt,
    Red32UnsignedInt,
    RedGreen8UnsignedInt,
    RedGreen16UnsignedInt,
    RedGreen32UnsignedInt,
    RedGreenBlue8UnsignedInt,
    RedGreenBlue16UnsignedInt,
    RedGreenBlue32UnsignedInt,
    RedGreenBlueAlpha8UnsignedInt,
    RedGreenBlueAlpha16UnsignedInt,
    RedGreenBlueAlpha32UnsignedInt,
    Red16Float,
    Red32Float,
    RedGreen16Float,
    RedGreen32Float,
    RedGreenBlue16Float,
    RedGreenBlue32Float,
    RedGreenBlueAlpha16Float,
    RedGreenBlueAlpha32Float,
    Depth24Stencil8,
    Depth24Typeless8,
    Typeless24Stencil8,
    Red8Typeless,
    Red16Typeless,
    Red32Typeless,
    RedGreen8Typeless,
    RedGreen16Typeless,
    RedGreen32Typeless,
    RedGreenBlue8Typeless,
    RedGreenBlue16Typeless,
    RedGreenBlue32Typeless,
    RedGreenBlueAlpha8Typeless,
    RedGreenBlueAlpha16Typeless,
    RedGreenBlueAlpha32Typeless,
    Typeless24Typeless8,
    MIN = Red8UnsignedInt,
    MAX = Typeless24Typeless8,
    MIN_TYPELESS = Red8Typeless,
    MAX_TYPELESS = Typeless24Typeless8,
    MIN_UINT = Red8UnsignedInt,
    MAX_UINT = RedGreenBlueAlpha32UnsignedInt,
    MIN_FLOAT = Red16Float,
    MAX_FLOAT = RedGreenBlueAlpha32Float
};

enum class CubeSide
{
    Front = 1,
    Back,
    Left,
    Right,
    Top,
    Bottom
};

enum class BindFlags
{
    None            = 0,
    /**
     * Allows for the texture to be bound as render target.
     *
     * Used for framebuffers.
     *
     *   Not required for mipmap generation. This differs
     * from DirectX
     */
    RenderTarget    = 1 << 0,
    /**
     * Allows for the texture to be read by a shader.
     *
     *   This is useful if you only need to render to texture
     * but never need to read the contents.
     */
    ShaderAccess    = 1 << 1,
    /**
     * Required for automatic mipmap generation
     */
    GenerateMipmaps = 1 << 2
};

enum class DepthStencilBindFlags
{
    None                = 0,
    /**
     * Allows for the texture to be bound as render target.
     *
     * Used for framebuffers.
     */
    RenderTarget        = 1 << 0,
    /**
     * Allows for the depth texture to be read by a shader.
     *
     *   This is useful if you only need to render to texture
     * but never need to read the contents (like a depth stencil
     * framebuffer).
     */
    DepthShaderAccess   = 1 << 1,
    /**
     * Allows for the stencil texture to be read by a shader.
     *
     *   This is useful if you only need to render to texture
     * but never need to read the contents (like a depth stencil
     * framebuffer).
     */
    StencilShaderAccess = 1 << 2
};

static constexpr inline uSys bytesPerComponent(const Format format) noexcept
{
    switch(format)
    {
        case Format::Red8UnsignedInt:                return 1;
        case Format::Red16UnsignedInt:               return 2;
        case Format::Red32UnsignedInt:               return 4;
        case Format::RedGreen8UnsignedInt:           return 1;
        case Format::RedGreen16UnsignedInt:          return 2;
        case Format::RedGreen32UnsignedInt:          return 4;
        case Format::RedGreenBlue8UnsignedInt:       return 1;
        case Format::RedGreenBlue16UnsignedInt:      return 2;
        case Format::RedGreenBlue32UnsignedInt:      return 4;
        case Format::RedGreenBlueAlpha8UnsignedInt:  return 1;
        case Format::RedGreenBlueAlpha16UnsignedInt: return 2;
        case Format::RedGreenBlueAlpha32UnsignedInt: return 4;
        case Format::Red16Float:                     return 2;
        case Format::Red32Float:                     return 4;
        case Format::RedGreen16Float:                return 2;
        case Format::RedGreen32Float:                return 4;
        case Format::RedGreenBlue16Float:            return 2;
        case Format::RedGreenBlue32Float:            return 4;
        case Format::RedGreenBlueAlpha16Float:       return 2;
        case Format::RedGreenBlueAlpha32Float:       return 4;
        case Format::Red8Typeless:                   return 1;
        case Format::Red16Typeless:                  return 2;
        case Format::Red32Typeless:                  return 4;
        case Format::RedGreen8Typeless:              return 1;
        case Format::RedGreen16Typeless:             return 2;
        case Format::RedGreen32Typeless:             return 4;
        case Format::RedGreenBlue8Typeless:          return 1;
        case Format::RedGreenBlue16Typeless:         return 2;
        case Format::RedGreenBlue32Typeless:         return 4;
        case Format::RedGreenBlueAlpha8Typeless:     return 1;
        case Format::RedGreenBlueAlpha16Typeless:    return 2;
        case Format::RedGreenBlueAlpha32Typeless:    return 4;
        default:                                     return 0;
    } 
}

static constexpr inline uSys numComponents(const Format format) noexcept
{
    switch(format)
    {
        case Format::Red8UnsignedInt:                return 1;
        case Format::Red16UnsignedInt:               return 1;
        case Format::Red32UnsignedInt:               return 1;
        case Format::RedGreen8UnsignedInt:           return 2;
        case Format::RedGreen16UnsignedInt:          return 2;
        case Format::RedGreen32UnsignedInt:          return 2;
        case Format::RedGreenBlue8UnsignedInt:       return 3;
        case Format::RedGreenBlue16UnsignedInt:      return 3;
        case Format::RedGreenBlue32UnsignedInt:      return 3;
        case Format::RedGreenBlueAlpha8UnsignedInt:  return 4;
        case Format::RedGreenBlueAlpha16UnsignedInt: return 4;
        case Format::RedGreenBlueAlpha32UnsignedInt: return 4;
        case Format::Red16Float:                     return 1;
        case Format::Red32Float:                     return 1;
        case Format::RedGreen16Float:                return 2;
        case Format::RedGreen32Float:                return 2;
        case Format::RedGreenBlue16Float:            return 3;
        case Format::RedGreenBlue32Float:            return 3;
        case Format::RedGreenBlueAlpha16Float:       return 4;
        case Format::RedGreenBlueAlpha32Float:       return 4;
        case Format::Red8Typeless:                   return 1;
        case Format::Red16Typeless:                  return 1;
        case Format::Red32Typeless:                  return 1;
        case Format::RedGreen8Typeless:              return 2;
        case Format::RedGreen16Typeless:             return 2;
        case Format::RedGreen32Typeless:             return 2;
        case Format::RedGreenBlue8Typeless:          return 3;
        case Format::RedGreenBlue16Typeless:         return 3;
        case Format::RedGreenBlue32Typeless:         return 3;
        case Format::RedGreenBlueAlpha8Typeless:     return 4;
        case Format::RedGreenBlueAlpha16Typeless:    return 4;
        case Format::RedGreenBlueAlpha32Typeless:    return 4;
        default:                                     return 0;
    }
}

static constexpr inline uSys bytesPerPixel(const Format format) noexcept
{
    switch(format)
    {
        case Format::Depth24Stencil8:    
        case Format::Depth24Typeless8:   
        case Format::Typeless24Stencil8: 
        case Format::Typeless24Typeless8: return 4;
        default: break;
    }

    return bytesPerComponent(format) * numComponents(format);
}

static constexpr bool isCompatible(const Format formatA, const Format formatB)
{
    if(formatA == formatB)
    { return true; }

    switch(formatA)
    {
        case Format::Depth24Stencil8:    
        case Format::Depth24Typeless8:   
        case Format::Typeless24Stencil8: 
        case Format::Typeless24Typeless8:
        {
            switch(formatB)
            {
                case Format::Depth24Stencil8:
                case Format::Depth24Typeless8:
                case Format::Typeless24Stencil8:
                case Format::Typeless24Typeless8: return true;
                default:                          return false;
            }
        }
        default: break;
    }

    return bytesPerComponent(formatA) == bytesPerComponent(formatB) && numComponents(formatA) == numComponents(formatB);
}

static inline uSys computeMipSide(const u32 width) noexcept
{ return maxT(width / 2, 1u); }

static inline uSys computeMipSide(const u32 width, const u32 mipLevel) noexcept
{
    const u32 divisor = 1 << mipLevel;
    return maxT(width / divisor, 1u);
}

static inline uSys computeMipLevels(const u32 width) noexcept
{ return 1 + log2i(width); }

static inline uSys computeMipLevels(const u32 width, const u32 height) noexcept
{ return 1 + log2i(maxT(width, height)); }

static inline uSys computeMipLevels(const u32 width, const u32 height, const u32 depth) noexcept
{ return 1 + log2i(maxT(width, height, depth)); }

static inline uSys computeSubResource(const u32 mipTarget, const u32 arrayIndex, const u32 mipmapLevels) noexcept
{ return mipTarget + (arrayIndex * mipmapLevels); }

static inline uSys computeSubResourceSize(const Format format, const u32 width) noexcept
{ return width * bytesPerPixel(format); }

static inline uSys computeSubResourceSize(const Format format, const u32 width, const u32 height) noexcept
{ return width * height * bytesPerPixel(format); }

static inline uSys computeSubResourceSize(const Format format, const u32 width, const u32 height, const u32 depth) noexcept
{ return width * height * depth * bytesPerPixel(format); }

static inline uSys computeSubResourceSizeMip(const Format format, const u32 width, const u32 mipLevel) noexcept
{
    const u32 divisor = 1 << mipLevel;
    return (width / divisor) * bytesPerPixel(format);
}

static inline uSys computeSubResourceSizeMip(const Format format, const u32 width, const u32 height, const u32 mipLevel) noexcept
{
    const u32 divisor = 1 << mipLevel;
    return (width / divisor) * (height / divisor) * bytesPerPixel(format);
}

static inline uSys computeSubResourceSizeMip(const Format format, const u32 width, const u32 height, const u32 depth, const u32 mipLevel) noexcept
{
    const u32 divisor = 1 << mipLevel;
    return (width / divisor) * (height / divisor) * (depth / divisor) * bytesPerPixel(format);
}

static inline uSys computeSize(const Format format, const u32 width) noexcept
{
    uSys size = 0;

    for(u32 w = width; w > 1; w = computeMipSide(w))
    {
        size += w;
    }

    return size * bytesPerPixel(format);
}

static inline uSys computeSizeArr(const Format format, const u32 width, const u32 arrayCount) noexcept
{
    uSys size = 0;

    for(u32 w = width; w > 1; w = computeMipSide(w))
    {
        size += w;
    }

    return size * arrayCount * bytesPerPixel(format);
}

static inline uSys computeSize(const Format format, const u32 width, const u32 height) noexcept
{
    uSys size = 0;

    for(u32 w = width, h = height; w > 1 && h > 1; w = computeMipSide(w), h = computeMipSide(h))
    {
        size += w * h;
    }

    return size * bytesPerPixel(format);
}

static inline uSys computeSizeArr(const Format format, const u32 width, const u32 height, const u32 arrayCount) noexcept
{
    uSys size = 0;

    for(u32 w = width, h = height; w > 1 && h > 1; w = computeMipSide(w), h = computeMipSide(h))
    {
        size += w * h;
    }

    return size * arrayCount * bytesPerPixel(format);
}

static inline uSys computeSize(const Format format, const u32 width, const u32 height, const u32 depth) noexcept
{
    uSys size = 0;

    for(u32 w = width, h = height, d = depth; w > 1 && h > 1 && d > 1; w = computeMipSide(w), h = computeMipSide(h), d = computeMipSide(d))
    {
        size += w * h * d;
    }

    return size * bytesPerPixel(format);
}

static inline uSys computeSizeMip(const Format format, const u32 width, const u32 mipmapLevels) noexcept
{
    uSys size = 0;

    for(u32 w = width, m = mipmapLevels; w > 1 && m > 0; w = computeMipSide(w), --m)
    {
        size += w;
    }

    return size * bytesPerPixel(format);
}

static inline uSys computeSizeMipArr(const Format format, const u32 width, const u32 mipmapLevels, const u32 arrayCount) noexcept
{
    uSys size = 0;

    for(u32 w = width, m = mipmapLevels; w > 1 && m > 0; w = computeMipSide(w), --m)
    {
        size += w;
    }

    return size * arrayCount * bytesPerPixel(format);
}

static inline uSys computeSizeMip(const Format format, const u32 width, const u32 height, const u32 mipmapLevels) noexcept
{
    uSys size = 0;

    for(u32 w = width, h = height, m = mipmapLevels; w > 1 && h > 1 && m > 0; w = computeMipSide(w), h = computeMipSide(h), --m)
    {
        size += w * h;
    }

    return size * bytesPerPixel(format);
}

static inline uSys computeSizeMipArr(const Format format, const u32 width, const u32 height, const u32 mipmapLevels, const u32 arrayCount) noexcept
{
    uSys size = 0;

    for(u32 w = width, h = height, m = mipmapLevels; w > 1 && h > 1 && m > 0; w = computeMipSide(w), h = computeMipSide(h), --m)
    {
        size += w * h;
    }

    return size * arrayCount * bytesPerPixel(format);
}

static inline uSys computeSizeMip(const Format format, const u32 width, const u32 height, const u32 depth, const u32 mipmapLevels) noexcept
{
    uSys size = 0;

    for(u32 w = width, h = height, d = depth, m = mipmapLevels; w > 1 && h > 1 && d > 1 && m > 0; w = computeMipSide(w), h = computeMipSide(h), d = computeMipSide(d), --m)
    {
        size += w * h * d;
    }

    return size * bytesPerPixel(format);
}

}
