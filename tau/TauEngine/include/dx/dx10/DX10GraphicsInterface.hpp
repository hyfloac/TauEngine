#pragma once

#include "system/GraphicsInterface.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10ShaderBuilder;
class DX10DepthStencilStateBuilder;
class DX10RasterizerStateBuilder;
class DX10RenderingContextBuilder;

class TAU_DLL DX10GraphicsInterface final : public IGraphicsInterface
{
    DELETE_COPY(DX10GraphicsInterface);
private:
    ID3D10Device* _d3d10Device;

    DX10ShaderBuilder* _shaderBuilder;
    DX10DepthStencilStateBuilder* _depthStencilStateBuilder;
    DX10RasterizerStateBuilder* _rasterizerStateBuilder;
    DX10RenderingContextBuilder* _renderingContextBuilder;
public:
    DX10GraphicsInterface(const RenderingMode& mode, ID3D10Device* d3dDevice) noexcept;
    ~DX10GraphicsInterface() noexcept;

    [[nodiscard]] const ID3D10Device* d3d10Device() const noexcept { return _d3d10Device; }
    [[nodiscard]] ID3D10Device* d3d10Device() noexcept { return _d3d10Device; }

    [[nodiscard]] RefDynArray<NullableRef<IGraphicsAccelerator>> graphicsAccelerators() noexcept override;

    [[nodiscard]] IShaderBuilder& createShader() noexcept override;
    [[nodiscard]] IDepthStencilStateBuilder& createDepthStencilState() noexcept override;
    [[nodiscard]] IRasterizerStateBuilder& createRasterizerState() noexcept override;
    [[nodiscard]] IRenderingContextBuilder& createRenderingContext() noexcept override;
};

struct DX10GraphicsInterfaceArgs final
{
    RenderingMode mode;
};

class DX10GraphicsInterfaceBuilder final
{
    DEFAULT_CONSTRUCT_PU(DX10GraphicsInterfaceBuilder);
    DEFAULT_DESTRUCT(DX10GraphicsInterfaceBuilder);
    DEFAULT_COPY(DX10GraphicsInterfaceBuilder);
public:
    [[nodiscard]] static NullableRef<DX10GraphicsInterface> build(const DX10GraphicsInterfaceArgs& args, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept;
};

#endif
