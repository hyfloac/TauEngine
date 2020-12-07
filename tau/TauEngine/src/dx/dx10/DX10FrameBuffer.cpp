#include "dx/dx10/DX10FrameBuffer.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10RenderTarget.hpp"
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


void DX10FrameBuffer::bind(IRenderingContext& context) noexcept
{
    CTX();
    auto* dsv = reinterpret_cast<DX10DepthStencilView*>(_depthStencilAttachment->renderTarget())->d3dDepthStencilView();
    ctx.d3dDevice()->OMSetRenderTargets(_colorAttachments.count(), _d3dColorAttachments, dsv);
}

void DX10FrameBuffer::unbind(IRenderingContext& context) noexcept
{
    CTX();
    ctx.resetFrameBuffer();
}

void DX10FrameBuffer::clearFrameBuffer(IRenderingContext& context, bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, u8 stencilValue) noexcept
{
    CTX();
    float colorF[4];

    colorF[0] = static_cast<float>(color.r) / 255.0f;
    colorF[1] = static_cast<float>(color.g) / 255.0f;
    colorF[2] = static_cast<float>(color.b) / 255.0f;
    colorF[3] = static_cast<float>(color.a) / 255.0f;

    if(clearColorBuffer)
    {
        for(uSys i = 0; i < _colorAttachments.count(); ++i)
        {
            ctx.d3dDevice()->ClearRenderTargetView(_d3dColorAttachments[i], colorF);
        }
    }

    UINT clearFlags = 0;
    if(clearDepthBuffer)
    {
        clearFlags |= D3D10_CLEAR_DEPTH;
    }
    if(clearStencilBuffer)
    {
        clearFlags |= D3D10_CLEAR_STENCIL;
    }

    auto* dsv = reinterpret_cast<DX10DepthStencilView*>(_depthStencilAttachment->renderTarget())->d3dDepthStencilView();
    ctx.d3dDevice()->ClearDepthStencilView(dsv, clearFlags, depthValue, stencilValue);
}

NullableRef<IFrameBuffer> DX10FrameBufferBuilder::buildTauRef(const FrameBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DX10FrameBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10FrameBuffer> frameBuffer(allocator, args.colorAttachments, args.depthStencilAttachment, dxArgs.d3dColorAttachments);
    ERROR_CODE_COND_N(!frameBuffer, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dColorAttachments = null;
    ERROR_CODE_V(Error::NoError, frameBuffer);
}

bool DX10FrameBufferBuilder::processArgs(const FrameBufferArgs& args, DX10FrameBufferArgs* dxArgs, Error* error) const noexcept
{
    for(uSys i = 0; i < args.colorAttachments.count(); ++i)
    {
#if TAU_NULL_CHECK
        ERROR_CODE_COND_F(!args.colorAttachments[i], Error::NullAttachment);
#endif
#if TAU_GENERAL_SAFETY_CHECK
        ERROR_CODE_COND_F(!RTT_CHECK(args.colorAttachments[i].get(), DX10RenderTargetView), Error::CrossAPIFailure);
#endif
    }

#if TAU_GENERAL_SAFETY_CHECK
    if(args.depthStencilAttachment)
    {
        ERROR_CODE_COND_F(!RTT_CHECK(args.depthStencilAttachment.get(), DX10DepthStencilView), Error::CrossAPIFailure);
    }
#endif

    dxArgs->d3dColorAttachments = new ID3D10RenderTargetView* [args.colorAttachments.count()];

    for(uSys i = 0; i < args.colorAttachments.count(); ++i)
    {
        NullableRef<DX10RenderTargetView> rtv = RefCast<DX10RenderTargetView>(args.colorAttachments[i]);
        dxArgs->d3dColorAttachments[i] = rtv->d3dRenderTargetView();
    }

    return true;
}
#endif
