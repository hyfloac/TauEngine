#include "dx/dx11/DX11FrameBuffer.hpp"

#ifdef _WIN32
#include "dx/dx11/DX11GraphicsInterface.hpp"
#include "dx/dx11/DX11RenderingContext.hpp"
#include "dx/dx11/DX11Texture.hpp"
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

void DX11FrameBuffer::bind(IRenderingContext& context, AccessMode) noexcept
{
    CTX();
    auto* dsv = reinterpret_cast<DX11DepthStencilTarget*>(_depthStencilAttachment->renderTarget())->d3dDepthStencilView();
    ctx.d3d11DeviceContext()->OMSetRenderTargets(_colorAttachments.count(), _d3dColorAttachments, dsv);
}

void DX11FrameBuffer::unbind(IRenderingContext& context) noexcept
{
    CTX();
    ctx.resetFrameBuffer();
}

void DX11FrameBuffer::clearFrameBuffer(IRenderingContext& context, bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, u8 stencilValue) noexcept
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
            ctx.d3d11DeviceContext()->ClearRenderTargetView(_d3dColorAttachments[i], colorF);
        }
    }

    UINT clearFlags = 0;
    if(clearDepthBuffer)
    {
        clearFlags |= D3D11_CLEAR_DEPTH;
    }
    if(clearStencilBuffer)
    {
        clearFlags |= D3D11_CLEAR_STENCIL;
    }

    auto* dsv = reinterpret_cast<DX11DepthStencilTarget*>(_depthStencilAttachment->renderTarget())->d3dDepthStencilView();
    ctx.d3d11DeviceContext()->ClearDepthStencilView(dsv, clearFlags, depthValue, stencilValue);
}

DX11FrameBuffer* DX11FrameBufferBuilder::build(const FrameBufferArgs& args, Error* error) const noexcept
{
    DX11FrameBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX11FrameBuffer* const frameBuffer = new(::std::nothrow) DX11FrameBuffer(args.colorAttachments, args.depthStencilAttachment, dxArgs.d3dColorAttachments);
    ERROR_CODE_COND_N(!frameBuffer, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dColorAttachments = null;
    ERROR_CODE_V(Error::NoError, frameBuffer);
}

DX11FrameBuffer* DX11FrameBufferBuilder::build(const FrameBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DX11FrameBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX11FrameBuffer* const frameBuffer = allocator.allocateT<DX11FrameBuffer>(args.colorAttachments, args.depthStencilAttachment, dxArgs.d3dColorAttachments);
    ERROR_CODE_COND_N(!frameBuffer, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dColorAttachments = null;
    ERROR_CODE_V(Error::NoError, frameBuffer);
}

CPPRef<IFrameBuffer> DX11FrameBufferBuilder::buildCPPRef(const FrameBufferArgs& args, Error* error) const noexcept
{
    DX11FrameBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX11FrameBuffer> frameBuffer = CPPRef<DX11FrameBuffer>(new(::std::nothrow) DX11FrameBuffer(args.colorAttachments, args.depthStencilAttachment, dxArgs.d3dColorAttachments));
    ERROR_CODE_COND_N(!frameBuffer, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dColorAttachments = null;
    ERROR_CODE_V(Error::NoError, frameBuffer);
}

NullableRef<IFrameBuffer> DX11FrameBufferBuilder::buildTauRef(const FrameBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DX11FrameBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX11FrameBuffer> frameBuffer(allocator, args.colorAttachments, args.depthStencilAttachment, dxArgs.d3dColorAttachments);
    ERROR_CODE_COND_N(!frameBuffer, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dColorAttachments = null;
    ERROR_CODE_V(Error::NoError, frameBuffer);
}

NullableStrongRef<IFrameBuffer> DX11FrameBufferBuilder::buildTauSRef(const FrameBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DX11FrameBufferArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX11FrameBuffer> frameBuffer(allocator, args.colorAttachments, args.depthStencilAttachment, dxArgs.d3dColorAttachments);
    ERROR_CODE_COND_N(!frameBuffer, Error::SystemMemoryAllocationFailure);

    dxArgs.d3dColorAttachments = null;
    ERROR_CODE_V(Error::NoError, frameBuffer);
}

bool DX11FrameBufferBuilder::processArgs(const FrameBufferArgs& args, DX11FrameBufferArgs* dxArgs, Error* error) const noexcept
{
    for(uSys i = 0; i < args.colorAttachments.count(); ++i)
    {
        ERROR_CODE_COND_F(!args.colorAttachments[i], Error::NullAttachment);
        ERROR_CODE_COND_F(args.colorAttachments[i]->textureType() == ETexture::Type::DepthStencil, Error::ColorCannotBeDepthStencil);
        ERROR_CODE_COND_F(args.colorAttachments[i]->dataFormat() == ETexture::Format::Depth24Stencil8, Error::ColorCannotBeDepthStencil);
        ERROR_CODE_COND_F(!RTT_CHECK(args.colorAttachments[i].get(), DX11Texture2D) && !RTT_CHECK(args.colorAttachments[i].get(), DX11TextureCube), Error::CrossAPIFailure);
    }

    if(args.depthStencilAttachment)
    {
        ERROR_CODE_COND_F(args.depthStencilAttachment->textureType() != ETexture::Type::DepthStencil, Error::DepthStencilNotDepthStencil);
        ERROR_CODE_COND_F(args.depthStencilAttachment->dataFormat() != ETexture::Format::Depth24Stencil8, Error::DepthStencilNotDepthStencil);
        ERROR_CODE_COND_F(!RTT_CHECK(args.depthStencilAttachment.get(), DX11TextureDepthStencil), Error::CrossAPIFailure);
    }

    dxArgs->d3dColorAttachments = new ID3D11RenderTargetView* [args.colorAttachments.count()];

    for(uSys i = 0; i < args.colorAttachments.count(); ++i)
    {
        D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
        ID3D11Resource* res;

        if(args.colorAttachments[i]->textureType() == ETexture::Type::T2D)
        {
            res = RefCast<DX11Texture2D>(args.colorAttachments[i])->d3dTexture();

            D3D11_TEXTURE2D_DESC texDesc;
            RefCast<DX11Texture2D>(args.colorAttachments[i])->d3dTexture()->GetDesc(&texDesc);
            
            rtvDesc.Format = texDesc.Format;
            rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
            rtvDesc.Texture2D.MipSlice = 0;
        }
        else if(args.colorAttachments[i]->textureType() == ETexture::Type::Cube)
        {
            res = RefCast<DX11TextureCube>(args.colorAttachments[i])->d3dTexture();

            D3D11_TEXTURE2D_DESC texDesc;
            RefCast<DX11TextureCube>(args.colorAttachments[i])->d3dTexture()->GetDesc(&texDesc);

            rtvDesc.Format = texDesc.Format;
            rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
            rtvDesc.Texture2DArray.MipSlice = 0;
            rtvDesc.Texture2DArray.FirstArraySlice = 0;
            rtvDesc.Texture2DArray.ArraySize = 6;
        }
        else
        {
            ERROR_CODE_F(Error::InternalError);
        }

        _gi.d3d11Device()->CreateRenderTargetView(res, &rtvDesc, &dxArgs->d3dColorAttachments[i]);
        ERROR_CODE_COND_F(!dxArgs->d3dColorAttachments[i], Error::DriverMemoryAllocationFailure);
    }

    return true;
}
#endif
