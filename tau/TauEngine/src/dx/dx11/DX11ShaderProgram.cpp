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

void DX11ShaderProgram::bind(IRenderingContext& context) noexcept
{
    CTX();
    if(_vertexShader)   { dxVertexShader()->bind(ctx);   }
    if(_hullShader)     { dxHullShader()->bind(ctx);     }
    if(_domainShader)   { dxDomainShader()->bind(ctx);   }
    if(_geometryShader) { dxGeometryShader()->bind(ctx); }
    if(_pixelShader)    { dxPixelShader()->bind(ctx);    }
}

void DX11ShaderProgram::unbind(IRenderingContext& context) noexcept
{
    CTX();
    if(_vertexShader)   { dxVertexShader()->unbind(ctx);   }
    if(_hullShader)     { dxHullShader()->unbind(ctx);     }
    if(_domainShader)   { dxDomainShader()->unbind(ctx);   }
    if(_geometryShader) { dxGeometryShader()->unbind(ctx); }
    if(_pixelShader)    { dxPixelShader()->unbind(ctx);    }
}
#endif
