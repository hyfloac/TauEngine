#include "dx/dx12/DX12BlendingState.hpp"

#ifdef _WIN32

NullableRef<IBlendingState> DX12BlendingStateBuilder::buildTauRef(const BlendingArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    D3D12_BLEND_DESC desc;
    if(!processArgs(args, &desc, error))
    { return null; }

    const NullableRef<DX12BlendingState> blendingState(allocator, args, desc);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

static D3D12_BLEND dxBlendFactor(BlendingArgs::BlendFactor blendFactor) noexcept;
static D3D12_BLEND_OP dxBlendOp(BlendingArgs::BlendOp blendOp) noexcept;

bool DX12BlendingStateBuilder::processArgs(const BlendingArgs& args, D3D12_BLEND_DESC* desc, Error* error) const noexcept
{
    desc->AlphaToCoverageEnable = false;
    desc->IndependentBlendEnable = args.independentBlending;
    for(uSys i = 0; i < 8; ++i)
    {
        desc->RenderTarget[i].BlendEnable = args.frameBuffers[i].enableBlending;
        desc->RenderTarget[i].SrcBlend = dxBlendFactor(args.frameBuffers[0].colorSrcFactor);
        desc->RenderTarget[i].DestBlend = dxBlendFactor(args.frameBuffers[0].colorDstFactor);
        desc->RenderTarget[i].SrcBlendAlpha = dxBlendFactor(args.frameBuffers[0].alphaSrcFactor);
        desc->RenderTarget[i].DestBlendAlpha = dxBlendFactor(args.frameBuffers[0].alphaDstFactor);
        desc->RenderTarget[i].BlendOp = dxBlendOp(args.frameBuffers[0].colorBlendOp);
        desc->RenderTarget[i].BlendOpAlpha = dxBlendOp(args.frameBuffers[0].alphaBlendOp);
        desc->RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

        ERROR_CODE_COND_F(desc->RenderTarget[i].SrcBlend == static_cast<D3D12_BLEND>(0), Error::InvalidBlendFactor);
        ERROR_CODE_COND_F(desc->RenderTarget[i].DestBlend == static_cast<D3D12_BLEND>(0), Error::InvalidBlendFactor);
        ERROR_CODE_COND_F(desc->RenderTarget[i].SrcBlendAlpha == static_cast<D3D12_BLEND>(0), Error::InvalidBlendFactor);
        ERROR_CODE_COND_F(desc->RenderTarget[i].DestBlendAlpha == static_cast<D3D12_BLEND>(0), Error::InvalidBlendFactor);
        ERROR_CODE_COND_F(desc->RenderTarget[i].BlendOp == static_cast<D3D12_BLEND_OP>(0), Error::InvalidBlendOp);
        ERROR_CODE_COND_F(desc->RenderTarget[i].BlendOpAlpha == static_cast<D3D12_BLEND_OP>(0), Error::InvalidBlendOp);
    }

    return true;
}

static D3D12_BLEND dxBlendFactor(const BlendingArgs::BlendFactor blendFactor) noexcept
{
    switch(blendFactor)
    {
        case BlendingArgs::BlendFactor::Zero:             return D3D12_BLEND_ZERO;
        case BlendingArgs::BlendFactor::One:              return D3D12_BLEND_ONE;
        case BlendingArgs::BlendFactor::SrcColor:         return D3D12_BLEND_SRC_COLOR;
        case BlendingArgs::BlendFactor::InvSrcColor:      return D3D12_BLEND_INV_SRC_COLOR;
        case BlendingArgs::BlendFactor::SrcAlpha:         return D3D12_BLEND_SRC_ALPHA;
        case BlendingArgs::BlendFactor::InvSrcAlpha:      return D3D12_BLEND_INV_SRC_ALPHA;
        case BlendingArgs::BlendFactor::DestAlpha:        return D3D12_BLEND_DEST_ALPHA;
        case BlendingArgs::BlendFactor::InvDestAlpha:     return D3D12_BLEND_INV_DEST_ALPHA;
        case BlendingArgs::BlendFactor::DestColor:        return D3D12_BLEND_DEST_COLOR;
        case BlendingArgs::BlendFactor::InvDestColor:     return D3D12_BLEND_INV_DEST_COLOR;
        case BlendingArgs::BlendFactor::SrcAlphaSat:      return D3D12_BLEND_SRC_ALPHA_SAT;
        case BlendingArgs::BlendFactor::BlendConstant:    return D3D12_BLEND_BLEND_FACTOR;
        case BlendingArgs::BlendFactor::InvBlendConstant: return D3D12_BLEND_INV_BLEND_FACTOR;
        case BlendingArgs::BlendFactor::Src1Color:        return D3D12_BLEND_SRC1_COLOR;
        case BlendingArgs::BlendFactor::InvSrc1Color:     return D3D12_BLEND_INV_SRC1_COLOR;
        case BlendingArgs::BlendFactor::Src1Alpha:        return D3D12_BLEND_SRC1_ALPHA;
        case BlendingArgs::BlendFactor::InvSrc1Alpha:     return D3D12_BLEND_INV_SRC1_ALPHA;
        default: return static_cast<D3D12_BLEND>(0);
    }
}

static D3D12_BLEND_OP dxBlendOp(const BlendingArgs::BlendOp blendOp) noexcept
{
    switch(blendOp)
    {
        case BlendingArgs::BlendOp::Add:         return D3D12_BLEND_OP_ADD;
        case BlendingArgs::BlendOp::Subtract:    return D3D12_BLEND_OP_SUBTRACT;
        case BlendingArgs::BlendOp::RevSubtract: return D3D12_BLEND_OP_REV_SUBTRACT;
        case BlendingArgs::BlendOp::Min:         return D3D12_BLEND_OP_MIN;
        case BlendingArgs::BlendOp::Max:         return D3D12_BLEND_OP_MAX;
        default: return static_cast<D3D12_BLEND_OP>(0);
    }
}
#endif
