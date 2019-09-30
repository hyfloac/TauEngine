#pragma warning(push, 0)
#include <GL/glew.h>
#include <vector>
#pragma warning(pop)
#include <shader/GLShader.hpp>
#include <file/FileHandling.hpp>
#include <Utils.hpp>
#include <maths/Matrix4x4f.hpp>
#include <maths/Vector3f.hpp>
#include <maths/Vector3i.hpp>
#include <maths/Vector4f.hpp>
#include <cstdio>
#include "VFS.hpp"

GLShader::GLShader(ShaderType shaderType, NotNull<const char> shaderPath, NotNull<GLProgram> glProgram) noexcept
    : IShader(shaderType), _shaderPath(shaderPath), _glProgram(glProgram), _shaderId(0)
{ }

GLShader::~GLShader() noexcept
{
    if(this->_shaderId)
    {
        glDetachShader(this->_glProgram->programId(), this->_shaderId);
        glDeleteShader(this->_shaderId);
        this->_shaderId = 0;
    }
}

bool GLShader::loadShader(const char* src) noexcept
{
    GLenum shaderType;
    
    switch(_shaderType)
    {
        case ShaderType::VERTEX: 
            shaderType = GL_VERTEX_SHADER;
            break;
        case ShaderType::FRAGMENT: 
            shaderType = GL_FRAGMENT_SHADER;
            break;
        default: return false;
    }

    const GLuint shaderId = glCreateShader(shaderType);

    GLint result;

    if(shaderId == GL_FALSE)
    {
        glGetShaderiv(GL_FALSE, GL_INFO_LOG_LENGTH, &result);
        if(result <= 0)
        {
            fprintf(stderr, "OpenGL failed to create a shader, but no error message was generated.\n");
        }
        else
        {
            GLchar* errorMsg = new GLchar[result];
            glGetShaderInfoLog(shaderId, result, &result, errorMsg);
            fprintf(stderr, "OpenGL failed to create a shader.\n  Error Message: %s\n", errorMsg);
            delete[] errorMsg;
        }
        return false;
    }

    // const GLchar* shaderSrc = src ? src : readFileFast(this->_shaderPath);
    const GLchar* shaderSrc;
    DynArray<u8> data(0);
    if(src)
    {
        shaderSrc = src;
    }
    else
    {
        data = VFS::Instance().openFile(this->_shaderPath)->readFile();
        shaderSrc = reinterpret_cast<GLchar*>(data.arr());
    }

    glShaderSource(shaderId, 1, &shaderSrc, null);
    glCompileShader(shaderId);

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &result);
        if(result <= 0)
        {
            fprintf(stderr, "OpenGL failed to compile a shader, but no error message was generated.\n");
        }
        else
        {
            GLchar* errorMsg = new GLchar[result];
            glGetShaderInfoLog(shaderId, result, &result, errorMsg);
            fprintf(stderr, "OpenGL failed to compile a shader.\n  Error Message: %s\n", errorMsg);
            delete[] errorMsg;
            fprintf(stderr, "File Path: %s\n", this->_shaderPath);
            fprintf(stderr, "File Data: \n%s\n", shaderSrc);
        }

        glDeleteShader(shaderId);
        // freeFileData(shaderSrc);
        return false;
    }

    if(src != shaderSrc)
    {
        // freeFileData(shaderSrc);
    }

    glAttachShader(this->_glProgram->programId(), shaderId);

    _shaderId = shaderId;

    return true;
}

void GLShader::activateShader() const noexcept
{
    this->_glProgram->activate();
}

i32 GLShader::createUniform(String name) noexcept
{
    const GLint location = glGetUniformLocation(_glProgram->programId(), name.c_str());
    if(location >= 0) { _uniforms[name] = location; }
    return location;
}

void GLShader::setUniform(i32 location, const i8 value) const noexcept
{
    glUniform1i(location, value);
}

void GLShader::setUniform(i32 location, const i16 value) const noexcept
{
    glUniform1i(location, value);
}

void GLShader::setUniform(i32 location, const i32 value) const noexcept
{
    glUniform1i(location, value);
}

void GLShader::setUniform(i32 location, const i64 value) const noexcept
{
    glUniform1i(location, static_cast<GLint>(value));
}

void GLShader::setUniform(i32 location, const u8 value) const noexcept
{
    glUniform1i(location, value);
}

void GLShader::setUniform(i32 location, const u16 value) const noexcept
{
    glUniform1i(location, value);
}

void GLShader::setUniform(i32 location, const u32 value) const noexcept
{
    glUniform1i(location, value);
}

void GLShader::setUniform(i32 location, const u64 value) const noexcept
{
    glUniform1i(location, static_cast<GLuint>(value));
}

void GLShader::setUniform(i32 location, const f32 value) const noexcept
{
    glUniform1f(location, value);
}

void GLShader::setUniform(i32 location, const f64 value) const noexcept
{
    glUniform1d(location, value);
}

void GLShader::setUniform(i32 location, const Vector3f& value) const noexcept
{
    glUniform3f(location, value.x(), value.y(), value.z());
}

void GLShader::setUniform(i32 location, const Vector3i& value) const noexcept
{
    glUniform3i(location, value.x(), value.y(), value.z());
}

void GLShader::setUniform(i32 location, const Vector4f& value) const noexcept
{
    glUniform4f(location, value.x(), value.y(), value.z(), value.w());
}

void GLShader::setUniform(i32 location, const Matrix4x4f& value) const noexcept
{
    glUniformMatrix4fv(location, 1, GL_FALSE, (float*) value.data().m);
}

void GLShader::setUniform(i32 location, const bool value) const noexcept
{
    this->setUniform(location, value ? 1 : 0);
}

void GLShader::setUniform(String& name, const i8 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, const i16 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, const i32 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, const i64 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, const u8 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, const u16 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, const u32 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, const u64 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, const f32 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, const f64 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, const Vector3f& value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, const Vector3i& value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, const Vector4f& value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, const Matrix4x4f& value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, const bool value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

