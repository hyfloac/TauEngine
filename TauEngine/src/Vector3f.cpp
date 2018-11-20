#pragma warning(push, 0)
#include <cmath>
#pragma warning(pop)

#include <maths/Vector3f.hpp>

#pragma region Constructor
Vector3f::Vector3f(const __m128 vec) noexcept
    : _data({ vec })
{ }

Vector3f::Vector3f(const __m128i vec) noexcept
    : _data({ *((__m128*) &vec) })
{ }

Vector3f::Vector3f(const float x, const float y, const float z) noexcept
    : _data({ x, y, z })
{ }

Vector3f::Vector3f(const i32 x, const i32 y, const i32 z) noexcept
    : _data({ (float) x, (float) y, (float) z })
{ }

Vector3f::Vector3f(const float filler) noexcept
    : _data({ filler, filler, filler })
{ }

Vector3f::Vector3f(const i32 filler) noexcept
    : _data({ (float) filler, (float) filler, (float) filler })
{ }

Vector3f::Vector3f() noexcept
    : _data({ 0, 0, 0 })
{ }

Vector3f::Vector3f(const CompVec4& data) noexcept
    : _data({ data.vec })
{ }
#pragma endregion

#pragma region Copy Assignment Operators
Vector3f& Vector3f::operator =(const float filler) noexcept
{
    this->_data.x = filler;
    this->_data.y = filler;
    this->_data.z = filler;

    return *this;
}

Vector3f& Vector3f::operator =(const i32 filler) noexcept
{
    this->_data.x = (float) filler;
    this->_data.y = (float) filler;
    this->_data.z = (float) filler;

    return *this;
}

Vector3f& Vector3f::operator =(const CompVec4& data) noexcept
{
    this->_data.vec = data.vec;

    return *this;
}
#pragma endregion

Vector3f& Vector3f::add(const float x, const float y, const float z) noexcept
{
    this->_data.x += x;
    this->_data.y += y;
    this->_data.z += z;

    return *this;
}

Vector3f& Vector3f::add(const CompVec4& data) noexcept
{
    this->_data.vec = _mm_add_ps(this->_data.vec, data.vec);

    return *this;
}

Vector3f& Vector3f::sub(const float x, const float y, const float z) noexcept
{
    this->_data.x -= x;
    this->_data.y -= y;
    this->_data.z -= z;

    return *this;
}

Vector3f& Vector3f::sub(const CompVec4& data) noexcept
{
    this->_data.vec = _mm_sub_ps(this->_data.vec, data.vec);

    return *this;
}

Vector3f Vector3f::addC(const float x, const float y, const float z) const noexcept
{
    const float _x = this->_data.x + x;
    const float _y = this->_data.y + y;
    const float _z = this->_data.z + z;

    return { _x, _y, _z };
}

Vector3f Vector3f::addC(const CompVec4& data) const noexcept
{
    return { _mm_add_ps(this->_data.vec, data.vec) };
}

Vector3f Vector3f::subC(const float x, const float y, const float z) const noexcept
{
    const float _x = this->_data.x - x;
    const float _y = this->_data.y - y;
    const float _z = this->_data.z - z;

    return { _x, _y, _z };
}

Vector3f Vector3f::subC(const CompVec4& data) const noexcept
{
    return { _mm_sub_ps(this->_data.vec, data.vec) };
}

Vector3f& Vector3f::add(const float scalar) noexcept
{
    this->_data.x += scalar;
    this->_data.y += scalar;
    this->_data.z += scalar;

    return *this;
}

Vector3f Vector3f::addC(const float scalar) const noexcept
{
    const float x = this->_data.x + scalar;
    const float y = this->_data.y + scalar;
    const float z = this->_data.z + scalar;

    return { x, y, z };
}

Vector3f& Vector3f::sub(const float scalar) noexcept
{
    this->_data.x -= scalar;
    this->_data.y -= scalar;
    this->_data.z -= scalar;

    return *this;
}

