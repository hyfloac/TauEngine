#pragma warning(push, 0)
#include <cstdio>
#include <GL/glew.h>
#pragma warning(pop)
#include <gl/GLProgram.hpp>
#include <VariableLengthArray.hpp>
#include <Safeties.hpp>

GLProgram::GLProgram() noexcept
    : _programId(glCreateProgram())
{
    Ensure(_programId != 0);
}

GLProgram::~GLProgram() noexcept
{
    GLProgram::deactivate();
    glDeleteProgram(_programId);
    _programId = 0;
}

static bool validateFail(GLuint& programId, const char* type) noexcept
{
    GLint length;

    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);

    if(!length)
    {
        printf("OpenGL failed to %s program, but no error message was generated.\n", type);
    }
    else if(length >= VLA_MAX_LEN)
    {
        GLchar* errorMsg = new GLchar[length];
        glGetShaderInfoLog(programId, length, &length, errorMsg);
        printf("OpenGL failed to %s program.\n  Error Message: %s\n", type, errorMsg);
        delete[] errorMsg;
    }
    else
    {
        VLA(GLchar, errorMsg, length);
        glGetShaderInfoLog(programId, length, &length, errorMsg);
        printf("OpenGL failed to %s program.\n  Error Message: %s\n", type, errorMsg);
    }

    glDeleteProgram(programId);
    programId = 0;

    return false;
}

bool GLProgram::linkAndValidate() noexcept 
{
    glLinkProgram(_programId);

    GLint result;
    glGetProgramiv(_programId, GL_LINK_STATUS, &result);
    if(result == GL_FALSE)
    {
        return validateFail(_programId, "link");
    }

    glValidateProgram(_programId);

    glGetProgramiv(_programId, GL_VALIDATE_STATUS, &result);
    if(result == GL_FALSE)
    {
        return validateFail(_programId, "validate");
    }

    return true;
}
