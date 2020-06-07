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

NullableRef<DX10VertexShader> DX10ShaderProgram::dxVertexShader() const noexcept
{ return _vertexShader; }

NullableRef<DX10GeometryShader> DX10ShaderProgram::dxGeometryShader() const noexcept
{ return _geometryShader; }

NullableRef<DX10PixelShader> DX10ShaderProgram::dxPixelShader() const noexcept
{ return _pixelShader; }

void DX10ShaderProgram::bind(IRenderingContext& context) noexcept
{
    CTX();
    if(_vertexShader)   { dxVertexShader()->bind(ctx);   }
    if(_geometryShader) { dxGeometryShader()->bind(ctx); }
    if(_pixelShader)    { dxPixelShader()->bind(ctx);    }
}

void DX10ShaderProgram::unbind(IRenderingContext& context) noexcept
{
    CTX();
    if(_vertexShader)   { dxVertexShader()->unbind(ctx);   }
    if(_geometryShader) { dxGeometryShader()->unbind(ctx); }
    if(_pixelShader)    { dxPixelShader()->unbind(ctx);    }
}
#endif
