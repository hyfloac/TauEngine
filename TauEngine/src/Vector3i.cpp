#pragma warning(push, 0)
#include <cmath>
#pragma warning(pop)

#include <maths/Vector3i.hpp>
#include <maths/Vector3f.hpp>

#pragma region Constructor
Vector3i::Vector3i(const __m128 vec) noexcept
    : _data({ vec })
{ }

Vector3i::Vector3i(const __m128i vec) noexcept
    : _data({ *((__m128*) &vec) })
{ }

Vector3i::Vector3i(const float x, const float y, const float z) noexcept
{
    this->_data.ix = (i32) x;
    this->_data.iy = (i32) y;
    this->_data.iz = (i32) z;
}

Vector3i::Vector3i(const i32 x, const i32 y, const i32 z) noexcept
{
    this->_data.ix = x;
    this->_data.iy = y;
    this->_data.iz = z;
}

Vector3i::Vector3i(const float filler) noexcept
{
    this->_data.ix = (i32) filler;
    this->_data.iy = (i32) filler;
    this->_data.iz = (i32) filler;
}

Vector3i::Vector3i(const i32 filler) noexcept
{
    this->_data.ix = filler;
    this->_data.iy = filler;
    this->_data.iz = filler;
}

Vector3i::Vector3i() noexcept
    : _data{ 0, 0, 0 }
{ }

Vector3i::Vector3i(const CompVec4& data) noexcept
    : _data({ data.vec })
{ }
#pragma endregion

#pragma region Copy Assignment Operators
Vector3i& Vector3i::operator =(const float filler) noexcept
{
    this->_data.ix = (i32) filler;
    this->_data.iy = (i32) filler;
    this->_data.iz = (i32) filler;

    return *this;
}

Vector3i& Vector3i::operator =(const i32 filler) noexcept
{
    this->_data.ix = filler;
    this->_data.iy = filler;
    this->_data.iz = filler;

    return *this;
}

Vector3i& Vector3i::operator =(const CompVec4& copy) noexcept
{
    this->_data.vec = copy.vec;

    return *this;
}
#pragma endregion

Vector3i& Vector3i::add(const i32 x, const i32 y, const i32 z) noexcept
{
    this->_data.ix += x;
    this->_data.iy += y;
    this->_data.iz += z;

    return *this;
}

Vector3i& Vector3i::add(const CompVec4& data) noexcept
{
    this->_data.ivec = _mm_add_epi32(this->_data.ivec, data.ivec);

    return *this;
}

Vector3i& Vector3i::sub(const i32 x, const i32 y, const i32 z) noexcept
{
    this->_data.ix -= x;
    this->_data.iy -= y;
    this->_data.iz -= z;

    return *this;
}

Vector3i& Vector3i::sub(const CompVec4& data) noexcept
{
    this->_data.ivec = _mm_sub_epi32(this->_data.ivec, data.ivec);

    return *this;
}

Vector3i Vector3i::addC(const i32 x, const i32 y, const i32 z) const noexcept
{
    const i32 _x = this->_data.ix + x;
    const i32 _y = this->_data.iy + y;
    const i32 _z = this->_data.iz + z;

    return { _x, _y, _z };
}

Vector3i Vector3i::addC(const CompVec4& data) const noexcept
{
    return { _mm_add_epi32(this->_data.ivec, data.ivec) };
}

Vector3i Vector3i::subC(const i32 x, const i32 y, const i32 z) const noexcept
{
    const i32 _x = this->_data.ix - x;
    const i32 _y = this->_data.iy - y;
    const i32 _z = this->_data.iz - z;

    return { _x, _y, _z };
}

Vector3i Vector3i::subC(const CompVec4& data) const noexcept
{
    return { _mm_sub_epi32(this->_data.ivec, data.ivec) };
}

Vector3i& Vector3i::add(const i32 scalar) noexcept
{
    this->_data.ix += scalar;
    this->_data.iy += scalar;
    this->_data.iz += scalar;

    return *this;
}

Vector3i Vector3i::addC(const i32 scalar) const noexcept
{
    const i32 x = this->_data.ix + scalar;
    const i32 y = this->_data.iy + scalar;
    const i32 z = this->_data.iz + scalar;

    return { x, y, z };
}

Vector3i& Vector3i::sub(const i32 scalar) noexcept
{
    this->_data.ix -= scalar;
    this->_data.iy -= scalar;
    this->_data.iz -= scalar;

    return *this;
}

Vector3i Vector3i::subC(const i32 scalar) const noexcept
{
    const i32 x = this->_data.ix - scalar;
    const i32 y = this->_data.iy - scalar;
    const i32 z = this->_data.iz - scalar;

    return { x, y, z };
}

Vector3i& Vector3i::mul(const float scalar) noexcept
{
    this->_data.ix = (i32) (this->_data.ix * scalar);
    this->_data.iy = (i32) (this->_data.iy * scalar);
    this->_data.iz = (i32) (this->_data.iz * scalar);

    return *this;
}

Vector3i& Vector3i::mul(const i32 scalar) noexcept
{
    this->_data.ix *= scalar;
    this->_data.iy *= scalar;
    this->_data.iz *= scalar;

    return *this;
}

Vector3f Vector3i::mulC(const float scalar) const noexcept
{
    const float x = this->_data.ix * scalar;
    const float y = this->_data.iy * scalar;
    const float z = this->_data.iz * scalar;

    return { x, y, z };
}

