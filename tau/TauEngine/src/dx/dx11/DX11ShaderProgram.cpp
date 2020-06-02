#include "dx/dx11/DX11ShaderProgram.hpp"

#ifdef _WIN32
#include "dx/dx11/DX11Shader.hpp"
#include "dx/dx11/DX11RenderingContext.hpp"
#include "TauEngine.hpp"
#include "TauConfig.hpp"

#if TAU_RTTI_CHECK
  #define CTX() \
      if(!RTT_CHECK(context, DX11RenderingContext)) \
      { TAU_THROW(IncorrectContextException); } \
      auto& ctx = reinterpret_cast<DX11RenderingContext&>(context)
#else
  #define CTX() \
      auto& ctx = reinterpret_cast<DX11RenderingContext&>(context)
#endif

CPPRef<DX11VertexShader> DX11ShaderProgram::dxVertexShader() const noexcept
{ return RefCast<DX11VertexShader>(_vertexShader); }

CPPRef<DX11HullShader> DX11ShaderProgram::dxHullShader() const noexcept
{ return RefCast<DX11HullShader>(_tessCtrlShader); }

CPPRef<DX11DomainShader> DX11ShaderProgram::dxDomainShader() const noexcept
{ return RefCast<DX11DomainShader>(_tessEvalShader); }

CPPRef<DX11GeometryShader> DX11ShaderProgram::dxGeometryShader() const noexcept
{ return RefCast<DX11GeometryShader>(_geometryShader); }

CPPRef<DX11PixelShader> DX11ShaderProgram::dxPixelShader() const noexcept
{ return RefCast<DX11PixelShader>(_pixelShader); }

void DX11ShaderProgram::bind(IRenderingContext& context) noexcept
{
    CTX();
    if(_vertexShader) { dxVertexShader()->bind(ctx); }
    if(_tessCtrlShader) { dxHullShader()->bind(ctx); }
    if(_tessEvalShader) { dxDomainShader()->bind(ctx); }
    if(_geometryShader) { dxGeometryShader()->bind(ctx); }
    if(_pixelShader) { dxPixelShader()->bind(ctx); }
}

void DX11ShaderProgram::unbind(IRenderingContext& context) noexcept
{
    CTX();
    if(_vertexShader) { dxVertexShader()->unbind(ctx); }
    if(_tessCtrlShader) { dxHullShader()->unbind(ctx); }
    if(_tessEvalShader) { dxDomainShader()->unbind(ctx); }
    if(_geometryShader) { dxGeometryShader()->unbind(ctx); }
    if(_pixelShader) { dxPixelShader()->unbind(ctx); }
}

bool DX11ShaderProgram::link(IRenderingContext&) noexcept
{ return true; }

bool DX11ShaderProgram::attach(IRenderingContext& context, const CPPRef<IShader>& shader) noexcept
{
#if TAU_RTTI_CHECK
    if(!RTT_CHECK(context, DX11RenderingContext))
    { TAU_THROW(IncorrectContextException); }
#endif

    if(RTT_CHECK(shader.get(), DX11Shader))
    { return true; }
    else
    {
        TAU_THROW(IncorrectAPIShaderException);
        return false;
    }
}
#endif
