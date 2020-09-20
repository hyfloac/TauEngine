#include "dx/dx12/DX12RasterizerState.hpp"

#ifdef _WIN32
NullableRef<IRasterizerState> DX12RasterizerStateBuilder::buildTauRef(const RasterizerArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    D3D12_RASTERIZER_DESC rasterizerDesc;
    if(!processArgs(args, &rasterizerDesc, error))
    { return null; }

    const NullableRef<DX12RasterizerState> rasterizerState(allocator, args, rasterizerDesc);
    ERROR_CODE_COND_N(!rasterizerState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, rasterizerState);
}

static D3D12_CULL_MODE dxCullMode(RasterizerArgs::CullMode cullMode) noexcept;
static D3D12_FILL_MODE dxFillMode(RasterizerArgs::FillMode fillMode) noexcept;

bool DX12RasterizerStateBuilder::processArgs(const RasterizerArgs& args, D3D12_RASTERIZER_DESC* rasterizerDesc, Error* error) const noexcept
{
    rasterizerDesc->CullMode = dxCullMode(args.cullMode);
    rasterizerDesc->FillMode = dxFillMode(args.fillMode);
    rasterizerDesc->FrontCounterClockwise = args.frontFaceCounterClockwise;
    rasterizerDesc->DepthBias = static_cast<INT>(args.depthBias);
    rasterizerDesc->DepthBiasClamp = args.depthBiasClamp;
    rasterizerDesc->SlopeScaledDepthBias = args.slopeScaledDepthBias;
    rasterizerDesc->DepthClipEnable = true;
    rasterizerDesc->MultisampleEnable = false;
    rasterizerDesc->AntialiasedLineEnable = false;
    rasterizerDesc->ForcedSampleCount = 0;
    rasterizerDesc->ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

    ERROR_CODE_COND_F(rasterizerDesc->CullMode == 0, Error::InvalidCullMode);
    ERROR_CODE_COND_F(rasterizerDesc->FillMode == 0, Error::InvalidFillMode);

    return true;
}

static D3D12_CULL_MODE dxCullMode(const RasterizerArgs::CullMode cullMode) noexcept
{
    switch(cullMode)
    {
        case RasterizerArgs::CullMode::None:  return D3D12_CULL_MODE_NONE;
        case RasterizerArgs::CullMode::Front: return D3D12_CULL_MODE_FRONT;
        case RasterizerArgs::CullMode::Back:  return D3D12_CULL_MODE_BACK;
        default: return static_cast<D3D12_CULL_MODE>(0);
    }
}

static D3D12_FILL_MODE dxFillMode(const RasterizerArgs::FillMode fillMode) noexcept
{
    switch(fillMode)
    {
        case RasterizerArgs::FillMode::Vertices:
        case RasterizerArgs::FillMode::Wireframe: return D3D12_FILL_MODE_WIREFRAME;
        case RasterizerArgs::FillMode::Filled:    return D3D12_FILL_MODE_SOLID;
        default: return static_cast<D3D12_FILL_MODE>(0);
    }
}
#endif
