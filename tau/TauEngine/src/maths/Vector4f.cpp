#pragma warning(push, 0)
#include <cmath>
#pragma warning(pop)

#include <maths/Vector4f.hpp>
#include <maths/Maths.hpp>

#pragma region Constructor
Vector4f::Vector4f(const __m128 vec) noexcept
    : _data({ vec })
{ }

Vector4f::Vector4f(const __m128i vec) noexcept
    : _data({ *((__m128*) &vec) })
{ }

Vector4f::Vector4f(const float x, const float y, const float z, const float w) noexcept
    : _data(x, y, z, w)
{ }

Vector4f::Vector4f(const i32 x, const i32 y, const i32 z, const i32 w) noexcept
    : _data((float) x, (float) y, (float) z, (float) w)
{ }

Vector4f::Vector4f(const float filler) noexcept
    : _data(filler, filler, filler, filler)
{ }

Vector4f::Vector4f(const i32 filler) noexcept
    : _data((float) filler, (float) filler, (float) filler, (float) filler)
{ }

Vector4f::Vector4f() noexcept
    : _data(0, 0, 0, 0)
{ }

Vector4f::Vector4f(const CompVec4& copy) noexcept
    : _data({ copy.vec })
{ }
#pragma endregion

#pragma region Copy Assignment Operators
Vector4f& Vector4f::operator =(const float filler) noexcept
{
    this->_data.x = filler;
    this->_data.y = filler;
    this->_data.z = filler;
    this->_data.w = filler;

    return *this;
}

Vector4f& Vector4f::operator =(const i32 filler) noexcept
{
    this->_data.x = (float) filler;
    this->_data.y = (float) filler;
    this->_data.z = (float) filler;
    this->_data.w = (float) filler;

    return *this;
}

Vector4f& Vector4f::operator =(const CompVec4& copy) noexcept
{
    this->_data.vec = copy.vec;

    return *this;
}
#pragma endregion

Vector4f& Vector4f::add(const float x, const float y, const float z, const float w) noexcept
{
    this->_data.x += x;
    this->_data.y += y;
    this->_data.z += z;
    this->_data.w += w;

    return *this;
}

Vector4f& Vector4f::add(const CompVec4& data) noexcept
{
    this->_data.vec = _mm_add_ps(this->_data.vec, data.vec);

    return *this;
}

Vector4f& Vector4f::sub(const float x, const float y, const float z, const float w) noexcept
{
    this->_data.x -= x;
    this->_data.y -= y;
    this->_data.z -= z;
    this->_data.w -= w;

    return *this;
}

Vector4f& Vector4f::sub(const CompVec4& data) noexcept
{
    this->_data.vec = _mm_sub_ps(this->_data.vec, data.vec);

    return *this;
}

Vector4f Vector4f::addC(const float x, const float y, const float z, const float w) const noexcept
{
    const float _x = this->_data.x + x;
    const float _y = this->_data.y + y;
    const float _z = this->_data.z + z;
    const float _w = this->_data.w + w;

    return { _x, _y, _z, _w };
}

Vector4f Vector4f::addC(const CompVec4& data) const noexcept
{
    return { _mm_add_ps(this->_data.vec, data.vec) };
}

Vector4f Vector4f::subC(const float x, const float y, const float z, const float w) const noexcept
{
    const float _x = this->_data.x - x;
    const float _y = this->_data.y - y;
    const float _z = this->_data.z - z;
    const float _w = this->_data.w - w;

    return { _x, _y, _z, _w };
}

Vector4f Vector4f::subC(const CompVec4& data) const noexcept
{
    return { _mm_sub_ps(this->_data.vec, data.vec) };
}

Vector4f& Vector4f::add(const float scalar) noexcept
{
    this->_data.x += scalar;
    this->_data.y += scalar;
    this->_data.z += scalar;
    this->_data.w += scalar;

    return *this;
}

