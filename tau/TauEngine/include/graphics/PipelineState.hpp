#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include <NumTypes.hpp>

#include "DLL.hpp"
#include "_GraphicsOpaqueObjects.hpp"

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

    NullableRef<IShaderProgram> shaderProgram;
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
    [[nodiscard]] _T* get() noexcept
    { return reinterpret_cast<_T*>(raw); }

    template<typename _T>
    [[nodiscard]] const _T* get() const noexcept
    { return reinterpret_cast<_T*>(raw); }

    [[nodiscard]] const PipelineArgs& args() const noexcept { return _args; }
};
