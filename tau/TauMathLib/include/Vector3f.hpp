#pragma once

#include <xmmintrin.h>
#include "TauMathLibInternal.h"

class TAU_DLL Vector3f final
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
    [[nodiscard]] static Vector3f add(Vector3f a, Vector3f b) noexcept;
    [[nodiscard]] static Vector3f sub(Vector3f a, Vector3f b) noexcept;
    [[nodiscard]] static Vector3f mul(Vector3f a, Vector3f b) noexcept;
    [[nodiscard]] static Vector3f div(Vector3f a, Vector3f b) noexcept;

    [[nodiscard]] static Vector3f add(Vector3f a, float b) noexcept;
    [[nodiscard]] static Vector3f sub(Vector3f a, float b) noexcept;
    [[nodiscard]] static Vector3f mul(Vector3f a, float b) noexcept;
    [[nodiscard]] static Vector3f div(Vector3f a, float b) noexcept;

    [[nodiscard]] static Vector3f div(float a, Vector3f b) noexcept;

    [[nodiscard]] static Vector3f negate(Vector3f a) noexcept;

    [[nodiscard]] static float magnitudeSquared(Vector3f a) noexcept;
    [[nodiscard]] static float magnitude(Vector3f a) noexcept;
    [[nodiscard]] static float inverseMagnitude(Vector3f a) noexcept;

    [[nodiscard]] static float lengthSquared(const Vector3f a) noexcept { return magnitudeSquared(a); }
    [[nodiscard]] static float length(const Vector3f a) noexcept { return magnitude(a); }
    [[nodiscard]] static float inverseLength(const Vector3f a) noexcept { return inverseMagnitude(a); }

    [[nodiscard]] static Vector3f normalize(Vector3f a) noexcept;
    [[nodiscard]] static Vector3f normalizeExact(Vector3f a) noexcept;

    [[nodiscard]] static float dot(Vector3f a, Vector3f b) noexcept;
    [[nodiscard]] static Vector3f cross(Vector3f a, Vector3f b) noexcept;
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

[[nodiscard]] static inline Vector3f operator+(const Vector3f a, const Vector3f b) noexcept
{ return Vector3f::add(a, b); }

[[nodiscard]] static inline Vector3f operator-(const Vector3f a, const Vector3f b) noexcept
{ return Vector3f::sub(a, b); }

[[nodiscard]] static inline Vector3f operator*(const Vector3f a, const Vector3f b) noexcept
{ return Vector3f::mul(a, b); }

[[nodiscard]] static inline Vector3f operator/(const Vector3f a, const Vector3f b) noexcept
{ return Vector3f::div(a, b); }
