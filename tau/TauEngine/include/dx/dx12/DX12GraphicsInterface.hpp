#pragma once

#include "system/GraphicsInterface.hpp"

#ifdef _WIN32
#include <d3d12.h>
#include <Safeties.hpp>

class TAU_DLL DX12GraphicsInterface final : public IGraphicsInterface
{
public:
    /**
     *   Always use the latest version of the ID3D12Device. The
     * version only corresponds to the version of Windows 10, not
     * the driver. It's free to update Windows 10 so there is no
     * excuse to not be on the latest version.
     */
    using DX12Device = ID3D12Device6;
private:
    DX12Device* _d3d12Device;


public:
    DX12GraphicsInterface(const RenderingMode& mode, DX12Device* const d3d12Device) noexcept
        : IGraphicsInterface(mode)
        , _d3d12Device(d3d12Device)
    { }

    [[nodiscard]] RefDynArray<NullableRef<IGraphicsAccelerator>> graphicsAccelerators() noexcept override;

    [[nodiscard]] IGraphicsCapabilities& capabilities() noexcept override;

    [[nodiscard]] IShaderBuilder& createShader() noexcept override;
    [[nodiscard]] IShaderProgramBuilder& createShaderProgram() noexcept override;
    [[nodiscard]] IResourceBuilder& createResource() noexcept override;
    [[nodiscard]] IInputLayoutBuilder& createInputLayout() noexcept override;
    [[nodiscard]] IVertexArrayBuilder& createVertexArray() noexcept override;
    [[nodiscard]] IDepthStencilStateBuilder& createDepthStencilState() noexcept override;
    [[nodiscard]] IRasterizerStateBuilder& createRasterizerState() noexcept override;
    [[nodiscard]] IBlendingStateBuilder& createBlendingState() noexcept override;
    [[nodiscard]] ITextureSamplerBuilder& createTextureSampler() noexcept override;
    [[nodiscard]] ITextureUploaderBuilder& createTextureUploader() noexcept override;
    [[nodiscard]] IFrameBufferBuilder& createFrameBuffer() noexcept override;
    [[nodiscard]] IRenderingContextBuilder& createRenderingContext() noexcept override;
};

class TAU_DLL DX12GraphicsInterfaceBuilder final : public IGraphicsInterfaceBuilder
{
    DEFAULT_CONSTRUCT_PU(DX12GraphicsInterfaceBuilder);
    DEFAULT_DESTRUCT(DX12GraphicsInterfaceBuilder);
    DEFAULT_CM_PU(DX12GraphicsInterfaceBuilder);
public:
    struct DXGraphicsInterfaceArgs final
    {
        DX12GraphicsInterface::DX12Device* d3dDevice;
    };
public:
    [[nodiscard]] IGraphicsInterface* build(const GraphicsInterfaceArgs& args, Error* error) const noexcept override;
    [[nodiscard]] IGraphicsInterface* build(const GraphicsInterfaceArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableRef<IGraphicsInterface> buildTauRef(const GraphicsInterfaceArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
public:
    [[nodiscard]] bool processArgs(const GraphicsInterfaceArgs& args, [[tau::out]] DXGraphicsInterfaceArgs* dxArgs, Error* error) const noexcept;
};
#endif
