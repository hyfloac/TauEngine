#pragma once

#include <maths/ComponentVector4.hpp>
#include <DLL.hpp>
#include <maths/Maths.hpp>

class TAU_DLL Vector3f final
{
private:
    CompVec4 _data;
public:
#pragma region Constructor
    inline Vector3f(const __m128 vec) noexcept : _data(vec) { }
    inline Vector3f(const float x, const float y, const float z) noexcept : _data(x, y, z, 0.0f) { }
    inline Vector3f(const float filler) noexcept : _data(filler, filler, filler, 0.0f) { }
    inline Vector3f() noexcept : _data(_mm_set1_ps(0.0f)) { }

    Vector3f(const Vector3f& copy) noexcept = default;
    Vector3f(Vector3f&&      move) noexcept = default;
#pragma endregion

    ~Vector3f() noexcept = default;

#pragma region Copy/Move Assignment Operators
    Vector3f& operator =(const float filler) noexcept
    {
        this->_data.x = filler;
        this->_data.y = filler;
        this->_data.z = filler;

        return *this;
    }

    inline Vector3f& operator =(const CompVec4 vec) noexcept { _data = vec; return *this; }
    inline Vector3f& operator =(const __m128 vec) noexcept { _data = { vec }; return *this; }

    inline Vector3f& operator =(const Vector3f& copy) noexcept = default;
    inline Vector3f& operator =(Vector3f&&      move) noexcept = default;
#pragma endregion

#pragma region Accessor
    inline float x() const noexcept { return _data.vx(); }
    inline float y() const noexcept { return _data.vy(); }
    inline float z() const noexcept { return _data.vz(); }

    inline float& x() noexcept { return _data.x; }
    inline float& y() noexcept { return _data.y; }
    inline float& z() noexcept { return _data.z; }

    inline Vector3f& set(float x, float y, float z) { _data.vec = _mm_set_ps(x, y, z, 0.0f); return *this; }

    inline CompVec4  data() const noexcept { return _data; }
    inline CompVec4& data()       noexcept { return _data; }

    inline operator CompVec4 () const noexcept { return _data; }
    inline operator CompVec4&()       noexcept { return _data; }
#pragma endregion

    Vector3f& add(const float x, const float y, const float z) noexcept
    {
        this->_data.x += x;
        this->_data.y += y;
        this->_data.z += z;

        return *this;
    }

    Vector3f& sub(const float x, const float y, const float z) noexcept
    {
        this->_data.x -= x;
        this->_data.y -= y;
        this->_data.z -= z;

        return *this;
    }

    Vector3f addC(const float x, const float y, const float z) const noexcept
    {
        const float _x = this->_data.x + x;
        const float _y = this->_data.y + y;
        const float _z = this->_data.z + z;

        return { _x, _y, _z };
    }

    Vector3f subC(const float x, const float y, const float z) const noexcept
    {
        const float _x = this->_data.x - x;
        const float _y = this->_data.y - y;
        const float _z = this->_data.z - z;

        return { _x, _y, _z };
    }

    Vector3f& addSIMD(const float x, const float y, const float z) noexcept
    {
        const __m128 vec = CompVec4(x, y, z, 0.0f).vec;
        this->_data.vec = _mm_add_ps(this->_data.vec, vec);
        return *this;
    }

    Vector3f& subSIMD(const float x, const float y, const float z) noexcept
    {
        const __m128 vec = CompVec4(x, y, z, 0.0f).vec;
        this->_data.vec = _mm_sub_ps(this->_data.vec, vec);
        return *this;
    }

    Vector3f addCSIMD(const float x, const float y, const float z) const noexcept
    {
        const __m128 vec = CompVec4(x, y, z, 0.0f).vec;
        return Vector3f(_mm_add_ps(this->_data.vec, vec));
    }

    Vector3f subCSIMD(const float x, const float y, const float z) const noexcept
    {
        const __m128 vec = CompVec4(x, y, z, 0.0f).vec;
        return Vector3f(_mm_sub_ps(this->_data.vec, vec));
    }

    Vector3f& add(const Vector3f vec) noexcept
    {
        this->_data.vec = _mm_add_ps(this->_data.vec, vec._data.vec);
        return *this;
    }

    Vector3f& sub(const Vector3f vec) noexcept
    {
        this->_data.vec = _mm_sub_ps(this->_data.vec, vec._data.vec);
        return *this;
    }

    Vector3f& mul(const Vector3f vec) noexcept
    {
        this->_data.vec = _mm_mul_ps(this->_data.vec, vec._data.vec);
        return *this;
    }

    Vector3f& div(const Vector3f vec) noexcept
    {
        this->_data.vec = _mm_div_ps(this->_data.vec, vec._data.vec);
        return *this;
    }

    [[nodiscard]] Vector3f addC(const Vector3f vec) const noexcept
    {
        return Vector3f(_mm_add_ps(this->_data.vec, vec._data.vec));
    }

    [[nodiscard]] Vector3f subC(const Vector3f vec) const noexcept
    {
        return Vector3f(_mm_sub_ps(this->_data.vec, vec._data.vec));
    }

