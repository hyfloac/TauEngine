#include "dx/dx11/DX11ShaderProgram.hpp"
#include "TauEngine.hpp"

#ifdef _WIN32
#include "dx/dx11/DX11Shader.hpp"
#include "dx/dx11/DX11RenderingContext.hpp"

CPPRef<DX11VertexShader> DX11ShaderProgram::dxVertexShader() const noexcept
{ return RefCast<DX11VertexShader>(_vertexShader); }

CPPRef<DX11HullShader> DX11ShaderProgram::dxHullShader() const noexcept
{ return RefCast<DX11HullShader>(_tessellationControlShader); }

CPPRef<DX11DomainShader> DX11ShaderProgram::dxDomainShader() const noexcept
{ return RefCast<DX11DomainShader>(_tessellationEvaluationShader); }

CPPRef<DX11GeometryShader> DX11ShaderProgram::dxGeometryShader() const noexcept
{ return RefCast<DX11GeometryShader>(_geometryShader); }

CPPRef<DX11PixelShader> DX11ShaderProgram::dxPixelShader() const noexcept
{ return RefCast<DX11PixelShader>(_pixelShader); }

void DX11ShaderProgram::bind(IRenderingContext& context) noexcept
{
    if(!RTT_CHECK(context, DX11RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& dxCtx = reinterpret_cast<DX11RenderingContext&>(context);
    if(_vertexShader) { dxVertexShader()->bind(dxCtx); }
    if(_tessellationControlShader) { dxHullShader()->bind(dxCtx); }
    if(_tessellationEvaluationShader) { dxDomainShader()->bind(dxCtx); }
    if(_geometryShader) { dxGeometryShader()->bind(dxCtx); }
    if(_pixelShader) { dxPixelShader()->bind(dxCtx); }
}

void DX11ShaderProgram::unbind(IRenderingContext& context) noexcept
{
    if(!RTT_CHECK(context, DX11RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return;
    }

    auto& dxCtx = reinterpret_cast<DX11RenderingContext&>(context);
    if(_vertexShader) { dxVertexShader()->unbind(dxCtx); }
    if(_tessellationControlShader) { dxHullShader()->unbind(dxCtx); }
    if(_tessellationEvaluationShader) { dxDomainShader()->unbind(dxCtx); }
    if(_geometryShader) { dxGeometryShader()->unbind(dxCtx); }
    if(_pixelShader) { dxPixelShader()->unbind(dxCtx); }
}

bool DX11ShaderProgram::link(IRenderingContext& context) noexcept
{ return true; }

bool DX11ShaderProgram::attach(IRenderingContext& context, CPPRef<IShader> shader) noexcept
{
    if(!RTT_CHECK(context, DX11RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return false;
    }
    if(RTT_CHECK(shader.get(), DX11Shader))
    { return true; }
    else
    {
        TAU_THROW(IncorrectAPIShaderException);
        return false;
    }
}

void DX11ShaderProgram::detach(IRenderingContext& context, CPPRef<IShader> shader) noexcept
{
}

#endif
