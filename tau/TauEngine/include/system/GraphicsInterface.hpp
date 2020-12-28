#pragma once

#include <Objects.hpp>
#include <DynArray.hpp>

#include "DLL.hpp"
#include "GraphicsAccelerator.hpp"
#include "RenderingMode.hpp"

class IGraphicsCapabilities;

class IShaderBuilder;
class IShaderProgramBuilder;
class IResourceBuilder;
class IInputLayoutBuilder;
class IVertexArrayBuilder;
class IDepthStencilStateBuilder;
class IRasterizerStateBuilder;
class IBlendingStateBuilder;
class ITextureSamplerBuilder;
class IFrameBufferBuilder;
class IDescriptorHeapBuilder;
class IDescriptorLayoutBuilder;
class ITextureViewBuilder;
class IRenderingContextBuilder;

class TAU_DLL TAU_NOVTABLE IGraphicsInterface
{
    DEFAULT_DESTRUCT_VI(IGraphicsInterface);
    DEFAULT_CM_PO(IGraphicsInterface);
protected:
    RenderingMode _mode;
protected:
    IGraphicsInterface(const RenderingMode& mode) noexcept
        : _mode(mode)
    { }
public:
    [[nodiscard]] const RenderingMode& renderingMode() const noexcept { return _mode; }

    [[nodiscard]] virtual IGraphicsCapabilities& capabilities() noexcept = 0;

    [[nodiscard]] virtual IShaderBuilder& createShader() noexcept = 0;
    [[nodiscard]] virtual IShaderProgramBuilder& createShaderProgram() noexcept = 0;
    [[nodiscard]] virtual IResourceBuilder& createResource() noexcept = 0;
    [[nodiscard]] virtual IInputLayoutBuilder& createInputLayout() noexcept = 0;
    [[nodiscard]] virtual IVertexArrayBuilder& createVertexArray() noexcept = 0;
    [[nodiscard]] virtual IDepthStencilStateBuilder& createDepthStencilState() noexcept = 0;
    [[nodiscard]] virtual IRasterizerStateBuilder& createRasterizerState() noexcept = 0;
    [[nodiscard]] virtual IBlendingStateBuilder& createBlendingState() noexcept = 0;
    [[nodiscard]] virtual ITextureSamplerBuilder& createTextureSampler() noexcept = 0;
    [[nodiscard]] virtual IFrameBufferBuilder& createFrameBuffer() noexcept = 0;
    [[nodiscard]] virtual IDescriptorHeapBuilder& createDescriptorHeap() noexcept = 0;
    [[nodiscard]] virtual IDescriptorLayoutBuilder& createDescriptorLayout() noexcept = 0;
    [[nodiscard]] virtual ITextureViewBuilder& createTextureView() noexcept = 0;
    [[nodiscard]] virtual IRenderingContextBuilder& createRenderingContext() noexcept = 0;
};

struct GraphicsInterfaceArgs final
{
    RenderingMode renderingMode;
    NullableRef<IGraphicsAccelerator> graphicsAccelerator;
};

class TAU_DLL TAU_NOVTABLE IGraphicsInterfaceBuilder
{
    DEFAULT_CONSTRUCT_PO(IGraphicsInterfaceBuilder);
    DEFAULT_DESTRUCT_VI(IGraphicsInterfaceBuilder);
    DEFAULT_CM_PO(IGraphicsInterfaceBuilder);
public:
    enum Error
    {
        NoError = 0,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        /**
         * Indicates that a chosen rendering API is not supported.
         *
         *   This can be caused by using an unimplemented API, using an
         * API that does not support the current system, or by using an
         * API with a graphics accelerator that does not carry support
         * for that API.
         */
        UnsupportedAPI
    };
public:
    [[nodiscard]] virtual IGraphicsInterface* build(const GraphicsInterfaceArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IGraphicsInterface* build(const GraphicsInterfaceArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IGraphicsInterface> buildTauRef(const GraphicsInterfaceArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
