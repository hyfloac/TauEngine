#include "dx/dx10/DX10ShaderProgram.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10Shader.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "TauEngine.hpp"
#include "TauConfig.hpp"

#if TAU_RTTI_CHECK
  #define CTX() \
      if(!RTT_CHECK(context, DX10RenderingContext)) \
      { TAU_THROW(IncorrectContextException); } \
      auto& ctx = reinterpret_cast<DX10RenderingContext&>(context)
#else
  #define CTX() \
      auto& ctx = reinterpret_cast<DX10RenderingContext&>(context)
#endif

CPPRef<DX10VertexShader> DX10ShaderProgram::dxVertexShader() const noexcept
{ return RefCast<DX10VertexShader>(_vertexShader); }

CPPRef<DX10GeometryShader> DX10ShaderProgram::dxGeometryShader() const noexcept
{ return RefCast<DX10GeometryShader>(_geometryShader); }

CPPRef<DX10PixelShader> DX10ShaderProgram::dxPixelShader() const noexcept
{ return RefCast<DX10PixelShader>(_pixelShader); }

void DX10ShaderProgram::bind(IRenderingContext& context) noexcept
{
    CTX();
    if(_vertexShader) { dxVertexShader()->bind(ctx); }
    if(_geometryShader) { dxGeometryShader()->bind(ctx); }
    if(_pixelShader) { dxPixelShader()->bind(ctx); }
}

void DX10ShaderProgram::unbind(IRenderingContext& context) noexcept
{
    CTX();
    if(_vertexShader) { dxVertexShader()->unbind(ctx); }
    if(_geometryShader) { dxGeometryShader()->unbind(ctx); }
    if(_pixelShader) { dxPixelShader()->unbind(ctx); }
}

bool DX10ShaderProgram::link(IRenderingContext&) noexcept
{ return true; }

bool DX10ShaderProgram::attach(IRenderingContext& context, const CPPRef<IShader>& shader) noexcept
{
#if TAU_RTTI_CHECK
    if(!RTT_CHECK(context, DX10RenderingContext))
    { TAU_THROW(IncorrectContextException); }
#endif

    if(RTT_CHECK(shader.get(), DX10Shader))
    { return true; }
    else
    {
        TAU_THROW(IncorrectAPIShaderException);
        return false;
    }
}
#endif
