#include "dx/dx11/DX11BlendingState.hpp"

#ifdef _WIN32
#include "dx/dx11/DX11GraphicsInterface.hpp"
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

void DX11BlendingState::apply(DX11RenderingContext& context, const FLOAT blendFactor[4]) const noexcept
{
    CTX();
    ctx.d3d11DeviceContext()->OMSetBlendState(_d3dBlendState, blendFactor, 0xFFFFFFFF);
}

DX11BlendingState* DX11BlendingStateBuilder::build(const BlendingArgs& args, Error* const error) const noexcept
{
    ID3D11BlendState* d3dBlendState;
    if(!processArgs(args, &d3dBlendState, error))
    { return null; }

    DX11BlendingState* const blendingState = new(::std::nothrow) DX11BlendingState(args, d3dBlendState);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

DX11BlendingState* DX11BlendingStateBuilder::build(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D11BlendState* d3dBlendState;
    if(!processArgs(args, &d3dBlendState, error))
    { return null; }

    DX11BlendingState* const blendingState = allocator.allocateT<DX11BlendingState>(args, d3dBlendState);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

CPPRef<IBlendingState> DX11BlendingStateBuilder::buildCPPRef(const BlendingArgs& args, Error* const error) const noexcept
{
    ID3D11BlendState* d3dBlendState;
    if(!processArgs(args, &d3dBlendState, error))
    { return null; }

    const CPPRef<DX11BlendingState> blendingState = CPPRef<DX11BlendingState>(new(::std::nothrow) DX11BlendingState(args, d3dBlendState));
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

NullableRef<IBlendingState> DX11BlendingStateBuilder::buildTauRef(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D11BlendState* d3dBlendState;
    if(!processArgs(args, &d3dBlendState, error))
    { return null; }

    const NullableRef<DX11BlendingState> blendingState(allocator, args, d3dBlendState);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

NullableStrongRef<IBlendingState> DX11BlendingStateBuilder::buildTauSRef(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D11BlendState* d3dBlendState;
    if(!processArgs(args, &d3dBlendState, error))
    { return null; }

    const NullableStrongRef<DX11BlendingState> blendingState(allocator, args, d3dBlendState);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

bool DX11BlendingStateBuilder::processArgs(const BlendingArgs& args, ID3D11BlendState** const d3dBlendState, Error* const error) const noexcept
{
    D3D11_BLEND_DESC desc;

    desc.AlphaToCoverageEnable = false;
    desc.IndependentBlendEnable = args.independentBlending;
    for(uSys i = 0; i < 8; ++i)
    {
        desc.RenderTarget[i].BlendEnable = args.frameBuffers[i].enableBlending;
        desc.RenderTarget[i].SrcBlend = dxBlendFactor(args.frameBuffers[0].colorSrcFactor);
        desc.RenderTarget[i].DestBlend = dxBlendFactor(args.frameBuffers[0].colorDstFactor);
        desc.RenderTarget[i].SrcBlendAlpha = dxBlendFactor(args.frameBuffers[0].alphaSrcFactor);
        desc.RenderTarget[i].DestBlendAlpha = dxBlendFactor(args.frameBuffers[0].alphaDstFactor);
        desc.RenderTarget[i].BlendOp = dxBlendOp(args.frameBuffers[0].colorBlendOp);
        desc.RenderTarget[i].BlendOpAlpha = dxBlendOp(args.frameBuffers[0].alphaBlendOp);
        desc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        ERROR_CODE_COND_F(desc.RenderTarget[i].SrcBlend == static_cast<D3D11_BLEND>(0), Error::InvalidBlendFactor);
        ERROR_CODE_COND_F(desc.RenderTarget[i].DestBlend == static_cast<D3D11_BLEND>(0), Error::InvalidBlendFactor);
        ERROR_CODE_COND_F(desc.RenderTarget[i].SrcBlendAlpha == static_cast<D3D11_BLEND>(0), Error::InvalidBlendFactor);
        ERROR_CODE_COND_F(desc.RenderTarget[i].DestBlendAlpha == static_cast<D3D11_BLEND>(0), Error::InvalidBlendFactor);
        ERROR_CODE_COND_F(desc.RenderTarget[i].BlendOp == static_cast<D3D11_BLEND_OP>(0), Error::InvalidBlendOp);
        ERROR_CODE_COND_F(desc.RenderTarget[i].BlendOpAlpha == static_cast<D3D11_BLEND_OP>(0), Error::InvalidBlendOp);
    }

    _gi.d3d11Device()->CreateBlendState(&desc, d3dBlendState);
    ERROR_CODE_COND_F(!(*d3dBlendState), Error::DriverMemoryAllocationFailure);

    return true;
}

D3D11_BLEND DX11BlendingStateBuilder::dxBlendFactor(const BlendingArgs::BlendFactor blendFactor) noexcept
{
    switch(blendFactor)
    {
        case BlendingArgs::BlendFactor::Zero:             return D3D11_BLEND_ZERO;
        case BlendingArgs::BlendFactor::One:              return D3D11_BLEND_ONE;
        case BlendingArgs::BlendFactor::SrcColor:         return D3D11_BLEND_SRC_COLOR;
        case BlendingArgs::BlendFactor::InvSrcColor:      return D3D11_BLEND_INV_SRC_COLOR;
        case BlendingArgs::BlendFactor::SrcAlpha:         return D3D11_BLEND_SRC_ALPHA;
        case BlendingArgs::BlendFactor::InvSrcAlpha:      return D3D11_BLEND_INV_SRC_ALPHA;
        case BlendingArgs::BlendFactor::DestAlpha:        return D3D11_BLEND_DEST_ALPHA;
        case BlendingArgs::BlendFactor::InvDestAlpha:     return D3D11_BLEND_INV_DEST_ALPHA;
        case BlendingArgs::BlendFactor::DestColor:        return D3D11_BLEND_DEST_COLOR;
        case BlendingArgs::BlendFactor::InvDestColor:     return D3D11_BLEND_INV_DEST_COLOR;
        case BlendingArgs::BlendFactor::SrcAlphaSat:      return D3D11_BLEND_SRC_ALPHA_SAT;
        case BlendingArgs::BlendFactor::BlendConstant:    return D3D11_BLEND_BLEND_FACTOR;
        case BlendingArgs::BlendFactor::InvBlendConstant: return D3D11_BLEND_INV_BLEND_FACTOR;
        case BlendingArgs::BlendFactor::Src1Color:        return D3D11_BLEND_SRC1_COLOR;
        case BlendingArgs::BlendFactor::InvSrc1Color:     return D3D11_BLEND_INV_SRC1_COLOR;
        case BlendingArgs::BlendFactor::Src1Alpha:        return D3D11_BLEND_SRC1_ALPHA;
        case BlendingArgs::BlendFactor::InvSrc1Alpha:     return D3D11_BLEND_INV_SRC1_ALPHA;
        default: return static_cast<D3D11_BLEND>(0);
    }
}

D3D11_BLEND_OP DX11BlendingStateBuilder::dxBlendOp(const BlendingArgs::BlendOp blendOp) noexcept
{
    switch(blendOp)
    {
        case BlendingArgs::BlendOp::Add:         return D3D11_BLEND_OP_ADD;
        case BlendingArgs::BlendOp::Subtract:    return D3D11_BLEND_OP_SUBTRACT;
        case BlendingArgs::BlendOp::RevSubtract: return D3D11_BLEND_OP_REV_SUBTRACT;
        case BlendingArgs::BlendOp::Min:         return D3D11_BLEND_OP_MIN;
        case BlendingArgs::BlendOp::Max:         return D3D11_BLEND_OP_MAX;
        default: return static_cast<D3D11_BLEND_OP>(0);
    }
}
#endif
