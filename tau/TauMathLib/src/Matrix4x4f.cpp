#include "Matrix4x4f.hpp"
#include "Matrix4x4fIntrin.h"

Matrix4x4f Matrix4x4f::add(const Matrix4x4f& a, const Matrix4x4f& b) noexcept
{
    Matrix4x4f ret { };
    matrix4x4f_add(a.mRaw, b.mRaw, ret.mRaw);
    return ret;
}

Matrix4x4f Matrix4x4f::sub(const Matrix4x4f& a, const Matrix4x4f& b) noexcept
{
    Matrix4x4f ret { };
    matrix4x4f_sub(a.mRaw, b.mRaw, ret.mRaw);
    return ret;
}

Matrix4x4f Matrix4x4f::mul(const Matrix4x4f& a, const Matrix4x4f& b) noexcept
{
    Matrix4x4f ret { };
    matrix4x4f_mul(a.mRaw, b.mRaw, ret.mRaw);
    return ret;
}

Matrix4x4f Matrix4x4f::add(const Matrix4x4f& a, const float b) noexcept
{
    Matrix4x4f ret { };
    matrix4x4f_addScalar(a.mRaw, b, ret.mRaw);
    return ret;
}

Matrix4x4f Matrix4x4f::sub(const Matrix4x4f& a, const float b) noexcept
{
    Matrix4x4f ret { };
    matrix4x4f_subScalar(a.mRaw, b, ret.mRaw);
    return ret;
}

Matrix4x4f Matrix4x4f::mul(const Matrix4x4f& a, const float b) noexcept
{
    Matrix4x4f ret{ };
    matrix4x4f_mulScalar(a.mRaw, b, ret.mRaw);
    return ret;
}

Matrix4x4f Matrix4x4f::div(const Matrix4x4f& a, const float b) noexcept
{
    Matrix4x4f ret { };
    matrix4x4f_divScalar(a.mRaw, b, ret.mRaw);
    return ret;
}

Matrix4x4f Matrix4x4f::divFast(const Matrix4x4f& a, const float b) noexcept
{
    Matrix4x4f ret { };
    matrix4x4f_divFastScalar(a.mRaw, b, ret.mRaw);
    return ret;
}

Vector4f Matrix4x4f::mul(const Matrix4x4f& a, const Vector4f b) noexcept
{ return Vector4f(matrix4x4f_mulVector(a.mRaw, b.vec)); }

Vector4f Matrix4x4f::mul(const Vector4f b, const Matrix4x4f& a) noexcept
{
    Matrix4x4f transpose { };
    matrix4x4f_transpose(a.mRaw, transpose.mRaw);
    return Vector4f(matrix4x4f_mulVector(transpose.mRaw, b.vec));
}
