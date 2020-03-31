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
class IShaderBuilder;

class TAU_DLL NOVTABLE IGraphicsInterface
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
    [[nodiscard]] virtual IDepthStencilStateBuilder& createDepthStencilState() noexcept = 0;
    [[nodiscard]] virtual IRasterizerStateBuilder& createRasterizerState() noexcept = 0;
    [[nodiscard]] virtual ITextureBuilder& createDepthTexture() noexcept = 0;
    [[nodiscard]] virtual IRenderingContextBuilder& createRenderingContext() noexcept = 0;
};
