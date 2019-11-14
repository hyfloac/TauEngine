#include "gl/GLUniform.hpp"
#include <maths/Vector2f.hpp>
#include <maths/Vector3i.hpp>
#include <maths/Vector3f.hpp>
#include <maths/Vector4f.hpp>

void GLUniform<bool>::set(bool val) noexcept
{
    glUniform1ui(_uniformLocation, val ? 1 : 0);
}

void GLUniform<int>::set(int val) noexcept
{
    glUniform1i(_uniformLocation, val);
}

void GLUniform<unsigned>::set(unsigned val) noexcept
{
    glUniform1ui(_uniformLocation, val);
}

void GLUniform<float>::set(float val) noexcept
{
    glUniform1f(_uniformLocation, val);
}

void GLUniform<double>::set(double val) noexcept
{
    glUniform1d(_uniformLocation, val);
}

void GLUniform<const Vector2f&>::set(const Vector2f& val) noexcept
{
    glUniform2f(_uniformLocation, val.x(), val.y());
}

void GLUniform<const Vector3i&>::set(const Vector3i& val) noexcept
{
    glUniform3i(_uniformLocation, val.x(), val.y(), val.z());
}

void GLUniform<const Vector3f&>::set(const Vector3f& val) noexcept
{
    glUniform3f(_uniformLocation, val.x(), val.y(), val.z());
}

void GLUniform<const Vector4f&>::set(const Vector4f& val) noexcept
{
    glUniform4f(_uniformLocation, val.x(), val.y(), val.z(), val.w());
}
