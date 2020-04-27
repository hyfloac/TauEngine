#include "TauEngine.hpp"
#include "gl/GLShaderProgram.hpp"
#include "shader/IShader.hpp"
#include "gl/GLShader.hpp"
#include <Safeties.hpp>
#include <VariableLengthArray.hpp>
#include "Timings.hpp"

GLShaderProgram::GLShaderProgram() noexcept
    : IShaderProgram()
    , _programID(glCreateProgram())
{ Ensure(_programID != 0); }

GLShaderProgram::~GLShaderProgram() noexcept
{
    glDeleteProgram(_programID);
    _programID = 0;
}

bool GLShaderProgram::attach(IRenderingContext&, const CPPRef<IShader>& shader) noexcept
{
    if(RTT_CHECK(shader.get(), GLShader))
    {
        const CPPRef<GLShader> glShader = RefCast<GLShader>(shader);
        glAttachShader(_programID, glShader->shaderId());
        return true;
    }
    else
    {
        TAU_THROW(IncorrectAPIShaderException);
        return false;
    }
}

void GLShaderProgram::detach(IRenderingContext&, const CPPRef<IShader>& shader) noexcept
{
    const CPPRef<GLShader> glShader = RefCast<GLShader>(shader);
    glDetachShader(_programID, glShader->shaderId());
}

void GLShaderProgram::bind(IRenderingContext&) noexcept
{ glUseProgram(_programID); }

void GLShaderProgram::unbind(IRenderingContext&) noexcept
{ glUseProgram(0); }

static bool validateFail(GLuint& programId, const char* const type) noexcept
{
    PERF();
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

bool GLShaderProgram::link(IRenderingContext&) noexcept
{
    PERF();
    glLinkProgram(_programID);

    GLint result;
    glGetProgramiv(_programID, GL_LINK_STATUS, &result);
    if(result == GL_FALSE)
    {
        return validateFail(_programID, "link");
    }

    glValidateProgram(_programID);

    glGetProgramiv(_programID, GL_VALIDATE_STATUS, &result);
    if(result == GL_FALSE)
    {
        return validateFail(_programID, "validate");
    }

    return true;
}
