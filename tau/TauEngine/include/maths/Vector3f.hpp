#pragma once

#include <maths/ComponentVector4.hpp>
#include <DLL.hpp>

class TAU_DLL Vector3f
{
private:
    CompVec4 _data;
public:
#pragma region Constructor
    Vector3f(const __m128 vec)  noexcept;
    Vector3f(const __m128i vec) noexcept;
    Vector3f(const float x, const float y, const float z) noexcept;
    Vector3f(const i32   x, const i32   y, const i32   z) noexcept;
    Vector3f(const float filler) noexcept;
    Vector3f(const i32   filler) noexcept;
    Vector3f() noexcept;

    Vector3f(const CompVec4& data) noexcept;
#pragma endregion

#pragma region Copy/Move Assignment Operators
    Vector3f& operator =(const float filler) noexcept;
    Vector3f& operator =(const i32   filler) noexcept;

    Vector3f& operator =(const CompVec4& copy) noexcept;
#pragma endregion

#pragma region Accessor
    inline float x() const noexcept { return _data.x; }
    inline float y() const noexcept { return _data.y; }
    inline float z() const noexcept { return _data.z; }

    inline float& x() noexcept { return _data.x; }
    inline float& y() noexcept { return _data.y; }
    inline float& z() noexcept { return _data.z; }

    inline CompVec4  data() const noexcept { return _data; }
    inline CompVec4& data()       noexcept { return _data; }

    inline operator CompVec4 () const noexcept { return _data; }
    inline operator CompVec4&()       noexcept { return _data; }
#pragma endregion

#define RAW_VEC_FUNC(_RET, _NAME, _MODS) _RET _NAME(const float x, const float y, const float z) _MODS; \
                                         inline _RET _NAME(const i32 x, const i32 y, const i32 z) _MODS { return _NAME((float) x, (float) y, (float) z); }

    RAW_VEC_FUNC(Vector3f&, add, noexcept);
    RAW_VEC_FUNC(Vector3f&, sub, noexcept);
    RAW_VEC_FUNC(Vector3f, addC, const noexcept);
    RAW_VEC_FUNC(Vector3f, subC, const noexcept);

    Vector3f& add(const CompVec4& data)       noexcept;
    Vector3f& sub(const CompVec4& data)       noexcept;
    Vector3f addC(const CompVec4& data) const noexcept;
    Vector3f subC(const CompVec4& data) const noexcept;

#define SCALAR_MATH(_NAME) Vector3f& _NAME(const float scalar) noexcept; \
                           inline Vector3f& _NAME(const i32 scalar) noexcept { return _NAME((float) scalar); } \
                           Vector3f _NAME##C(const float scalar) const noexcept; \
                           inline Vector3f _NAME##C(const i32 scalar) const noexcept { return _NAME##C((float) scalar); }

    SCALAR_MATH(add);
    SCALAR_MATH(sub);
    SCALAR_MATH(mul);
    SCALAR_MATH(div);

#undef SCALAR_MATH

    inline Vector3f& scale(const float scalar)       noexcept { return mul (scalar); }
    inline Vector3f& scale(const i32   scalar)       noexcept { return mul (scalar); }
    inline Vector3f scaleC(const float scalar) const noexcept { return mulC(scalar); }
    inline Vector3f scaleC(const i32   scalar) const noexcept { return mulC(scalar); }

#pragma region Negate
    Vector3f& negate()               noexcept;
    Vector3f  negateCopy()     const noexcept;
    inline Vector3f& neg()           noexcept { return negate();     }
    inline Vector3f  negCopy() const noexcept { return negateCopy(); }
#pragma endregion

#pragma region Magnitude
    float magnitudeSquared()     const noexcept;
    float magnitude()            const noexcept;
    inline float lengthSquared() const noexcept { return magnitudeSquared(); }
    inline float length()        const noexcept { return magnitude();        }
#pragma endregion

    Vector3f inverseSqrt() const noexcept;

    RAW_VEC_FUNC(float, dot, const noexcept);
    RAW_VEC_FUNC(Vector3f&, cross, noexcept);
    RAW_VEC_FUNC(Vector3f, crossC, const noexcept);

    float dot(const CompVec4& data) const noexcept;

    Vector3f& cross(const CompVec4& data)       noexcept;
    Vector3f crossC(const CompVec4& data) const noexcept;

    Vector3f& normalize()       noexcept;
    Vector3f normalizeC() const noexcept;

#undef RAW_VEC_FUNC

#pragma region Operators
#define OPERATOR(_RET, _OP, _PARAM, _MODS, _FUNC) inline _RET operator _OP(const _PARAM o) _MODS { return this->_FUNC(o); }
#define SCALAR_OPERATOR(_RET, _OP, _MODS, _FUNC) OPERATOR(_RET, _OP, float, _MODS, _FUNC); \
                                                 OPERATOR(_RET, _OP, i32, _MODS, _FUNC);

    OPERATOR(Vector3f, +, CompVec4&, const noexcept, addC);
    OPERATOR(Vector3f, -, CompVec4&, const noexcept, subC);
    OPERATOR(Vector3f, *, CompVec4&, const noexcept, crossC);

    OPERATOR(Vector3f&, +=, CompVec4&, noexcept, add);
    OPERATOR(Vector3f&, -=, CompVec4&, noexcept, sub);
    OPERATOR(Vector3f&, *=, CompVec4&, noexcept, cross);

    SCALAR_OPERATOR(Vector3f, +, const noexcept, addC);
    SCALAR_OPERATOR(Vector3f, -, const noexcept, subC);
    SCALAR_OPERATOR(Vector3f, *, const noexcept, mulC);
    SCALAR_OPERATOR(Vector3f, /, const noexcept, divC);

    SCALAR_OPERATOR(Vector3f&, +=, noexcept, add);
    SCALAR_OPERATOR(Vector3f&, -=, noexcept, sub);
    SCALAR_OPERATOR(Vector3f&, *=, noexcept, mul);
    SCALAR_OPERATOR(Vector3f&, /=, noexcept, div);

#undef OPERATOR
#undef SCALAR_OPERATOR

    inline Vector3f& operator -()       noexcept { return neg();     }
    inline Vector3f  operator -() const noexcept { return negCopy(); }
#pragma endregion
};

typedef Vector3f vec3f;
typedef Vector3f vec3;
