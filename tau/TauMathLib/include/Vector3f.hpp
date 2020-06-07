#pragma once

#include <xmmintrin.h>
#include "TauMathLibInternal.h"

class TAU_DLL Vector3f
{
public:
    union ALIGN_MSVC(16)
    {
        __m128 vec;
        struct
        {
            float x;
            float y;
            float z;
        };
    } ALIGN_GCC(16);
public:
    static Vector3f add(Vector3f a, Vector3f b) noexcept;
    static Vector3f sub(Vector3f a, Vector3f b) noexcept;
    static Vector3f mul(Vector3f a, Vector3f b) noexcept;
    static Vector3f div(Vector3f a, Vector3f b) noexcept;

    static Vector3f add(Vector3f a, float b) noexcept;
    static Vector3f sub(Vector3f a, float b) noexcept;
    static Vector3f mul(Vector3f a, float b) noexcept;
    static Vector3f div(Vector3f a, float b) noexcept;

    static Vector3f div(float a, Vector3f b) noexcept;

    static Vector3f negate(Vector3f a) noexcept;

    static float magnitudeSquared(Vector3f a) noexcept;
    static float magnitude(Vector3f a) noexcept;
    static float inverseMagnitude(Vector3f a) noexcept;

    static float lengthSquared(const Vector3f a) noexcept { return magnitudeSquared(a); }
    static float length(const Vector3f a) noexcept { return magnitude(a); }
    static float inverseLength(const Vector3f a) noexcept { return inverseMagnitude(a); }

    static Vector3f normalize(Vector3f a) noexcept;
    static Vector3f normalizeExact(Vector3f a) noexcept;

    static float dot(Vector3f a, Vector3f b) noexcept;
    static Vector3f cross(Vector3f a, Vector3f b) noexcept;
public:
    inline Vector3f() noexcept
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
    { }

    inline Vector3f(const float _x, const float _y, const float _z) noexcept
        : x(_x)
        , y(_y)
        , z(_z)
    { }

    inline Vector3f(const float filler) noexcept
        : x(filler)
        , y(filler)
        , z(filler)
    { }

    inline Vector3f(const __m128 _vec)
        : vec(_vec)
    { }

    inline ~Vector3f() noexcept = default;

    inline Vector3f(const Vector3f& copy) noexcept = default;
    inline Vector3f(Vector3f&& move) noexcept = default;

    inline Vector3f& operator=(const Vector3f& copy) noexcept = default;
    inline Vector3f& operator=(Vector3f&& move) noexcept = default;

    inline Vector3f& operator=(const float filler) noexcept
    {
        x = filler;
        y = filler;
        z = filler;

        return *this;
    }
};