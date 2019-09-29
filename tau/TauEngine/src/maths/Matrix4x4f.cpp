#include <maths/Matrix4x4f.hpp>
#include <maths/Vector4f.hpp>
#include <maths/Vector3f.hpp>
#include <Safeties.hpp>
#include <maths/Maths.hpp>
#include <maths/ComponentVector4.hpp>
#include <pmmintrin.h>

#ifdef __clang__
#define UNROLL_LOOP __pragma(unroll)
#else
#define UNROLL_LOOP 
#endif

Matrix4x4f::Matrix4x4f() noexcept
{
    this->_data.m00 = 1.0f;
    this->_data.m11 = 1.0f;
    this->_data.m22 = 1.0f;
    this->_data.m33 = 1.0f;

    this->_data.m01 = 0.0f;
    this->_data.m02 = 0.0f;
    this->_data.m03 = 0.0f;

    this->_data.m10 = 0.0f;
    this->_data.m12 = 0.0f;
    this->_data.m13 = 0.0f;

    this->_data.m20 = 0.0f;
    this->_data.m21 = 0.0f;
    this->_data.m23 = 0.0f;

    this->_data.m30 = 0.0f;
    this->_data.m31 = 0.0f;
    this->_data.m32 = 0.0f;
}

Matrix4x4f::Matrix4x4f(float filler) noexcept
{
UNROLL_LOOP
    for(u32 i = 0; i < 4; ++i)
    {
UNROLL_LOOP
        for(u32 j = 0; j < 4; ++j)
        {
            this->_data.glColumnMajorArr[i][j] = filler;
        }
    }
}

Matrix4x4f::Matrix4x4f(const Matrix4x4fData& store) noexcept
    : _data(store)
{ }


Matrix4x4f& Matrix4x4f::identity() noexcept
{
    this->_data.m00 = 1.0f;
    this->_data.m11 = 1.0f;
    this->_data.m22 = 1.0f;
    this->_data.m33 = 1.0f;

    this->_data.m01 = 0.0f;
    this->_data.m02 = 0.0f;
    this->_data.m03 = 0.0f;

    this->_data.m10 = 0.0f;
    this->_data.m12 = 0.0f;
    this->_data.m13 = 0.0f;

    this->_data.m20 = 0.0f;
    this->_data.m21 = 0.0f;
    this->_data.m23 = 0.0f;

    this->_data.m30 = 0.0f;
    this->_data.m31 = 0.0f;
    this->_data.m32 = 0.0f;

    return *this;
}

Matrix4x4f& Matrix4x4f::add(const Matrix4x4f& other) noexcept
{
UNROLL_LOOP
    for(u32 i = 0; i < 4; ++i)
    {
UNROLL_LOOP
        for(u32 j = 0; j < 4; ++j)
        {
            this->get(j, i) += other.get(j, i);
        }
    }

    return *this;
}

Matrix4x4f& Matrix4x4f::sub(const Matrix4x4f& other) noexcept
{
UNROLL_LOOP
    for(u32 i = 0; i < 4; ++i)
    {
UNROLL_LOOP
        for(u32 j = 0; j < 4; ++j)
        {
            this->get(j, i) -= other.get(j, i);
        }
    }

    return *this;
}

Matrix4x4f& Matrix4x4f::mul(const Matrix4x4f& other) noexcept
{
#define MAT_MUL(__X, __Y) this->_data.m ## __X ## __Y = oldData.m0 ## __X * other._data.m ## __Y ## 0 + \
                                                        oldData.m1 ## __X * other._data.m ## __Y ## 1 + \
                                                        oldData.m2 ## __X * other._data.m ## __Y ## 2 + \
                                                        oldData.m3 ## __X * other._data.m ## __Y ## 3;

    const Matrix4x4fData oldData = this->_data;

    MAT_MUL(0, 0);
    MAT_MUL(0, 1);
    MAT_MUL(0, 2);
    MAT_MUL(0, 3);

    MAT_MUL(1, 0);
    MAT_MUL(1, 1);
    MAT_MUL(1, 2);
    MAT_MUL(1, 3);

    MAT_MUL(2, 0);
    MAT_MUL(2, 1);
    MAT_MUL(2, 2);
    MAT_MUL(2, 3);

    MAT_MUL(3, 0);
    MAT_MUL(3, 1);
    MAT_MUL(3, 2);
    MAT_MUL(3, 3);

    return *this;
}