    [[nodiscard]] Vector3f mulC(const Vector3f vec) const noexcept
    {
        return Vector3f(_mm_mul_ps(this->_data.vec, vec._data.vec));
    }

    [[nodiscard]] Vector3f divC(const Vector3f vec) const noexcept
    {
        return Vector3f(_mm_div_ps(this->_data.vec, vec._data.vec));
    }

    Vector3f& add(const float scalar) noexcept
    {
        this->_data.x += scalar;
        this->_data.y += scalar;
        this->_data.z += scalar;

        return *this;
    }

    Vector3f addC(const float scalar) const noexcept
    {
        const float x = this->_data.x + scalar;
        const float y = this->_data.y + scalar;
        const float z = this->_data.z + scalar;

        return Vector3f(x, y, z);
    }

    Vector3f& addSIMD(const float scalar) noexcept
    {
        const __m128 tmp = CompVec4(scalar, scalar, scalar, 0.0f).vec;
        this->_data.vec = _mm_add_ps(this->_data.vec, tmp);

        return *this;
    }

    Vector3f addCSIMD(const float scalar) const noexcept
    {
        const __m128 tmp = CompVec4(scalar, scalar, scalar, 0.0f).vec;
        return Vector3f(_mm_add_ps(this->_data.vec, tmp));
    }

    Vector3f& sub(const float scalar) noexcept
    {
        this->_data.x -= scalar;
        this->_data.y -= scalar;
        this->_data.z -= scalar;

        return *this;
    }

    Vector3f subC(const float scalar) const noexcept
    {
        const float x = this->_data.x - scalar;
        const float y = this->_data.y - scalar;
        const float z = this->_data.z - scalar;

        return Vector3f(x, y, z);
    }

    Vector3f& subSIMD(const float scalar) noexcept
    {
        const __m128 tmp = CompVec4(scalar, scalar, scalar, 0.0f).vec;
        this->_data.vec = _mm_sub_ps(this->_data.vec, tmp);

        return *this;
    }

    Vector3f subCSIMD(const float scalar) const noexcept
    {
        const __m128 tmp = CompVec4(scalar, scalar, scalar, 0.0f).vec;
        return Vector3f(_mm_sub_ps(this->_data.vec, tmp));
    }
    
    Vector3f& mul(const float scalar) noexcept
    {
        this->_data.x *= scalar;
        this->_data.y *= scalar;
        this->_data.z *= scalar;

        return *this;
    }

    Vector3f mulC(const float scalar) const noexcept
    {
        const float x = this->_data.x * scalar;
        const float y = this->_data.y * scalar;
        const float z = this->_data.z * scalar;

        return Vector3f(x, y, z);
    }

    Vector3f& mulSIMD(const float scalar) noexcept
    {
        const __m128 tmp = CompVec4(scalar, scalar, scalar, 0.0f).vec;
        this->_data.vec = _mm_mul_ps(this->_data.vec, tmp);

        return *this;
    }

    Vector3f mulCSIMD(const float scalar) const noexcept
    {
        const __m128 tmp = CompVec4(scalar, scalar, scalar, 0.0f).vec;
        return Vector3f(_mm_mul_ps(this->_data.vec, tmp));
    }

    Vector3f& div(const float scalar) noexcept
    {
        const float recip = 1.0f / scalar;

        this->_data.x *= recip;
        this->_data.y *= recip;
        this->_data.z *= recip;

        return *this;
    }

    Vector3f divC(const float scalar) const noexcept
    {
        const float recip = 1.0f / scalar;

        const float x = this->_data.x * recip;
        const float y = this->_data.y * recip;
        const float z = this->_data.z * recip;

        return Vector3f(x, y, z);
    }

    Vector3f& divSIMD(const float scalar) noexcept
    {
        const float recip = 1.0f / scalar;

        const __m128 tmp = CompVec4(recip, recip, recip, 0.0f).vec;
        this->_data.vec = _mm_mul_ps(this->_data.vec, tmp);

        return *this;
    }

    Vector3f divCSIMD(const float scalar) const noexcept
    {
        const float recip = 1.0f / scalar;

        const __m128 tmp = CompVec4(recip, recip, recip, 0.0f).vec;

        return Vector3f(_mm_mul_ps(this->_data.vec, tmp));
    }

#undef SCALAR_MATH

    inline Vector3f& scale(const float scalar)           noexcept { return mul (scalar);     }
    inline Vector3f scaleC(const float scalar)     const noexcept { return mulC(scalar);     }
    inline Vector3f& scaleSIMD(const float scalar)       noexcept { return mulSIMD (scalar); }
    inline Vector3f scaleCSIMD(const float scalar) const noexcept { return mulCSIMD(scalar); }

#pragma region Negate
    Vector3f& negate() noexcept
    {
        this->_data.x = -this->_data.x;
        this->_data.y = -this->_data.y;
        this->_data.z = -this->_data.z;
        this->_data.w = -this->_data.w;

        return *this;
    }

