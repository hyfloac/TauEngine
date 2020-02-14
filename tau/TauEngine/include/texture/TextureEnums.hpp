#pragma once

#include <NumTypes.hpp>

namespace ETexture {

enum class Type : u8
{
    T2D = 1,
    T3D,
    Cube,
    Depth
};

enum class Filter : u8
{
    Nearest = 1,
    Point = Nearest,
    Linear
};

enum class WrapMode : u8
{
    ClampToEdge = 1,
    ClampToBorder,
    MirroredRepeat,
    Repeat,
    MirrorClampToEdge
};

enum class DepthCompareFunc : u8
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

enum class Format : u16
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
    Depth16UnsignedInt,
    Depth32UnsignedInt,
    Depth32Float,
    Stencil8UnsignedInt,
    Stencil16UnsignedInt,
    Depth24Stencil8,
    Depth32FloatStencil8UnsignedInt
};

enum class CubeSide : u8
{
    Front = 1,
    Back,
    Left,
    Right,
    Top,
    Bottom
};

static inline uSys bytesPerPixel(const Format format) noexcept
{
    switch(format)
    {
        case Format::Red8UnsignedInt:                 return 1 * 1;
        case Format::Red16UnsignedInt:                return 2 * 1;
        case Format::Red32UnsignedInt:                return 4 * 1;
        case Format::RedGreen8UnsignedInt:            return 1 * 2;
        case Format::RedGreen16UnsignedInt:           return 2 * 2;
        case Format::RedGreen32UnsignedInt:           return 4 * 2;
        case Format::RedGreenBlue8UnsignedInt:        return 1 * 3;
        case Format::RedGreenBlue16UnsignedInt:       return 2 * 3;
        case Format::RedGreenBlue32UnsignedInt:       return 4 * 3;
        case Format::RedGreenBlueAlpha8UnsignedInt:   return 1 * 4;
        case Format::RedGreenBlueAlpha16UnsignedInt:  return 2 * 4;
        case Format::RedGreenBlueAlpha32UnsignedInt:  return 4 * 4;
        case Format::Red16Float:                      return 2 * 1;
        case Format::Red32Float:                      return 4 * 1;
        case Format::RedGreen16Float:                 return 2 * 2;
        case Format::RedGreen32Float:                 return 4 * 2;
        case Format::RedGreenBlue16Float:             return 2 * 3;
        case Format::RedGreenBlue32Float:             return 4 * 3;
        case Format::RedGreenBlueAlpha16Float:        return 2 * 4;
        case Format::RedGreenBlueAlpha32Float:        return 4 * 4;
        case Format::Depth16UnsignedInt:              return 2;
        case Format::Depth32UnsignedInt:              return 4;
        case Format::Depth32Float:                    return 4;
        case Format::Stencil8UnsignedInt:             return 1;
        case Format::Stencil16UnsignedInt:            return 2;
        case Format::Depth24Stencil8:                 return 4;
        case Format::Depth32FloatStencil8UnsignedInt: return 5;
        default: return 0;
    }
}

}