// Matrix4x4f& Matrix4x4f::mulSIMD(const Matrix4x4f& other) noexcept
// {
//     const Matrix4x4fData oldData = this->_data;
//     const Matrix4x4fData otherData = {
//         other._data.m00, other._data.m10, other._data.m20, other._data.m30,
//         other._data.m01, other._data.m11, other._data.m21, other._data.m31,
//         other._data.m02, other._data.m12, other._data.m22, other._data.m32,
//         other._data.m03, other._data.m13, other._data.m23, other._data.m33
//     };
//
//     CompVec4 postMul;
//
//     postMul = { _mm_mul_ps(oldData.x, otherData.x) };
//     this->_data.m00 = postMul.x + postMul.y + postMul.z + postMul.w;
//     postMul = { _mm_mul_ps(oldData.x, otherData.y) };
//     this->_data.m01 = postMul.x + postMul.y + postMul.z + postMul.w;
//     postMul = { _mm_mul_ps(oldData.x, otherData.z) };
//     this->_data.m02 = postMul.x + postMul.y + postMul.z + postMul.w;
//     postMul = { _mm_mul_ps(oldData.x, otherData.w) };
//     this->_data.m03 = postMul.x + postMul.y + postMul.z + postMul.w;
//
//     postMul = { _mm_mul_ps(oldData.y, otherData.x) };
//     this->_data.m10 = postMul.x + postMul.y + postMul.z + postMul.w;
//     postMul = { _mm_mul_ps(oldData.y, otherData.y) };
//     this->_data.m11 = postMul.x + postMul.y + postMul.z + postMul.w;
//     postMul = { _mm_mul_ps(oldData.y, otherData.z) };
//     this->_data.m12 = postMul.x + postMul.y + postMul.z + postMul.w;
//     postMul = { _mm_mul_ps(oldData.y, otherData.w) };
//     this->_data.m13 = postMul.x + postMul.y + postMul.z + postMul.w;
//
//     postMul = { _mm_mul_ps(oldData.z, otherData.x) };
//     this->_data.m20 = postMul.x + postMul.y + postMul.z + postMul.w;
//     postMul = { _mm_mul_ps(oldData.z, otherData.y) };
//     this->_data.m21 = postMul.x + postMul.y + postMul.z + postMul.w;
//     postMul = { _mm_mul_ps(oldData.z, otherData.z) };
//     this->_data.m22 = postMul.x + postMul.y + postMul.z + postMul.w;
//     postMul = { _mm_mul_ps(oldData.z, otherData.w) };
//     this->_data.m23 = postMul.x + postMul.y + postMul.z + postMul.w;
//
//     postMul = { _mm_mul_ps(oldData.w, otherData.x) };
//     this->_data.m30 = postMul.x + postMul.y + postMul.z + postMul.w;
//     postMul = { _mm_mul_ps(oldData.w, otherData.y) };
//     this->_data.m31 = postMul.x + postMul.y + postMul.z + postMul.w;
//     postMul = { _mm_mul_ps(oldData.w, otherData.z) };
//     this->_data.m32 = postMul.x + postMul.y + postMul.z + postMul.w;
//     postMul = { _mm_mul_ps(oldData.w, otherData.w) };
//     this->_data.m33 = postMul.x + postMul.y + postMul.z + postMul.w;
//
//     return *this;
// }

Matrix4x4f& Matrix4x4f::mulSIMD_SSE3(const Matrix4x4f& other) noexcept
{
    const Matrix4x4fData oldData = this->_data;
    const Matrix4x4fData otherData = {
        other._data.m00, other._data.m01, other._data.m02, other._data.m03,
        other._data.m10, other._data.m11, other._data.m12, other._data.m13,
        other._data.m20, other._data.m21, other._data.m22, other._data.m23,
        other._data.m30, other._data.m31, other._data.m32, other._data.m33
    };

    CompVec4 postMul;
    __m128 vec;

#define MAT_MUL_SIMD(__STORE, __LEFT, __RIGHT) vec = _mm_mul_ps(oldData.__LEFT, otherData.__RIGHT); \
                                               vec = _mm_hadd_ps(vec, vec);                         \
                                               postMul = { _mm_hadd_ps(vec, vec) };                 \
                                               this->_data.__STORE = postMul.x;

    MAT_MUL_SIMD(m00, x, x);
    MAT_MUL_SIMD(m01, x, y);
    MAT_MUL_SIMD(m02, x, z);
    MAT_MUL_SIMD(m03, x, w);

    MAT_MUL_SIMD(m10, y, x);
    MAT_MUL_SIMD(m11, y, y);
    MAT_MUL_SIMD(m12, y, z);
    MAT_MUL_SIMD(m13, y, w);

    MAT_MUL_SIMD(m20, z, x);
    MAT_MUL_SIMD(m21, z, y);
    MAT_MUL_SIMD(m22, z, z);
    MAT_MUL_SIMD(m23, z, w);

    MAT_MUL_SIMD(m30, w, x);
    MAT_MUL_SIMD(m31, w, y);
    MAT_MUL_SIMD(m32, w, z);
    MAT_MUL_SIMD(m33, w, w);

    return *this;
}

Matrix4x4f& Matrix4x4f::add(float scalar) noexcept
{
UNROLL_LOOP
    for(u32 i = 0; i < 4; ++i)
    {
UNROLL_LOOP
        for(u32 j = 0; j < 4; ++j)
        {
            this->get(j, i) += scalar;
        }
    }

    return *this;
}

Matrix4x4f& Matrix4x4f::sub(float scalar) noexcept
{
UNROLL_LOOP
    for(u32 i = 0; i < 4; ++i)
    {
UNROLL_LOOP
        for(u32 j = 0; j < 4; ++j)
        {
            this->get(j, i) -= scalar;
        }
    }

    return *this;
}

