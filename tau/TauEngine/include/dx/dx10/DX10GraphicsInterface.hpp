#pragma once

#include "system/GraphicsInterface.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10ShaderBuilder;
class DX10DepthStencilStateBuilder;
class DX10RenderingContextBuilder;

class TAU_DLL DX10GraphicsInterface final : public IGraphicsInterface
{
    DELETE_COPY(DX10GraphicsInterface);
private:
    ID3D10Device* _d3dDevice;

    DX10ShaderBuilder* _shaderBuilder;
    DX10DepthStencilStateBuilder* _depthStencilStateBuilder;
    DX10RenderingContextBuilder* _renderingContextBuilder;
public:
    DX10GraphicsInterface(const RenderingMode& mode, ID3D10Device* d3dDevice) noexcept;
    ~DX10GraphicsInterface() noexcept;

    [[nodiscard]] const ID3D10Device* d3dDevice() const noexcept { return _d3dDevice; }
    [[nodiscard]] ID3D10Device* d3dDevice() noexcept { return _d3dDevice; }

    [[nodiscard]] RefDynArray<NullableRef<IGraphicsAccelerator>> graphicsAccelerators() noexcept override;

    [[nodiscard]] IShaderBuilder& createShader() noexcept override;
    [[nodiscard]] IDepthStencilStateBuilder& createDepthStencilState() noexcept override;
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
