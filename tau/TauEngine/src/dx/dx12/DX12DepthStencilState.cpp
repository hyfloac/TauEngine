#include "dx/dx12/DX12DepthStencilState.hpp"

#ifdef _WIN32
NullableRef<IDepthStencilState> DX12DepthStencilStateBuilder::buildTauRef(const DepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    D3D12_DEPTH_STENCIL_DESC desc;
    if(!processArgs(args, &desc, error))
    { return null; }

    const NullableRef<DX12DepthStencilState> depthStencilState(allocator, args, desc);
    ERROR_CODE_COND_N(!depthStencilState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, depthStencilState);
}

static D3D12_DEPTH_WRITE_MASK dxDepthWriteMask(DepthStencilArgs::DepthWriteMask depthWriteMask) noexcept;
static D3D12_COMPARISON_FUNC dxComparisonFunc(DepthStencilArgs::CompareFunc compareFunc) noexcept;
static D3D12_STENCIL_OP dxStencilOp(DepthStencilArgs::StencilOp stencilOp) noexcept;

bool DX12DepthStencilStateBuilder::processArgs(const DepthStencilArgs& args, D3D12_DEPTH_STENCIL_DESC* desc, Error* error) const noexcept
{
    desc->DepthEnable = args.enableDepthTest;
    desc->DepthWriteMask = dxDepthWriteMask(args.depthWriteMask);
    desc->DepthFunc = dxComparisonFunc(args.depthCompareFunc);

    desc->StencilEnable = args.enableStencilTest;
    desc->StencilReadMask = args.stencilReadMask;
    desc->StencilWriteMask = args.stencilWriteMask;

    desc->FrontFace.StencilFailOp = dxStencilOp(args.frontFace.failOp);
    desc->FrontFace.StencilDepthFailOp = dxStencilOp(args.frontFace.stencilPassDepthFailOp);
    desc->FrontFace.StencilPassOp = dxStencilOp(args.frontFace.passOp);
    desc->FrontFace.StencilFunc = dxComparisonFunc(args.frontFace.compareFunc);

    desc->BackFace.StencilFailOp = dxStencilOp(args.backFace.failOp);
    desc->BackFace.StencilDepthFailOp = dxStencilOp(args.backFace.stencilPassDepthFailOp);
    desc->BackFace.StencilPassOp = dxStencilOp(args.backFace.passOp);
    desc->BackFace.StencilFunc = dxComparisonFunc(args.backFace.compareFunc);

    ERROR_CODE_COND_F(static_cast<u32>(desc->DepthWriteMask) == IntMaxMin<u32>::Max, Error::InvalidDepthWriteMask);
    ERROR_CODE_COND_F(static_cast<u32>(desc->DepthFunc) == IntMaxMin<u32>::Max, Error::InvalidDepthCompareFunc);
    ERROR_CODE_COND_F(static_cast<u32>(desc->FrontFace.StencilFailOp) == IntMaxMin<u32>::Max, Error::InvalidFrontFaceStencilFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(desc->FrontFace.StencilDepthFailOp) == IntMaxMin<u32>::Max, Error::InvalidFrontFaceStencilPassDepthFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(desc->FrontFace.StencilPassOp) == IntMaxMin<u32>::Max, Error::InvalidFrontFaceStencilPassOp);
    ERROR_CODE_COND_F(static_cast<u32>(desc->FrontFace.StencilFunc) == IntMaxMin<u32>::Max, Error::InvalidFrontFaceStencilCompareFunc);
    ERROR_CODE_COND_F(static_cast<u32>(desc->BackFace.StencilFailOp) == IntMaxMin<u32>::Max, Error::InvalidBackFaceStencilFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(desc->BackFace.StencilDepthFailOp) == IntMaxMin<u32>::Max, Error::InvalidBackFaceStencilPassDepthFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(desc->BackFace.StencilPassOp) == IntMaxMin<u32>::Max, Error::InvalidBackFaceStencilPassOp);
    ERROR_CODE_COND_F(static_cast<u32>(desc->BackFace.StencilFunc) == IntMaxMin<u32>::Max, Error::InvalidBackFaceStencilCompareFunc);

    return true;
}

static D3D12_DEPTH_WRITE_MASK dxDepthWriteMask(const DepthStencilArgs::DepthWriteMask depthWriteMask) noexcept
{
    switch(depthWriteMask)
    {
        case DepthStencilArgs::DepthWriteMask::Zero: return D3D12_DEPTH_WRITE_MASK_ZERO;
        case DepthStencilArgs::DepthWriteMask::All:  return D3D12_DEPTH_WRITE_MASK_ALL;
        default:                                     return static_cast<D3D12_DEPTH_WRITE_MASK>(IntMaxMin<u32>::Max);
    }
}

static D3D12_COMPARISON_FUNC dxComparisonFunc(const DepthStencilArgs::CompareFunc compareFunc) noexcept
{
    switch(compareFunc)
    {
        case DepthStencilArgs::CompareFunc::LessThanOrEqual:    return D3D12_COMPARISON_FUNC_LESS_EQUAL;
        case DepthStencilArgs::CompareFunc::GreaterThanOrEqual: return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
        case DepthStencilArgs::CompareFunc::LessThan:           return D3D12_COMPARISON_FUNC_LESS;
        case DepthStencilArgs::CompareFunc::GreaterThan:        return D3D12_COMPARISON_FUNC_GREATER;
        case DepthStencilArgs::CompareFunc::Equal:              return D3D12_COMPARISON_FUNC_EQUAL;
        case DepthStencilArgs::CompareFunc::NotEqual:           return D3D12_COMPARISON_FUNC_NOT_EQUAL;
        case DepthStencilArgs::CompareFunc::Always:             return D3D12_COMPARISON_FUNC_ALWAYS;
        case DepthStencilArgs::CompareFunc::Never:              return D3D12_COMPARISON_FUNC_NEVER;
        default:                                                return static_cast<D3D12_COMPARISON_FUNC>(IntMaxMin<u32>::Max);
    }
}

static D3D12_STENCIL_OP dxStencilOp(const DepthStencilArgs::StencilOp stencilOp) noexcept
{
    switch(stencilOp)
    {
        case DepthStencilArgs::StencilOp::Keep:           return D3D12_STENCIL_OP_KEEP;
        case DepthStencilArgs::StencilOp::Zero:           return D3D12_STENCIL_OP_ZERO;
        case DepthStencilArgs::StencilOp::Replace:        return D3D12_STENCIL_OP_REPLACE;
        case DepthStencilArgs::StencilOp::Invert:         return D3D12_STENCIL_OP_INVERT;
        case DepthStencilArgs::StencilOp::IncrementClamp: return D3D12_STENCIL_OP_INCR_SAT;
        case DepthStencilArgs::StencilOp::DecrementClamp: return D3D12_STENCIL_OP_DECR_SAT;
        case DepthStencilArgs::StencilOp::IncrementWrap:  return D3D12_STENCIL_OP_INCR;
        case DepthStencilArgs::StencilOp::DecrementWrap:  return D3D12_STENCIL_OP_DECR;
        default:                                          return static_cast<D3D12_STENCIL_OP>(IntMaxMin<u32>::Max);
    }
}
#endif
