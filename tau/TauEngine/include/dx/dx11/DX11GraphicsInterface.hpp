#pragma once

#include "system/GraphicsInterface.hpp"

#ifdef _WIN32
#include <d3d11.h>

class DX11ShaderBuilder;
class DX11DepthStencilStateBuilder;
class DX11RasterizerStateBuilder;
class DX11RenderingContextBuilder;

class TAU_DLL DX11GraphicsInterface final : public IGraphicsInterface
{
    DELETE_COPY(DX11GraphicsInterface);
private:
    ID3D11Device* _d3d11Device;

    DX11ShaderBuilder* _shaderBuilder;
    DX11DepthStencilStateBuilder* _depthStencilStateBuilder;
    DX11RasterizerStateBuilder* _rasterizerStateBuilder;
    DX11RenderingContextBuilder* _renderingContextBuilder;
public:
    DX11GraphicsInterface(const RenderingMode& mode, ID3D11Device* d3dDevice) noexcept;
    ~DX11GraphicsInterface() noexcept;

    [[nodiscard]] const ID3D11Device* d3d11Device() const noexcept { return _d3d11Device; }
    [[nodiscard]] ID3D11Device* d3d11Device() noexcept { return _d3d11Device; }

    [[nodiscard]] RefDynArray<NullableRef<IGraphicsAccelerator>> graphicsAccelerators() noexcept override;

    [[nodiscard]] IShaderBuilder& createShader() noexcept override;
    [[nodiscard]] IDepthStencilStateBuilder& createDepthStencilState() noexcept override;
    [[nodiscard]] IRasterizerStateBuilder& createRasterizerState() noexcept override;
    [[nodiscard]] IRenderingContextBuilder& createRenderingContext() noexcept override;
};

struct DX11GraphicsInterfaceArgs final
{
    RenderingMode mode;
};

class DX11GraphicsInterfaceBuilder final
{
    DEFAULT_CONSTRUCT_PU(DX11GraphicsInterfaceBuilder);
    DEFAULT_DESTRUCT(DX11GraphicsInterfaceBuilder);
    DEFAULT_COPY(DX11GraphicsInterfaceBuilder);
public:
    [[nodiscard]] static NullableRef<DX11GraphicsInterface> build(const DX11GraphicsInterfaceArgs& args, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept;
};

#endif
