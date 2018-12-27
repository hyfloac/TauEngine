#pragma warning(push, 0)
#include <GL/glew.h>
#include <vector>
#pragma warning(pop)
#include <shader/GLShader.hpp>
#include <file/FileHandling.hpp>
#include <Utils.hpp>
#include <DynamicallySizedArray.hpp>
#include <maths/Matrix4x4f.hpp>

GLShader::GLShader(ShaderType shaderType, NotNull<const char> shaderPath, NotNull<GLProgram> glProgram) noexcept
    : _shaderType(shaderType), _shaderPath(shaderPath), _glProgram(glProgram), _shaderId(0)
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

bool GLShader::loadShader() noexcept
{
    GLenum shaderType;
    
    switch(_shaderType)
    {
        case VERTEX: 
            shaderType = GL_VERTEX_SHADER;
            break;
        case FRAGMENT: 
            shaderType = GL_FRAGMENT_SHADER;
            break;
        default: return false;
    }

    const GLuint shaderId = glCreateShader(shaderType);

    GLint result;

    if(shaderId == GL_FALSE)
    {
        glGetShaderiv(GL_FALSE, GL_INFO_LOG_LENGTH, &result);
        if(!result)
        {
            printf("OpenGL failed to create a shader, but no error message was generated.\n");
        }
        else if(result >= 8192)
        {
            GLchar* errorMsg = new GLchar[result];
            glGetShaderInfoLog(shaderId, result, &result, errorMsg);
            printf("OpenGL failed to create a shader.\n  Error Message: %s\n", errorMsg);
            delete[] errorMsg;
        }
        else
        {
            DSA(GLchar, errorMsg, result);
            glGetShaderInfoLog(shaderId, result, &result, errorMsg);
            printf("OpenGL failed to create a shader.\n  Error Message: %s\n", errorMsg);
        }
        return false;
    }

    const GLchar* shaderSrc = readFile(this->_shaderPath);

    glShaderSource(shaderId, 1, &shaderSrc, null);
    glCompileShader(shaderId);

    delete[] shaderSrc;

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &result);
        if(!result)
        {
            printf("OpenGL failed to compile a shader, but no error message was generated.\n");
        }
        else if(result >= 8192)
        {
            GLchar* errorMsg = new GLchar[result];
            glGetShaderInfoLog(shaderId, result, &result, errorMsg);
            printf("OpenGL failed to compile a shader.\n  Error Message: %s\n", errorMsg);
            delete[] errorMsg;
        }
        else
        {
            DSA(GLchar, errorMsg, result);
            glGetShaderInfoLog(shaderId, result, &result, errorMsg);
            printf("OpenGL failed to compile a shader.\n  Error Message: %s\n", errorMsg);
        }
        glDeleteShader(shaderId);
        return false;
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

void GLShader::setUniform(i32 location, i8 value) const noexcept
{
    glUniform1i(location, value);
}

void GLShader::setUniform(i32 location, i16 value) const noexcept
{
    glUniform1i(location, value);
}

void GLShader::setUniform(i32 location, i32 value) const noexcept
{
    glUniform1i(location, value);
}

void GLShader::setUniform(i32 location, i64 value) const noexcept
{
    glUniform1i(location, value);
}

void GLShader::setUniform(i32 location, u8 value) const noexcept
{
    glUniform1i(location, value);
}

void GLShader::setUniform(i32 location, u16 value) const noexcept
{
    glUniform1i(location, value);
}

void GLShader::setUniform(i32 location, u32 value) const noexcept
{
    glUniform1i(location, value);
}

void GLShader::setUniform(i32 location, u64 value) const noexcept
{
    glUniform1i(location, value);
}

void GLShader::setUniform(i32 location, f32 value) const noexcept
{
    glUniform1f(location, value);
}

void GLShader::setUniform(i32 location, f64 value) const noexcept
{
    glUniform1d(location, value);
}

void GLShader::setUniform(i32 location, Vector3f& value) const noexcept
{
    glUniform3f(location, value.x(), value.y(), value.z());
}

void GLShader::setUniform(i32 location, Vector3i& value) const noexcept
{
    glUniform3i(location, value.x(), value.y(), value.z());
}

void GLShader::setUniform(i32 location, Vector4f& value) const noexcept
{
    glUniform4f(location, value.x(), value.y(), value.z(), value.w());
}

void GLShader::setUniform(i32 location, Matrix4x4f& value) const noexcept
{
    glUniformMatrix4fv(location, 1, GL_FALSE, (float*) value.data().m);
}

void GLShader::setUniform(i32 location, bool value) const noexcept
{
    this->setUniform(location, value ? 1 : 0);
}

void GLShader::setUniform(String& name, i8 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, i16 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, i32 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, i64 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, u8 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, u16 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, u32 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, u64 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, f32 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, f64 value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, Vector3f& value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, Vector3i& value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, Vector4f& value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, Matrix4x4f& value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

void GLShader::setUniform(String& name, bool value) const noexcept
{
    if(_uniforms.count(name) > 0)
    {
        this->setUniform(_uniforms.at(name), value);
    }
}

