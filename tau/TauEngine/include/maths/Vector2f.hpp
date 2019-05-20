#pragma once

#include <DLL.hpp>
#include <Maths.hpp>

class TAU_DLL Vector2f final
{
private:
    float _x;
    float _y;
public:
#pragma region Constructor
    Vector2f(const float x, const float y) noexcept
        : _x(x), _y(y)
    { }
    Vector2f(const float filler) noexcept
        : _x(filler), _y(filler)
    { }
    Vector2f() noexcept
        : _x(0.0f), _y(0.0f)
    { }

    Vector2f(const Vector2f& copy) noexcept = default;
    Vector2f(Vector2f&& move) noexcept = default;
#pragma endregion

    ~Vector2f() noexcept = default;

#pragma region Copy/Move Assignment Operators
    inline Vector2f& operator =(const float filler) noexcept
    {
        _x = filler;
        _y = filler;
        return *this;
    }

    Vector2f& operator =(const Vector2f& copy) noexcept = default;
    Vector2f& operator =(Vector2f&& move) noexcept = default;
#pragma endregion

#pragma region Accessor
    inline float x() const noexcept { return _x; }
    inline float y() const noexcept { return _y; }

    inline float& x() noexcept { return _x; }
    inline float& y() noexcept { return _y; }
#pragma endregion

#define RAW_VEC_FUNC(_RET, _NAME, _MODS) _RET _NAME(const float x, const float y) _MODS { return _NAME(Vector2f(x, y)); }

    RAW_VEC_FUNC(Vector2f&, add, noexcept);
    RAW_VEC_FUNC(Vector2f&, sub, noexcept);
    RAW_VEC_FUNC(Vector2f, addC, const noexcept);
    RAW_VEC_FUNC(Vector2f, subC, const noexcept);

    inline Vector2f& add(Vector2f&& data) noexcept
    {
        this->_x += data._x;
        this->_y += data._y;
        return *this;
    }

    inline Vector2f& sub(Vector2f&& data) noexcept
    {
        this->_x -= data._x;
        this->_y -= data._y;
        return *this;
    }

    inline Vector2f addC(Vector2f&& data) const noexcept
    {
        const float x = this->_x + data._x;
        const float y = this->_y + data._y;
        return Vector2f(x, y);
    }

    inline Vector2f subC(Vector2f&& data) const noexcept
    {
        const float x = this->_x - data._x;
        const float y = this->_y - data._y;
        return Vector2f(x, y);
    }

    inline Vector2f& add(const Vector2f& data) noexcept
    {
        this->_x += data._x;
        this->_y += data._y;
        return *this;
    }

    inline Vector2f& sub(const Vector2f& data) noexcept
    {
        this->_x -= data._x;
        this->_y -= data._y;
        return *this;
    }

    inline Vector2f addC(const Vector2f& data) const noexcept
    {
        const float x = this->_x + data._x;
        const float y = this->_y + data._y;
        return Vector2f(x, y);
    }

    inline Vector2f subC(const Vector2f& data) const noexcept
    {
        const float x = this->_x - data._x;
        const float y = this->_y - data._y;
        return Vector2f(x, y);
    }

#define SCALAR_MATH(_NAME) Vector2f& _NAME(const float scalar) noexcept; \
                           Vector2f _NAME##C(const float scalar) const noexcept;

    SCALAR_MATH(add)
    SCALAR_MATH(sub)
    SCALAR_MATH(mul)
    SCALAR_MATH(div)

#undef SCALAR_MATH

    inline Vector2f& scale(const float scalar)       noexcept { return mul(scalar); }
    inline Vector2f scaleC(const float scalar) const noexcept { return mulC(scalar); }

#pragma region Negate
    inline Vector2f& negate() noexcept
    {
        this->_x = -this->_x;
        this->_y = -this->_y;
        return *this;
    }

    inline Vector2f negateCopy() const noexcept
    {
        const float x = -this->_x;
        const float y = -this->_y;

        return Vector2f(x, y);
    }

    inline Vector2f& neg()           noexcept { return negate();     }
    inline Vector2f  negCopy() const noexcept { return negateCopy(); }
#pragma endregion

#pragma region Magnitude
    float magnitudeSquared() const noexcept
    { 
        return this->_x * this->_x + this->_y * this->_y;
    }

    float magnitude() const noexcept
    {
        return sqrtf(magnitudeSquared());
    }

    inline float lengthSquared() const noexcept { return magnitudeSquared(); }
    inline float length()        const noexcept { return magnitude();        }
#pragma endregion

    RAW_VEC_FUNC(float, dot, const noexcept)
    
    float dot(Vector2f&& data) const noexcept
    {
        return this->_x * data._x + this->_y * data._y;
    }
    
    float dot(const Vector2f& data) const noexcept
    {
        return this->_x * data._x + this->_y * data._y;
    }

    Vector2f& normalize() noexcept
    {
        const float invMag = rSqrt(magnitudeSquared());
        
        this->_x *= invMag;
        this->_y *= invMag;
        return *this;
    }

    Vector2f normalizeC() const noexcept
    {
        const float invMag = rSqrt(magnitudeSquared());

        const float x = this->_x * invMag;
        const float y = this->_y * invMag;
        return Vector2f(x, y);
    }

#undef RAW_VEC_FUNC

#pragma region Operators
#define OPERATOR(_RET, _OP, _PARAM, _MODS, _FUNC) inline _RET operator _OP(const _PARAM o) _MODS { return this->_FUNC(o); }
#define SCALAR_OPERATOR(_RET, _OP, _MODS, _FUNC) OPERATOR(_RET, _OP, float, _MODS, _FUNC);

    OPERATOR(Vector2f, +, Vector2f&&, const noexcept, addC);
    OPERATOR(Vector2f, -, Vector2f&&, const noexcept, subC);

    OPERATOR(Vector2f&, +=, Vector2f&&, noexcept, add);
    OPERATOR(Vector2f&, -=, Vector2f&&, noexcept, sub);

    OPERATOR(Vector2f, +, float, const noexcept, addC);;
    OPERATOR(Vector2f, -, float, const noexcept, subC);;
    OPERATOR(Vector2f, *, float, const noexcept, mulC);;
    OPERATOR(Vector2f, /, float, const noexcept, divC);;

    OPERATOR(Vector2f&, +=, float, noexcept, add);;
    OPERATOR(Vector2f&, -=, float, noexcept, sub);;
    OPERATOR(Vector2f&, *=, float, noexcept, mul);;
    OPERATOR(Vector2f&, /=, float, noexcept, div);;

#undef OPERATOR
#undef SCALAR_OPERATOR
#undef RAW_VEC_FUNC

    inline Vector2f& operator -()       noexcept { return neg();     }
    inline Vector2f  operator -() const noexcept { return negCopy(); }
#pragma endregion
};

typedef Vector2f vec2f;
typedef Vector2f vec2;