Matrix4x4f& Matrix4x4f::mul(float scalar) noexcept
{
UNROLL_LOOP
    for(u32 i = 0; i < 4; ++i)
    {
UNROLL_LOOP
        for(u32 j = 0; j < 4; ++j)
        {
            this->get(j, i) *= scalar;
        }
    }

    return *this;
}

Matrix4x4f& Matrix4x4f::addSIMD(float scalar) noexcept
{
    const __m128 vecFill = { scalar, scalar, scalar, scalar };

    this->_data.x = _mm_add_ps(vecFill, this->_data.x);
    this->_data.y = _mm_add_ps(vecFill, this->_data.y);
    this->_data.w = _mm_add_ps(vecFill, this->_data.z);
    this->_data.z = _mm_add_ps(vecFill, this->_data.w);

    return *this;
}

Matrix4x4f& Matrix4x4f::subSIMD(float scalar) noexcept
{
    const __m128 vecFill = { scalar, scalar, scalar, scalar };

    this->_data.x = _mm_sub_ps(vecFill, this->_data.x);
    this->_data.y = _mm_sub_ps(vecFill, this->_data.y);
    this->_data.w = _mm_sub_ps(vecFill, this->_data.z);
    this->_data.z = _mm_sub_ps(vecFill, this->_data.w);

    return *this;
}

Matrix4x4f& Matrix4x4f::mulSIMD(float scalar) noexcept
{
    const __m128 vecFill = { scalar, scalar, scalar, scalar };

    this->_data.x = _mm_mul_ps(vecFill, this->_data.x);
    this->_data.y = _mm_mul_ps(vecFill, this->_data.y);
    this->_data.w = _mm_mul_ps(vecFill, this->_data.z);
    this->_data.z = _mm_mul_ps(vecFill, this->_data.w);

    return *this;
}

CompVec4& Matrix4x4f::mul(Vector3f& vec) const noexcept
{
    const float x = vec.x();
    const float y = vec.y();
    const float z = vec.z();

    vec.x() = x * this->_data.m00 + y * this->_data.m10 + z * this->_data.m20 + this->_data.m30;
    vec.y() = x * this->_data.m01 + y * this->_data.m11 + z * this->_data.m21 + this->_data.m31;
    vec.z() = x * this->_data.m02 + y * this->_data.m12 + z * this->_data.m22 + this->_data.m32;
    vec.data().w = 1.0f;

    return vec;
}

CompVec4& Matrix4x4f::mul(Vector4f& vec) const noexcept
{
    const float x = vec.x();
    const float y = vec.y();
    const float z = vec.z();
    const float w = vec.w();

    vec.x() = x * this->_data.m00 + y * this->_data.m10 + z * this->_data.m20 + w * this->_data.m30;
    vec.y() = x * this->_data.m01 + y * this->_data.m11 + z * this->_data.m21 + w * this->_data.m31;
    vec.z() = x * this->_data.m02 + y * this->_data.m12 + z * this->_data.m22 + w * this->_data.m32;
    vec.w() = x * this->_data.m03 + y * this->_data.m13 + z * this->_data.m23 + w * this->_data.m33;

    return vec;
}

CompVec4& Matrix4x4f::mulSIMD(Vector3f& vec) const noexcept
{
    vec.data().w = 1.0f;
    const __m128 simdVec = vec.data().vec;

    CompVec4 compute;

    compute = { _mm_mul_ps(simdVec, this->_data.x) };
    vec.x() = compute.x + compute.y + compute.z + compute.w;
    compute = { _mm_mul_ps(simdVec, this->_data.y) };
    vec.y() = compute.x + compute.y + compute.z + compute.w;
    compute = { _mm_mul_ps(simdVec, this->_data.z) };
    vec.z() = compute.x + compute.y + compute.z + compute.w;

    return vec;
}

CompVec4& Matrix4x4f::mulSIMD(Vector4f& vec) const noexcept
{
    const __m128 simdVec = vec.data().vec;

    CompVec4 compute;

    compute = { _mm_mul_ps(simdVec, this->_data.x) };
    vec.x() = compute.x + compute.y + compute.z + compute.w;
    compute = { _mm_mul_ps(simdVec, this->_data.y) };
    vec.y() = compute.x + compute.y + compute.z + compute.w;
    compute = { _mm_mul_ps(simdVec, this->_data.z) };
    vec.z() = compute.x + compute.y + compute.z + compute.w;
    compute = { _mm_mul_ps(simdVec, this->_data.w) };
    vec.w() = compute.x + compute.y + compute.z + compute.w;

    return vec;
}

#define MAT_VEC_MUL_SIMD(__SLOT) computeVec = _mm_mul_ps(simdVec, this->_data.__SLOT); \
                                 computeVec = _mm_hadd_ps(computeVec, computeVec);     \
                                 compute = { _mm_hadd_ps(computeVec, computeVec) };    \
                                 vec.__SLOT() = compute.x;

