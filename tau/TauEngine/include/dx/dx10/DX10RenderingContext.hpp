#pragma once

#include "system/RenderingContext.hpp"

#ifdef _WIN32
#pragma warning(push, 0)
#include <d3d10.h>
#pragma warning(pop)

#include "DLL.hpp"

class DX10BufferBuilder;
class DX10IndexBufferBuilder;
class DX10UniformBufferBuilder;
class DX10TextureSamplerBuilder;
class DX10ShaderBuilder;
class DX10Texture2DBuilder;
class DX10NullTextureBuilder;
class DX10TextureCubeBuilder;
class DX10DepthStencilState;
class DX10DepthStencilStateBuilder;

class TAU_DLL DX10RenderingContext final : public IRenderingContext
{
private:
    ID3D10Device* _d3d10Device;
    ID3D10RenderTargetView* _renderTargetView;
    ID3D10Texture2D* _depthStencilBuffer;
    ID3D10DepthStencilState* _depthStencilState;
    ID3D10DepthStencilView* _depthStencilView;
    ID3D10RasterizerState* _rasterizerState;
    IDXGISwapChain* _swapChain;

    bool _vsync;
    NullableRef<DX10DepthStencilState> _defaultDepthStencilState;
    NullableRef<DX10DepthStencilState> _currentDepthStencilState;

    DX10BufferBuilder* _bufferBuilder;
    DX10IndexBufferBuilder* _indexBufferBuilder;
    DX10UniformBufferBuilder* _uniformBufferBuilder;
    DX10TextureSamplerBuilder* _textureSamplerBuilder;
    DX10ShaderBuilder* _shaderBuilder;
    DX10Texture2DBuilder* _texture2DBuilder;
    DX10NullTextureBuilder* _textureNullBuilder;
    DX10TextureCubeBuilder* _textureCubeBuilder;
    DX10DepthStencilStateBuilder* _depthStencilStateBuilder;
public:
    DX10RenderingContext(const RenderingMode& mode) noexcept;

    ~DX10RenderingContext() noexcept override;

    [[nodiscard]] const ID3D10Device* d3d10Device() const noexcept { return _d3d10Device; }
    [[nodiscard]] ID3D10Device* d3d10Device() noexcept { return _d3d10Device; }

    [[nodiscard]] bool createContext(Window& window) noexcept override;
    void deactivateContext() noexcept override { }
    void activateContext() noexcept override { }
    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) noexcept override;
    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, u8 stencilValue) noexcept override;
    void setVSync(bool vsync) noexcept override;
    void setFaceWinding(bool clockwise) noexcept override;
    void enableDepthWriting(bool writing) noexcept override;

    NullableRef<IDepthStencilState> setDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept override;
    void setDefaultDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept override;
    void resetDepthStencilState() noexcept override;
    const DepthStencilParams& getDefaultDepthStencilStateParams() noexcept override;

    void beginFrame() noexcept override;
    void endFrame() noexcept override;
    void swapFrame() noexcept override;

    [[nodiscard]] CPPRef<IVertexArrayBuilder> createVertexArray(uSys bufferCount) noexcept override;
    // [[nodiscard]] CPPRef<IBufferBuilder> createBuffer(uSys descriptorCount) noexcept;
    [[nodiscard]] IBufferBuilder& createBuffer() noexcept override;
    [[nodiscard]] IIndexBufferBuilder& createIndexBuffer() noexcept override;
    [[nodiscard]] IUniformBufferBuilder& createUniformBuffer() noexcept override;
    [[nodiscard]] CPPRef<IFrameBufferBuilder> createFrameBuffer() noexcept override { return null; }
    [[nodiscard]] ITextureBuilder& createTexture2D() noexcept override;
    [[nodiscard]] ITextureBuilder& createNullTexture() noexcept override;
    [[nodiscard]] ITextureBuilder& createTextureDepth() noexcept override;
    [[nodiscard]] ITextureCubeBuilder& createTextureCube() noexcept override;
    [[nodiscard]] ITextureSamplerBuilder& createTextureSampler() noexcept override;
    [[nodiscard]] CPPRef<ITextureUploaderBuilder> createTextureUploader(uSys textureCount) noexcept override;
    [[nodiscard]] CPPRef<ISingleTextureUploaderBuilder> createSingleTextureUploader() noexcept override;
    [[nodiscard]] IShaderBuilder& createShader() noexcept override;
    [[nodiscard]] IDepthStencilStateBuilder& createDepthStencilState() noexcept override;
protected:
    RC_IMPL(DX10RenderingContext);
};

#endif
