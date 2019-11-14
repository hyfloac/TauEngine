#include "gl/GLUniform.hpp"
#include <glm/gtc/type_ptr.hpp>

void GLUniform<const glm::bvec2&>::set(const glm::bvec2& val) noexcept
{
    glUniform2ui(_uniformLocation, val.x ? 1 : 0, val.y ? 1 : 0);
}

void GLUniform<const glm::bvec3&>::set(const glm::bvec3& val) noexcept
{
    glUniform3ui(_uniformLocation, val.x ? 1 : 0, val.y ? 1 : 0, val.z ? 1 : 0);
}

void GLUniform<const glm::bvec4&>::set(const glm::bvec4& val) noexcept
{
    glUniform4ui(_uniformLocation, val.x ? 1 : 0, val.y ? 1 : 0, val.z ? 1 : 0, val.w ? 1 : 0);
}

void GLUniform<const glm::ivec2&>::set(const glm::ivec2& val) noexcept
{
    glUniform2i(_uniformLocation, val.x, val.y);
}

void GLUniform<const glm::ivec3&>::set(const glm::ivec3& val) noexcept
{
    glUniform3i(_uniformLocation, val.x, val.y, val.z);
}

void GLUniform<const glm::ivec4&>::set(const glm::ivec4& val) noexcept
{
    glUniform4i(_uniformLocation, val.x, val.y, val.z, val.w);
}

void GLUniform<const glm::uvec2&>::set(const glm::uvec2& val) noexcept
{
    glUniform2ui(_uniformLocation, val.x, val.y);
}

void GLUniform<const glm::uvec3&>::set(const glm::uvec3& val) noexcept
{
    glUniform3ui(_uniformLocation, val.x, val.y, val.z);
}

void GLUniform<const glm::uvec4&>::set(const glm::uvec4& val) noexcept
{
    glUniform4ui(_uniformLocation, val.x, val.y, val.z, val.w);
}

void GLUniform<const glm::vec2&>::set(const glm::vec2& val) noexcept
{
    glUniform2f(_uniformLocation, val.x, val.y);
}

void GLUniform<const glm::vec3&>::set(const glm::vec3& val) noexcept
{
    glUniform3f(_uniformLocation, val.x, val.y, val.z);
}

void GLUniform<const glm::vec4&>::set(const glm::vec4& val) noexcept
{
    glUniform4f(_uniformLocation, val.x, val.y, val.z, val.w);
}

void GLUniform<const glm::dvec2&>::set(const glm::dvec2& val) noexcept
{
    glUniform2d(_uniformLocation, val.x, val.y);
}

void GLUniform<const glm::dvec3&>::set(const glm::dvec3& val) noexcept
{
    glUniform3d(_uniformLocation, val.x, val.y, val.z);
}

void GLUniform<const glm::dvec4&>::set(const glm::dvec4& val) noexcept
{
    glUniform4d(_uniformLocation, val.x, val.y, val.z, val.w);
}

void GLUniform<const glm::mat2x2&>::set(const glm::mat2x2& val) noexcept
{
    glUniformMatrix2fv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::mat2x3&>::set(const glm::mat2x3& val) noexcept
{
    glUniformMatrix2x3fv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::mat2x4&>::set(const glm::mat2x4& val) noexcept
{
    glUniformMatrix2x4fv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::mat3x2&>::set(const glm::mat3x2& val) noexcept
{
    glUniformMatrix3x2fv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::mat3x3&>::set(const glm::mat3x3& val) noexcept
{
    glUniformMatrix3fv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::mat3x4&>::set(const glm::mat3x4& val) noexcept
{
    glUniformMatrix3x4fv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::mat4x2&>::set(const glm::mat4x2& val) noexcept
{
    glUniformMatrix4x2fv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::mat4x3&>::set(const glm::mat4x3& val) noexcept
{
    glUniformMatrix4x3fv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::mat4x4&>::set(const glm::mat4x4& val) noexcept
{
    glUniformMatrix4fv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::dmat2x2&>::set(const glm::dmat2x2& val) noexcept
{
    glUniformMatrix2dv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::dmat2x3&>::set(const glm::dmat2x3& val) noexcept
{
    glUniformMatrix2x3dv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::dmat2x4&>::set(const glm::dmat2x4& val) noexcept
{
    glUniformMatrix2x4dv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::dmat3x2&>::set(const glm::dmat3x2& val) noexcept
{
    glUniformMatrix3x2dv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::dmat3x3&>::set(const glm::dmat3x3& val) noexcept
{
    glUniformMatrix3dv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::dmat3x4&>::set(const glm::dmat3x4& val) noexcept
{
    glUniformMatrix3x4dv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::dmat4x2&>::set(const glm::dmat4x2& val) noexcept
{
    glUniformMatrix4x2dv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::dmat4x3&>::set(const glm::dmat4x3& val) noexcept
{
    glUniformMatrix4x3dv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}

void GLUniform<const glm::dmat4x4&>::set(const glm::dmat4x4& val) noexcept
{
    glUniformMatrix4dv(_uniformLocation, 1, _transpose ? GL_TRUE : GL_FALSE, glm::value_ptr(val));
}
