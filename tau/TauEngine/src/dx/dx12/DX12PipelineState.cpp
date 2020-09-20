#include "dx/dx12/DX12PipelineState.hpp"

#ifdef _WIN32
#include "dx/dx12/DX12BlendingState.hpp"
#include "dx/dx12/DX12DepthStencilState.hpp"
#include "dx/dx12/DX12RasterizerState.hpp"
#include "dx/dx12/DX12InputLayout.hpp"
#include "TauConfig.hpp"

PipelineState DX12PipelineStateBuilder::build(const PipelineArgs& args, Error* error) const noexcept
{
#ifdef TAU_NULL_CHECK
    ERROR_CODE_COND_V(!args.blendingState, Error::InvalidBlendingState, PipelineState(null, args));
    ERROR_CODE_COND_V(!args.depthStencilState, Error::InvalidDepthStencilState, PipelineState(null, args));
    ERROR_CODE_COND_V(!args.rasterizerState, Error::InvalidRasterizerState, PipelineState(null, args));
    ERROR_CODE_COND_V(!args.inputLayout, Error::InvalidInputLayout, PipelineState(null, args));
#endif

#ifdef TAU_RTTI_CHECK
    ERROR_CODE_COND_V(!RTT_CHECK(args.blendingState, DX12BlendingState), Error::InvalidBlendingState, PipelineState(null, args));
    ERROR_CODE_COND_V(!RTT_CHECK(args.depthStencilState, DX12DepthStencilState), Error::InvalidDepthStencilState, PipelineState(null, args));
    ERROR_CODE_COND_V(!RTT_CHECK(args.rasterizerState, DX12RasterizerState), Error::InvalidRasterizerState, PipelineState(null, args));
    ERROR_CODE_COND_V(!RTT_CHECK(args.inputLayout, DX12InputLayout), Error::InvalidInputLayout, PipelineState(null, args));
#endif

    NullableRef<DX12InputLayout> inputLayout = RefStaticCast<DX12InputLayout>(args.inputLayout);

    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc;
    desc.pRootSignature = null;
    desc.VS = { null, 0 };
    desc.PS = { null, 0 };
    desc.DS = { null, 0 };
    desc.HS = { null, 0 };
    desc.GS = { null, 0 };
    desc.StreamOutput = { null, 0, null, 0, 0 };
    desc.BlendState = RefStaticCast<DX12BlendingState>(args.blendingState)->blendDesc();
    desc.SampleMask = 0xFFFFFFFF;
    desc.RasterizerState = RefStaticCast<DX12RasterizerState>(args.rasterizerState)->rasterizerDesc();
    desc.DepthStencilState = RefStaticCast<DX12DepthStencilState>(args.depthStencilState)->depthStencilDesc();
    desc.InputLayout = { inputLayout->inputLayout().arr(), static_cast<UINT>(inputLayout->inputLayout().count()) };
    desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
    desc.NumRenderTargets = static_cast<UINT>(args.numRenderTargets);
    desc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
    desc.RTVFormats[1] = DXGI_FORMAT_UNKNOWN;
    desc.RTVFormats[2] = DXGI_FORMAT_UNKNOWN;
    desc.RTVFormats[3] = DXGI_FORMAT_UNKNOWN;
    desc.RTVFormats[4] = DXGI_FORMAT_UNKNOWN;
    desc.RTVFormats[5] = DXGI_FORMAT_UNKNOWN;
    desc.RTVFormats[6] = DXGI_FORMAT_UNKNOWN;
    desc.RTVFormats[7] = DXGI_FORMAT_UNKNOWN;
    desc.DSVFormat = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc = { 1, 0 };
    desc.NodeMask = 0;
    desc.CachedPSO = { null, 0 };
    desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

    ID3D12PipelineState* pipelineState;
    const HRESULT res = _device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipelineState));

    ERROR_CODE_COND_V(FAILED(res) || !pipelineState, Error::DriverMemoryAllocationFailure, PipelineState(null, args));

    ERROR_CODE_V(Error::NoError, PipelineState(pipelineState, args));
}
#endif
