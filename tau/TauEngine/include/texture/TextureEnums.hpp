#pragma once

#include <NumTypes.hpp>

namespace ETexture {

enum class Type
{
    T2D = 1,
    T3D,
    Cube,
    DepthStencil
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
    Depth24Stencil8
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

static inline uSys bytesPerPixel(const Format format) noexcept
{
    switch(format)
    {
        case Format::Red8UnsignedInt:                return 1 * 1;
        case Format::Red16UnsignedInt:               return 2 * 1;
        case Format::Red32UnsignedInt:               return 4 * 1;
        case Format::RedGreen8UnsignedInt:           return 1 * 2;
        case Format::RedGreen16UnsignedInt:          return 2 * 2;
        case Format::RedGreen32UnsignedInt:          return 4 * 2;
        case Format::RedGreenBlue8UnsignedInt:       return 1 * 3;
        case Format::RedGreenBlue16UnsignedInt:      return 2 * 3;
        case Format::RedGreenBlue32UnsignedInt:      return 4 * 3;
        case Format::RedGreenBlueAlpha8UnsignedInt:  return 1 * 4;
        case Format::RedGreenBlueAlpha16UnsignedInt: return 2 * 4;
        case Format::RedGreenBlueAlpha32UnsignedInt: return 4 * 4;
        case Format::Red16Float:                     return 2 * 1;
        case Format::Red32Float:                     return 4 * 1;
        case Format::RedGreen16Float:                return 2 * 2;
        case Format::RedGreen32Float:                return 4 * 2;
        case Format::RedGreenBlue16Float:            return 2 * 3;
        case Format::RedGreenBlue32Float:            return 4 * 3;
        case Format::RedGreenBlueAlpha16Float:       return 2 * 4;
        case Format::RedGreenBlueAlpha32Float:       return 4 * 4;
        case Format::Depth24Stencil8:                return 4;
        default: return 0;
    }
}

}
