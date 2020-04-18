#include "dx/dx10/DX10BlendingState.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10GraphicsInterface.hpp"
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

void DX10BlendingState::apply(DX10RenderingContext& context, const FLOAT blendFactor[4]) const noexcept
{
    CTX();
    ctx.d3dDevice()->OMSetBlendState(_d3dBlendState, blendFactor, 0xFFFFFFFF);
}

DX10BlendingState* DX10BlendingStateBuilder::build(const BlendingArgs& args, Error* const error) const noexcept
{
    ID3D10BlendState* d3dBlendState;
    if(!processArgs(args, &d3dBlendState, error))
    { return null; }

    DX10BlendingState* const blendingState = new(::std::nothrow) DX10BlendingState(args, d3dBlendState);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

DX10BlendingState* DX10BlendingStateBuilder::build(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10BlendState* d3dBlendState;
    if(!processArgs(args, &d3dBlendState, error))
    { return null; }

    DX10BlendingState* const blendingState = allocator.allocateT<DX10BlendingState>(args, d3dBlendState);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

CPPRef<IBlendingState> DX10BlendingStateBuilder::buildCPPRef(const BlendingArgs& args, Error* const error) const noexcept
{
    ID3D10BlendState* d3dBlendState;
    if(!processArgs(args, &d3dBlendState, error))
    { return null; }

    const CPPRef<DX10BlendingState> blendingState = CPPRef<DX10BlendingState>(new(::std::nothrow) DX10BlendingState(args, d3dBlendState));
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

NullableRef<IBlendingState> DX10BlendingStateBuilder::buildTauRef(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10BlendState* d3dBlendState;
    if(!processArgs(args, &d3dBlendState, error))
    { return null; }

    const NullableRef<DX10BlendingState> blendingState(allocator, args, d3dBlendState);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

NullableStrongRef<IBlendingState> DX10BlendingStateBuilder::buildTauSRef(const BlendingArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D10BlendState* d3dBlendState;
    if(!processArgs(args, &d3dBlendState, error))
    { return null; }

    const NullableStrongRef<DX10BlendingState> blendingState(allocator, args, d3dBlendState);
    ERROR_CODE_COND_N(!blendingState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, blendingState);
}

bool DX10BlendingStateBuilder::processArgs(const BlendingArgs& args, ID3D10BlendState** const d3dBlendState, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(args.independentBlending, Error::DriverDoesNotSupportIndependentBlending);

    D3D10_BLEND_DESC desc;

    desc.AlphaToCoverageEnable = false;
    for(uSys i = 0; i < 8; ++i)
    {
        desc.BlendEnable[i] = args.frameBuffers[i].enableBlending;
    }
    desc.SrcBlend = dxBlendFactor(args.frameBuffers[0].colorSrcFactor);
    desc.DestBlend = dxBlendFactor(args.frameBuffers[0].colorDstFactor);
    desc.SrcBlendAlpha = dxBlendFactor(args.frameBuffers[0].alphaSrcFactor);
    desc.DestBlendAlpha = dxBlendFactor(args.frameBuffers[0].alphaDstFactor);
    desc.BlendOp = dxBlendOp(args.frameBuffers[0].colorBlendOp);
    desc.BlendOpAlpha = dxBlendOp(args.frameBuffers[0].alphaBlendOp);
    for(uSys i = 0; i < 8; ++i)
    {
        desc.RenderTargetWriteMask[i] = D3D10_COLOR_WRITE_ENABLE_ALL;
    }

    ERROR_CODE_COND_F(desc.SrcBlend == static_cast<D3D10_BLEND>(0), Error::InvalidBlendFactor);
    ERROR_CODE_COND_F(desc.DestBlend == static_cast<D3D10_BLEND>(0), Error::InvalidBlendFactor);
    ERROR_CODE_COND_F(desc.SrcBlendAlpha == static_cast<D3D10_BLEND>(0), Error::InvalidBlendFactor);
    ERROR_CODE_COND_F(desc.DestBlendAlpha == static_cast<D3D10_BLEND>(0), Error::InvalidBlendFactor);
    ERROR_CODE_COND_F(desc.BlendOp == static_cast<D3D10_BLEND_OP>(0), Error::InvalidBlendOp);
    ERROR_CODE_COND_F(desc.BlendOpAlpha == static_cast<D3D10_BLEND_OP>(0), Error::InvalidBlendOp);

    _gi.d3d10Device()->CreateBlendState(&desc, d3dBlendState);
    ERROR_CODE_COND_F(!(*d3dBlendState), Error::DriverMemoryAllocationFailure);

    return true;
}

D3D10_BLEND DX10BlendingStateBuilder::dxBlendFactor(const BlendingArgs::BlendFactor blendFactor) noexcept
{
    switch(blendFactor)
    {
        case BlendingArgs::BlendFactor::Zero:             return D3D10_BLEND_ZERO;
        case BlendingArgs::BlendFactor::One:              return D3D10_BLEND_ONE;
        case BlendingArgs::BlendFactor::SrcColor:         return D3D10_BLEND_SRC_COLOR;
        case BlendingArgs::BlendFactor::InvSrcColor:      return D3D10_BLEND_INV_SRC_COLOR;
        case BlendingArgs::BlendFactor::SrcAlpha:         return D3D10_BLEND_SRC_ALPHA;
        case BlendingArgs::BlendFactor::InvSrcAlpha:      return D3D10_BLEND_INV_SRC_ALPHA;
        case BlendingArgs::BlendFactor::DestAlpha:        return D3D10_BLEND_DEST_ALPHA;
        case BlendingArgs::BlendFactor::InvDestAlpha:     return D3D10_BLEND_INV_DEST_ALPHA;
        case BlendingArgs::BlendFactor::DestColor:        return D3D10_BLEND_DEST_COLOR;
        case BlendingArgs::BlendFactor::InvDestColor:     return D3D10_BLEND_INV_DEST_COLOR;
        case BlendingArgs::BlendFactor::SrcAlphaSat:      return D3D10_BLEND_SRC_ALPHA_SAT;
        case BlendingArgs::BlendFactor::BlendConstant:    return D3D10_BLEND_BLEND_FACTOR;
        case BlendingArgs::BlendFactor::InvBlendConstant: return D3D10_BLEND_INV_BLEND_FACTOR;
        case BlendingArgs::BlendFactor::Src1Color:        return D3D10_BLEND_SRC1_COLOR;
        case BlendingArgs::BlendFactor::InvSrc1Color:     return D3D10_BLEND_INV_SRC1_COLOR;
        case BlendingArgs::BlendFactor::Src1Alpha:        return D3D10_BLEND_SRC1_ALPHA;
        case BlendingArgs::BlendFactor::InvSrc1Alpha:     return D3D10_BLEND_INV_SRC1_ALPHA;
        default: return static_cast<D3D10_BLEND>(0);
    }
}

D3D10_BLEND_OP DX10BlendingStateBuilder::dxBlendOp(const BlendingArgs::BlendOp blendOp) noexcept
{
    switch(blendOp)
    {
        case BlendingArgs::BlendOp::Add:         return D3D10_BLEND_OP_ADD;
        case BlendingArgs::BlendOp::Subtract:    return D3D10_BLEND_OP_SUBTRACT;
        case BlendingArgs::BlendOp::RevSubtract: return D3D10_BLEND_OP_REV_SUBTRACT;
        case BlendingArgs::BlendOp::Min:         return D3D10_BLEND_OP_MIN;
        case BlendingArgs::BlendOp::Max:         return D3D10_BLEND_OP_MAX;
        default: return static_cast<D3D10_BLEND_OP>(0);
    }
}
#endif
