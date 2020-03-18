#pragma once

#include "system/RenderingContext.hpp"

#ifdef _WIN32
#pragma warning(push, 0)
#include <d3d11.h>
#pragma warning(pop)

#include "DLL.hpp"

class DX11BufferBuilder;
class DX11IndexBufferBuilder;
class DX11UniformBufferBuilder;
class DX11TextureSamplerBuilder;
class DX11ShaderBuilder;
class DX11Texture2DBuilder;
class DX11NullTextureBuilder;
class DX11TextureCubeBuilder;
class DX11DepthStencilState;
class DX11DepthStencilStateBuilder;

class TAU_DLL DX11RenderingContext final : public IRenderingContext
{
private:
    ID3D11Device* _d3d11Device;
    ID3D11DeviceContext* _d3d11DeviceContext;
    ID3D11RenderTargetView* _renderTargetView;
    ID3D11Texture2D* _depthStencilBuffer;
    ID3D11DepthStencilState* _depthStencilState;
    ID3D11DepthStencilView* _depthStencilView;
    ID3D11RasterizerState* _rasterizerState;
    IDXGISwapChain* _swapChain;

    bool _vsync;
    NullableRef<DX11DepthStencilState> _defaultDepthStencilState;
    NullableRef<DX11DepthStencilState> _currentDepthStencilState;

    DX11BufferBuilder* _bufferBuilder;
    DX11IndexBufferBuilder* _indexBufferBuilder;
    DX11UniformBufferBuilder* _uniformBufferBuilder;
    // DX11TextureSamplerBuilder* _textureSamplerBuilder;
    DX11ShaderBuilder* _shaderBuilder;
    // DX11Texture2DBuilder* _texture2DBuilder;
    // DX11NullTextureBuilder* _textureNullBuilder;
    // DX11TextureCubeBuilder* _textureCubeBuilder;
    DX11DepthStencilStateBuilder* _depthStencilStateBuilder;
public:
    DX11RenderingContext(const RenderingMode& mode) noexcept;

    ~DX11RenderingContext() noexcept override;

    [[nodiscard]] const ID3D11Device* d3d11Device() const noexcept { return _d3d11Device; }
    [[nodiscard]] ID3D11Device* d3d11Device() noexcept { return _d3d11Device; }

    [[nodiscard]] const ID3D11DeviceContext* d3d11DeviceContext() const noexcept { return _d3d11DeviceContext; }
    [[nodiscard]] ID3D11DeviceContext* d3d11DeviceContext() noexcept { return _d3d11DeviceContext; }

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
    const DepthStencilArgs& getDefaultDepthStencilStateParams() noexcept override;

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
    RC_IMPL(DX11RenderingContext);
};

#endif
