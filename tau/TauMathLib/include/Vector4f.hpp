#pragma once

#include <xmmintrin.h>
#include "TauMathLibInternal.h"

class TAU_DLL Vector4f final
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
            float w;
        };
    } ALIGN_GCC(16);
public:
    static Vector4f add(Vector4f a, Vector4f b) noexcept;
    static Vector4f sub(Vector4f a, Vector4f b) noexcept;
    static Vector4f mul(Vector4f a, Vector4f b) noexcept;
    static Vector4f div(Vector4f a, Vector4f b) noexcept;

    static Vector4f add(Vector4f a, float b) noexcept;
    static Vector4f sub(Vector4f a, float b) noexcept;
    static Vector4f mul(Vector4f a, float b) noexcept;
    static Vector4f div(Vector4f a, float b) noexcept;

    static Vector4f div(float a, Vector4f b) noexcept;

    static Vector4f negate(Vector4f a) noexcept;

    static float magnitudeSquared(Vector4f a) noexcept;
    static float magnitude(Vector4f a) noexcept;
    static float inverseMagnitude(Vector4f a) noexcept;

    static float lengthSquared(const Vector4f a) noexcept { return magnitudeSquared(a); }
    static float length(const Vector4f a) noexcept { return magnitude(a); }
    static float inverseLength(const Vector4f a) noexcept { return inverseMagnitude(a); }

    static Vector4f normalize(Vector4f a) noexcept;
    static Vector4f normalizeExact(Vector4f a) noexcept;

    static float dot(Vector4f a, Vector4f b) noexcept;
public:
    inline Vector4f() noexcept
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
        , w(0.0f)
    { }

    inline Vector4f(const float _x, const float _y, const float _z, const float _w) noexcept
        : x(_x)
        , y(_y)
        , z(_z)
        , w(_w)
    { }

    inline Vector4f(const float filler) noexcept
        : x(filler)
        , y(filler)
        , z(filler)
        , w(filler)
    { }

    inline Vector4f(const __m128 _vec)
        : vec(_vec)
    { }

    inline ~Vector4f() noexcept = default;

    inline Vector4f(const Vector4f& copy) noexcept = default;
    inline Vector4f(Vector4f&& move) noexcept = default;

    inline Vector4f& operator=(const Vector4f& copy) noexcept = default;
    inline Vector4f& operator=(Vector4f && move) noexcept = default;

    inline Vector4f& operator=(const float filler) noexcept
    {
        x = filler;
        y = filler;
        z = filler;
        w = filler;

        return *this;
    }
};

[[nodiscard]] static inline Vector4f operator+(const Vector4f a, const Vector4f b) noexcept
{ return Vector4f::add(a, b); }

[[nodiscard]] static inline Vector4f operator-(const Vector4f a, const Vector4f b) noexcept
{ return Vector4f::sub(a, b); }

[[nodiscard]] static inline Vector4f operator*(const Vector4f a, const Vector4f b) noexcept
{ return Vector4f::mul(a, b); }

[[nodiscard]] static inline Vector4f operator/(const Vector4f a, const Vector4f b) noexcept
{ return Vector4f::div(a, b); }
