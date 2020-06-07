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
    Matrix4x4f ret{ };
    matrix4x4f_sub(a.mRaw, b.mRaw, ret.mRaw);
    return ret;
}

Matrix4x4f Matrix4x4f::mul(const Matrix4x4f& a, const Matrix4x4f& b) noexcept
{
    Matrix4x4f ret{ };
    matrix4x4f_mul(a.mRaw, b.mRaw, ret.mRaw);
    return ret;
}


