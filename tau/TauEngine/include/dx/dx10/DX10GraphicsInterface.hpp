#pragma once

#include "shader/bundle/ShaderInfoExtractorVisitor.hpp"
#include "system/GraphicsInterface.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10ShaderBuilder;
class DX10ShaderProgramBuilder;
class DX10ResourceBuilder;
class DX10InputLayoutBuilder;
class DX10VertexArrayBuilder;
class DX10BufferBuilder;
class DX10DepthStencilStateBuilder;
class DX10RasterizerStateBuilder;
class DX10BlendingStateBuilder;
class DX10TextureBuilder;
class DX10TextureSamplerBuilder;
class DX10SingleTextureUploaderBuilder;
class DX10TextureUploaderBuilder;
class DX10FrameBufferBuilder;
class DX10RenderingContextBuilder;

class TAU_DLL DX10GraphicsInterface final : public IGraphicsInterface
{
    DELETE_CM(DX10GraphicsInterface);
private:
    ID3D10Device* _d3d10Device;
    IDXGIAdapter1* _dxgiAdapter;

    ShaderInfoExtractorVisitor _shaderInfoExtractor;

    DX10ShaderBuilder* _shaderBuilder;
    DX10ShaderProgramBuilder* _shaderProgramBuilder;
    DX10ResourceBuilder* _resourceBuilder;
    DX10InputLayoutBuilder* _inputLayoutBuilder;
    DX10VertexArrayBuilder* _vertexArrayBuilder;
    DX10BufferBuilder* _bufferBuilder;
    DX10DepthStencilStateBuilder* _depthStencilStateBuilder;
    DX10RasterizerStateBuilder* _rasterizerStateBuilder;
    DX10BlendingStateBuilder* _blendingStateBuilder;
    DX10TextureSamplerBuilder* _textureSamplerBuilder;
    DX10FrameBufferBuilder* _frameBufferBuilder;
    DX10RenderingContextBuilder* _renderingContextBuilder;
public:
    DX10GraphicsInterface(const RenderingMode& mode, ID3D10Device* d3dDevice, IDXGIAdapter1* dxgiAdapter) noexcept;
    ~DX10GraphicsInterface() noexcept override;

    [[nodiscard]] ID3D10Device*  d3d10Device() const noexcept { return _d3d10Device; }
    [[nodiscard]] IDXGIAdapter1* dxgiAdapter() const noexcept { return _dxgiAdapter; }

    [[nodiscard]] IShaderBuilder& createShader() noexcept override;
    [[nodiscard]] IShaderProgramBuilder& createShaderProgram() noexcept override;
    [[nodiscard]] IResourceBuilder& createResource() noexcept override;
    [[nodiscard]] IInputLayoutBuilder& createInputLayout() noexcept override;
    [[nodiscard]] IVertexArrayBuilder& createVertexArray() noexcept override;
    [[nodiscard]] IDepthStencilStateBuilder& createDepthStencilState() noexcept override;
    [[nodiscard]] IRasterizerStateBuilder& createRasterizerState() noexcept override;
    [[nodiscard]] IBlendingStateBuilder& createBlendingState() noexcept override;
    [[nodiscard]] ITextureSamplerBuilder& createTextureSampler() noexcept override;
    [[nodiscard]] IFrameBufferBuilder& createFrameBuffer() noexcept override;
    [[nodiscard]] IRenderingContextBuilder& createRenderingContext() noexcept override;
};

struct DX10GraphicsInterfaceArgs final
{
    RenderingMode mode;
};

class TAU_DLL DX10GraphicsInterfaceBuilder final
{
    DELETE_CONSTRUCT(DX10GraphicsInterfaceBuilder);
    DELETE_DESTRUCT(DX10GraphicsInterfaceBuilder);
    DELETE_CM(DX10GraphicsInterfaceBuilder);
public:
    [[nodiscard]] static NullableRef<DX10GraphicsInterface> build(const GraphicsInterfaceArgs& args, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept;
};

#endif
