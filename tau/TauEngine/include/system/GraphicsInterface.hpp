#pragma once

#include <Objects.hpp>
#include <DynArray.hpp>

#include "DLL.hpp"
#include "GraphicsAccelerator.hpp"
#include "RenderingMode.hpp"

class Window;
class IRenderingContext;
class IRenderingContextBuilder;
class IDepthStencilStateBuilder;
class IRasterizerStateBuilder;
class ITextureBuilder;
class ISingleTextureUploaderBuilder;
class ITextureUploaderBuilder;
class IShaderBuilder;
class IVertexArrayBuilder;
class IBufferBuilder;
class IIndexBufferBuilder;
class IUniformBufferBuilder;
class ITextureSamplerBuilder;
class IFrameBufferBuilder;

class TAU_DLL TAU_NOVTABLE IGraphicsInterface
{
    DEFAULT_DESTRUCT_VI(IGraphicsInterface);
    DELETE_COPY(IGraphicsInterface);
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
    [[nodiscard]] virtual IVertexArrayBuilder& createVertexArray() noexcept = 0;
    [[nodiscard]] virtual IBufferBuilder& createBuffer() noexcept = 0;
    [[nodiscard]] virtual IIndexBufferBuilder& createIndexBuffer() noexcept = 0;
    [[nodiscard]] virtual IUniformBufferBuilder& createUniformBuffer() noexcept = 0;
    [[nodiscard]] virtual IDepthStencilStateBuilder& createDepthStencilState() noexcept = 0;
    [[nodiscard]] virtual IRasterizerStateBuilder& createRasterizerState() noexcept = 0;
    [[nodiscard]] virtual ITextureBuilder& createTexture() noexcept = 0;
    [[nodiscard]] virtual ITextureSamplerBuilder& createTextureSampler() noexcept = 0;
    [[nodiscard]] virtual ISingleTextureUploaderBuilder& createSingleTextureUploader() noexcept = 0;
    [[nodiscard]] virtual ITextureUploaderBuilder& createTextureUploader() noexcept = 0;
    [[nodiscard]] virtual IFrameBufferBuilder& createFrameBuffer() noexcept = 0;
    [[nodiscard]] virtual IRenderingContextBuilder& createRenderingContext() noexcept = 0;
};
