#include "Vector3f.hpp"
#include "Vector3fIntrin.h"

Vector3f Vector3f::add(const Vector3f a, const Vector3f b) noexcept
{ return vector3f_add(a.vec, b.vec); }

Vector3f Vector3f::sub(const Vector3f a, const Vector3f b) noexcept
{ return vector3f_sub(a.vec, b.vec); }

Vector3f Vector3f::mul(const Vector3f a, const Vector3f b) noexcept
{ return vector3f_mul(a.vec, b.vec); }

Vector3f Vector3f::div(const Vector3f a, const Vector3f b) noexcept
{ return vector3f_div(a.vec, b.vec); }

Vector3f Vector3f::add(const Vector3f a, const float b) noexcept
{ return vector3f_addF(a.vec, b); }

Vector3f Vector3f::sub(const Vector3f a, const float b) noexcept
{ return vector3f_subF(a.vec, b); }

Vector3f Vector3f::mul(const Vector3f a, const float b) noexcept
{ return vector3f_mulF(a.vec, b); }

Vector3f Vector3f::div(const Vector3f a, const float b) noexcept
{ return vector3f_divF(a.vec, b); }

Vector3f Vector3f::div(const float a, const Vector3f b) noexcept
{ return vector3f_divFInv(a, b.vec); }

Vector3f Vector3f::negate(const Vector3f a) noexcept
{ return vector3f_neg(a.vec); }

float Vector3f::magnitudeSquared(const Vector3f a) noexcept
{ return vector3f_magnitudeSquared(a.vec); }

float Vector3f::magnitude(const Vector3f a) noexcept
{ return vector3f_magnitude(a.vec); }

float Vector3f::inverseMagnitude(const Vector3f a) noexcept
{ return vector3f_inverseMagnitude(a.vec); }

Vector3f Vector3f::normalize(const Vector3f a) noexcept
{ return vector3f_normalizeFast(a.vec); }

Vector3f Vector3f::normalizeExact(const Vector3f a) noexcept
{ return vector3f_normalizeExact(a.vec); }

float Vector3f::dot(const Vector3f a, const Vector3f b) noexcept
{ return vector3f_dot(a.vec, b.vec); }

Vector3f Vector3f::cross(const Vector3f a, const Vector3f b) noexcept
{ return vector3f_cross(a.vec, b.vec); }
