#include "dx/dx10/DX10FrameBuffer.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10Texture.hpp"
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


void DX10FrameBuffer::bind(IRenderingContext& context, AccessMode) noexcept
{
    CTX();
    auto* dsv = reinterpret_cast<DX10DepthStencilTarget*>(_depthStencilAttachment->renderTarget())->d3dDepthStencilView();
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

    auto* dsv = reinterpret_cast<DX10DepthStencilTarget*>(_depthStencilAttachment->renderTarget())->d3dDepthStencilView();
    ctx.d3dDevice()->ClearDepthStencilView(dsv, clearFlags, depthValue, stencilValue);
}

DX10FrameBuffer* DX10FrameBufferBuilder::build(const FrameBufferArgs& args, Error* error) const noexcept
{
    DX10FrameBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10FrameBuffer* const frameBuffer = new(::std::nothrow) DX10FrameBuffer(args.colorAttachments, args.depthStencilAttachment, dxArgs.d3dColorAttachments);
    ERROR_CODE_COND_N(!frameBuffer, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dColorAttachments = null;
    ERROR_CODE_V(Error::NoError, frameBuffer);
}

DX10FrameBuffer* DX10FrameBufferBuilder::build(const FrameBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DX10FrameBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10FrameBuffer* const frameBuffer = allocator.allocateT<DX10FrameBuffer>(args.colorAttachments, args.depthStencilAttachment, dxArgs.d3dColorAttachments);
    ERROR_CODE_COND_N(!frameBuffer, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dColorAttachments = null;
    ERROR_CODE_V(Error::NoError, frameBuffer);
}

CPPRef<IFrameBuffer> DX10FrameBufferBuilder::buildCPPRef(const FrameBufferArgs& args, Error* error) const noexcept
{
    DX10FrameBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10FrameBuffer> frameBuffer = CPPRef<DX10FrameBuffer>(new(::std::nothrow) DX10FrameBuffer(args.colorAttachments, args.depthStencilAttachment, dxArgs.d3dColorAttachments));
    ERROR_CODE_COND_N(!frameBuffer, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dColorAttachments = null;
    ERROR_CODE_V(Error::NoError, frameBuffer);
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

NullableStrongRef<IFrameBuffer> DX10FrameBufferBuilder::buildTauSRef(const FrameBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DX10FrameBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10FrameBuffer> frameBuffer(allocator, args.colorAttachments, args.depthStencilAttachment, dxArgs.d3dColorAttachments);
    ERROR_CODE_COND_N(!frameBuffer, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dColorAttachments = null;
    ERROR_CODE_V(Error::NoError, frameBuffer);
}

bool DX10FrameBufferBuilder::processArgs(const FrameBufferArgs& args, DX10FrameBufferArgs* dxArgs, Error* error) const noexcept
{
    for(uSys i = 0; i < args.colorAttachments.count(); ++i)
    {
        ERROR_CODE_COND_F(!args.colorAttachments[i], Error::NullAttachment);
        ERROR_CODE_COND_F(args.colorAttachments[i]->textureType() == ETexture::Type::DepthStencil, Error::ColorCannotBeDepthStencil);
        ERROR_CODE_COND_F(args.colorAttachments[i]->dataFormat() == ETexture::Format::Depth24Stencil8, Error::ColorCannotBeDepthStencil);
        ERROR_CODE_COND_F(!RTT_CHECK(args.colorAttachments[i].get(), DX10Texture2D) && !RTT_CHECK(args.colorAttachments[i].get(), DX10TextureCube), Error::CrossAPIFailure);
    }

    if(args.depthStencilAttachment)
    {
        ERROR_CODE_COND_F(args.depthStencilAttachment->textureType() != ETexture::Type::DepthStencil, Error::DepthStencilNotDepthStencil);
        ERROR_CODE_COND_F(args.depthStencilAttachment->dataFormat() != ETexture::Format::Depth24Stencil8, Error::DepthStencilNotDepthStencil);
        ERROR_CODE_COND_F(!RTT_CHECK(args.depthStencilAttachment.get(), DX10TextureDepthStencil), Error::CrossAPIFailure);
    }

    dxArgs->d3dColorAttachments = new ID3D10RenderTargetView* [args.colorAttachments.count()];

    for(uSys i = 0; i < args.colorAttachments.count(); ++i)
    {
        D3D10_RENDER_TARGET_VIEW_DESC rtvDesc;
        ID3D10Resource* res;

        if(args.colorAttachments[i]->textureType() == ETexture::Type::T2D)
        {
            res = RefCast<DX10Texture2D>(args.colorAttachments[i])->d3dTexture();

            D3D10_TEXTURE2D_DESC texDesc;
            RefCast<DX10Texture2D>(args.colorAttachments[i])->d3dTexture()->GetDesc(&texDesc);
            
            rtvDesc.Format = texDesc.Format;
            rtvDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2D;
            rtvDesc.Texture2D.MipSlice = 0;
        }
        else if(args.colorAttachments[i]->textureType() == ETexture::Type::Cube)
        {
            res = RefCast<DX10TextureCube>(args.colorAttachments[i])->d3dTexture();

            D3D10_TEXTURE2D_DESC texDesc;
            RefCast<DX10TextureCube>(args.colorAttachments[i])->d3dTexture()->GetDesc(&texDesc);

            rtvDesc.Format = texDesc.Format;
            rtvDesc.ViewDimension = D3D10_RTV_DIMENSION_TEXTURE2DARRAY;
            rtvDesc.Texture2DArray.MipSlice = 0;
            rtvDesc.Texture2DArray.FirstArraySlice = 0;
            rtvDesc.Texture2DArray.ArraySize = 6;
        }
        else
        {
            ERROR_CODE_F(Error::InternalError);
        }

        _gi.d3d10Device()->CreateRenderTargetView(res, &rtvDesc, &dxArgs->d3dColorAttachments[i]);
        ERROR_CODE_COND_F(!dxArgs->d3dColorAttachments[i], Error::DriverMemoryAllocationFailure);
    }

    return true;
}
#endif
