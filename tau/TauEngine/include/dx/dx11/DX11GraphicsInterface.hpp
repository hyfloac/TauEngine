#pragma once

#include "shader/bundle/ShaderInfoExtractorVisitor.hpp"
#include "system/GraphicsInterface.hpp"

#ifdef _WIN32
#include <d3d11.h>

class DX11ShaderBuilder;
class DX11InputLayoutBuilder;
class DX11VertexArrayBuilder;
class DX11BufferBuilder;
class DX11DepthStencilStateBuilder;
class DX11RasterizerStateBuilder;
class DX11BlendingStateBuilder;
class DX11TextureBuilder;
class DX11TextureSamplerBuilder;
class DX11SingleTextureUploaderBuilder;
class DX11TextureUploaderBuilder;
class DX11FrameBufferBuilder;
class DX11RenderingContextBuilder;

class TAU_DLL DX11GraphicsInterface final : public IGraphicsInterface
{
    DELETE_CM(DX11GraphicsInterface);
private:
    ID3D11Device* _d3d11Device;

    ShaderInfoExtractorVisitor _shaderInfoExtractor;

    DX11ShaderBuilder* _shaderBuilder;
    DX11InputLayoutBuilder* _inputLayoutBuilder;
    DX11VertexArrayBuilder* _vertexArrayBuilder;
    DX11BufferBuilder* _bufferBuilder;
    DX11DepthStencilStateBuilder* _depthStencilStateBuilder;
    DX11RasterizerStateBuilder* _rasterizerStateBuilder;
    DX11BlendingStateBuilder* _blendingStateBuilder;
    DX11TextureBuilder* _textureBuilder;
    DX11TextureSamplerBuilder* _textureSamplerBuilder;
    DX11TextureUploaderBuilder* _textureUploaderBuilder;
    DX11FrameBufferBuilder* _frameBufferBuilder;
    DX11RenderingContextBuilder* _renderingContextBuilder;
public:
    DX11GraphicsInterface(const RenderingMode& mode, ID3D11Device* d3dDevice) noexcept;
    ~DX11GraphicsInterface() noexcept;

    [[nodiscard]] const ID3D11Device* d3d11Device() const noexcept { return _d3d11Device; }
    [[nodiscard]] ID3D11Device* d3d11Device() noexcept { return _d3d11Device; }

    [[nodiscard]] RefDynArray<NullableRef<IGraphicsAccelerator>> graphicsAccelerators() noexcept override;

    [[nodiscard]] IShaderBuilder& createShader() noexcept override;
    [[nodiscard]] IInputLayoutBuilder& createInputLayout() noexcept override;
    [[nodiscard]] IVertexArrayBuilder& createVertexArray() noexcept override;
    [[nodiscard]] IBufferBuilder& createBuffer() noexcept override;
    [[nodiscard]] IDepthStencilStateBuilder& createDepthStencilState() noexcept override;
    [[nodiscard]] IRasterizerStateBuilder& createRasterizerState() noexcept override;
    [[nodiscard]] IBlendingStateBuilder& createBlendingState() noexcept override;
    [[nodiscard]] ITextureBuilder& createTexture() noexcept override;
    [[nodiscard]] ITextureSamplerBuilder& createTextureSampler() noexcept override;
    [[nodiscard]] ITextureUploaderBuilder& createTextureUploader() noexcept override;
    [[nodiscard]] IFrameBufferBuilder& createFrameBuffer() noexcept override;
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
    [[nodiscard]] static NullableRef<DX11GraphicsInterface> build(const GraphicsInterfaceArgs& args, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept;
};

#endif
