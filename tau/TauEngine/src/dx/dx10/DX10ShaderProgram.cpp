#include "dx/dx10/DX10ShaderProgram.hpp"
#include "TauEngine.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10Shader.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"

CPPRef<DX10VertexShader> DX10ShaderProgram::dxVertexShader() const noexcept
{
    return RefCast<DX10VertexShader>(_vertexShader);
}

CPPRef<DX10GeometryShader> DX10ShaderProgram::dxGeometryShader() const noexcept
{
    return RefCast<DX10GeometryShader>(_geometryShader);
}

CPPRef<DX10PixelShader> DX10ShaderProgram::dxPixelShader() const noexcept
{
    return RefCast<DX10PixelShader>(_pixelShader);
}

void DX10ShaderProgram::bind(IRenderingContext& context) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& dxCtx = reinterpret_cast<DX10RenderingContext&>(context);
    if(_vertexShader)
    {
        dxVertexShader()->bind(dxCtx);
    }
    if(_geometryShader)
    {
        dxGeometryShader()->bind(dxCtx);
    }
    if(_pixelShader)
    {
        dxPixelShader()->bind(dxCtx);
    }
}

void DX10ShaderProgram::unbind(IRenderingContext& context) noexcept
{
}

bool DX10ShaderProgram::link(IRenderingContext& context) noexcept
{
    return true;
}

bool DX10ShaderProgram::attach(IRenderingContext& context, CPPRef<IShader> shader) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return false;
    }
    if(RTT_CHECK(shader.get(), DX10Shader))
    {
        // const CPPRef<DX10Shader> dxShader = RefCast<DX10Shader>(shader);
        // dxShader->bind(reinterpret_cast<DX10RenderingContext&>(context));
        return true;
    }
    else
    {
        TAU_THROW(IncorrectAPIShaderException);
        return false;
    }
}

void DX10ShaderProgram::detach(IRenderingContext& context, CPPRef<IShader> shader) noexcept
{
}

#if 0
  #define GET_DX10_UNIFORM(_FUNC, _TYPE) \
      CPPRef<IUniform<_TYPE>> DX10ShaderProgram::_FUNC(const char* name) noexcept \
      { return CPPRef<IUniform<_TYPE>>(new(std::nothrow) DX10Uniform<_TYPE>(DX10Uniform<_TYPE>::create(_programID, name))); }
  
  #define GET_DX10_UNIFORM_MATRIX(_FUNC, _TYPE) \
      CPPRef<IUniform<_TYPE>> DX10ShaderProgram::_FUNC(const char* name, bool transpose) noexcept \
      { return CPPRef<IUniform<_TYPE>>(new(std::nothrow) DX10Uniform<_TYPE>(DX10Uniform<_TYPE>::create(_programID, name), transpose)); }
#else
  #define GET_DX10_UNIFORM(_FUNC, _TYPE) \
      CPPRef<IUniform<_TYPE>> DX10ShaderProgram::_FUNC(const char* name) noexcept \
      { return null; }
  
  #define GET_DX10_UNIFORM_MATRIX(_FUNC, _TYPE) \
      CPPRef<IUniform<_TYPE>> DX10ShaderProgram::_FUNC(const char* name, bool transpose) noexcept \
      { return null; }
#endif


// GET_DX10_UNIFORM(getUniformBool, bool);
// GET_DX10_UNIFORM(getUniformInt, int);
// GET_DX10_UNIFORM(getUniformUInt, unsigned int);
// GET_DX10_UNIFORM(getUniformFloat, float);
// GET_DX10_UNIFORM(getUniformDouble, double);
// GET_DX10_UNIFORM(getUniformVector2f, const Vector2f&);
// GET_DX10_UNIFORM(getUniformVector3i, const Vector3i&);
// GET_DX10_UNIFORM(getUniformVector3f, const Vector3f&);
// GET_DX10_UNIFORM(getUniformVector4f, const Vector4f&);
//
// GET_DX10_UNIFORM(getUniformVector2Bool,   const glm::bvec2&);
// GET_DX10_UNIFORM(getUniformVector3Bool,   const glm::bvec3&);
// GET_DX10_UNIFORM(getUniformVector4Bool,   const glm::bvec4&);
// GET_DX10_UNIFORM(getUniformVector2Int,    const glm::ivec2&);
// GET_DX10_UNIFORM(getUniformVector3Int,    const glm::ivec3&);
// GET_DX10_UNIFORM(getUniformVector4Int,    const glm::ivec4&);
// GET_DX10_UNIFORM(getUniformVector2UInt,   const glm::uvec2&);
// GET_DX10_UNIFORM(getUniformVector3UInt,   const glm::uvec3&);
// GET_DX10_UNIFORM(getUniformVector4UInt,   const glm::uvec4&);
// GET_DX10_UNIFORM(getUniformVector2Float,  const glm:: vec2&);
// GET_DX10_UNIFORM(getUniformVector3Float,  const glm:: vec3&);
// GET_DX10_UNIFORM(getUniformVector4Float,  const glm:: vec4&);
// GET_DX10_UNIFORM(getUniformVector2Double, const glm::dvec2&);
// GET_DX10_UNIFORM(getUniformVector3Double, const glm::dvec3&);
// GET_DX10_UNIFORM(getUniformVector4Double, const glm::dvec4&);
//
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix2x2Float,  const glm:: mat2x2&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix2x3Float,  const glm:: mat2x3&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix2x4Float,  const glm:: mat2x4&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix3x2Float,  const glm:: mat3x2&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix3x3Float,  const glm:: mat3x3&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix3x4Float,  const glm:: mat3x4&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix4x2Float,  const glm:: mat4x2&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix4x3Float,  const glm:: mat4x3&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix4x4Float,  const glm:: mat4x4&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix2x2Double, const glm::dmat2x2&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix2x3Double, const glm::dmat2x3&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix2x4Double, const glm::dmat2x4&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix3x2Double, const glm::dmat3x2&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix3x3Double, const glm::dmat3x3&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix3x4Double, const glm::dmat3x4&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix4x2Double, const glm::dmat4x2&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix4x3Double, const glm::dmat4x3&);
// GET_DX10_UNIFORM_MATRIX(getUniformMatrix4x4Double, const glm::dmat4x4&);

#endif
