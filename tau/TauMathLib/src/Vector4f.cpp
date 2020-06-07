#include "Vector4f.hpp"
#include "Vector4fIntrin.h"

Vector4f Vector4f::add(const Vector4f a, const Vector4f b) noexcept
{ return vector4f_add(a.vec, b.vec); }

Vector4f Vector4f::sub(const Vector4f a, const Vector4f b) noexcept
{ return vector4f_sub(a.vec, b.vec); }

Vector4f Vector4f::mul(const Vector4f a, const Vector4f b) noexcept
{ return vector4f_mul(a.vec, b.vec); }

Vector4f Vector4f::div(const Vector4f a, const Vector4f b) noexcept
{ return vector4f_div(a.vec, b.vec); }

Vector4f Vector4f::add(const Vector4f a, const float b) noexcept
{ return vector4f_addScalar(a.vec, b); }

Vector4f Vector4f::sub(const Vector4f a, const float b) noexcept
{ return vector4f_subScalar(a.vec, b); }

Vector4f Vector4f::mul(const Vector4f a, const float b) noexcept
{ return vector4f_mulScalar(a.vec, b); }

Vector4f Vector4f::div(const Vector4f a, const float b) noexcept
{ return vector4f_divScalar(a.vec, b); }

Vector4f Vector4f::div(const float a, const Vector4f b) noexcept
{ return vector4f_divScalarInv(a, b.vec); }

Vector4f Vector4f::negate(const Vector4f a) noexcept
{ return vector4f_neg(a.vec); }

float Vector4f::magnitudeSquared(const Vector4f a) noexcept
{ return vector4f_magnitudeSquared(a.vec); }

float Vector4f::magnitude(const Vector4f a) noexcept
{ return vector4f_magnitude(a.vec); }

float Vector4f::inverseMagnitude(const Vector4f a) noexcept
{ return vector4f_inverseMagnitude(a.vec); }

Vector4f Vector4f::normalize(const Vector4f a) noexcept
{ return vector4f_normalizeFast(a.vec); }

Vector4f Vector4f::normalizeExact(const Vector4f a) noexcept
{ return vector4f_normalizeExact(a.vec); }

float Vector4f::dot(const Vector4f a, const Vector4f b) noexcept
{ return vector4f_dot(a.vec, b.vec); }
