#pragma warning(push, 0)
#include <GL/glew.h>
#include <vector>
#pragma warning(pop)
#include <shader/GLShader.hpp>
#include <EnumBitFields.hpp>
#include <file/FileHandling.hpp>
#include <Utils.hpp>
#include <DynamicallySizedArray.hpp>

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
    GLenum shaderType = 0;
    
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