Vector3f Vector3i::mulC(const i32 scalar) const noexcept
{
    const i32 x = this->_data.ix * scalar;
    const i32 y = this->_data.iy * scalar;
    const i32 z = this->_data.iz * scalar;

    return { x, y, z };
}

Vector3i& Vector3i::div(const float scalar) noexcept
{
    this->_data.ix = (i32) (this->_data.ix / scalar);
    this->_data.iy = (i32) (this->_data.iy / scalar);
    this->_data.iz = (i32) (this->_data.iz / scalar);

    return *this;
}

Vector3i& Vector3i::div(const i32 scalar) noexcept
{
    this->_data.ix /= scalar;
    this->_data.iy /= scalar;
    this->_data.iz /= scalar;

    return *this;
}

Vector3f Vector3i::divC(const float scalar) const noexcept
{
    const float x = this->_data.ix / scalar;
    const float y = this->_data.iy / scalar;
    const float z = this->_data.iz / scalar;

    return { x, y, z };
}

Vector3f Vector3i::divC(const i32 scalar) const noexcept
{
    const i32 x = this->_data.ix / scalar;
    const i32 y = this->_data.iy / scalar;
    const i32 z = this->_data.iz / scalar;

    return { x, y, z };
}

Vector3i& Vector3i::negate() noexcept
{
    this->_data.ix = -this->_data.ix;
    this->_data.iy = -this->_data.iy;
    this->_data.iz = -this->_data.iz;

    return *this;
}

Vector3i Vector3i::negateCopy() const noexcept
{
    return { -this->_data.ix, -this->_data.iy, -this->_data.iz };
}

#pragma region Magnitude
i32 Vector3i::magnitudeSquared() const noexcept
{
    return this->_data.ix * this->_data.ix + this->_data.iy * this->_data.iy + this->_data.iz * this->_data.iz;
}

float Vector3i::magnitude() const noexcept
{
    return (float) sqrt(this->magnitudeSquared());
}
#pragma endregion

i32 Vector3i::dot(const i32 x, const i32 y, const i32 z) const noexcept
{
    return this->_data.ix * x + this->_data.iy * y + this->_data.iz * z;
}

float Vector3i::dot(const float x, const float y, const float z) const noexcept
{
    return this->_data.ix * x + this->_data.iy * y + this->_data.iz * z;
}

i32 Vector3i::dot(const CompVec4& data) const noexcept
{
    return this->_data.ix * data.ix + this->_data.iy * data.ix + this->_data.iz * data.ix;
}

Vector3i& Vector3i::cross(const i32 x, const i32 y, const i32 z) noexcept
{
    const i32 _x = this->_data.iy * z + this->_data.iz * y;
    const i32 _y = this->_data.iz * x + this->_data.ix * z;
    const i32 _z = this->_data.ix * y + this->_data.iy * x;

    this->_data.ix = _x;
    this->_data.iy = _y;
    this->_data.iz = _z;

    return *this;
}

Vector3i& Vector3i::cross(const float x, const float y, const float z) noexcept
{
    const i32 _x = (i32) (this->_data.iy * z + this->_data.iz * y);
    const i32 _y = (i32) (this->_data.iz * x + this->_data.ix * z);
    const i32 _z = (i32) (this->_data.ix * y + this->_data.iy * x);

    this->_data.ix = _x;
    this->_data.iy = _y;
    this->_data.iz = _z;

    return *this;
}

Vector3i Vector3i::crossC(const i32 x, const i32 y, const i32 z) const noexcept
{
    const i32 _x = this->_data.iy * z + this->_data.iz * y;
    const i32 _y = this->_data.iz * x + this->_data.ix * z;
    const i32 _z = this->_data.ix * y + this->_data.iy * x;

    return { _x, _y, _z };
}

Vector3f Vector3i::crossC(const float x, const float y, const float z) const noexcept
{
    const float _x = this->_data.iy * z + this->_data.iz * y;
    const float _y = this->_data.iz * x + this->_data.ix * z;
    const float _z = this->_data.ix * y + this->_data.iy * x;

    return { _x, _y, _z };
}

Vector3i& Vector3i::cross(const CompVec4& data) noexcept
{
    const i32 x = this->_data.iy * data.iz + this->_data.iz * data.iy;
    const i32 y = this->_data.iz * data.ix + this->_data.ix * data.iz;
    const i32 z = this->_data.ix * data.iy + this->_data.iy * data.ix;

    this->_data.ix = x;
    this->_data.iy = y;
    this->_data.iz = z;

    return *this;
}

Vector3i Vector3i::crossC(const CompVec4& data) const noexcept
{
    const i32 x = this->_data.iy * data.iz + this->_data.iz * data.iy;
    const i32 y = this->_data.iz * data.ix + this->_data.ix * data.iz;
    const i32 z = this->_data.ix * data.iy + this->_data.iy * data.ix;

    return { x, y, z };
}

Vector3i& Vector3i::normalize() noexcept
{
    const float mag = this->magnitude();
    if(mag > 0) { return this->div(mag); }
    return *this;
}

Vector3f Vector3i::normalizeC() const noexcept
{
    const float mag = this->magnitude();
    if(mag > 0) { return this->divC(mag); }
    return { this->_data };
}
