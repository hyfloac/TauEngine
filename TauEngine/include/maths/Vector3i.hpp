#pragma once

#include <maths/ComponentVector4.hpp>
#include <maths/Vector3f.hpp>

class Vector3i
{
private:
    CompVec4 _data;
public:
#pragma region Constructor
    Vector3i(const __m128 vec)  noexcept;
    Vector3i(const __m128i vec) noexcept;
    Vector3i(const float x, const float y, const float z) noexcept;
    Vector3i(const i32   x, const i32   y, const i32   z) noexcept;
    Vector3i(const float filler) noexcept;
    Vector3i(const i32   filler) noexcept;
    Vector3i() noexcept;

    Vector3i(const CompVec4& data) noexcept;
#pragma endregion

#pragma region Copy/Move Assignment Operators
    Vector3i& operator =(const float filler) noexcept;
    Vector3i& operator =(const i32   filler) noexcept;

    Vector3i& operator =(const CompVec4& other) noexcept;
#pragma endregion

#pragma region Accessor
    inline i32 x() const noexcept { return _data.ix; }
    inline i32 y() const noexcept { return _data.iy; }
    inline i32 z() const noexcept { return _data.iz; }

    inline i32& x() noexcept { return _data.ix; }
    inline i32& y() noexcept { return _data.iy; }
    inline i32& z() noexcept { return _data.iz; }

    inline CompVec4  data() const noexcept { return _data; }
    inline CompVec4& data()       noexcept { return _data; }

    inline operator CompVec4 () const noexcept { return _data; }
    inline operator CompVec4&()       noexcept { return _data; }
#pragma endregion

#define RAW_VEC_FUNC(_RET, _NAME, _MODS) _RET _NAME(const i32 x, const i32 y, const i32 z) _MODS; \
                                         inline _RET _NAME(const float x, const float y, const float z) _MODS { return _NAME((i32) x, (i32) y, (i32) z); }

    RAW_VEC_FUNC(Vector3i&, add, noexcept);
    RAW_VEC_FUNC(Vector3i&, sub, noexcept);
    RAW_VEC_FUNC(Vector3i, addC, const noexcept);
    RAW_VEC_FUNC(Vector3i, subC, const noexcept);

    Vector3i& add(const CompVec4& data)       noexcept;
    Vector3i& sub(const CompVec4& data)       noexcept;
    Vector3i addC(const CompVec4& data) const noexcept;
    Vector3i subC(const CompVec4& data) const noexcept;
    
#define SCALAR_MATH(_NAME) Vector3i& _NAME(const i32 scalar) noexcept; \
                           inline Vector3i& _NAME(const float scalar) noexcept { return _NAME((float) scalar); } \
                           Vector3i _NAME##C(const i32 scalar) const noexcept; \
                           inline Vector3i _NAME##C(const float scalar) const noexcept { return _NAME##C((i32) scalar); }

    SCALAR_MATH(add);
    SCALAR_MATH(sub);

    Vector3i& mul(const float scalar)       noexcept; 
    Vector3i& mul(const i32 scalar)         noexcept;
    Vector3f mulC(const float scalar) const noexcept;
    Vector3f mulC(const i32 scalar) const noexcept;
           
    Vector3i& div(const float scalar)       noexcept; 
    Vector3i& div(const i32 scalar)         noexcept;
    Vector3f divC(const float scalar) const noexcept;
    Vector3f divC(const i32 scalar) const noexcept;

#undef SCALAR_MATH

    inline Vector3i& scale(const float scalar)       noexcept { return mul (scalar); }
    inline Vector3i& scale(const i32   scalar)       noexcept { return mul (scalar); }
    inline Vector3f scaleC(const float scalar) const noexcept { return mulC(scalar); }
    inline Vector3f scaleC(const i32   scalar) const noexcept { return mulC(scalar); }

#pragma region Negate
    Vector3i& negate()               noexcept;
    Vector3i  negateCopy()     const noexcept;
    inline Vector3i& neg()           noexcept { return negate();     }
    inline Vector3i  negCopy() const noexcept { return negateCopy(); }
#pragma endregion

#pragma region Magnitude
    i32   magnitudeSquared()     const noexcept;
    float magnitude()            const noexcept;
    inline i32   lengthSquared() const noexcept { return magnitudeSquared(); }
    inline float length()        const noexcept { return magnitude();        }
#pragma endregion

    i32 dot(const i32 x, const i32 y, const i32 z) const noexcept;
    float dot(const float x, const float y, const float z) const noexcept;

    Vector3i& cross(const i32 x, const i32 y, const i32 z) noexcept;
    Vector3i& cross(const float x, const float y, const float z) noexcept;

    Vector3i crossC(const i32 x, const i32 y, const i32 z) const noexcept;
    Vector3f crossC(const float x, const float y, const float z) const noexcept;

    i32 dot(const CompVec4& data) const noexcept;

    Vector3i& cross(const CompVec4& data)       noexcept;
    Vector3i crossC(const CompVec4& data) const noexcept;

    Vector3i& normalize()       noexcept;
    Vector3f normalizeC() const noexcept;

#pragma region Operators

#define OPERATOR(_RET, _OP, _PARAM, _MODS, _FUNC) inline _RET operator _OP(const _PARAM o) _MODS { return this->_FUNC(o); }
#define SCALAR_OPERATOR(_RET, _OP, _MODS, _FUNC) OPERATOR(_RET, _OP, float, _MODS, _FUNC); \
                                                 OPERATOR(_RET, _OP, i32, _MODS, _FUNC);

    OPERATOR(Vector3i, +, CompVec4&, const noexcept, addC);
    OPERATOR(Vector3i, -, CompVec4&, const noexcept, subC);
    OPERATOR(Vector3i, *, CompVec4&, const noexcept, crossC);

    OPERATOR(Vector3i&, +=, CompVec4&, noexcept, add);
    OPERATOR(Vector3i&, -=, CompVec4&, noexcept, sub);
    OPERATOR(Vector3i&, *=, CompVec4&, noexcept, cross);

    SCALAR_OPERATOR(Vector3i, +, const noexcept, addC);
    SCALAR_OPERATOR(Vector3i, -, const noexcept, subC);
    SCALAR_OPERATOR(Vector3f, *, const noexcept, mulC);
    SCALAR_OPERATOR(Vector3f, / , const noexcept, divC);

    SCALAR_OPERATOR(Vector3i&, +=, noexcept, add);
    SCALAR_OPERATOR(Vector3i&, -=, noexcept, sub);
    SCALAR_OPERATOR(Vector3i&, *=, noexcept, mul);
    SCALAR_OPERATOR(Vector3i&, /=, noexcept, div);

#undef OPERATOR
#undef SCALAR_OPERATOR

    inline Vector3i& operator -()       noexcept { return neg();     }
    inline Vector3i  operator -() const noexcept { return negCopy(); }
#pragma endregion
};

typedef Vector3i vec3i;
