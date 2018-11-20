#pragma once

#include <emmintrin.h>
#include <NumTypes.hpp>
#include <Alignment.h>

ALIGN(16) union ComponentVector4
{
    __m128 vec;
    __m128i ivec;
    ALIGN(16) struct
    {
        float x, y, z, w;
    } ALIGN(16);
    ALIGN(16) struct
    {
        i32 ix, iy, iz, iw;
    } ALIGN(16);
    ALIGN(16) struct
    {
        float r, g, b, a;
    } ALIGN(16);
} ALIGN(16);

typedef ComponentVector4 CompVec4;