CompVec4& Matrix4x4f::mulSIMD_SSE3(Vector3f& vec) const noexcept
{
    vec.data().w = 1.0f;
    const __m128 simdVec = vec.data().vec;

    __m128 computeVec;
    CompVec4 compute;

    MAT_VEC_MUL_SIMD(x);
    MAT_VEC_MUL_SIMD(y);
    MAT_VEC_MUL_SIMD(z);

    return vec;
}

CompVec4& Matrix4x4f::mulSIMD_SSE3(Vector4f& vec) const noexcept
{
    const __m128 simdVec = vec.data().vec;

    __m128 computeVec;
    CompVec4 compute;

    MAT_VEC_MUL_SIMD(x);
    MAT_VEC_MUL_SIMD(y);
    MAT_VEC_MUL_SIMD(z);
    MAT_VEC_MUL_SIMD(w);

    return vec;
}

#undef MAT_VEC_MUL_SIMD

Matrix4x4f& Matrix4x4f::rotateX(float angle) noexcept
{
    const float sin = fastSinR(angle);
    const float cos = fastCosR(angle);
    const float nSin = -sin;

    const float nm10 = _data.glColumnMajorArr[1][0] * cos + _data.glColumnMajorArr[2][0] * sin;
    const float nm11 = _data.glColumnMajorArr[1][1] * cos + _data.glColumnMajorArr[2][1] * sin;
    const float nm12 = _data.glColumnMajorArr[1][2] * cos + _data.glColumnMajorArr[2][2] * sin;
    const float nm13 = _data.glColumnMajorArr[1][3] * cos + _data.glColumnMajorArr[2][3] * sin;

    _data.glColumnMajorArr[2][0] = _data.glColumnMajorArr[1][0] * nSin + _data.glColumnMajorArr[2][0] * cos;
    _data.glColumnMajorArr[2][1] = _data.glColumnMajorArr[1][1] * nSin + _data.glColumnMajorArr[2][1] * cos;
    _data.glColumnMajorArr[2][2] = _data.glColumnMajorArr[1][2] * nSin + _data.glColumnMajorArr[2][2] * cos;
    _data.glColumnMajorArr[2][3] = _data.glColumnMajorArr[1][3] * nSin + _data.glColumnMajorArr[2][3] * cos;

    _data.glColumnMajorArr[1][0] = nm10;
    _data.glColumnMajorArr[1][1] = nm11;
    _data.glColumnMajorArr[1][2] = nm12;
    _data.glColumnMajorArr[1][3] = nm13;

    return *this;
}

Matrix4x4f& Matrix4x4f::rotateY(float angle) noexcept
{
    const float sin = fastSinR(angle);
    const float cos = fastCosR(angle);
    const float nSin = -sin;

    const float nm00 = _data.glColumnMajorArr[0][0] * cos + _data.glColumnMajorArr[2][0] * nSin;
    const float nm01 = _data.glColumnMajorArr[0][1] * cos + _data.glColumnMajorArr[2][1] * nSin;
    const float nm02 = _data.glColumnMajorArr[0][2] * cos + _data.glColumnMajorArr[2][2] * nSin;
    const float nm03 = _data.glColumnMajorArr[0][3] * cos + _data.glColumnMajorArr[2][3] * nSin;

    _data.glColumnMajorArr[2][0] = _data.glColumnMajorArr[0][0] * sin + _data.glColumnMajorArr[2][0] * cos;
    _data.glColumnMajorArr[2][1] = _data.glColumnMajorArr[0][1] * sin + _data.glColumnMajorArr[2][1] * cos;
    _data.glColumnMajorArr[2][2] = _data.glColumnMajorArr[0][2] * sin + _data.glColumnMajorArr[2][2] * cos;
    _data.glColumnMajorArr[2][3] = _data.glColumnMajorArr[0][3] * sin + _data.glColumnMajorArr[2][3] * cos;

    _data.glColumnMajorArr[0][0] = nm00;
    _data.glColumnMajorArr[0][1] = nm01;
    _data.glColumnMajorArr[0][2] = nm02;
    _data.glColumnMajorArr[0][3] = nm03;

    return *this;
}

Matrix4x4f& Matrix4x4f::rotateZ(float angle) noexcept
{
    const float sin = fastSinR(angle);
    const float cos = fastCosR(angle);
    const float nSin = -sin;

    const float nm00 = _data.glColumnMajorArr[0][0] * cos + _data.glColumnMajorArr[1][0] * sin;
    const float nm01 = _data.glColumnMajorArr[0][1] * cos + _data.glColumnMajorArr[1][1] * sin;
    const float nm02 = _data.glColumnMajorArr[0][2] * cos + _data.glColumnMajorArr[1][2] * sin;
    const float nm03 = _data.glColumnMajorArr[0][3] * cos + _data.glColumnMajorArr[1][3] * sin;

    _data.glColumnMajorArr[1][0] = _data.glColumnMajorArr[0][0] * nSin + _data.glColumnMajorArr[1][0] * cos;
    _data.glColumnMajorArr[1][1] = _data.glColumnMajorArr[0][1] * nSin + _data.glColumnMajorArr[1][1] * cos;
    _data.glColumnMajorArr[1][2] = _data.glColumnMajorArr[0][2] * nSin + _data.glColumnMajorArr[1][2] * cos;
    _data.glColumnMajorArr[1][3] = _data.glColumnMajorArr[0][3] * nSin + _data.glColumnMajorArr[1][3] * cos;

    _data.glColumnMajorArr[0][0] = nm00;
    _data.glColumnMajorArr[0][1] = nm01;
    _data.glColumnMajorArr[0][2] = nm02;
    _data.glColumnMajorArr[0][3] = nm03;

    return *this;
}

