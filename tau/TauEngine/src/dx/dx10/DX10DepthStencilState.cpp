#include "dx/dx10/DX10DepthStencilState.hpp"

#ifdef _WIN32

#include "dx/dx10/DX10RenderingContext.hpp"

#include "TauEngine.hpp"

void DX10DepthStencilState::apply(DX10RenderingContext& ctx) const noexcept
{
    ctx.d3d10Device()->OMSetDepthStencilState(_d3dDepthStencilState, 1);
}

DX10DepthStencilState* DX10DepthStencilStateBuilder::build(const DepthStencilParams& args, Error* error) const noexcept
{
    ID3D10DepthStencilState* d3dDepthStencilState;
    if(!processArgs(args, &d3dDepthStencilState, error))
    { return null; }

    DX10DepthStencilState* const ret = new(::std::nothrow) DX10DepthStencilState(args, d3dDepthStencilState);

    if(!ret)
    {
        d3dDepthStencilState->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, ret);
}

DX10DepthStencilState* DX10DepthStencilStateBuilder::build(const DepthStencilParams& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D10DepthStencilState* d3dDepthStencilState;
    if(!processArgs(args, &d3dDepthStencilState, error))
    { return null; }

    DX10DepthStencilState* const ret = allocator.allocateT<DX10DepthStencilState>(args, d3dDepthStencilState);

    if(!ret)
    {
        d3dDepthStencilState->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, ret);
}

CPPRef<IDepthStencilState> DX10DepthStencilStateBuilder::buildCPPRef(const DepthStencilParams& args, Error* error) const noexcept
{
    ID3D10DepthStencilState* d3dDepthStencilState;
    if(!processArgs(args, &d3dDepthStencilState, error))
    { return null; }

    const CPPRef<DX10DepthStencilState> ret = CPPRef<DX10DepthStencilState>(new(::std::nothrow) DX10DepthStencilState(args, d3dDepthStencilState));

    if(!ret)
    {
        d3dDepthStencilState->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, ret);
}

NullableRef<IDepthStencilState> DX10DepthStencilStateBuilder::buildTauRef(const DepthStencilParams& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D10DepthStencilState* d3dDepthStencilState;
    if(!processArgs(args, &d3dDepthStencilState, error))
    { return null; }

    const NullableRef<DX10DepthStencilState> ret(allocator, args, d3dDepthStencilState);

    if(!ret)
    {
        d3dDepthStencilState->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RefCast<IDepthStencilState>(ret));
}

NullableStrongRef<IDepthStencilState> DX10DepthStencilStateBuilder::buildTauSRef(const DepthStencilParams& args, Error* error, TauAllocator& allocator) const noexcept
{
    ID3D10DepthStencilState* d3dDepthStencilState;
    if(!processArgs(args, &d3dDepthStencilState, error))
    { return null; }

    const NullableStrongRef<DX10DepthStencilState> ret(allocator, args, d3dDepthStencilState);

    if(!ret)
    {
        d3dDepthStencilState->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RefCast<IDepthStencilState>(ret));
}

bool DX10DepthStencilStateBuilder::processArgs(const DepthStencilParams& args, ID3D10DepthStencilState** const d3dDepthStencilState, Error* const error) const noexcept
{
    D3D10_DEPTH_STENCIL_DESC depthStencilDesc;
    depthStencilDesc.DepthEnable = args.enableDepthTest;
    depthStencilDesc.DepthWriteMask = dxDepthWriteMask(args.depthWriteMask);
    depthStencilDesc.DepthFunc = dxComparisonFunc(args.depthCompareFunc);

    depthStencilDesc.StencilEnable = args.enableStencilTest;
    depthStencilDesc.StencilReadMask = args.stencilReadMask;
    depthStencilDesc.StencilWriteMask = args.stencilWriteMask;

    depthStencilDesc.FrontFace.StencilFailOp = dxStencilOp(args.frontFace.failOp);
    depthStencilDesc.FrontFace.StencilDepthFailOp = dxStencilOp(args.frontFace.stencilPassDepthFailOp);
    depthStencilDesc.FrontFace.StencilPassOp = dxStencilOp(args.frontFace.passOp);
    depthStencilDesc.FrontFace.StencilFunc = dxComparisonFunc(args.frontFace.compareFunc);

    depthStencilDesc.BackFace.StencilFailOp = dxStencilOp(args.backFace.failOp);
    depthStencilDesc.BackFace.StencilDepthFailOp = dxStencilOp(args.backFace.stencilPassDepthFailOp);
    depthStencilDesc.BackFace.StencilPassOp = dxStencilOp(args.backFace.passOp);
    depthStencilDesc.BackFace.StencilFunc = dxComparisonFunc(args.backFace.compareFunc);

    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.DepthWriteMask) == IntMaxMin<u32>::Max(), Error::InvalidDepthWriteMask);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.DepthFunc) == IntMaxMin<u32>::Max(), Error::InvalidDepthCompareFunc);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.FrontFace.StencilFailOp) == IntMaxMin<u32>::Max(), Error::InvalidFrontFaceStencilFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.FrontFace.StencilDepthFailOp) == IntMaxMin<u32>::Max(), Error::InvalidFrontFaceStencilPassDepthFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.FrontFace.StencilPassOp) == IntMaxMin<u32>::Max(), Error::InvalidFrontFaceStencilPassOp);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.FrontFace.StencilFunc) == IntMaxMin<u32>::Max(), Error::InvalidFrontFaceStencilCompareFunc);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.BackFace.StencilFailOp) == IntMaxMin<u32>::Max(), Error::InvalidBackFaceStencilFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.BackFace.StencilDepthFailOp) == IntMaxMin<u32>::Max(), Error::InvalidBackFaceStencilPassDepthFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.BackFace.StencilPassOp) == IntMaxMin<u32>::Max(), Error::InvalidBackFaceStencilPassOp);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.BackFace.StencilFunc) == IntMaxMin<u32>::Max(), Error::InvalidBackFaceStencilCompareFunc);

    const HRESULT res = ctx.d3d10Device()->CreateDepthStencilState(&depthStencilDesc, d3dDepthStencilState);

    ERROR_CODE_COND_F(FAILED(res), Error::SystemMemoryAllocationFailure);

    return true;
}

