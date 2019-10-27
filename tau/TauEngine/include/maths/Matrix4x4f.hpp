// #pragma once
// #include <NumTypes.hpp>
// #include "ComponentVector4.hpp"
// #include <DLL.hpp>
// #include <Alignment.h>
// #include <Utils.hpp>
// #include "Maths.hpp"
//
// class Vector3f;
// class Vector4f;
//
// class TAU_DLL Matrix4x4f
// {
// public:
//     /**
//      * Proven to be the correct layout, for LE at least.
//      *
//      * https://gcc.godbolt.org/z/7ZioYi
//      */
//     union ALIGN_MSVC(16) Matrix4x4fData
//     {
//         float m[16];
//         // float glColumnMajorArr[4][4];
//         /**
//          *   In mathematics matrices are accessed as m[i][j], where
//          * `i` is the row and `j` is the column. GPU's require that
//          * the position elements are in indices 12, 13, 14 in the raw
//          * memory layout. But typical display convention of the matrix
//          * has elements m[0][3], m[1][3], and m[2][3] as the position
//          * elements, as such this structure appears flipped, but
//          * should still be accessed as if it were row major.
//          */
//         struct ALIGN_MSVC(16)
//         {
//             float m00, m10, m20, m30,
//                   m01, m11, m21, m31,
//                   m02, m12, m22, m32,
//                   m03, m13, m23, m33;
//         } ALIGN_GCC(16);
//         // struct ALIGN_MSVC(16)
//         // {
//         //     float m00, m01, m02, m03,
//         //           m10, m11, m12, m13,
//         //           m20, m21, m22, m23,
//         //           m30, m31, m32, m33;
//         // } ALIGN_GCC(16);
//         struct ALIGN_MSVC(16)
//         {
//             __m128 x, y, z, w;
//         } ALIGN_GCC(16);
//     } ALIGN_GCC(16);
// private:
//     Matrix4x4fData _data;
// public:
//     static Matrix4x4f perspective(float fov, float aspect, float nearPlane, float farPlane, bool leftHanded = true) noexcept;
//     static Matrix4x4f infDepthPerspective(float fov, float aspect, bool leftHanded = true) noexcept;
//
//     static Matrix4x4f orthographic(float left, float right, float bottom, float top, float zNear, float zFar) noexcept;
//     static Matrix4x4f orthographic(float left, float right, float bottom, float top) noexcept;
//
//     static Matrix4x4f transformation(const Vector3f& RESTRICT translation, const Vector3f& RESTRICT rotation, const Vector3f& RESTRICT scale) noexcept;
//     static Matrix4x4f transformation(const Vector3f& RESTRICT translation, const Vector3f& RESTRICT rotation, const float scale) noexcept;
//
//     static Matrix4x4f translation(const Vector3f& translation) noexcept;
//     static Matrix4x4f rotation(const Vector3f& rotation) noexcept;
//     static Matrix4x4f scalar(const Vector3f& scaling) noexcept;
//
//     static Matrix4x4f rotationXR(const float angleR) noexcept;
// public:
//     Matrix4x4f() noexcept;
//     Matrix4x4f(float filler) noexcept;
//     Matrix4x4f(const Matrix4x4fData& store) noexcept;
//
// #pragma region Accessor
//     /*
//      * Called in row major format, m[r][c].
//      */
//
//     inline float m00() const noexcept { return _data.m00; }
//     inline float m01() const noexcept { return _data.m10; }
//     inline float m02() const noexcept { return _data.m20; }
//     inline float m03() const noexcept { return _data.m30; }
//                               
//     inline float m10() const noexcept { return _data.m01; }
//     inline float m11() const noexcept { return _data.m11; }
//     inline float m12() const noexcept { return _data.m21; }
//     inline float m13() const noexcept { return _data.m31; }
//                               
//     inline float m20() const noexcept { return _data.m02; }
//     inline float m21() const noexcept { return _data.m12; }
//     inline float m22() const noexcept { return _data.m22; }
//     inline float m23() const noexcept { return _data.m32; }
//                               
//     inline float m30() const noexcept { return _data.m03; }
//     inline float m31() const noexcept { return _data.m13; }
//     inline float m32() const noexcept { return _data.m23; }
//     inline float m33() const noexcept { return _data.m33; }
//     
//     inline float& m00() noexcept { return _data.m00; }
//     inline float& m01() noexcept { return _data.m10; }
//     inline float& m02() noexcept { return _data.m20; }
//     inline float& m03() noexcept { return _data.m30; }
//                          
//     inline float& m10() noexcept { return _data.m01; }
//     inline float& m11() noexcept { return _data.m11; }
//     inline float& m12() noexcept { return _data.m21; }
//     inline float& m13() noexcept { return _data.m31; }
//     
//     inline float& m20() noexcept { return _data.m02; }
//     inline float& m21() noexcept { return _data.m12; }
//     inline float& m22() noexcept { return _data.m22; }
//     inline float& m23() noexcept { return _data.m32; }
//                          
//     inline float& m30() noexcept { return _data.m03; }
//     inline float& m31() noexcept { return _data.m13; }
//     inline float& m32() noexcept { return _data.m23; }
//     inline float& m33() noexcept { return _data.m33; }
//
//     inline const Matrix4x4fData& data() const noexcept { return _data; }
//     inline       Matrix4x4fData& data()       noexcept { return _data; }
// #pragma endregion
//
//     // inline const float* operator[](u32 row) const noexcept { return &_data.m[row]; }
//     // inline       float* operator[](u32 row)       noexcept { return &_data.m[row]; }
//
//     inline float  get(u32 row, u32 column) const noexcept { return _data.m[column * 4 + row]; }
//     inline float& get(u32 row, u32 column)       noexcept { return _data.m[column * 4 + row]; }
//
//     Matrix4x4f& identity() noexcept;
//
//     Matrix4x4f& addComp(const Matrix4x4f& other) noexcept;
//     Matrix4x4f& subComp(const Matrix4x4f& other) noexcept;
//     Matrix4x4f& mulComp(const Matrix4x4f& other) noexcept;
//     Matrix4x4f& divComp(const Matrix4x4f& other) noexcept;
//
//     Matrix4x4f& add(const Matrix4x4f& other) noexcept { return addComp(other); }
//     Matrix4x4f& sub(const Matrix4x4f& other) noexcept { return subComp(other); }
//     Matrix4x4f mul(const Matrix4x4f& other) const noexcept;
//     Matrix4x4f mulSIMD(const Matrix4x4f& other) const noexcept;
//
//     Matrix4x4f& add(float scalar) noexcept;
//     Matrix4x4f& sub(float scalar) noexcept;
//     Matrix4x4f& mul(float scalar) noexcept;
//     inline Matrix4x4f& div(float scalar) noexcept { return mul(1.0f / scalar); }
//
//     Matrix4x4f& addSIMD(float scalar) noexcept;
//     Matrix4x4f& subSIMD(float scalar) noexcept;
//     Matrix4x4f& mulSIMD(float scalar) noexcept;
//     inline Matrix4x4f& divSIMD(float scalar) noexcept { return mulSIMD(1.0f / scalar); }
//
//     CompVec4 preMul(Vector3f vec) const noexcept;
//     CompVec4 preMul(Vector4f vec) const noexcept;
//
//     CompVec4 postMul(Vector3f vec) const noexcept;
//     CompVec4 postMul(Vector4f vec) const noexcept;
//
//     // CompVec4& mul(Vector3f& vec) const noexcept;
//     // CompVec4& mul(Vector4f& vec) const noexcept;
//
//     // CompVec4& mulSIMD(Vector3f& vec) const noexcept;
//     // CompVec4& mulSIMD(Vector4f& vec) const noexcept;
//
//     // CompVec4& mulSIMD_SSE3(Vector3f& vec) const noexcept;
//     // CompVec4& mulSIMD_SSE3(Vector4f& vec) const noexcept;
//
//     Matrix4x4f& rotateX(float angle) noexcept;
//     Matrix4x4f& rotateY(float angle) noexcept;
//     Matrix4x4f& rotateZ(float angle) noexcept;
//
//     Matrix4x4f& translate(const Vector3f& translation) noexcept;
//     Matrix4x4f& rotate(const Vector3f& rotation) noexcept;
//     Matrix4x4f& scale(const Vector3f& scaling) noexcept;
//     Matrix4x4f& scale(const float scaling) noexcept;
//
//     Matrix4x4f& setScale(const Vector3f& scale) noexcept;
//     Matrix4x4f& setScale(const float scale) noexcept;
//
//     Matrix4x4f& setTranslation(const Vector3f& translation) noexcept;
//
//     Matrix4x4f& transformation(const Vector3f& RESTRICT rotation, const Vector3f& RESTRICT scale) noexcept;
//     Matrix4x4f& transformation(const Vector3f& rotation, const float scale = 1.0f) noexcept;
//
//     // Matrix4x4f& rotateST(const Vector3f& rotation) noexcept;
//     Matrix4x4f& rotateOptimized(const Vector3f& rotation) noexcept;
//     Matrix4x4f& rotateCamera(const float pitch, const float yaw) noexcept;
//
//     Matrix4x4f& view(const Vector3f& translation, const float pitch, const float yaw) noexcept;
//     Matrix4x4f& fps(const Vector3f translation, const SinCos<float> pitchSC, const SinCos<float> yawSC) noexcept;
//     Matrix4x4f& fps(const Vector3f translation, const float pitch, const float yaw) noexcept;
//     Matrix4x4f& fpsD(const Vector3f translation, const float pitch, const float yaw) noexcept;
// };
//
// CompVec4 operator *(const Matrix4x4f& m, Vector3f v) noexcept;
// CompVec4 operator *(Vector3f v, const Matrix4x4f& m) noexcept;
// inline Matrix4x4f operator *(const Matrix4x4f& a, const Matrix4x4f& b) noexcept { return a.mulSIMD(b); }
//
// typedef Matrix4x4f Matrix4;
// typedef Matrix4x4f Matrix4f;
//
// typedef Matrix4x4f mat4x4f;
// typedef Matrix4x4f mat4x4;
// typedef Matrix4x4f mat4f;
// typedef Matrix4x4f mat4;
