#pragma warning(push, 0)
#include <cmath>
#pragma warning(pop)

#include <maths/Vector3i.hpp>
#include <maths/Vector3f.hpp>

Vector3i& Vector3i::add(const i32 x, const i32 y, const i32 z) noexcept
{
    this->_data.x += x;
    this->_data.y += y;
    this->_data.z += z;

    return *this;
}

Vector3i& Vector3i::add(const Vector3i& data) noexcept
{
    this->_data.vec = _mm_add_epi32(this->_data.vec, data._data.vec);

    return *this;
}

Vector3i& Vector3i::sub(const i32 x, const i32 y, const i32 z) noexcept
{
    this->_data.x -= x;
    this->_data.y -= y;
    this->_data.z -= z;

    return *this;
}

Vector3i& Vector3i::sub(const Vector3i& data) noexcept
{
    this->_data.vec = _mm_sub_epi32(this->_data.vec, data._data.vec);

    return *this;
}

Vector3i Vector3i::addC(const i32 x, const i32 y, const i32 z) const noexcept
{
    const i32 _x = this->_data.x + x;
    const i32 _y = this->_data.y + y;
    const i32 _z = this->_data.z + z;

    return { _x, _y, _z };
}

Vector3i Vector3i::addC(const Vector3i& data) const noexcept
{
    return { _mm_add_epi32(this->_data.vec, data._data.vec) };
}

Vector3i Vector3i::subC(const i32 x, const i32 y, const i32 z) const noexcept
{
    const i32 _x = this->_data.x - x;
    const i32 _y = this->_data.y - y;
    const i32 _z = this->_data.z - z;

    return { _x, _y, _z };
}

Vector3i Vector3i::subC(const Vector3i& data) const noexcept
{
    return { _mm_sub_epi32(this->_data.vec, data._data.vec) };
}

Vector3i& Vector3i::add(const i32 scalar) noexcept
{
    this->_data.x += scalar;
    this->_data.y += scalar;
    this->_data.z += scalar;

    return *this;
}

Vector3i Vector3i::addC(const i32 scalar) const noexcept
{
    const i32 x = this->_data.x + scalar;
    const i32 y = this->_data.y + scalar;
    const i32 z = this->_data.z + scalar;

    return { x, y, z };
}

Vector3i& Vector3i::sub(const i32 scalar) noexcept
{
    this->_data.x -= scalar;
    this->_data.y -= scalar;
    this->_data.z -= scalar;

    return *this;
}

Vector3i Vector3i::subC(const i32 scalar) const noexcept
{
    const i32 x = this->_data.x - scalar;
    const i32 y = this->_data.y - scalar;
    const i32 z = this->_data.z - scalar;

    return { x, y, z };
}

Vector3i& Vector3i::mul(const float scalar) noexcept
{
    this->_data.x = (i32) (this->_data.x * scalar);
    this->_data.y = (i32) (this->_data.y * scalar);
    this->_data.z = (i32) (this->_data.z * scalar);

    return *this;
}

Vector3i& Vector3i::mul(const i32 scalar) noexcept
{
    this->_data.x *= scalar;
    this->_data.y *= scalar;
    this->_data.z *= scalar;

    return *this;
}

Vector3i Vector3i::mulC(const float scalar) const noexcept
{
    const i32 x = static_cast<i32>(this->_data.x * scalar);
    const i32 y = static_cast<i32>(this->_data.y * scalar);
    const i32 z = static_cast<i32>(this->_data.z * scalar);

    return { x, y, z };
}

Vector3i Vector3i::mulC(const i32 scalar) const noexcept
{
    const i32 x = static_cast<i32>(this->_data.x * scalar);
    const i32 y = static_cast<i32>(this->_data.y * scalar);
    const i32 z = static_cast<i32>(this->_data.z * scalar);

    return { x, y, z };
}

Vector3i& Vector3i::div(const float scalar) noexcept
{
    this->_data.x = static_cast<i32>(this->_data.x / scalar);
    this->_data.y = static_cast<i32>(this->_data.y / scalar);
    this->_data.z = static_cast<i32>(this->_data.z / scalar);

    return *this;
}

Vector3i& Vector3i::div(const i32 scalar) noexcept
{
    this->_data.x /= scalar;
    this->_data.y /= scalar;
    this->_data.z /= scalar;

    return *this;
}

