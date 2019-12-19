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
    Depth32FloatStencil8UnsignedInt,
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

}
