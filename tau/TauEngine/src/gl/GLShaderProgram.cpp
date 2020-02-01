#include "TauEngine.hpp"
#include "gl/GLShaderProgram.hpp"
#include "shader/IShader.hpp"
#include "gl/GLShader.hpp"
#include "gl/GLUniform.hpp"
#include <Safeties.hpp>
#include <VariableLengthArray.hpp>
#include "Timings.hpp"

GLShaderProgram::GLShaderProgram(IRenderingContext& context) noexcept
    : IShaderProgram(), _programID(glCreateProgram())
{
    Ensure(_programID != 0);
}

GLShaderProgram::~GLShaderProgram() noexcept
{
    glDeleteProgram(_programID);
    _programID = 0;
}

bool GLShaderProgram::attach(IRenderingContext& context, Ref<IShader> shader) noexcept
{
    if(RTT_CHECK(shader.get(), GLShader))
    {
        const Ref<GLShader> glShader = RefCast<GLShader>(shader);
        glAttachShader(_programID, glShader->shaderId());
        return true;
    }
    else
    {
        TAU_THROW(IncorrectAPIShaderException);
        return false;
    }
}

void GLShaderProgram::detach(IRenderingContext& context, Ref<IShader> shader) noexcept
{
    const Ref<GLShader> glShader = RefCast<GLShader>(shader);
    glDetachShader(_programID, glShader->shaderId());
}

void GLShaderProgram::bind(IRenderingContext& context) noexcept
{
    glUseProgram(_programID);
}

void GLShaderProgram::unbind(IRenderingContext& context) noexcept
{
    glUseProgram(0);
}

static bool validateFail(GLuint& programId, const char* type) noexcept
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

bool GLShaderProgram::link(IRenderingContext& context) noexcept
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

#define GET_GL_UNIFORM(_FUNC, _TYPE) \
    Ref<IUniform<_TYPE>> GLShaderProgram::_FUNC(const char* name) noexcept \
    { return Ref<IUniform<_TYPE>>(new(std::nothrow) GLUniform<_TYPE>(GLUniform<_TYPE>::create(_programID, name))); }

#define GET_GL_UNIFORM_MATRIX(_FUNC, _TYPE) \
    Ref<IUniform<_TYPE>> GLShaderProgram::_FUNC(const char* name, bool transpose) noexcept \
    { return Ref<IUniform<_TYPE>>(new(std::nothrow) GLUniform<_TYPE>(GLUniform<_TYPE>::create(_programID, name), transpose)); }

// GET_GL_UNIFORM(getUniformBool, bool);
// GET_GL_UNIFORM(getUniformInt, int);
// GET_GL_UNIFORM(getUniformUInt, unsigned int);
// GET_GL_UNIFORM(getUniformFloat, float);
// GET_GL_UNIFORM(getUniformDouble, double);
// GET_GL_UNIFORM(getUniformVector2f, const Vector2f&);
// GET_GL_UNIFORM(getUniformVector3i, const Vector3i&);
// GET_GL_UNIFORM(getUniformVector3f, const Vector3f&);
// GET_GL_UNIFORM(getUniformVector4f, const Vector4f&);
//
// GET_GL_UNIFORM(getUniformVector2Bool,   const glm::bvec2&);
// GET_GL_UNIFORM(getUniformVector3Bool,   const glm::bvec3&);
// GET_GL_UNIFORM(getUniformVector4Bool,   const glm::bvec4&);
// GET_GL_UNIFORM(getUniformVector2Int,    const glm::ivec2&);
// GET_GL_UNIFORM(getUniformVector3Int,    const glm::ivec3&);
// GET_GL_UNIFORM(getUniformVector4Int,    const glm::ivec4&);
// GET_GL_UNIFORM(getUniformVector2UInt,   const glm::uvec2&);
// GET_GL_UNIFORM(getUniformVector3UInt,   const glm::uvec3&);
// GET_GL_UNIFORM(getUniformVector4UInt,   const glm::uvec4&);
// GET_GL_UNIFORM(getUniformVector2Float,  const glm:: vec2&);
// GET_GL_UNIFORM(getUniformVector3Float,  const glm:: vec3&);
// GET_GL_UNIFORM(getUniformVector4Float,  const glm:: vec4&);
// GET_GL_UNIFORM(getUniformVector2Double, const glm::dvec2&);
// GET_GL_UNIFORM(getUniformVector3Double, const glm::dvec3&);
// GET_GL_UNIFORM(getUniformVector4Double, const glm::dvec4&);
//
// GET_GL_UNIFORM_MATRIX(getUniformMatrix2x2Float,  const glm:: mat2x2&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix2x3Float,  const glm:: mat2x3&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix2x4Float,  const glm:: mat2x4&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix3x2Float,  const glm:: mat3x2&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix3x3Float,  const glm:: mat3x3&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix3x4Float,  const glm:: mat3x4&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix4x2Float,  const glm:: mat4x2&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix4x3Float,  const glm:: mat4x3&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix4x4Float,  const glm:: mat4x4&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix2x2Double, const glm::dmat2x2&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix2x3Double, const glm::dmat2x3&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix2x4Double, const glm::dmat2x4&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix3x2Double, const glm::dmat3x2&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix3x3Double, const glm::dmat3x3&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix3x4Double, const glm::dmat3x4&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix4x2Double, const glm::dmat4x2&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix4x3Double, const glm::dmat4x3&);
// GET_GL_UNIFORM_MATRIX(getUniformMatrix4x4Double, const glm::dmat4x4&);