Vector4f Vector4f::addC(const float scalar) const noexcept
{
    const float x = this->_data.x + scalar;
    const float y = this->_data.y + scalar;
    const float z = this->_data.z + scalar;
    const float w = this->_data.w + scalar;

    return { x, y, z, w };
}

Vector4f& Vector4f::sub(const float scalar) noexcept
{
    this->_data.x -= scalar;
    this->_data.y -= scalar;
    this->_data.z -= scalar;
    this->_data.w -= scalar;

    return *this;
}

Vector4f Vector4f::subC(const float scalar) const noexcept
{
    const float x = this->_data.x - scalar;
    const float y = this->_data.y - scalar;
    const float z = this->_data.z - scalar;
    const float w = this->_data.w - scalar;

    return { x, y, z, w };
}

Vector4f& Vector4f::mul(const float scalar) noexcept
{
    this->_data.x *= scalar;
    this->_data.y *= scalar;
    this->_data.z *= scalar;
    this->_data.w *= scalar;

    return *this;
}

Vector4f Vector4f::mulC(const float scalar) const noexcept
{
    const float x = this->_data.x * scalar;
    const float y = this->_data.y * scalar;
    const float z = this->_data.z * scalar;
    const float w = this->_data.w * scalar;

    return { x, y, z, w };
}

Vector4f& Vector4f::div(const float scalar) noexcept
{
    this->_data.x /= scalar;
    this->_data.y /= scalar;
    this->_data.z /= scalar;
    this->_data.w /= scalar;

    return *this;
}

Vector4f Vector4f::divC(const float scalar) const noexcept
{
    const float x = this->_data.x / scalar;
    const float y = this->_data.y / scalar;
    const float z = this->_data.z / scalar;
    const float w = this->_data.w / scalar;

    return { x, y, z, w };
}

Vector4f& Vector4f::negate() noexcept
{
    this->_data.x = -this->_data.x;
    this->_data.y = -this->_data.y;
    this->_data.z = -this->_data.z;
    this->_data.w = -this->_data.w;

    return *this;
}

Vector4f Vector4f::negateCopy() const noexcept
{
    return { -this->_data.x, -this->_data.y, -this->_data.z, -this->_data.w };
}

#pragma region Magnitude
float Vector4f::magnitudeSquared() const noexcept
{
    CompVec4 comp;
    comp.vec = _mm_mul_ps(this->_data.vec, this->_data.vec);

    return comp.x + comp.y + comp.z + comp.w;
}

float Vector4f::magnitude() const noexcept
{
    return sqrt(this->magnitudeSquared());
}
#pragma endregion

Vector4f Vector4f::inverseSqrt() const noexcept
{
    __m128 inv = _mm_rsqrt_ss(this->_data.vec);

    const __m128 tmp = _mm_mul_ps(inv, _mm_set1_ps(1.5f));

    inv = _mm_mul_ps(inv, inv);
    inv = _mm_mul_ps(this->_data.vec, inv);
    inv = _mm_mul_ps(inv, _mm_set1_ps(0.5f));

    return _mm_sub_ps(tmp, inv);
}

float Vector4f::dot(const float x, const float y, const float z, const float w) const noexcept
{
    return this->_data.x * x + this->_data.y * y + this->_data.z * z + this->_data.w * w;
}

float Vector4f::dot(const CompVec4& data) const noexcept
{ 
    CompVec4 comp;
    comp.vec = _mm_mul_ps(this->_data.vec, data.vec);

    return comp.x + comp.y + comp.z + comp.w;
}

Vector4f& Vector4f::normalize() noexcept
{
    const float mag = this->magnitudeSquared();
    if(mag > 0) { return this->mul(rSqrt(mag)); }
    return *this;
}

Vector4f Vector4f::normalizeC() const noexcept
{
    const float mag = this->magnitudeSquared();
    if(mag > 0) { return this->mulC(rSqrt(mag)); }
    return { this->_data };
}
