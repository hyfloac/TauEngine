#pragma once

#include <maths/ComponentVector4.hpp>
#include <DLL.hpp>
#include <maths/Vector3f.hpp>

class TAU_DLL Quaternion
{
private:
    CompVec4 _data;
public:
#pragma region Constructor
    Quaternion(const __m128 vec)  noexcept;
    Quaternion(const float x, const float y, const float z, const float w) noexcept;
    Quaternion() noexcept;

    Quaternion(const Quaternion& copy) noexcept = default;
    Quaternion(Quaternion&& copy) noexcept = default;
#pragma endregion

    Quaternion& operator =(const Quaternion& copy) noexcept = default;
    Quaternion& operator =(Quaternion&& copy) noexcept = default;

#pragma region Accessor
    inline float x() const noexcept { return _data.x; }
    inline float y() const noexcept { return _data.y; }
    inline float z() const noexcept { return _data.z; }
    inline float w() const noexcept { return _data.w; }

    inline float& x() noexcept { return _data.x; }
    inline float& y() noexcept { return _data.y; }
    inline float& z() noexcept { return _data.z; }
    inline float& w() noexcept { return _data.w; }

    inline CompVec4  data() const noexcept { return _data; }
    inline CompVec4& data()       noexcept { return _data; }

    inline operator CompVec4 () const noexcept { return _data; }
    inline operator CompVec4&()       noexcept { return _data; }
#pragma endregion

private:
    static __m128 easyRotate(__m128 vec) noexcept
    {
        return _mm_shuffle_ps(vec, vec, 0x39); // vec(y, z, w, x)
    }

    static __m128 easyRotate2(__m128 vec) noexcept
    {
        return _mm_shuffle_ps(vec, vec, 0x4E); // vec(z, w, x, y)
    }
public:
#pragma region Simple Math
    Quaternion& add(const Quaternion other) noexcept
    {
        _data = CompVec4(_mm_add_ps(_data.vec, other._data.vec));
        return *this;
    }

    Quaternion& sub(const Quaternion other) noexcept
    {
        _data = CompVec4(_mm_sub_ps(_data.vec, other._data.vec));
        return *this;
    }

    Quaternion& add(const float scalar) noexcept
    {
        const __m128 tmp = CompVec4(scalar, scalar, scalar, scalar).vec;
        _data = CompVec4(_mm_add_ps(_data.vec, tmp));
        return *this;
    }

    Quaternion& sub(const float scalar) noexcept
    {
        const __m128 tmp = CompVec4(scalar, scalar, scalar, scalar).vec;
        _data = CompVec4(_mm_sub_ps(_data.vec, tmp));
        return *this;
    }

    Quaternion& mul(const float scalar) noexcept
    {
        const __m128 tmp = CompVec4(scalar, scalar, scalar, scalar).vec;
        _data = CompVec4(_mm_mul_ps(_data.vec, tmp));
        return *this;
    }

    Quaternion& div(const float scalar) noexcept { return mul(1.0f / scalar); }

    Quaternion& scale(const float scalar) noexcept { return mul(scalar); }

#define OPERATOR(_RET, _OP, _PARAM, _MODS, _FUNC) inline _RET operator _OP(const _PARAM o) _MODS { return this->_FUNC(o); }

    OPERATOR(Quaternion&, +=, Quaternion, noexcept, add);
    OPERATOR(Quaternion&, -=, Quaternion, noexcept, sub);

    OPERATOR(Quaternion&, +=, float, noexcept, add);
    OPERATOR(Quaternion&, -=, float, noexcept, sub);
    OPERATOR(Quaternion&, *=, float, noexcept, mul);
    OPERATOR(Quaternion&, /=, float, noexcept, div);
#pragma endregion 

    Vector3f getAxis() const noexcept
    {
        const float x = 1.0f - _data.w * _data.w;
        if(x < 0.000001f)
        {
            return Vector3f(1.0f, 0.0f, 0.0f);
        }

        const float x2 = x * x;
        return Vector3f(_data.x, _data.y, _data.z) / x2;
    }

    Vector3f toEulerAngles() const noexcept
    {
        const CompVec4 square = CompVec4(_mm_mul_ps(_data.vec, _data.vec));
        const __m128 rot = easyRotate(_data.vec);
        const CompVec4 rotMul = CompVec4(_mm_mul_ps(_data.vec, rot));
        const __m128 rot2 = easyRotate2(_data.vec);
        const CompVec4 rot2Mul = CompVec4(_mm_mul_ps(_data.vec, rot2));

        const float a = (2 * rotMul.w) - (2 * rotMul.y);
        const float b = 1 - (2 * square.x) - (2 * square.z);

        const float c = (2 * rot2Mul.y) - (2 * rot2Mul.x);
        const float d = 1 - (2 * square.y) - (2 * square.z);

        const float e = (2 * rotMul.x) + (2 * rotMul.z);

        return vec3(atan2f(a, b),
                    atan2f(c, d),
                    asinf(e));
    }

    float lengthSquared() const noexcept
    {
        const CompVec4 square = CompVec4(_mm_mul_ps(_data.vec, _data.vec));

        float result = 0.0f;
        result += square.x;
        result += square.y;
        result += square.z;
        result += square.w;
        return result;
    }

    float length() const noexcept
    {
        return sqrtf(lengthSquared());
    }

    Quaternion& Normalize()
    {
        float rLen = rSqrt(lengthSquared());
        return scale(rLen);
    }
};
