#include "dx/dx11/DX11RasterizerState.hpp"

#ifdef _WIN32
#include "dx/dx11/DX11GraphicsInterface.hpp"
#include "dx/dx11/DX11RenderingContext.hpp"

void DX11RasterizerState::apply(DX11RenderingContext& ctx) const noexcept
{
    ctx.d3d11DeviceContext()->RSSetState(_d3dRasterizerState);
}

DX11RasterizerState* DX11RasterizerStateBuilder::build(const RasterizerArgs& args, Error* const error) const noexcept
{
    DXRasterizerArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX11RasterizerState* const state = new(::std::nothrow) DX11RasterizerState(args, dxArgs.state);
    ERROR_CODE_COND_N(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, state);
}

DX11RasterizerState* DX11RasterizerStateBuilder::build(const RasterizerArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXRasterizerArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX11RasterizerState* const state = allocator.allocateT<DX11RasterizerState>(args, dxArgs.state);
    ERROR_CODE_COND_N(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, state);
}

CPPRef<IRasterizerState> DX11RasterizerStateBuilder::buildCPPRef(const RasterizerArgs& args, Error* const error) const noexcept
{
    DXRasterizerArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX11RasterizerState> state = CPPRef<DX11RasterizerState>(new(::std::nothrow) DX11RasterizerState(args, dxArgs.state));
    ERROR_CODE_COND_N(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, state);
}

NullableRef<IRasterizerState> DX11RasterizerStateBuilder::buildTauRef(const RasterizerArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXRasterizerArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX11RasterizerState> state(allocator, args, dxArgs.state);
    ERROR_CODE_COND_N(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, RefCast<IRasterizerState>(state));
}

NullableStrongRef<IRasterizerState> DX11RasterizerStateBuilder::buildTauSRef(const RasterizerArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    DXRasterizerArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX11RasterizerState> state(allocator, args, dxArgs.state);
    ERROR_CODE_COND_N(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, RefCast<IRasterizerState>(state));
}

bool DX11RasterizerStateBuilder::processArgs(const RasterizerArgs& args, DXRasterizerArgs* const dxArgs, Error* const error) const noexcept
{
    dxArgs->desc.ScissorEnable = args.enableScissorTest;
    dxArgs->desc.FrontCounterClockwise = args.frontFaceCounterClockwise;
    dxArgs->desc.CullMode = dxCullMode(args.cullMode);
    dxArgs->desc.FillMode = dxFillMode(args.fillMode);

    dxArgs->desc.DepthBias = static_cast<INT>(args.depthBias);
    dxArgs->desc.DepthBiasClamp = args.depthBiasClamp;
    dxArgs->desc.SlopeScaledDepthBias = args.slopeScaledDepthBias;
    dxArgs->desc.DepthClipEnable = true;
    dxArgs->desc.MultisampleEnable = false;
    dxArgs->desc.AntialiasedLineEnable = false;

    ERROR_CODE_COND_F(dxArgs->desc.CullMode == 0, Error::InvalidCullMode);
    ERROR_CODE_COND_F(dxArgs->desc.FillMode == 0, Error::InvalidFillMode);

    _gi.d3d11Device()->CreateRasterizerState(&dxArgs->desc, &dxArgs->state);

    ERROR_CODE_COND_F(!dxArgs->state, Error::DriverMemoryAllocationFailure);

    return true;
}

D3D11_CULL_MODE DX11RasterizerStateBuilder::dxCullMode(const RasterizerArgs::CullMode cullMode) noexcept
{
    switch(cullMode)
    {
        case RasterizerArgs::CullMode::None:  return D3D11_CULL_NONE;
        case RasterizerArgs::CullMode::Front: return D3D11_CULL_FRONT;
        case RasterizerArgs::CullMode::Back:  return D3D11_CULL_BACK;
        default: return static_cast<D3D11_CULL_MODE>(0);
    }
}

D3D11_FILL_MODE DX11RasterizerStateBuilder::dxFillMode(const RasterizerArgs::FillMode fillMode) noexcept
{
    switch(fillMode)
    {
        case RasterizerArgs::FillMode::Vertices:
        case RasterizerArgs::FillMode::Wireframe: return D3D11_FILL_WIREFRAME;
        case RasterizerArgs::FillMode::Filled:    return D3D11_FILL_SOLID;
        default: return static_cast<D3D11_FILL_MODE>(0);
    }
}
#endif
