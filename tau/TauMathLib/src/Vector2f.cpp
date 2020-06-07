#include "Vector2f.hpp"
#include "Vector2fIntrin.h"

Vector2f Vector2f::add(const Vector2f a, const Vector2f b) noexcept
{
    Vector2f ret { };
    vector2f_add(&a.vec, &b.vec, &ret.vec);
    return ret;
}

Vector2f Vector2f::sub(const Vector2f a, const Vector2f b) noexcept
{
    Vector2f ret { };
    vector2f_sub(&a.vec, &b.vec, &ret.vec);
    return ret;
}

Vector2f Vector2f::mul(const Vector2f a, const Vector2f b) noexcept
{
    Vector2f ret { };
    vector2f_mul(&a.vec, &b.vec, &ret.vec);
    return ret;
}

Vector2f Vector2f::div(const Vector2f a, const Vector2f b) noexcept
{
    Vector2f ret { };
    vector2f_div(&a.vec, &b.vec, &ret.vec);
    return ret;
}

Vector2f Vector2f::add(const Vector2f a, const float b) noexcept
{
    Vector2f ret { };
    vector2f_addScalar(&a.vec, b, &ret.vec);
    return ret;
}

Vector2f Vector2f::sub(const Vector2f a, const float b) noexcept
{
    Vector2f ret { };
    vector2f_subScalar(&a.vec, b, &ret.vec);
    return ret;
}

Vector2f Vector2f::mul(const Vector2f a, const float b) noexcept
{
    Vector2f ret { };
    vector2f_mulScalar(&a.vec, b, &ret.vec);
    return ret;
}

Vector2f Vector2f::div(const Vector2f a, const float b) noexcept
{
    Vector2f ret { };
    vector2f_divScalar(&a.vec, b, &ret.vec);
    return ret;
}

Vector2f Vector2f::div(const float a, const Vector2f b) noexcept
{
    Vector2f ret { };
    vector2f_divScalarInv(a, &b.vec, &ret.vec);
    return ret;
}

Vector2f Vector2f::negate(const Vector2f a) noexcept
{
    Vector2f ret { };
    vector2f_neg(&a.vec, &ret.vec);
    return ret;
}

float Vector2f::magnitudeSquared(const Vector2f a) noexcept
{ return vector2f_magnitudeSquared(&a.vec); }

float Vector2f::magnitude(const Vector2f a) noexcept
{ return vector2f_magnitude(&a.vec); }

float Vector2f::inverseMagnitude(const Vector2f a) noexcept
{ return vector2f_inverseMagnitude(&a.vec); }

Vector2f Vector2f::normalize(const Vector2f a) noexcept
{
    Vector2f ret { };
    vector2f_normalizeFast(&a.vec, &ret.vec);
    return ret;
}

Vector2f Vector2f::normalizeExact(const Vector2f a) noexcept
{
    Vector2f ret { };
    vector2f_normalizeExact(&a.vec, &ret.vec);
    return ret;
}

float Vector2f::dot(const Vector2f a, const Vector2f b) noexcept
{ return vector2f_dot(&a.vec, &b.vec); }
