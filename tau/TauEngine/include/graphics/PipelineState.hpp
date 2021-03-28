#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include <NumTypes.hpp>
#include <RunTimeType.hpp>

#include "DescriptorLayout.hpp"
#include "_GraphicsOpaqueObjects.hpp"
#include "DLL.hpp"

class IBlendingState;
class IDepthStencilState;
class IRasterizerState;
class IShaderProgram;
class IInputLayout;

struct PipelineArgs final
{
    DEFAULT_CONSTRUCT_PU(PipelineArgs);
    DEFAULT_DESTRUCT(PipelineArgs);
    DEFAULT_CM_PU(PipelineArgs);
public:
    NullableRef<IDescriptorLayout> descriptorLayout;

    NullableRef<IBlendingState> blendingState;
    NullableRef<IDepthStencilState> depthStencilState;
    NullableRef<IRasterizerState> rasterizerState;

    ShaderProgram shaderProgram;
    NullableRef<IInputLayout> inputLayout;

    uSys numRenderTargets;
};

#define PIPELINE_STATE_IMPL(_TYPE) RTT_IMPL(_TYPE, IPipelineState)

class TAU_DLL TAU_NOVTABLE IPipelineState
{
    DEFAULT_DESTRUCT_VI(IPipelineState);
    DEFAULT_CM_PO(IPipelineState);
private:
    PipelineArgs _args;
public:
    IPipelineState(const PipelineArgs& args) noexcept
        : _args(args)
    { }

    [[nodiscard]] const PipelineArgs& args() const noexcept { return _args; }

    RTT_BASE_IMPL(IPipelineState);
    RTT_BASE_CHECK(IPipelineState);
    RTT_BASE_CAST(IPipelineState);
};

class TAU_DLL SimplePipelineState final : public IPipelineState
{
    PIPELINE_STATE_IMPL(SimplePipelineState);
    DEFAULT_DESTRUCT(SimplePipelineState);
    DEFAULT_CM_PU(SimplePipelineState);
public:
    SimplePipelineState(const PipelineArgs& args) noexcept
        : IPipelineState(args)
    { }
};

class TAU_DLL PipelineStateBuilder
{
    DEFAULT_CONSTRUCT_PO(PipelineStateBuilder);
    DEFAULT_DESTRUCT_VI(PipelineStateBuilder);
    DEFAULT_CM_PO(PipelineStateBuilder);
public:
    enum Error
    {
        NoError = 0,
        InvalidBlendingState,
        InvalidDepthStencilState,
        InvalidRasterizerState,
        InvalidShaderProgram,
        InvalidInputLayout,
        DriverMemoryAllocationFailure,
        SystemMemoryAllocationFailure
    };
public:
    [[nodiscard]] virtual NullableRef<IPipelineState> build(const PipelineArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
