#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include <NumTypes.hpp>

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
    DescriptorLayout descriptorLayout;

    NullableRef<IBlendingState> blendingState;
    NullableRef<IDepthStencilState> depthStencilState;
    NullableRef<IRasterizerState> rasterizerState;

    ShaderProgram shaderProgram;
    NullableRef<IInputLayout> inputLayout;

    uSys numRenderTargets;
};

struct PipelineState final
{
    DEFAULT_CONSTRUCT_PU(PipelineState);
    DEFAULT_DESTRUCT(PipelineState);
    DEFAULT_CM_PU(PipelineState);
public:
    void* raw;
    PipelineArgs _args;
public:
    PipelineState(void* const _raw, const PipelineArgs& args) noexcept
        : raw(_raw)
        , _args(args)
    { }

    template<typename _T>
    [[nodiscard]] _T* get() const noexcept
    { return reinterpret_cast<_T*>(raw); }

    [[nodiscard]] const PipelineArgs& args() const noexcept { return _args; }
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
        DriverMemoryAllocationFailure
    };
public:
    [[nodiscard]] virtual PipelineState build(const PipelineArgs& args, [[tau::out]] Error* error) const noexcept = 0;
};