Matrix4x4f& Matrix4x4f::translate(const Vector3f& translation) noexcept
{
    this->_data.glColumnMajorArr[3][0] += translation.x();
    this->_data.glColumnMajorArr[3][1] += translation.y();
    this->_data.glColumnMajorArr[3][2] += translation.z();

    return *this;
}

Matrix4x4f& Matrix4x4f::rotateOptimized(const Vector3f& rotation) noexcept
{
    const float z_sin = fastSinR(rotation.z());
    const float z_cos = fastCosR(rotation.z());
    const float z_nSin = -z_sin;

    const float y_sin = fastSinR(rotation.y());
    const float y_cos = fastCosR(rotation.y());
    const float y_nSin = -y_sin;

    const float x_sin = fastSinR(rotation.x());
    const float x_cos = fastCosR(rotation.x());
    const float x_nSin = -x_sin;


    const float z_nm00 = _data.glColumnMajorArr[0][0] * z_cos;
    const float z_nm01 = _data.glColumnMajorArr[1][1] * z_sin;

    _data.glColumnMajorArr[1][0] = _data.glColumnMajorArr[0][0] * z_nSin;
    _data.glColumnMajorArr[1][1] = _data.glColumnMajorArr[1][1] * z_cos;

    _data.glColumnMajorArr[0][0] = z_nm00;
    _data.glColumnMajorArr[0][1] = z_nm01;
                          

    const float y_nm00 = _data.glColumnMajorArr[0][0] * y_cos;
    const float y_nm01 = _data.glColumnMajorArr[0][1] * y_cos;
    const float y_nm02 = _data.glColumnMajorArr[2][2] * y_nSin;

    _data.glColumnMajorArr[2][0] = _data.glColumnMajorArr[0][0] * y_sin;
    _data.glColumnMajorArr[2][1] = _data.glColumnMajorArr[0][1] * y_sin;
    _data.glColumnMajorArr[2][2] = _data.glColumnMajorArr[2][2] * y_cos;

    _data.glColumnMajorArr[0][0] = y_nm00;
    _data.glColumnMajorArr[0][1] = y_nm01;
    _data.glColumnMajorArr[0][2] = y_nm02;


    const float x_nm10 = _data.glColumnMajorArr[1][0] * x_cos + _data.glColumnMajorArr[2][0] * x_sin;
    const float x_nm11 = _data.glColumnMajorArr[1][1] * x_cos + _data.glColumnMajorArr[2][1] * x_sin;
    const float x_nm12 = _data.glColumnMajorArr[2][2] * x_sin;

    _data.glColumnMajorArr[2][0] = _data.glColumnMajorArr[1][0] * x_nSin + _data.glColumnMajorArr[2][0] * x_cos;
    _data.glColumnMajorArr[2][1] = _data.glColumnMajorArr[1][1] * x_nSin + _data.glColumnMajorArr[2][1] * x_cos;
    _data.glColumnMajorArr[2][2] = _data.glColumnMajorArr[2][2] * x_cos;

    _data.glColumnMajorArr[1][0] = x_nm10;
    _data.glColumnMajorArr[1][1] = x_nm11;
    _data.glColumnMajorArr[1][2] = x_nm12;

    return *this;
}

/**
 * Rotate X, then Y.
 */
Matrix4x4f& Matrix4x4f::rotateCamera(const float pitch, const float yaw) noexcept
{
    const float pitch_sin = fastSinR(pitch);
    const float pitch_cos = fastCosR(pitch);
    const float pitch_nSin = -pitch_sin;

    const float yaw_sin = fastSinR(yaw);
    const float yaw_cos = fastCosR(yaw);
    const float yaw_nSin = -yaw_sin;


    _data.glColumnMajorArr[2][1] = pitch_nSin;
    _data.glColumnMajorArr[2][2] = pitch_cos;

    _data.glColumnMajorArr[1][1] = pitch_cos;
    _data.glColumnMajorArr[1][2] = pitch_sin;


    _data.glColumnMajorArr[2][0] = yaw_sin;
    _data.glColumnMajorArr[2][1] = pitch_nSin * yaw_cos;
    _data.glColumnMajorArr[2][2] = pitch_cos  * yaw_cos;

    _data.glColumnMajorArr[0][0] = yaw_cos;
    _data.glColumnMajorArr[0][1] = pitch_nSin * yaw_nSin;
    _data.glColumnMajorArr[0][2] = pitch_cos  * yaw_nSin;

    return *this;
}