Vector3i Vector3i::divC(const float scalar) const noexcept
{
    const i32 x = static_cast<i32>(this->_data.x / scalar);
    const i32 y = static_cast<i32>(this->_data.y / scalar);
    const i32 z = static_cast<i32>(this->_data.z / scalar);

    return { x, y, z };
}

Vector3i Vector3i::divC(const i32 scalar) const noexcept
{
    const i32 x = this->_data.x / scalar;
    const i32 y = this->_data.y / scalar;
    const i32 z = this->_data.z / scalar;

    return { x, y, z };
}

Vector3i& Vector3i::negate() noexcept
{
    this->_data.x = -this->_data.x;
    this->_data.y = -this->_data.y;
    this->_data.z = -this->_data.z;

    return *this;
}

Vector3i Vector3i::negateCopy() const noexcept
{
    return { -this->_data.x, -this->_data.y, -this->_data.z };
}

#pragma region Magnitude
i32 Vector3i::magnitudeSquared() const noexcept
{
    return this->_data.x * this->_data.x + this->_data.y * this->_data.y + this->_data.z * this->_data.z;
}

float Vector3i::magnitude() const noexcept
{
    return (float) sqrt(this->magnitudeSquared());
}
#pragma endregion

i32 Vector3i::dot(const i32 x, const i32 y, const i32 z) const noexcept
{
    return this->_data.x * x + this->_data.y * y + this->_data.z * z;
}

float Vector3i::dot(const float x, const float y, const float z) const noexcept
{
    return this->_data.x * x + this->_data.y * y + this->_data.z * z;
}

i32 Vector3i::dot(const Vector3i& data) const noexcept
{
    return this->_data.x * data._data.x + this->_data.y * data._data.y + this->_data.z * data._data.z;
}

Vector3i& Vector3i::cross(const i32 x, const i32 y, const i32 z) noexcept
{
    const i32 _x = this->_data.y * z + this->_data.z * y;
    const i32 _y = this->_data.z * x + this->_data.x * z;
    const i32 _z = this->_data.x * y + this->_data.y * x;

    this->_data.x = _x;
    this->_data.y = _y;
    this->_data.z = _z;

    return *this;
}

Vector3i& Vector3i::cross(const float x, const float y, const float z) noexcept
{
    const i32 _x = (i32) (this->_data.y * z + this->_data.z * y);
    const i32 _y = (i32) (this->_data.z * x + this->_data.x * z);
    const i32 _z = (i32) (this->_data.x * y + this->_data.y * x);

    this->_data.x = _x;
    this->_data.y = _y;
    this->_data.z = _z;

    return *this;
}

Vector3i Vector3i::crossC(const i32 x, const i32 y, const i32 z) const noexcept
{
    const i32 _x = this->_data.y * z + this->_data.z * y;
    const i32 _y = this->_data.z * x + this->_data.x * z;
    const i32 _z = this->_data.x * y + this->_data.y * x;

    return { _x, _y, _z };
}

Vector3f Vector3i::crossC(const float x, const float y, const float z) const noexcept
{
    const float _x = this->_data.y * z + this->_data.z * y;
    const float _y = this->_data.z * x + this->_data.x * z;
    const float _z = this->_data.x * y + this->_data.y * x;

    return { _x, _y, _z };
}

Vector3i& Vector3i::cross(const Vector3i& data) noexcept
{
    const i32 x = this->_data.y * data._data.z + this->_data.z * data._data.y;
    const i32 y = this->_data.z * data._data.x + this->_data.x * data._data.z;
    const i32 z = this->_data.x * data._data.y + this->_data.y * data._data.x;

    this->_data.x = x;
    this->_data.y = y;
    this->_data.z = z;

    return *this;
}

Vector3i Vector3i::crossC(const Vector3i& data) const noexcept
{
    const i32 x = this->_data.y * data._data.z + this->_data.z * data._data.y;
    const i32 y = this->_data.z * data._data.x + this->_data.x * data._data.z;
    const i32 z = this->_data.x * data._data.y + this->_data.y * data._data.x;

    return { x, y, z };
}

Vector3i& Vector3i::normalize() noexcept
{
    const float mag = this->magnitude();
    if(mag > 0) { return this->div(mag); }
    return *this;
}

Vector3i Vector3i::normalizeC() const noexcept
{
    const float mag = this->magnitude();
    if(mag > 0) { return this->divC(mag); }
    return Vector3i(*this);
}
