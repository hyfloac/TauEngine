#include "dx/dx10/DX10PipelineState.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10BlendingState.hpp"
#include "dx/dx10/DX10DepthStencilState.hpp"
#include "dx/dx10/DX10RasterizerState.hpp"
#include "dx/dx10/DX10InputLayout.hpp"
#include "TauConfig.hpp"

NullableRef<IPipelineState> DX10PipelineStateBuilder::build(const PipelineArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
#if TAU_NULL_CHECK
    ERROR_CODE_COND_N(!args.blendingState, Error::InvalidBlendingState);
    ERROR_CODE_COND_N(!args.depthStencilState, Error::InvalidDepthStencilState);
    ERROR_CODE_COND_N(!args.rasterizerState, Error::InvalidRasterizerState);
    ERROR_CODE_COND_N(!args.shaderProgram.raw, Error::InvalidShaderProgram);
    ERROR_CODE_COND_N(!args.inputLayout, Error::InvalidInputLayout);
#endif

#if TAU_RTTI_CHECK
    ERROR_CODE_COND_N(rtt_check<DX10BlendingState>(args.blendingState), Error::InvalidBlendingState);
    ERROR_CODE_COND_N(rtt_check<DX10DepthStencilState>(args.depthStencilState), Error::InvalidDepthStencilState);
    ERROR_CODE_COND_N(rtt_check<DX10RasterizerState>(args.rasterizerState), Error::InvalidRasterizerState);
    ERROR_CODE_COND_N(rtt_check<DX10InputLayout>(args.inputLayout), Error::InvalidInputLayout);
#endif

    NullableRef<SimplePipelineState> pipelineState(allocator, args);
    ERROR_CODE_COND_N(!pipelineState, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, ::std::move(pipelineState));
}
#endif
