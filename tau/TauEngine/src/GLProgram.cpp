#pragma warning(push, 0)
#include <cstdio>
#pragma warning(pop)
#include <gl/GLProgram.hpp>
#include <DynamicallySizedArray.hpp>
#include <Safeties.hpp>

GLProgram::GLProgram() noexcept
    : _programId(glCreateProgram())
{
    Ensure(_programId != 0);
}

GLProgram::~GLProgram() noexcept
{
    glDeleteProgram(_programId);
    _programId = 0;
}

bool GLProgram::linkAndValidate() noexcept 
{
    glLinkProgram(_programId);

    GLint result;
    glGetProgramiv(_programId, GL_LINK_STATUS, &result);
    if(result == GL_FALSE)
    {
        glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &result);

        if(!result)
        {
            printf("OpenGL failed to link program, but no error message was generated.\n");
        }
        else if(result >= 8192)
        {
            GLchar* errorMsg = new GLchar[result];
            glGetShaderInfoLog(_programId, result, &result, errorMsg);
            printf("OpenGL failed to link program.\n  Error Message: %s\n", errorMsg);
            delete[] errorMsg;
        }
        else
        {
            DSA(GLchar, errorMsg, result);
            glGetShaderInfoLog(_programId, result, &result, errorMsg);
            printf("OpenGL failed to link program.\n  Error Message: %s\n", errorMsg);
        }
        glDeleteProgram(_programId);
        _programId = 0;
        return false;
    }

    glValidateProgram(_programId);
    glGetProgramiv(_programId, GL_VALIDATE_STATUS, &result);
    if(result == GL_FALSE)
    {
        glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &result);

        if(!result)
        {
            printf("OpenGL failed to validate program, but no error message was generated.\n");
        }
        else if(result >= 8192)
        {
            GLchar* errorMsg = new GLchar[result];
            glGetShaderInfoLog(_programId, result, &result, errorMsg);
            printf("OpenGL failed to validate program.\n  Error Message: %s\n", errorMsg);
            delete[] errorMsg;
        }
        else
        {
            DSA(GLchar, errorMsg, result);
            glGetShaderInfoLog(_programId, result, &result, errorMsg);
            printf("OpenGL failed to validate program.\n  Error Message: %s\n", errorMsg);
        }
        glDeleteProgram(_programId);
        _programId = 0;
        return false;
    }

    return true;
}
