#pragma warning(push, 0)
#include <GL/glew.h>
#include <vector>
#include <cstring>
#pragma warning(pop)
#include <gl/GLShader.hpp>
#include <Utils.hpp>
#include <maths/Vector2f.hpp>
#include <maths/Vector3f.hpp>
#include <cstdio>
#include "VFS.hpp"
#include "Timings.hpp"

GLShader::GLShader(const IShader::Type shaderType, const NotNull<const char>& shaderPath, const GLuint shaderID) noexcept
    : IShader(shaderType), _shaderPath(shaderPath), _shaderID(shaderID)
{ }

Ref<GLShader> GLShader::create(const IShader::Type shaderType, const NotNull<const char>& shaderPath) noexcept
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

static void clearWhiteSpace(uSys& index, uSys length, const u8* const arr) noexcept
{
    for(; index < length && isspace(arr[index]); ++index);
}

static void clearLine(uSys& index, uSys length, const u8* const arr) noexcept
{
    for(; index < length && arr[index] != '\n'; ++index);
}

static uSys readInclude(uSys index, uSys length, const u8* const arr) noexcept
{
    uSys i;
    for(i = index; i < length && arr[i] != '>'; ++i);
    return i - index;
}

static RefDynArray<u8> handleIncludes(RefDynArray<u8>& shader) noexcept
{
    RefDynArray<u8> ret = shader;
    // const uSys len = shader.count();
    // u8* arr = shader.arr();
    for(uSys i = 0; i < ret.count(); ++i)
    {
        clearWhiteSpace(i, ret.count(), ret.arr());
        if(ret.arr()[i] == '#')
        {
            const u32 includeBegin = i;
            ++i;
            if(i + 7 < ret.count())
            {
                if(strncmp(reinterpret_cast<const char*>(ret.arr() + i), "include", 7) == 0)
                {
                    i += 7;
                    clearWhiteSpace(i, ret.count(), ret.arr());
                    if(ret.arr()[i] == '<')
                    {
                        ++i;
                        const uSys includeLen = readInclude(i, ret.count(), ret.arr());
                        char* includePath = new char[includeLen + 1];
                        includePath[includeLen] = '\0';
                        memcpy(includePath, ret.arr() + i, includeLen);

                        i += includeLen + 1;

                        const uSys includeLineLen = i - includeBegin;

                        RefDynArray<u8> includeFileData = VFS::Instance().openFile(includePath, FileProps::Read)->readFile();

                        delete[] includePath;

                        RefDynArray<u8> includedFileData(ret.count() - includeLineLen + includeFileData.count());

                        memcpy(includedFileData.arr(), ret.arr(), includeBegin);
                        memcpy(includedFileData.arr() + includeBegin, includeFileData.arr(), includeFileData.count() - 1);
                        memcpy(includedFileData.arr() + includeBegin + includeFileData.count() - 1, ret.arr() + i, ret.count() - i);

                        ret = includedFileData;
                    }
                }
            }
        }
        clearLine(i, ret.count(), ret.arr());
    }
    return ret;
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

        data = handleIncludes(data);

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