Vector3f Vector3f::subC(const float scalar) const noexcept
{
    const float x = this->_data.x - scalar;
    const float y = this->_data.y - scalar;
    const float z = this->_data.z - scalar;

    return { x, y, z };
}

Vector3f& Vector3f::mul(const float scalar) noexcept
{
    this->_data.x *= scalar;
    this->_data.y *= scalar;
    this->_data.z *= scalar;

    return *this;
}

Vector3f Vector3f::mulC(const float scalar) const noexcept
{
    const float x = this->_data.x * scalar;
    const float y = this->_data.y * scalar;
    const float z = this->_data.z * scalar;

    return { x, y, z };
}

Vector3f& Vector3f::div(const float scalar) noexcept
{
    this->_data.x /= scalar;
    this->_data.y /= scalar;
    this->_data.z /= scalar;

    return *this;
}

Vector3f Vector3f::divC(const float scalar) const noexcept
{
    const float x = this->_data.x / scalar;
    const float y = this->_data.y / scalar;
    const float z = this->_data.z / scalar;

    return { x, y, z };
}

Vector3f& Vector3f::negate() noexcept
{
    this->_data.x = -this->_data.x;
    this->_data.y = -this->_data.y;
    this->_data.z = -this->_data.z;

    return *this;
}

Vector3f Vector3f::negateCopy() const noexcept
{
    return { -this->_data.x, -this->_data.y, -this->_data.z };
}

#pragma region Magnitude
float Vector3f::magnitudeSquared() const noexcept
{
    CompVec4 comp;
    comp.vec = _mm_mul_ps(this->_data.vec, this->_data.vec);

    return comp.x + comp.y + comp.z;
}

float Vector3f::magnitude() const noexcept
{
    return sqrt(this->magnitudeSquared());
}
#pragma endregion

float Vector3f::dot(const float x, const float y, const float z) const noexcept
{
    return this->_data.x * x + this->_data.y * y + this->_data.z * z;
}

float Vector3f::dot(const CompVec4& vec) const noexcept
{
    CompVec4 comp;
    comp.vec = _mm_mul_ps(this->_data.vec, vec.vec);

    return comp.x + comp.y + comp.z;
}

Vector3f& Vector3f::cross(const float x, const float y, const float z) noexcept
{
    const float _x = this->_data.y * z + this->_data.z * y;
    const float _y = this->_data.z * x + this->_data.x * z;
    const float _z = this->_data.x * y + this->_data.y * x;

    this->_data.x = _x;
    this->_data.y = _y;
    this->_data.z = _z;

    return *this;
}

Vector3f Vector3f::crossC(const float x, const float y, const float z) const noexcept
{
    const float _x = this->_data.y * z + this->_data.z * y;
    const float _y = this->_data.z * x + this->_data.x * z;
    const float _z = this->_data.x * y + this->_data.y * x;

    return { _x, _y, _z };
}

Vector3f& Vector3f::cross(const CompVec4& data) noexcept
{
    const float x = this->_data.y * data.z + this->_data.z * data.y;
    const float y = this->_data.z * data.x + this->_data.x * data.z;
    const float z = this->_data.x * data.y + this->_data.y * data.x;

    this->_data.x = x;
    this->_data.y = y;
    this->_data.z = z;

    return *this;
}

Vector3f Vector3f::crossC(const CompVec4& data) const noexcept
{
    const float x = this->_data.y * data.z + this->_data.z * data.y;
    const float y = this->_data.z * data.x + this->_data.x * data.z;
    const float z = this->_data.x * data.y + this->_data.y * data.x;

    return { x, y, z };
}

Vector3f& Vector3f::normalize() noexcept
{
    const float mag = this->magnitude();
    if(mag > 0) { return this->div(mag); }
    return *this;
}

Vector3f Vector3f::normalizeC() const noexcept
{
    const float mag = this->magnitude();
    if(mag > 0) { return this->divC(mag); }
    return { this->_data };
}
