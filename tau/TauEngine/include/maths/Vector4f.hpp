#pragma once

#include <maths/ComponentVector4.hpp>
#include <DLL.hpp>

class TAU_DLL Vector4f final
{
private:
    CompVec4 _data;
public:
#pragma region Constructor
    Vector4f(const __m128 vec)  noexcept;
    Vector4f(const __m128i vec) noexcept;
    Vector4f(const float x, const float y, const float z, const float w) noexcept;
    Vector4f(const i32   x, const i32   y, const i32   z, const i32   w) noexcept;
    Vector4f(const float filler) noexcept;
    Vector4f(const i32   filler) noexcept;
    Vector4f() noexcept;

    Vector4f(const CompVec4& copy) noexcept;
#pragma endregion

#pragma region Copy/Move Assignment Operators
    Vector4f& operator =(const float filler) noexcept;
    Vector4f& operator =(const i32   filler) noexcept;

    Vector4f& operator =(const CompVec4& copy) noexcept;
#pragma endregion

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

#define RAW_VEC_FUNC(_RET, _NAME, _MODS) _RET _NAME(const float x, const float y, const float z, const float w) _MODS; \
                                         inline _RET _NAME(const i32 x, const i32 y, const i32 z, const i32 w) _MODS { return _NAME((float) x, (float) y, (float) z, (float) w); }

    RAW_VEC_FUNC(Vector4f&, add, noexcept);
    RAW_VEC_FUNC(Vector4f&, sub, noexcept);
    RAW_VEC_FUNC(Vector4f, addC, const noexcept);
    RAW_VEC_FUNC(Vector4f, subC, const noexcept);

    Vector4f& add(const CompVec4& data)       noexcept;
    Vector4f& sub(const CompVec4& data)       noexcept;
    Vector4f addC(const CompVec4& data) const noexcept;
    Vector4f subC(const CompVec4& data) const noexcept;

#define SCALAR_MATH(_NAME) Vector4f& _NAME(const float scalar) noexcept; \
                           inline Vector4f& _NAME(const i32 scalar) noexcept { return _NAME((float) scalar); } \
                           Vector4f _NAME##C(const float scalar) const noexcept; \
                           inline Vector4f _NAME##C(const i32 scalar) const noexcept { return _NAME##C((float) scalar); }

    SCALAR_MATH(add);
    SCALAR_MATH(sub);
    SCALAR_MATH(mul);
    SCALAR_MATH(div);

#undef SCALAR_MATH

    inline Vector4f& scale(const float scalar) noexcept { return mul(scalar); }
    inline Vector4f& scale(const i32   scalar) noexcept { return mul(scalar); }
    inline Vector4f scaleC(const float scalar)           const noexcept { return mulC(scalar); }
    inline Vector4f scaleC(const i32   scalar)           const noexcept { return mulC(scalar); }

#pragma region Negate
    Vector4f& negate()               noexcept;
    Vector4f  negateCopy()     const noexcept;
    inline Vector4f& neg()           noexcept { return negate(); }
    inline Vector4f  negCopy() const noexcept { return negateCopy(); }
#pragma endregion

#pragma region Magnitude
    float magnitudeSquared()     const noexcept;
    float magnitude()            const noexcept;
    inline float lengthSquared() const noexcept { return magnitudeSquared(); }
    inline float length()        const noexcept { return magnitude(); }
#pragma endregion

    Vector4f inverseSqrt() const noexcept;

    RAW_VEC_FUNC(float, dot, const noexcept);
    float dot(const CompVec4& data) const noexcept;

    Vector4f& normalize()       noexcept;
    Vector4f normalizeC() const noexcept;

#undef RAW_VEC_FUNC

#pragma region Operators
#define OPERATOR(_RET, _OP, _PARAM, _MODS, _FUNC) inline _RET operator _OP(const _PARAM o) _MODS { return this->_FUNC(o); }
#define SCALAR_OPERATOR(_RET, _OP, _MODS, _FUNC) OPERATOR(_RET, _OP, float, _MODS, _FUNC); \
                                                 OPERATOR(_RET, _OP, i32, _MODS, _FUNC);

    OPERATOR(Vector4f, +, CompVec4&, const noexcept, addC);
    OPERATOR(Vector4f, -, CompVec4&, const noexcept, subC);

    OPERATOR(Vector4f&, +=, CompVec4&, noexcept, add);
    OPERATOR(Vector4f&, -=, CompVec4&, noexcept, sub);

    SCALAR_OPERATOR(Vector4f, +, const noexcept, addC);
    SCALAR_OPERATOR(Vector4f, -, const noexcept, subC);
    SCALAR_OPERATOR(Vector4f, *, const noexcept, mulC);
    SCALAR_OPERATOR(Vector4f, / , const noexcept, divC);

    SCALAR_OPERATOR(Vector4f&, +=, noexcept, add);
    SCALAR_OPERATOR(Vector4f&, -=, noexcept, sub);
    SCALAR_OPERATOR(Vector4f&, *=, noexcept, mul);
    SCALAR_OPERATOR(Vector4f&, /=, noexcept, div);

#undef OPERATOR
#undef SCALAR_OPERATOR
#undef RAW_VEC_FUNC

    inline Vector4f& operator -()       noexcept { return neg(); }
    inline Vector4f  operator -() const noexcept { return negCopy(); }
#pragma endregion
};

typedef Vector4f vec4f;
typedef Vector4f vec4;
