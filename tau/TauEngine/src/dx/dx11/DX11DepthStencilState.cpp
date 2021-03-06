#include "dx/dx11/DX11DepthStencilState.hpp"

#ifdef _WIN32
#include "dx/dx11/DX11GraphicsInterface.hpp"
#include "dx/dx11/DX11RenderingContext.hpp"

#include "TauEngine.hpp"

void DX11DepthStencilState::apply(DX11RenderingContext& ctx) const noexcept
{
    ctx.d3d11DeviceContext()->OMSetDepthStencilState(_d3dDepthStencilState, 1);
}

DX11DepthStencilState* DX11DepthStencilStateBuilder::build(const DepthStencilArgs& args, Error* const error) const noexcept
{
    ID3D11DepthStencilState* d3dDepthStencilState;
    if(!processArgs(args, &d3dDepthStencilState, error))
    { return null; }

    DX11DepthStencilState* const ret = new(::std::nothrow) DX11DepthStencilState(args, d3dDepthStencilState);

    if (!ret)
    {
        d3dDepthStencilState->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, ret);
}

DX11DepthStencilState* DX11DepthStencilStateBuilder::build(const DepthStencilArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D11DepthStencilState* d3dDepthStencilState;
    if(!processArgs(args, &d3dDepthStencilState, error))
    { return null; }

    DX11DepthStencilState* const ret = allocator.allocateT<DX11DepthStencilState>(args, d3dDepthStencilState);

    if(!ret)
    {
        d3dDepthStencilState->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, ret);
}

CPPRef<IDepthStencilState> DX11DepthStencilStateBuilder::buildCPPRef(const DepthStencilArgs& args, Error* const error) const noexcept
{
    ID3D11DepthStencilState* d3dDepthStencilState;
    if(!processArgs(args, &d3dDepthStencilState, error))
    { return null; }

    const CPPRef<DX11DepthStencilState> ret = CPPRef<DX11DepthStencilState>(new(::std::nothrow) DX11DepthStencilState(args, d3dDepthStencilState));

    if(!ret)
    {
        d3dDepthStencilState->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, ret);
}

NullableRef<IDepthStencilState> DX11DepthStencilStateBuilder::buildTauRef(const DepthStencilArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D11DepthStencilState* d3dDepthStencilState;
    if(!processArgs(args, &d3dDepthStencilState, error))
    { return null; }

    const NullableRef<DX11DepthStencilState> ret(allocator, args, d3dDepthStencilState);

    if(!ret)
    {
        d3dDepthStencilState->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RefCast<IDepthStencilState>(ret));
}

NullableStrongRef<IDepthStencilState> DX11DepthStencilStateBuilder::buildTauSRef(const DepthStencilArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    ID3D11DepthStencilState* d3dDepthStencilState;
    if(!processArgs(args, &d3dDepthStencilState, error))
    { return null; }

    const NullableStrongRef<DX11DepthStencilState> ret(allocator, args, d3dDepthStencilState);

    if(!ret)
    {
        d3dDepthStencilState->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, RefCast<IDepthStencilState>(ret));
}

bool DX11DepthStencilStateBuilder::processArgs(const DepthStencilArgs& args, ID3D11DepthStencilState** const d3dDepthStencilState, Error* const error) const noexcept
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
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

    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.DepthWriteMask) == IntMaxMin<u32>::Max, Error::InvalidDepthWriteMask);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.DepthFunc) == IntMaxMin<u32>::Max, Error::InvalidDepthCompareFunc);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.FrontFace.StencilFailOp) == IntMaxMin<u32>::Max, Error::InvalidFrontFaceStencilFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.FrontFace.StencilDepthFailOp) == IntMaxMin<u32>::Max, Error::InvalidFrontFaceStencilPassDepthFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.FrontFace.StencilPassOp) == IntMaxMin<u32>::Max, Error::InvalidFrontFaceStencilPassOp);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.FrontFace.StencilFunc) == IntMaxMin<u32>::Max, Error::InvalidFrontFaceStencilCompareFunc);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.BackFace.StencilFailOp) == IntMaxMin<u32>::Max, Error::InvalidBackFaceStencilFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.BackFace.StencilDepthFailOp) == IntMaxMin<u32>::Max, Error::InvalidBackFaceStencilPassDepthFailOp);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.BackFace.StencilPassOp) == IntMaxMin<u32>::Max, Error::InvalidBackFaceStencilPassOp);
    ERROR_CODE_COND_F(static_cast<u32>(depthStencilDesc.BackFace.StencilFunc) == IntMaxMin<u32>::Max, Error::InvalidBackFaceStencilCompareFunc);

    const HRESULT res = _gi.d3d11Device()->CreateDepthStencilState(&depthStencilDesc, d3dDepthStencilState);
    ERROR_CODE_COND_F(FAILED(res), Error::DriverMemoryAllocationFailure);

    return true;
}

