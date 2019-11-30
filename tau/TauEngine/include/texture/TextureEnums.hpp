#pragma once

#include <NumTypes.hpp>
#include <Objects.hpp>

class ETexture final
{
    DELETE_CONSTRUCT(ETexture);
    DELETE_DESTRUCT(ETexture);
    DELETE_COPY(ETexture);
public:
    enum class Type : u8
    {
        TEXTURE_2D = 1,
        TEXTURE_3D,
        TEXTURE_CUBE
    };

    enum class Filter : u8
    {
        Nearest = 1,
        Linear
    };

    enum class Format : u16
    {
        Red8UnsignedInt,
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
        RedGreenBlueAlpha32Float
    };
};
