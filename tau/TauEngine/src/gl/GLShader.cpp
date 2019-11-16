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
#include "Timings.hpp"

GLShader::GLShader(const IShader::Type shaderType, const NotNull<const char> shaderPath, const GLuint shaderID) noexcept
    : IShader(shaderType), _shaderPath(shaderPath), _shaderID(shaderID)
{ }

Ref<GLShader> GLShader::create(const IShader::Type shaderType, const NotNull<const char> shaderPath) noexcept
{
    PERF();
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
    PERF();
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