D3D11_DEPTH_WRITE_MASK DX11DepthStencilStateBuilder::dxDepthWriteMask(const DepthStencilArgs::DepthWriteMask depthWriteMask) noexcept
{
    switch(depthWriteMask)
    {
        case DepthStencilArgs::DepthWriteMask::Zero: return D3D11_DEPTH_WRITE_MASK_ZERO;
        case DepthStencilArgs::DepthWriteMask::All:  return D3D11_DEPTH_WRITE_MASK_ALL;
        default:                                     return static_cast<D3D11_DEPTH_WRITE_MASK>(IntMaxMin<u32>::Max);
    }
}

D3D11_COMPARISON_FUNC DX11DepthStencilStateBuilder::dxComparisonFunc(const DepthStencilArgs::CompareFunc compareFunc) noexcept
{
    switch(compareFunc)
    {
        case DepthStencilArgs::CompareFunc::LessThanOrEqual:    return D3D11_COMPARISON_LESS_EQUAL;
        case DepthStencilArgs::CompareFunc::GreaterThanOrEqual: return D3D11_COMPARISON_GREATER_EQUAL;
        case DepthStencilArgs::CompareFunc::LessThan:           return D3D11_COMPARISON_LESS;
        case DepthStencilArgs::CompareFunc::GreaterThan:        return D3D11_COMPARISON_GREATER;
        case DepthStencilArgs::CompareFunc::Equal:              return D3D11_COMPARISON_EQUAL;
        case DepthStencilArgs::CompareFunc::NotEqual:           return D3D11_COMPARISON_NOT_EQUAL;
        case DepthStencilArgs::CompareFunc::Always:             return D3D11_COMPARISON_ALWAYS;
        case DepthStencilArgs::CompareFunc::Never:              return D3D11_COMPARISON_NEVER;
        default:                                                return static_cast<D3D11_COMPARISON_FUNC>(IntMaxMin<u32>::Max);
    }
}

D3D11_STENCIL_OP DX11DepthStencilStateBuilder::dxStencilOp(const DepthStencilArgs::StencilOp stencilOp) noexcept
{
    switch(stencilOp)
    {
        case DepthStencilArgs::StencilOp::Keep:           return D3D11_STENCIL_OP_KEEP;
        case DepthStencilArgs::StencilOp::Zero:           return D3D11_STENCIL_OP_ZERO;
        case DepthStencilArgs::StencilOp::Replace:        return D3D11_STENCIL_OP_REPLACE;
        case DepthStencilArgs::StencilOp::Invert:         return D3D11_STENCIL_OP_INVERT;
        case DepthStencilArgs::StencilOp::IncrementClamp: return D3D11_STENCIL_OP_INCR_SAT;
        case DepthStencilArgs::StencilOp::DecrementClamp: return D3D11_STENCIL_OP_DECR_SAT;
        case DepthStencilArgs::StencilOp::IncrementWrap:  return D3D11_STENCIL_OP_INCR;
        case DepthStencilArgs::StencilOp::DecrementWrap:  return D3D11_STENCIL_OP_DECR;
        default:                                          return static_cast<D3D11_STENCIL_OP>(IntMaxMin<u32>::Max);
    }
}
#endif