D3D10_DEPTH_WRITE_MASK DX10DepthStencilStateBuilder::dxDepthWriteMask(const DepthStencilParams::DepthWriteMask depthWriteMask) noexcept
{
    switch(depthWriteMask)
    {
        case DepthStencilParams::DepthWriteMask::Zero: return D3D10_DEPTH_WRITE_MASK_ZERO;
        case DepthStencilParams::DepthWriteMask::All:  return D3D10_DEPTH_WRITE_MASK_ALL;
        default:                                       return static_cast<D3D10_DEPTH_WRITE_MASK>(IntMaxMin<u32>::Max());
    }
}

D3D10_COMPARISON_FUNC DX10DepthStencilStateBuilder::dxComparisonFunc(const DepthStencilParams::CompareFunc compareFunc) noexcept
{
    switch(compareFunc)
    {
        case DepthStencilParams::CompareFunc::LessThanOrEqual:     return D3D10_COMPARISON_LESS_EQUAL;
        case DepthStencilParams::CompareFunc::GreaterThanOrEqual:  return D3D10_COMPARISON_GREATER_EQUAL;
        case DepthStencilParams::CompareFunc::LessThan:            return D3D10_COMPARISON_LESS;
        case DepthStencilParams::CompareFunc::GreaterThan:         return D3D10_COMPARISON_GREATER;
        case DepthStencilParams::CompareFunc::Equal:               return D3D10_COMPARISON_EQUAL;
        case DepthStencilParams::CompareFunc::NotEqual:            return D3D10_COMPARISON_NOT_EQUAL;
        case DepthStencilParams::CompareFunc::Always:              return D3D10_COMPARISON_ALWAYS;
        case DepthStencilParams::CompareFunc::Never:               return D3D10_COMPARISON_NEVER;
        default:                                                   return static_cast<D3D10_COMPARISON_FUNC>(IntMaxMin<u32>::Max());
    }
}

D3D10_STENCIL_OP DX10DepthStencilStateBuilder::dxStencilOp(const DepthStencilParams::StencilOp stencilOp) noexcept
{
    switch(stencilOp)
    {
        case DepthStencilParams::StencilOp::Keep:           return D3D10_STENCIL_OP_KEEP;
        case DepthStencilParams::StencilOp::Zero:           return D3D10_STENCIL_OP_ZERO;
        case DepthStencilParams::StencilOp::Replace:        return D3D10_STENCIL_OP_REPLACE;
        case DepthStencilParams::StencilOp::Invert:         return D3D10_STENCIL_OP_INVERT;
        case DepthStencilParams::StencilOp::IncrementClamp: return D3D10_STENCIL_OP_INCR_SAT;
        case DepthStencilParams::StencilOp::DecrementClamp: return D3D10_STENCIL_OP_DECR_SAT;
        case DepthStencilParams::StencilOp::IncrementWrap:  return D3D10_STENCIL_OP_INCR;
        case DepthStencilParams::StencilOp::DecrementWrap:  return D3D10_STENCIL_OP_DECR;
        default:                                            return static_cast<D3D10_STENCIL_OP>(IntMaxMin<u32>::Max());
    }
}
#endif
