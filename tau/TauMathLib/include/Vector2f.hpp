#pragma once

#include "TauMathLibInternal.h"
#include "Vector2fBase.h"

class TAU_DLL Vector2f final
{
public:
    union
    {
        _Vector2f vec;
        struct
        {
            float x;
            float y;
        };
    };
public:
    static Vector2f add(Vector2f a, Vector2f b) noexcept;
    static Vector2f sub(Vector2f a, Vector2f b) noexcept;
    static Vector2f mul(Vector2f a, Vector2f b) noexcept;
    static Vector2f div(Vector2f a, Vector2f b) noexcept;

    static Vector2f add(Vector2f a, float b) noexcept;
    static Vector2f sub(Vector2f a, float b) noexcept;
    static Vector2f mul(Vector2f a, float b) noexcept;
    static Vector2f div(Vector2f a, float b) noexcept;

    static Vector2f div(float a, Vector2f b) noexcept;

    static Vector2f negate(Vector2f a) noexcept;

    static float magnitudeSquared(Vector2f a) noexcept;
    static float magnitude(Vector2f a) noexcept;
    static float inverseMagnitude(Vector2f a) noexcept;

    static float lengthSquared(const Vector2f a) noexcept { return magnitudeSquared(a); }
    static float length(const Vector2f a) noexcept { return magnitude(a); }
    static float inverseLength(const Vector2f a) noexcept { return inverseMagnitude(a); }

    static Vector2f normalize(Vector2f a) noexcept;
    static Vector2f normalizeExact(Vector2f a) noexcept;

    static float dot(Vector2f a, Vector2f b) noexcept;
public:
    inline Vector2f() noexcept
        : x(0.0f)
        , y(0.0f)
    { }

    inline Vector2f(const float _x, const float _y) noexcept
        : x(_x)
        , y(_y)
    { }

    inline Vector2f(const float filler) noexcept
        : x(filler)
        , y(filler)
    { }

    inline Vector2f(const _Vector2f _vec) noexcept
        : vec(_vec)
    { }

    inline ~Vector2f() noexcept = default;

    inline Vector2f(const Vector2f& copy) noexcept = default;
    inline Vector2f(Vector2f&& move) noexcept = default;

    inline Vector2f& operator=(const Vector2f& copy) noexcept = default;
    inline Vector2f& operator=(Vector2f&& move) noexcept = default;

    inline Vector2f& operator=(const float filler) noexcept
    {
        x = filler;
        y = filler;

        return *this;
    }
};

[[nodiscard]] static inline Vector2f operator+(const Vector2f a, const Vector2f b) noexcept
{ return Vector2f::add(a, b); }

[[nodiscard]] static inline Vector2f operator-(const Vector2f a, const Vector2f b) noexcept
{ return Vector2f::sub(a, b); }

[[nodiscard]] static inline Vector2f operator*(const Vector2f a, const Vector2f b) noexcept
{ return Vector2f::mul(a, b); }

[[nodiscard]] static inline Vector2f operator/(const Vector2f a, const Vector2f b) noexcept
{ return Vector2f::div(a, b); }
