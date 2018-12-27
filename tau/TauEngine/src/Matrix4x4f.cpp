#include <maths/Matrix4x4f.hpp>
#include <maths/Vector4f.hpp>
#include <maths/Vector3f.hpp>
#include <Safeties.hpp>
#include <Maths.hpp>

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
    for(u32 i = 0; i < 4; ++i)
    {
        for(u32 j = 0; j < 4; ++j)
        {
            this->get(j, i) = filler;
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
#pragma unroll
    for(u32 i = 0; i < 4; ++i)
    {
#pragma unroll
        for(u32 j = 0; j < 4; ++j)
        {
            this->get(j, i) += other.get(j, i);
        }
    }

    return *this;
}

Matrix4x4f& Matrix4x4f::sub(const Matrix4x4f& other) noexcept
{
#pragma unroll
    for(u32 i = 0; i < 4; ++i)
    {
#pragma unroll
        for(u32 j = 0; j < 4; ++j)
        {
            this->get(j, i) -= other.get(j, i);
        }
    }

    return *this;
}

Matrix4x4f& Matrix4x4f::mul(const Matrix4x4f& other) noexcept
{
#define MAT_MUL(__X, __Y) this->_data.m ## __X ## __Y = this->_data.m0 ## __X * other._data.m ## __Y ## 0 + \
                                                        this->_data.m1 ## __X * other._data.m ## __Y ## 1 + \
                                                        this->_data.m2 ## __X * other._data.m ## __Y ## 2 + \
                                                        this->_data.m3 ## __X * other._data.m ## __Y ## 3;

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

Matrix4x4f& Matrix4x4f::add(float scalar) noexcept
{
#pragma unroll
    for(u32 i = 0; i < 4; ++i)
    {
#pragma unroll
        for(u32 j = 0; j < 4; ++j)
        {
            this->get(j, i) += scalar;
        }
    }

    return *this;
}

Matrix4x4f& Matrix4x4f::sub(float scalar) noexcept
{
#pragma unroll
    for(u32 i = 0; i < 4; ++i)
    {
#pragma unroll
        for(u32 j = 0; j < 4; ++j)
        {
            this->get(j, i) -= scalar;
        }
    }

    return *this;
}

Matrix4x4f& Matrix4x4f::mul(float scalar) noexcept
{
#pragma unroll
    for(u32 i = 0; i < 4; ++i)
    {
#pragma unroll
        for(u32 j = 0; j < 4; ++j)
        {
            this->get(j, i) *= scalar;
        }
    }

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

Matrix4x4f& Matrix4x4f::rotateX(float angle) noexcept
{
    const float sin = fastSin(angle);
    const float cos = fastCos(angle);
    const float nSin = -sin;

    const float nm10 = _data.rowMajorArr[1][0] * cos + _data.rowMajorArr[2][0] * sin;
    const float nm11 = _data.rowMajorArr[1][1] * cos + _data.rowMajorArr[2][1] * sin;
    const float nm12 = _data.rowMajorArr[1][2] * cos + _data.rowMajorArr[2][2] * sin;
    const float nm13 = _data.rowMajorArr[1][3] * cos + _data.rowMajorArr[2][3] * sin;

    _data.rowMajorArr[2][0] = _data.rowMajorArr[1][0] * nSin + _data.rowMajorArr[2][0] * cos;
    _data.rowMajorArr[2][1] = _data.rowMajorArr[1][1] * nSin + _data.rowMajorArr[2][1] * cos;
    _data.rowMajorArr[2][2] = _data.rowMajorArr[1][2] * nSin + _data.rowMajorArr[2][2] * cos;
    _data.rowMajorArr[2][3] = _data.rowMajorArr[1][3] * nSin + _data.rowMajorArr[2][3] * cos;

    _data.rowMajorArr[1][0] = nm10;
    _data.rowMajorArr[1][1] = nm11;
    _data.rowMajorArr[1][2] = nm12;
    _data.rowMajorArr[1][3] = nm13;

    return *this;
}

Matrix4x4f& Matrix4x4f::rotateY(float angle) noexcept
{
    const float sin = fastSin(angle);
    const float cos = fastCos(angle);
    const float nSin = -sin;

    const float nm00 = _data.rowMajorArr[0][0] * cos + _data.rowMajorArr[2][0] * nSin;
    const float nm01 = _data.rowMajorArr[0][1] * cos + _data.rowMajorArr[2][1] * nSin;
    const float nm02 = _data.rowMajorArr[0][2] * cos + _data.rowMajorArr[2][2] * nSin;
    const float nm03 = _data.rowMajorArr[0][3] * cos + _data.rowMajorArr[2][3] * nSin;

    _data.rowMajorArr[2][0] = _data.rowMajorArr[0][0] * sin + _data.rowMajorArr[2][0] * cos;
    _data.rowMajorArr[2][1] = _data.rowMajorArr[0][1] * sin + _data.rowMajorArr[2][1] * cos;
    _data.rowMajorArr[2][2] = _data.rowMajorArr[0][2] * sin + _data.rowMajorArr[2][2] * cos;
    _data.rowMajorArr[2][3] = _data.rowMajorArr[0][3] * sin + _data.rowMajorArr[2][3] * cos;

    _data.rowMajorArr[0][0] = nm00;
    _data.rowMajorArr[0][1] = nm01;
    _data.rowMajorArr[0][2] = nm02;
    _data.rowMajorArr[0][3] = nm03;

    return *this;
}

Matrix4x4f& Matrix4x4f::rotateZ(float angle) noexcept
{
    const float sin = fastSin(angle);
    const float cos = fastCos(angle);
    const float nSin = -sin;

    const float nm00 = _data.rowMajorArr[0][0] * cos + _data.rowMajorArr[1][0] * sin;
    const float nm01 = _data.rowMajorArr[0][1] * cos + _data.rowMajorArr[1][1] * sin;
    const float nm02 = _data.rowMajorArr[0][2] * cos + _data.rowMajorArr[1][2] * sin;
    const float nm03 = _data.rowMajorArr[0][3] * cos + _data.rowMajorArr[1][3] * sin;

    _data.rowMajorArr[1][0] = _data.rowMajorArr[0][0] * nSin + _data.rowMajorArr[1][0] * cos;
    _data.rowMajorArr[1][1] = _data.rowMajorArr[0][1] * nSin + _data.rowMajorArr[1][1] * cos;
    _data.rowMajorArr[1][2] = _data.rowMajorArr[0][2] * nSin + _data.rowMajorArr[1][2] * cos;
    _data.rowMajorArr[1][3] = _data.rowMajorArr[0][3] * nSin + _data.rowMajorArr[1][3] * cos;

    _data.rowMajorArr[0][0] = nm00;
    _data.rowMajorArr[0][1] = nm01;
    _data.rowMajorArr[0][2] = nm02;
    _data.rowMajorArr[0][3] = nm03;

    return *this;
}

Matrix4x4f& Matrix4x4f::translate(Vector3f& translation) noexcept
{
    this->_data.rowMajorArr[3][0] += translation.x();
    this->_data.rowMajorArr[3][1] += translation.y();
    this->_data.rowMajorArr[3][2] += translation.z();

    return *this;
}

Matrix4x4f& Matrix4x4f::rotate(Vector3f& rotation) noexcept
{
    this->rotateZ(rotation.z());
    this->rotateX(rotation.x());
    this->rotateY(rotation.y());

    return *this;
}

Matrix4x4f& Matrix4x4f::scale(Vector3f& scaling) noexcept
{
    this->_data.m00 += scaling.x();
    this->_data.m11 += scaling.y();
    this->_data.m22 += scaling.z();

    return *this;
}

Matrix4x4f& Matrix4x4f::scale(float scaling) noexcept
{
    this->_data.m00 += scaling;
    this->_data.m11 += scaling;
    this->_data.m22 += scaling;

    return *this;
}

Matrix4x4f Matrix4x4f::translation(Vector3f& translation) noexcept
{
    Matrix4x4f matrix;

    matrix._data.rowMajorArr[3][0] = translation.x();
    matrix._data.rowMajorArr[3][1] = translation.y();
    matrix._data.rowMajorArr[3][2] = translation.z();
    matrix._data.rowMajorArr[3][3] = 1.0f;

    matrix._data.rowMajorArr[0][0] = 0.0f;
    matrix._data.rowMajorArr[0][1] = 0.0f;
    matrix._data.rowMajorArr[0][2] = 0.0f;

    matrix._data.rowMajorArr[1][0] = 0.0f;
    matrix._data.rowMajorArr[1][1] = 0.0f;
    matrix._data.rowMajorArr[1][2] = 0.0f;

    matrix._data.rowMajorArr[2][0] = 0.0f;
    matrix._data.rowMajorArr[2][1] = 0.0f;
    matrix._data.rowMajorArr[2][2] = 0.0f;

    matrix._data.rowMajorArr[3][0] = 0.0f;
    matrix._data.rowMajorArr[3][1] = 0.0f;
    matrix._data.rowMajorArr[3][2] = 0.0f;

    return matrix;
}

Matrix4x4f Matrix4x4f::rotation(Vector3f& rotation) noexcept
{
    Matrix4x4f matrix;

    matrix.rotate(rotation);

    return matrix;
}

Matrix4x4f Matrix4x4f::scalar(Vector3f& scaling) noexcept
{
    Matrix4x4f matrix;

    matrix._data.rowMajorArr[0][0] = scaling.x();
    matrix._data.rowMajorArr[1][1] = scaling.y();
    matrix._data.rowMajorArr[2][2] = scaling.z();
    matrix._data.rowMajorArr[3][3] = 1.0f;

    matrix._data.rowMajorArr[0][1] = 0.0f;
    matrix._data.rowMajorArr[0][2] = 0.0f;
    matrix._data.rowMajorArr[0][3] = 0.0f;

    matrix._data.rowMajorArr[1][0] = 0.0f;
    matrix._data.rowMajorArr[1][2] = 0.0f;
    matrix._data.rowMajorArr[1][3] = 0.0f;

    matrix._data.rowMajorArr[2][0] = 0.0f;
    matrix._data.rowMajorArr[2][1] = 0.0f;
    matrix._data.rowMajorArr[2][3] = 0.0f;

    matrix._data.rowMajorArr[3][0] = 0.0f;
    matrix._data.rowMajorArr[3][1] = 0.0f;
    matrix._data.rowMajorArr[3][2] = 0.0f;

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

    matrix.m11 = fastCos(halfFOV) / fastSin(halfFOV);
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

Matrix4x4f& Matrix4x4f::tMatRotX(float angle) noexcept
{
    const float sin = fastSin(angle);
    const float cos = fastCos(angle);

    _data.rowMajorArr[1][1] *= cos;
    _data.rowMajorArr[2][2] *= cos;
    _data.rowMajorArr[2][1] = -sin;
    _data.rowMajorArr[1][2] =  sin;

    return *this;
}

Matrix4x4f& Matrix4x4f::tMatRotY(float angle) noexcept
{
    const float sin = fastSin(angle);
    const float cos = fastCos(angle);
    _data.rowMajorArr[0][0] *= cos;
    _data.rowMajorArr[2][2] *= cos;
    _data.rowMajorArr[0][2] = -sin;
    _data.rowMajorArr[2][0] =  sin;

    return *this;
}

Matrix4x4f& Matrix4x4f::tMatRotZ(float angle) noexcept
{
    const float sin = fastSin(angle);
    const float cos = fastCos(angle);

    _data.rowMajorArr[0][0] *= cos;
    _data.rowMajorArr[1][1] *= cos;
    _data.rowMajorArr[1][0] = -sin;
    _data.rowMajorArr[0][1] =  sin;

    return *this;
}

Matrix4x4f Matrix4x4f::transformation(Vector3f& translation, Vector3f& rotation, Vector3f& scale) noexcept
{
    Matrix4x4f matrix;

    matrix.translate(translation);
    matrix.scale(scale);
    matrix.rotate(rotation);

    return matrix;
}