    Vector3f  negateCopy() const noexcept
    {
        return Vector3f(CompVec4(-this->_data.x, -this->_data.y, -this->_data.z, -this->_data.w).vec);
    }

    inline Vector3f& neg()           noexcept { return negate();     }
    inline Vector3f  negCopy() const noexcept { return negateCopy(); }
#pragma endregion

#pragma region Magnitude
    float magnitudeSquared() const noexcept
    {
        const CompVec4 comp(_mm_mul_ps(this->_data.vec, this->_data.vec));

        return comp.x + comp.y + comp.z;
    }

    float magnitude() const noexcept
    {
        return sqrt(this->magnitudeSquared());
    }

    inline float lengthSquared() const noexcept { return magnitudeSquared(); }
    inline float length()        const noexcept { return magnitude();        }
#pragma endregion

    Vector3f inverseSqrt() const noexcept
    {
        __m128 inv = _mm_rsqrt_ss(this->_data.vec);

        const __m128 tmp = _mm_mul_ps(inv, _mm_set1_ps(1.5f));

        inv = _mm_mul_ps(inv, inv);
        inv = _mm_mul_ps(this->_data.vec, inv);
        inv = _mm_mul_ps(inv, _mm_set1_ps(0.5f));

        return _mm_sub_ps(tmp, inv);
    }

    float dot(const float x, const float y, const float z) const noexcept
    {
        return this->_data.x * x + this->_data.y * y + this->_data.z * z;
    }

    float dot(const Vector3f data) const noexcept
    {
        const CompVec4 comp(_mm_mul_ps(this->_data.vec, data._data.vec));
        return comp.x + comp.y + comp.z;
    }

    Vector3f& cross(const float x, const float y, const float z) noexcept
    {
        const float _x = this->_data.y * z + this->_data.z * y;
        const float _y = this->_data.z * x + this->_data.x * z;
        const float _z = this->_data.x * y + this->_data.y * x;

        this->_data.x = _x;
        this->_data.y = _y;
        this->_data.z = _z;

        return *this;
    }

    Vector3f crossC(const float x, const float y, const float z) const noexcept
    {
        const float _x = this->_data.y * z + this->_data.z * y;
        const float _y = this->_data.z * x + this->_data.x * z;
        const float _z = this->_data.x * y + this->_data.y * x;

        return Vector3f(_x, _y, _z);
    }

    Vector3f& cross(const Vector3f data) noexcept
    {
        const float x = this->_data.y * data._data.z + this->_data.z * data._data.y;
        const float y = this->_data.z * data._data.x + this->_data.x * data._data.z;
        const float z = this->_data.x * data._data.y + this->_data.y * data._data.x;

        this->_data.x = x;
        this->_data.y = y;
        this->_data.z = z;

        return *this;
    }

    Vector3f crossC(const Vector3f data) const noexcept
    {
        const float x = this->_data.y * data._data.z + this->_data.z * data._data.y;
        const float y = this->_data.z * data._data.x + this->_data.x * data._data.z;
        const float z = this->_data.x * data._data.y + this->_data.y * data._data.x;

        return Vector3f(x, y, z);
    }

    Vector3f& normalize() noexcept
    {
        const float mag = this->magnitudeSquared();
        if(mag > 0) { return this->mul(rSqrt(mag)); }
        return *this;
    }

    Vector3f normalizeC() const noexcept
    {
        const float mag = this->magnitudeSquared();
        if(mag > 0) { return this->mulC(rSqrt(mag)); }
        return Vector3f(*this);
    }

#pragma region Operators
#define OPERATOR(_RET, _OP, _PARAM, _MODS, _FUNC) inline _RET operator _OP(const _PARAM o) _MODS { return this->_FUNC(o); }

    OPERATOR(Vector3f, +, Vector3f, const noexcept, addC);
    OPERATOR(Vector3f, -, Vector3f, const noexcept, subC);
    OPERATOR(Vector3f, *, Vector3f, const noexcept, crossC);

    OPERATOR(Vector3f&, +=, Vector3f, noexcept, add);
    OPERATOR(Vector3f&, -=, Vector3f, noexcept, sub);
    OPERATOR(Vector3f&, *=, Vector3f, noexcept, cross);

    OPERATOR(Vector3f, +, float, const noexcept, addC);
    OPERATOR(Vector3f, -, float, const noexcept, subC);
    OPERATOR(Vector3f, *, float, const noexcept, mulC);
    OPERATOR(Vector3f, /, float, const noexcept, divC);

    OPERATOR(Vector3f&, +=, float, noexcept, add);
    OPERATOR(Vector3f&, -=, float, noexcept, sub);
    OPERATOR(Vector3f&, *=, float, noexcept, mul);
    OPERATOR(Vector3f&, /=, float, noexcept, div);

#undef OPERATOR
#undef SCALAR_OPERATOR

    inline Vector3f& operator -()       noexcept { return neg();     }
    inline Vector3f  operator -() const noexcept { return negCopy(); }
#pragma endregion
};

typedef Vector3f vec3f;
typedef Vector3f vec3;