Matrix4x4f& Matrix4x4f::rotate(const Vector3f& rotation) noexcept
{
    this->rotateZ(rotation.z());
    this->rotateY(rotation.y());
    this->rotateX(rotation.x());

    return *this;
}

Matrix4x4f& Matrix4x4f::scale(const Vector3f& scaling) noexcept
{
    this->_data.m00 += scaling.x();
    this->_data.m11 += scaling.y();
    this->_data.m22 += scaling.z();

    return *this;
}

Matrix4x4f& Matrix4x4f::scale(const float scaling) noexcept
{
    this->_data.m00 += scaling;
    this->_data.m11 += scaling;
    this->_data.m22 += scaling;

    return *this;
}

Matrix4x4f& Matrix4x4f::setScale(const Vector3f& scale) noexcept
{

    this->_data.m00 = scale.x();
    this->_data.m11 = scale.y();
    this->_data.m22 = scale.z();

    return *this;
}

Matrix4x4f& Matrix4x4f::setScale(const float scale) noexcept
{

    this->_data.m00 = scale;
    this->_data.m11 = scale;
    this->_data.m22 = scale;

    return *this;
}

Matrix4x4f Matrix4x4f::translation(const Vector3f& translation) noexcept
{
    Matrix4x4f matrix;

    matrix._data.glColumnMajorArr[3][0] = translation.x();
    matrix._data.glColumnMajorArr[3][1] = translation.y();
    matrix._data.glColumnMajorArr[3][2] = translation.z();
    matrix._data.glColumnMajorArr[3][3] = 1.0f;

    matrix._data.glColumnMajorArr[0][1] = 0.0f;
    matrix._data.glColumnMajorArr[0][2] = 0.0f;
    matrix._data.glColumnMajorArr[0][3] = 0.0f;

    matrix._data.glColumnMajorArr[1][0] = 0.0f;
    matrix._data.glColumnMajorArr[1][2] = 0.0f;
    matrix._data.glColumnMajorArr[1][3] = 0.0f;

    matrix._data.glColumnMajorArr[2][0] = 0.0f;
    matrix._data.glColumnMajorArr[2][1] = 0.0f;
    matrix._data.glColumnMajorArr[2][3] = 0.0f;

    matrix._data.glColumnMajorArr[0][0] = 0.0f;
    matrix._data.glColumnMajorArr[1][1] = 0.0f;
    matrix._data.glColumnMajorArr[2][2] = 0.0f;

    return matrix;
}

Matrix4x4f& Matrix4x4f::setTranslation(const Vector3f& translation) noexcept
{
    this->_data.glColumnMajorArr[3][0] = translation.x();
    this->_data.glColumnMajorArr[3][1] = translation.y();
    this->_data.glColumnMajorArr[3][2] = translation.z();

    return *this;
}

Matrix4x4f Matrix4x4f::rotation(const Vector3f& rotation) noexcept
{
    Matrix4x4f matrix;

    matrix.rotate(rotation);

    return matrix;
}

Matrix4x4f Matrix4x4f::scalar(const Vector3f& scaling) noexcept
{
    Matrix4x4f matrix;

    matrix._data.glColumnMajorArr[0][0] = scaling.x();
    matrix._data.glColumnMajorArr[1][1] = scaling.y();
    matrix._data.glColumnMajorArr[2][2] = scaling.z();
    matrix._data.glColumnMajorArr[3][3] = 1.0f;

    matrix._data.glColumnMajorArr[0][1] = 0.0f;
    matrix._data.glColumnMajorArr[0][2] = 0.0f;
    matrix._data.glColumnMajorArr[0][3] = 0.0f;

    matrix._data.glColumnMajorArr[1][0] = 0.0f;
    matrix._data.glColumnMajorArr[1][2] = 0.0f;
    matrix._data.glColumnMajorArr[1][3] = 0.0f;

    matrix._data.glColumnMajorArr[2][0] = 0.0f;
    matrix._data.glColumnMajorArr[2][1] = 0.0f;
    matrix._data.glColumnMajorArr[2][3] = 0.0f;

    matrix._data.glColumnMajorArr[3][0] = 0.0f;
    matrix._data.glColumnMajorArr[3][1] = 0.0f;
    matrix._data.glColumnMajorArr[3][2] = 0.0f;

    return matrix;
}

Matrix4x4f Matrix4x4f::perspective(float fov, float aspect, float nearPlane, float farPlane, bool leftHanded) noexcept
{
    Assert(fov > 0);
    Assert(aspect != 0);
    Assert(nearPlane < farPlane);

    const float depth = farPlane - nearPlane;
    const float rDepth = 1.0f / depth;

    Matrix4x4fData matrix = { };

    const float halfFOV = fov * 0.5f;

    matrix.m11 = fastCosR(halfFOV) / fastSinR(halfFOV);
    matrix.m00 = (leftHanded ? 1 : -1) * matrix.m11 / aspect;
    matrix.m22 = (farPlane + nearPlane) * rDepth;
    matrix.m23 = (-2 * farPlane * nearPlane) * rDepth;
    matrix.m32 = 1.0f;
    matrix.m33 = 0.0f;

    matrix.m01 = 0.0f;
    matrix.m02 = 0.0f;
    matrix.m03 = 0.0f;

    matrix.m10 = 0.0f;
    matrix.m12 = 0.0f;
    matrix.m13 = 0.0f;

    matrix.m20 = 0.0f;
    matrix.m21 = 0.0f;

    matrix.m30 = 0.0f;
    matrix.m31 = 0.0f;

    return matrix;
}

