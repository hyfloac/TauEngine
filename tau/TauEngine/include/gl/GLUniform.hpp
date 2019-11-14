#pragma once

#include "DLL.hpp"
#include "shader/IUniform.hpp"
#include <glm/matrix.hpp>
#include <GL/glew.h>

#define GL_UNIFORM_IMPL(_SDT, _T)                       \
    template<> class TAU_DLL GLUniform<_T> final : public IUniform<_T> { \
        DEFAULT_CONSTRUCT_PO(GLUniform);                \
        DEFAULT_DESTRUCT(GLUniform);                    \
        DELETE_COPY(GLUniform);                         \
        private:                                        \
            GLint _uniformLocation;                     \
        public:                                         \
        static GLint create(GLuint program, const char* name) noexcept \
        { return glGetUniformLocation(program, name); } \
        public:                                         \
            GLUniform(GLint uniformLocation) noexcept   \
                : _uniformLocation(uniformLocation)     \
            { }                                         \
            void set(_T val) noexcept override final;   \
            [[nodiscard]] virtual ShaderDataType::Type dataType() noexcept override final \
            { return ShaderDataType::_SDT; }            \
    };

#define GL_UNIFORM_MATRIX_IMPL(_SDT, _T)                \
    template<> class TAU_DLL GLUniform<_T> final : public IUniform<_T> { \
        DEFAULT_CONSTRUCT_PO(GLUniform);                \
        DEFAULT_DESTRUCT(GLUniform);                    \
        DELETE_COPY(GLUniform);                         \
        private:                                        \
            GLint _uniformLocation;                     \
            bool _transpose;                            \
        public:                                         \
        static GLint create(GLuint program, const char* name) noexcept \
        { return glGetUniformLocation(program, name); } \
        public:                                         \
            GLUniform(GLint uniformLocation,            \
                      bool transpose) noexcept          \
                : _uniformLocation(uniformLocation),    \
                  _transpose(transpose)                 \
            { }                                         \
            void set(_T val) noexcept override final;   \
            [[nodiscard]] virtual ShaderDataType::Type dataType() noexcept override final \
            { return ShaderDataType::_SDT; }            \
    };

template<typename _T>
class GLUniform : public IUniform<_T>
{
    DEFAULT_CONSTRUCT_PO(GLUniform);
    DEFAULT_DESTRUCT_VI(GLUniform);
    DELETE_COPY(GLUniform);
private:
    GLint _uniformLocation;
public:
    static GLint create(GLuint program, const char* name) noexcept
    { return glGetUniformLocation(program, name); }
public:
    void set(_T value) noexcept override = 0;
};

class Vector2f;
class Vector3i;
class Vector3f;
class Vector4f;

GL_UNIFORM_IMPL(Bool, bool);
GL_UNIFORM_IMPL(Int, int);
GL_UNIFORM_IMPL(UInt, unsigned int);
GL_UNIFORM_IMPL(Float, float);
GL_UNIFORM_IMPL(Double, double);
GL_UNIFORM_IMPL(Vector2Float, const Vector2f&);
GL_UNIFORM_IMPL(Vector3Int, const Vector3i&);
GL_UNIFORM_IMPL(Vector3Float, const Vector3f&);
GL_UNIFORM_IMPL(Vector4Float, const Vector4f&);

GL_UNIFORM_IMPL(Vector2Bool,   const glm::bvec2&);
GL_UNIFORM_IMPL(Vector3Bool,   const glm::bvec3&);
GL_UNIFORM_IMPL(Vector4Bool,   const glm::bvec4&);
GL_UNIFORM_IMPL(Vector2Int,    const glm::ivec2&);
GL_UNIFORM_IMPL(Vector3Int,    const glm::ivec3&);
GL_UNIFORM_IMPL(Vector4Int,    const glm::ivec4&);
GL_UNIFORM_IMPL(Vector2UInt,   const glm::uvec2&);
GL_UNIFORM_IMPL(Vector3UInt,   const glm::uvec3&);
GL_UNIFORM_IMPL(Vector4UInt,   const glm::uvec4&);
GL_UNIFORM_IMPL(Vector2Float,  const glm:: vec2&);
GL_UNIFORM_IMPL(Vector3Float,  const glm:: vec3&);
GL_UNIFORM_IMPL(Vector4Float,  const glm:: vec4&);
GL_UNIFORM_IMPL(Vector2Double, const glm::dvec2&);
GL_UNIFORM_IMPL(Vector3Double, const glm::dvec3&);
GL_UNIFORM_IMPL(Vector4Double, const glm::dvec4&);

GL_UNIFORM_MATRIX_IMPL(Matrix2x2Float,  const glm:: mat2x2&);
GL_UNIFORM_MATRIX_IMPL(Matrix2x3Float,  const glm:: mat2x3&);
GL_UNIFORM_MATRIX_IMPL(Matrix2x4Float,  const glm:: mat2x4&);
GL_UNIFORM_MATRIX_IMPL(Matrix3x2Float,  const glm:: mat3x2&);
GL_UNIFORM_MATRIX_IMPL(Matrix3x3Float,  const glm:: mat3x3&);
GL_UNIFORM_MATRIX_IMPL(Matrix3x4Float,  const glm:: mat3x4&);
GL_UNIFORM_MATRIX_IMPL(Matrix4x2Float,  const glm:: mat4x2&);
GL_UNIFORM_MATRIX_IMPL(Matrix4x3Float,  const glm:: mat4x3&);
GL_UNIFORM_MATRIX_IMPL(Matrix4x4Float,  const glm:: mat4x4&);
GL_UNIFORM_MATRIX_IMPL(Matrix2x2Double, const glm::dmat2x2&);
GL_UNIFORM_MATRIX_IMPL(Matrix2x3Double, const glm::dmat2x3&);
GL_UNIFORM_MATRIX_IMPL(Matrix2x4Double, const glm::dmat2x4&);
GL_UNIFORM_MATRIX_IMPL(Matrix3x2Double, const glm::dmat3x2&);
GL_UNIFORM_MATRIX_IMPL(Matrix3x3Double, const glm::dmat3x3&);
GL_UNIFORM_MATRIX_IMPL(Matrix3x4Double, const glm::dmat3x4&);
GL_UNIFORM_MATRIX_IMPL(Matrix4x2Double, const glm::dmat4x2&);
GL_UNIFORM_MATRIX_IMPL(Matrix4x3Double, const glm::dmat4x3&);
GL_UNIFORM_MATRIX_IMPL(Matrix4x4Double, const glm::dmat4x4&);
