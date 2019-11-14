#pragma warning(push, 0)
#include <GL/glew.h>
#include <vector>
#pragma warning(pop)
#include <gl/GLShader.hpp>
#include <Utils.hpp>
#include <maths/Vector2f.hpp>
#include <maths/Vector3f.hpp>
#include <maths/Vector3i.hpp>
#include <maths/Vector4f.hpp>
#include <cstdio>
#include "VFS.hpp"
#include <glm/gtc/type_ptr.hpp>

GLShader::GLShader(const IShader::Type shaderType, const NotNull<const char> shaderPath, const GLuint shaderID) noexcept
    : IShader(shaderType), _shaderPath(shaderPath), _shaderID(shaderID)
{ }

Ref<GLShader> GLShader::create(const IShader::Type shaderType, const NotNull<const char> shaderPath) noexcept
{
    GLenum glShaderType;

    switch(shaderType)
    {
        case IShader::Type::Vertex:
            glShaderType = GL_VERTEX_SHADER;
            break;
        case IShader::Type::TessellationControl:
            glShaderType = GL_TESS_CONTROL_SHADER;
            break;
        case IShader::Type::TessellationEvaluation:
            glShaderType = GL_TESS_EVALUATION_SHADER;
            break;
        case IShader::Type::Geometry:
            glShaderType = GL_GEOMETRY_SHADER;
            break;
        case IShader::Type::Fragment:
            glShaderType = GL_FRAGMENT_SHADER;
            break;
        default: return null;
    }

    const GLuint shaderID = glCreateShader(glShaderType);

    if(shaderID == GL_FALSE)
    {
    #if !defined(TAU_PRODUCTION)
        GLint result;
        glGetShaderiv(GL_FALSE, GL_INFO_LOG_LENGTH, &result);
        if(result <= 0)
        {
            fprintf(stderr, "OpenGL failed to create a shader, but no error message was generated.\n");
        }
        else
        {
            GLchar* errorMsg = new GLchar[result];
            glGetShaderInfoLog(shaderID, result, &result, errorMsg);
            fprintf(stderr, "OpenGL failed to create a shader.\n  Error Message: %s\n", errorMsg);
            delete[] errorMsg;
        }
    #endif
        return null;
    }

    return Ref<GLShader>(new(std::nothrow) GLShader(shaderType, shaderPath, shaderID));
}

GLShader::~GLShader() noexcept
{
    if(this->_shaderID)
    {
        // glDetachShader(this->_glProgram->programId(), this->_shaderId);
        glDeleteShader(this->_shaderID);
        this->_shaderID = 0;
    }
}

bool GLShader::loadShader(const char* src) noexcept
{
    GLint result;

    const GLchar* shaderSrc;
    RefDynArray<u8> data(0);
    if(src)
    {
        shaderSrc = src;
    }
    else
    {
        data = VFS::Instance().openFile(this->_shaderPath, FileProps::Read)->readFile();
        shaderSrc = reinterpret_cast<GLchar*>(data.arr());
    }

    glShaderSource(this->_shaderID, 1, &shaderSrc, null);
    glCompileShader(this->_shaderID);

    glGetShaderiv(this->_shaderID, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        glGetShaderiv(this->_shaderID, GL_INFO_LOG_LENGTH, &result);
        if(result <= 0)
        {
            fprintf(stderr, "OpenGL failed to compile a shader, but no error message was generated.\n");
        }
        else
        {
            GLchar* errorMsg = new GLchar[result];
            glGetShaderInfoLog(this->_shaderID, result, &result, errorMsg);
            fprintf(stderr, "OpenGL failed to compile a shader.\n  Error Message: %s\n", errorMsg);
            delete[] errorMsg;
            fprintf(stderr, "File Path: %s\n", this->_shaderPath);
            fprintf(stderr, "File Data: \n%s\n", shaderSrc);
        }

        glDeleteShader(this->_shaderID);
        return false;
    }

    return true;
}

// i32 GLShader::createUniform(String name) noexcept
// {
//     const GLint location = glGetUniformLocation(_glProgram->programId(), name.c_str());
//     if(location >= 0) { _uniforms[name] = location; }
//     return location;
// }
//
// void GLShader::setUniform(i32 location, const i8 value) const noexcept
// {
//     glUniform1i(location, value);
// }
//
// void GLShader::setUniform(i32 location, const i16 value) const noexcept
// {
//     glUniform1i(location, value);
// }
//
// void GLShader::setUniform(i32 location, const i32 value) const noexcept
// {
//     glUniform1i(location, value);
// }
//
// void GLShader::setUniform(i32 location, const i64 value) const noexcept
// {
//     glUniform1i(location, static_cast<GLint>(value));
// }
//
// void GLShader::setUniform(i32 location, const u8 value) const noexcept
// {
//     glUniform1i(location, value);
// }
//
// void GLShader::setUniform(i32 location, const u16 value) const noexcept
// {
//     glUniform1i(location, value);
// }
//
// void GLShader::setUniform(i32 location, const u32 value) const noexcept
// {
//     glUniform1i(location, value);
// }
//
// void GLShader::setUniform(i32 location, const u64 value) const noexcept
// {
//     glUniform1i(location, static_cast<GLuint>(value));
// }
//
// void GLShader::setUniform(i32 location, const f32 value) const noexcept
// {
//     glUniform1f(location, value);
// }
//
// void GLShader::setUniform(i32 location, const f64 value) const noexcept
// {
//     glUniform1d(location, value);
// }
//
// void GLShader::setUniform(i32 location, const Vector2f& value) const noexcept
// {
//     glUniform2f(location, value.x(), value.y());
// }
//
// void GLShader::setUniform(i32 location, const Vector3f& value) const noexcept
// {
//     glUniform3f(location, value.x(), value.y(), value.z());
// }
//
// void GLShader::setUniform(i32 location, const Vector3i& value) const noexcept
// {
//     glUniform3i(location, value.x(), value.y(), value.z());
// }
//
// void GLShader::setUniform(i32 location, const Vector4f& value) const noexcept
// {
//     glUniform4f(location, value.x(), value.y(), value.z(), value.w());
// }
//
// void GLShader::setUniform(i32 location, const glm::mat4& value) const noexcept
// {
//     glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
// }
//
// void GLShader::setUniform(i32 location, const bool value) const noexcept
// {
//     this->setUniform(location, value ? 1 : 0);
// }
//
// void GLShader::setUniform(String& name, const i8 value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }
//
// void GLShader::setUniform(String& name, const i16 value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }
//
// void GLShader::setUniform(String& name, const i32 value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }
//
// void GLShader::setUniform(String& name, const i64 value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }
//
// void GLShader::setUniform(String& name, const u8 value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }
//
// void GLShader::setUniform(String& name, const u16 value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }
//
// void GLShader::setUniform(String& name, const u32 value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }
//
// void GLShader::setUniform(String& name, const u64 value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }
//
// void GLShader::setUniform(String& name, const f32 value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }
//
// void GLShader::setUniform(String& name, const f64 value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }
//
// void GLShader::setUniform(String& name, const Vector2f& value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }
//
// void GLShader::setUniform(String& name, const Vector3f& value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }
//
// void GLShader::setUniform(String& name, const Vector3i& value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }
//
// void GLShader::setUniform(String& name, const Vector4f& value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }
//
// void GLShader::setUniform(String& name, const glm::mat4& value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), glm::value_ptr(value));
//     }
// }
//
// void GLShader::setUniform(String& name, const bool value) const noexcept
// {
//     if(_uniforms.count(name) > 0)
//     {
//         this->setUniform(_uniforms.at(name), value);
//     }
// }