Matrix4x4f Matrix4x4f::infDepthPerspective(float fov, float aspect, bool leftHanded) noexcept
{
    Assert(fov > 0);
    Assert(aspect != 0);

    Matrix4x4fData matrix = { };

    const float halfFOV = fov * 0.5f;

    matrix.m11 = fastCosR(halfFOV) / fastSinR(halfFOV);
    matrix.m00 = (leftHanded ? 1 : -1) * matrix.m11 / aspect;
    matrix.m22 = 1.0f;
    matrix.m23 = 0.0f;
    matrix.m32 = 1.0f;
    matrix.m33 = 0.0f;

    matrix.m01 = 0.0f;
    matrix.m02 = 0.0f;
    matrix.m03 = 0.0f;

    matrix.m10 = 0.0f;
    matrix.m12 = 0.0f;
    matrix.m13 = 0.0f;

    matrix.m20 = 0.0f;
    matrix.m21 = 0.0f;

    matrix.m30 = 0.0f;
    matrix.m31 = 0.0f;

    return matrix;
}

Matrix4x4f Matrix4x4f::orthographic(float left, float right, float bottom, float top, float zNear, float zFar) noexcept
{
    Matrix4x4fData matrix = { };

    matrix.m00 = (2.0f / (right - left)) * 800;
    matrix.m10 = 0.0f;
    matrix.m20 = 0.0f;
    matrix.m30 = 0.0f;

    matrix.m01 = 0.0f;
    matrix.m11 = (2.0f / (top - bottom)) * 800;
    matrix.m21 = 0.0f;
    matrix.m31 = 0.0f;

    matrix.m02 = 0.0f;
    matrix.m12 = 0.0f;
    matrix.m22 = 2.0f / (zFar - zNear);
    matrix.m32 = 0.0f;

    matrix.m03 = -(right + left) / (right - left);
    matrix.m13 = -(top + bottom) / (top - bottom);
    matrix.m23 = -(zFar + zNear) / (zFar - zNear);
    matrix.m33 = 1.0f;

    return matrix;
}

Matrix4x4f Matrix4x4f::orthographic(float left, float right, float bottom, float top) noexcept
{
    Matrix4x4fData matrix = { };

    matrix.m00 = (2.0f / (right - left)) * 1;
    matrix.m10 = 0.0f;
    matrix.m20 = 0.0f;
    matrix.m30 = 0.0f;

    matrix.m01 = 0.0f;
    matrix.m11 = (2.0f / (top - bottom)) * 1;
    matrix.m21 = 0.0f;
    matrix.m31 = 0.0f;

    matrix.m02 = 0.0f;
    matrix.m12 = 0.0f;
    matrix.m22 = -1.0f;
    matrix.m32 = 0.0f;

    matrix.m03 = -(right + left) / (right - left);
    matrix.m13 = -(top + bottom) / (top - bottom);
    matrix.m23 = 0.0f;
    matrix.m33 = 1.0f;

    return matrix;
}

Matrix4x4f Matrix4x4f::transformation(const Vector3f& RESTRICT translation, const Vector3f& RESTRICT rotation, const Vector3f& scale) noexcept
{
    Matrix4x4f matrix(0.0f);

    matrix._data.glColumnMajorArr[3][0] = translation.x();
    matrix._data.glColumnMajorArr[3][1] = translation.y();
    matrix._data.glColumnMajorArr[3][2] = translation.z();

    matrix._data.m00 = scale.x();
    matrix._data.m11 = scale.y();
    matrix._data.m22 = scale.z();

    matrix._data.m33 = 1.0f;

    matrix.rotateOptimized(rotation);

    return matrix;
}

Matrix4x4f Matrix4x4f::transformation(const Vector3f& RESTRICT translation, const Vector3f& RESTRICT rotation, const float scale) noexcept
{
    Matrix4x4f matrix(0.0f);

    matrix._data.glColumnMajorArr[3][0] = translation.x();
    matrix._data.glColumnMajorArr[3][1] = translation.y();
    matrix._data.glColumnMajorArr[3][2] = translation.z();

    matrix._data.m00 = scale;
    matrix._data.m11 = scale;
    matrix._data.m22 = scale;

    matrix._data.m33 = 1.0f;

    matrix.rotateOptimized(rotation);

    return matrix;
}

