#pragma once

#include <Objects.hpp>
#include <DynArray.hpp>

#include "DLL.hpp"
#include "GraphicsAccelerator.hpp"
#include "RenderingMode.hpp"

class IShaderBuilder;
class IShaderProgramBuilder;
class IResourceBuilder;
class IInputLayoutBuilder;
class IVertexArrayBuilder;
class IDepthStencilStateBuilder;
class IRasterizerStateBuilder;
class IBlendingStateBuilder;
class ITextureSamplerBuilder;
class ITextureUploaderBuilder;
class IFrameBufferBuilder;
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

    [[nodiscard]] virtual RefDynArray<NullableRef<IGraphicsAccelerator>> graphicsAccelerators() noexcept = 0;

    [[nodiscard]] virtual IShaderBuilder& createShader() noexcept = 0;
    [[nodiscard]] virtual IShaderProgramBuilder& createShaderProgram() noexcept = 0;
    [[nodiscard]] virtual IResourceBuilder& createResource() noexcept = 0;
    [[nodiscard]] virtual IInputLayoutBuilder& createInputLayout() noexcept = 0;
    [[nodiscard]] virtual IVertexArrayBuilder& createVertexArray() noexcept = 0;
    [[nodiscard]] virtual IDepthStencilStateBuilder& createDepthStencilState() noexcept = 0;
    [[nodiscard]] virtual IRasterizerStateBuilder& createRasterizerState() noexcept = 0;
    [[nodiscard]] virtual IBlendingStateBuilder& createBlendingState() noexcept = 0;
    [[nodiscard]] virtual ITextureSamplerBuilder& createTextureSampler() noexcept = 0;
    [[nodiscard]] virtual ITextureUploaderBuilder& createTextureUploader() noexcept = 0;
    [[nodiscard]] virtual IFrameBufferBuilder& createFrameBuffer() noexcept = 0;
    [[nodiscard]] virtual IRenderingContextBuilder& createRenderingContext() noexcept = 0;
};
