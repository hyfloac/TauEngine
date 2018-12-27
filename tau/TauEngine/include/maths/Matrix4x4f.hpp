#pragma once
#include <NumTypes.hpp>
#include "ComponentVector4.hpp"
#include <DLL.hpp>

class Vector3f;
class Vector4f;

class TAU_DLL Matrix4x4f
{
public:
    /**
     * Proven to be the correct layout, for LE at least.
     *
     * https://gcc.godbolt.org/z/7ZioYi
     */
    union Matrix4x4fData
    {
        float rowMajorArr[4][4];
        float m[16];
        struct
        {
            float m00, m10, m20, m30,
                  m01, m11, m21, m31,
                  m02, m12, m22, m32,
                  m03, m13, m23, m33;
        };
        struct rowMajor
        {
            float m00, m01, m02, m03,
                  m10, m11, m12, m13,
                  m20, m21, m22, m23,
                  m30, m31, m32, m33;
        };
    };
private:
    Matrix4x4fData _data;
private:
    Matrix4x4f& tMatRotX(float angle) noexcept;
    Matrix4x4f& tMatRotY(float angle) noexcept;
    Matrix4x4f& tMatRotZ(float angle) noexcept;
public:
    static Matrix4x4f perspective(float fov, float aspect, float nearPlane, float farPlane, bool leftHanded = true) noexcept;

    static Matrix4x4f transformation(Vector3f& translation, Vector3f& rotation, Vector3f& scale) noexcept;

    static Matrix4x4f translation(Vector3f& translation) noexcept;
    static Matrix4x4f rotation(Vector3f& rotation) noexcept;
    static Matrix4x4f scalar(Vector3f& scaling) noexcept;
public:
    Matrix4x4f() noexcept;
    Matrix4x4f(float filler) noexcept;
    Matrix4x4f(const Matrix4x4fData& store) noexcept;

#pragma region Accessor
    inline float m00() const noexcept { return _data.m00; }
    inline float m01() const noexcept { return _data.m01; }
    inline float m02() const noexcept { return _data.m02; }
    inline float m03() const noexcept { return _data.m03; }
                              
    inline float m10() const noexcept { return _data.m10; }
    inline float m11() const noexcept { return _data.m11; }
    inline float m12() const noexcept { return _data.m12; }
    inline float m13() const noexcept { return _data.m13; }
                              
    inline float m20() const noexcept { return _data.m20; }
    inline float m21() const noexcept { return _data.m21; }
    inline float m22() const noexcept { return _data.m22; }
    inline float m23() const noexcept { return _data.m23; }
                              
    inline float m30() const noexcept { return _data.m30; }
    inline float m31() const noexcept { return _data.m31; }
    inline float m32() const noexcept { return _data.m32; }
    inline float m33() const noexcept { return _data.m33; }

    inline float& m00() noexcept { return _data.m00; }
    inline float& m01() noexcept { return _data.m01; }
    inline float& m02() noexcept { return _data.m02; }
    inline float& m03() noexcept { return _data.m03; }
                         
    inline float& m10() noexcept { return _data.m10; }
    inline float& m11() noexcept { return _data.m11; }
    inline float& m12() noexcept { return _data.m12; }
    inline float& m13() noexcept { return _data.m13; }

    inline float& m20() noexcept { return _data.m20; }
    inline float& m21() noexcept { return _data.m21; }
    inline float& m22() noexcept { return _data.m22; }
    inline float& m23() noexcept { return _data.m23; }
                         
    inline float& m30() noexcept { return _data.m30; }
    inline float& m31() noexcept { return _data.m31; }
    inline float& m32() noexcept { return _data.m32; }
    inline float& m33() noexcept { return _data.m33; }

    inline const Matrix4x4fData& data() const noexcept { return _data; }
    inline       Matrix4x4fData& data()       noexcept { return _data; }
#pragma endregion

    inline const float* operator[](u32 index) const noexcept { return _data.rowMajorArr[index]; }
    inline       float* operator[](u32 index)       noexcept { return _data.rowMajorArr[index]; }

    inline float  get(u32 x, u32 y) const noexcept { return _data.m[x + y * 4]; }
    inline float& get(u32 x, u32 y)       noexcept { return _data.m[x + y * 4]; }

    Matrix4x4f& identity() noexcept;

    Matrix4x4f& add(const Matrix4x4f& other) noexcept;
    Matrix4x4f& sub(const Matrix4x4f& other) noexcept;
    Matrix4x4f& mul(const Matrix4x4f& other) noexcept;

    Matrix4x4f& add(float scalar) noexcept;
    Matrix4x4f& sub(float scalar) noexcept;
    Matrix4x4f& mul(float scalar) noexcept;
    inline Matrix4x4f& div(float scalar) noexcept { return mul(1.0f / scalar); }

    CompVec4& mul(Vector3f& vec) const noexcept;
    CompVec4& mul(Vector4f& vec) const noexcept;

    Matrix4x4f& rotateX(float angle) noexcept;
    Matrix4x4f& rotateY(float angle) noexcept;
    Matrix4x4f& rotateZ(float angle) noexcept;

    Matrix4x4f& translate(Vector3f& translation) noexcept;
    Matrix4x4f& rotate(Vector3f& rotation) noexcept;
    Matrix4x4f& scale(Vector3f& scaling) noexcept;
    Matrix4x4f& scale(float scaling) noexcept;
};

typedef Matrix4x4f Matrix4;
typedef Matrix4x4f Matrix4f;

typedef Matrix4x4f mat4x4f;
typedef Matrix4x4f mat4x4;
typedef Matrix4x4f mat4f;
typedef Matrix4x4f mat4;