Matrix4x4f& Matrix4x4f::transformation(const Vector3f& RESTRICT rotation, const Vector3f& RESTRICT scale) noexcept
{
    this->_data.glColumnMajorArr[0][0] = scale.x();
    this->_data.glColumnMajorArr[1][1] = scale.y();
    this->_data.glColumnMajorArr[2][2] = scale.z();

    this->_data.glColumnMajorArr[0][1] = 0.0f;
    this->_data.glColumnMajorArr[0][2] = 0.0f;
    this->_data.glColumnMajorArr[0][3] = 0.0f;

    this->_data.glColumnMajorArr[1][0] = 0.0f;
    this->_data.glColumnMajorArr[1][2] = 0.0f;
    this->_data.glColumnMajorArr[1][3] = 0.0f;

    this->_data.glColumnMajorArr[2][0] = 0.0f;
    this->_data.glColumnMajorArr[2][1] = 0.0f;
    this->_data.glColumnMajorArr[2][3] = 0.0f;

    this->rotateOptimized(rotation);

    return *this;
}

Matrix4x4f& Matrix4x4f::transformation(const Vector3f& rotation, const float scale) noexcept
{
    this->_data.glColumnMajorArr[0][0] = scale;
    this->_data.glColumnMajorArr[1][1] = scale;
    this->_data.glColumnMajorArr[2][2] = scale;

    this->_data.glColumnMajorArr[0][1] = 0.0f;
    this->_data.glColumnMajorArr[0][2] = 0.0f;
    this->_data.glColumnMajorArr[0][3] = 0.0f;

    this->_data.glColumnMajorArr[1][0] = 0.0f;
    this->_data.glColumnMajorArr[1][2] = 0.0f;
    this->_data.glColumnMajorArr[1][3] = 0.0f;

    this->_data.glColumnMajorArr[2][0] = 0.0f;
    this->_data.glColumnMajorArr[2][1] = 0.0f;
    this->_data.glColumnMajorArr[2][3] = 0.0f;

    // this->rotateST(rotation);
    // this->rotateZ(rotation.z());
    // this->rotateY(rotation.y());
    // this->rotateX(rotation.x());
    this->rotateOptimized(rotation);

    return *this;
}

Matrix4x4f& Matrix4x4f::view(const Vector3f& translation, const float pitch, const float yaw) noexcept
{
    this->_data.glColumnMajorArr[3][0] = translation.x();
    this->_data.glColumnMajorArr[3][1] = translation.y();
    this->_data.glColumnMajorArr[3][2] = translation.z();

    this->_data.glColumnMajorArr[0][1] = 0.0f;

    this->_data.glColumnMajorArr[0][3] = 0.0f;
    this->_data.glColumnMajorArr[1][3] = 0.0f;
    this->_data.glColumnMajorArr[2][3] = 0.0f;

    this->rotateCamera(pitch, yaw);

    return *this;
}

Matrix4x4f& Matrix4x4f::fps(const Vector3f translation, const SinCos<float> pitchSC, const SinCos<float> yawSC) noexcept
{
    const float pitch_nSin = -pitchSC.sin;
    const float yaw_nSin = -yawSC.sin;

    const float ySpS = yawSC.sin * pitchSC.sin;
    const float ySpC = yawSC.sin * pitchSC.cos;
    const float yCpS = yawSC.cos * pitchSC.sin;
    const float pCyC = pitchSC.cos * yawSC.cos;

    this->_data.glColumnMajorArr[0][0] = yawSC.cos;
    this->_data.glColumnMajorArr[0][1] = ySpS;
    this->_data.glColumnMajorArr[0][2] = ySpC;
    this->_data.glColumnMajorArr[0][3] = 0.0f;

    this->_data.glColumnMajorArr[1][0] = 0.0f;
    this->_data.glColumnMajorArr[1][1] = pitchSC.cos;
    this->_data.glColumnMajorArr[1][2] = pitch_nSin;
    this->_data.glColumnMajorArr[1][3] = 0.0f;

    this->_data.glColumnMajorArr[2][0] = yaw_nSin;
    this->_data.glColumnMajorArr[2][1] = yCpS;
    this->_data.glColumnMajorArr[2][2] = pCyC;
    this->_data.glColumnMajorArr[2][3] = 0.0f;

    this->_data.glColumnMajorArr[3][0] = -(yawSC.cos * translation.x() + yaw_nSin * translation.z());
    this->_data.glColumnMajorArr[3][1] = -(ySpS * translation.x() + pitchSC.cos * translation.y() + yCpS * translation.z());
    this->_data.glColumnMajorArr[3][2] = -(ySpC * translation.x() + pitch_nSin * translation.y() + pCyC * translation.z());
    this->_data.glColumnMajorArr[3][3] = 1.0f;

    return *this;
}

Matrix4x4f& Matrix4x4f::fps(const Vector3f translation, const float pitch, const float yaw) noexcept
{
    const SinCos<float> pitchSC = fastSinCosR(-pitch);
    const SinCos<float> yawSC = fastSinCosR(-yaw);

    return fps(translation, pitchSC, yawSC);
}

Matrix4x4f& Matrix4x4f::fpsD(const Vector3f translation, const float pitch, const float yaw) noexcept
{
    const SinCos<float> pitchSC = fastSinCosD(-pitch);
    const SinCos<float> yawSC = fastSinCosD(-yaw);

    return fps(translation, pitchSC, yawSC);
}
