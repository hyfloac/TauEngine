#include "dx/dx10/DX10RasterizerState.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"

void DX10RasterizerState::apply(DX10RenderingContext& ctx) const noexcept
{
    ctx.d3dDevice()->RSSetState(_d3dRasterizerState);
}

D3D10_CULL_MODE DX10RasterizerStateBuilder::dxCullMode(RasterizerArgs::CullMode cullMode) noexcept
{
    switch(cullMode)
    {
        case RasterizerArgs::CullMode::None: return D3D10_CULL_NONE;
        case RasterizerArgs::CullMode::Front: return D3D10_CULL_FRONT;
        case RasterizerArgs::CullMode::Back: return D3D10_CULL_BACK;
        default: return static_cast<D3D10_CULL_MODE>(0);
    }
}

D3D10_FILL_MODE DX10RasterizerStateBuilder::dxFillMode(RasterizerArgs::FillMode fillMode) noexcept
{
    switch(fillMode)
    {
        case RasterizerArgs::FillMode::Vertices:
        case RasterizerArgs::FillMode::Wireframe: return D3D10_FILL_WIREFRAME;
        case RasterizerArgs::FillMode::Filled: return D3D10_FILL_SOLID;
        default: return static_cast<D3D10_FILL_MODE>(0);
    }
}

DX10RasterizerState* DX10RasterizerStateBuilder::build(const RasterizerArgs& args, Error* error) const noexcept
{
    DXRasterizerArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10RasterizerState* const state = new(::std::nothrow) DX10RasterizerState(args, dxArgs.state);
    ERROR_CODE_COND_N(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, state);
}

DX10RasterizerState* DX10RasterizerStateBuilder::build(const RasterizerArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXRasterizerArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10RasterizerState* const state = allocator.allocateT<DX10RasterizerState>(args, dxArgs.state);
    ERROR_CODE_COND_N(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, state);
}

CPPRef<IRasterizerState> DX10RasterizerStateBuilder::buildCPPRef(const RasterizerArgs& args, Error* error) const noexcept
{
    DXRasterizerArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10RasterizerState> state = CPPRef<DX10RasterizerState>(new(::std::nothrow) DX10RasterizerState(args, dxArgs.state));
    ERROR_CODE_COND_N(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, state);
}

NullableRef<IRasterizerState> DX10RasterizerStateBuilder::buildTauRef(const RasterizerArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXRasterizerArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10RasterizerState> state(allocator, args, dxArgs.state);
    ERROR_CODE_COND_N(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, RefCast<IRasterizerState>(state));
}

NullableStrongRef<IRasterizerState> DX10RasterizerStateBuilder::buildTauSRef(const RasterizerArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXRasterizerArgs dxArgs;
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10RasterizerState> state(allocator, args, dxArgs.state);
    ERROR_CODE_COND_N(!state, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, RefCast<IRasterizerState>(state));
}

bool DX10RasterizerStateBuilder::processArgs(const RasterizerArgs& args, DXRasterizerArgs* dxArgs, Error* error) const noexcept
{
    dxArgs->desc.ScissorEnable = args.enableScissorTest;
    dxArgs->desc.FrontCounterClockwise = args.frontFaceCounterClockwise;
    dxArgs->desc.CullMode = dxCullMode(args.cullMode);
    dxArgs->desc.FillMode = dxFillMode(args.fillMode);

    dxArgs->desc.DepthBias = 0.0f;
    dxArgs->desc.DepthBiasClamp = 0.0f;
    dxArgs->desc.SlopeScaledDepthBias = 0.0f;
    dxArgs->desc.DepthClipEnable = true;
    dxArgs->desc.MultisampleEnable = false;
    dxArgs->desc.AntialiasedLineEnable = false;

    ERROR_CODE_COND_F(dxArgs->desc.CullMode == 0, Error::InvalidCullMode);
    ERROR_CODE_COND_F(dxArgs->desc.FillMode == 0, Error::InvalidFillMode);

    _gi.d3d10Device()->CreateRasterizerState(&dxArgs->desc, &dxArgs->state);

    ERROR_CODE_COND_F(!dxArgs->state, Error::DriverMemoryAllocationFailure);

    return true;
}
#endif
