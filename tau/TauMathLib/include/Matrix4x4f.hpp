#pragma once

#include <xmmintrin.h>
#include "TauMathLibInternal.h"
#include "Vector4f.hpp"

class TAU_DLL Matrix4x4f final
{
public:
    union ALIGN_MSVC(16)
    {
        struct
        {
            __m128 col0;
            __m128 col1;
            __m128 col2;
            __m128 col3;
        };
        float m[4][4];
        float mRaw[16];
    } ALIGN_GCC(16);
public:
    static Matrix4x4f identity() noexcept
    {
        Matrix4x4f matrix { };
        matrix.m[0][0] = 1.0f;
        matrix.m[1][1] = 1.0f;
        matrix.m[2][2] = 1.0f;
        matrix.m[3][3] = 1.0f;
        return matrix;
    }

    static Matrix4x4f add(const Matrix4x4f& a, const Matrix4x4f& b) noexcept;
    static Matrix4x4f sub(const Matrix4x4f& a, const Matrix4x4f& b) noexcept;
    static Matrix4x4f mul(const Matrix4x4f& a, const Matrix4x4f& b) noexcept;

    static Matrix4x4f add(const Matrix4x4f& a, float b) noexcept;
    static Matrix4x4f sub(const Matrix4x4f& a, float b) noexcept;
    static Matrix4x4f mul(const Matrix4x4f& a, float b) noexcept;
    static Matrix4x4f div(const Matrix4x4f& a, float b) noexcept;
    static Matrix4x4f divFast(const Matrix4x4f& a, float b) noexcept;

    static Vector4f mul(const Matrix4x4f& a, Vector4f b) noexcept;
    static Vector4f mul(Vector4f b, const Matrix4x4f& a) noexcept;
public:
    inline Matrix4x4f() noexcept
    {
        for(int i = 0; i < 16; ++i)
        { mRaw[i] = 0.0f; }
    }

    inline Matrix4x4f(const float* matrix) noexcept
    {
        for(int i = 0; i < 16; ++i)
        { mRaw[i] = matrix[i]; }
    }

    inline Matrix4x4f(const __m128* vectors) noexcept
    {
        col0 = vectors[0];
        col1 = vectors[1];
        col2 = vectors[2];
        col3 = vectors[3];
    }

    inline Matrix4x4f(const __m128 _col0, const __m128 _col1, const __m128 _col2, const __m128 _col3) noexcept
    {
        col0 = _col0;
        col1 = _col1;
        col2 = _col2;
        col3 = _col3;
    }

    inline ~Matrix4x4f() noexcept = default;

    inline Matrix4x4f(const Matrix4x4f& copy) noexcept = default;
    inline Matrix4x4f(Matrix4x4f&& move) noexcept = default;

    inline Matrix4x4f& operator=(const Matrix4x4f& copy) noexcept = default;
    inline Matrix4x4f& operator=(Matrix4x4f&& move) noexcept = default;

};
