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

    DX10BufferBuilder* _bufferBuilder;
    DX10IndexBufferBuilder* _indexBufferBuilder;
    DX10UniformBufferBuilder* _uniformBufferBuilder;
    DX10TextureSamplerBuilder* _textureSamplerBuilder;
public:
    DX10RenderingContext(const RenderingMode& mode) noexcept;

    ~DX10RenderingContext() noexcept override;

    [[nodiscard]] const ID3D10Device* d3d10Device() const noexcept { return _d3d10Device; }
    [[nodiscard]] ID3D10Device* d3d10Device() noexcept { return _d3d10Device; }

    [[nodiscard]] bool createContext(Window& window) noexcept override;
    void deactivateContext() noexcept override { }
    void activateContext() noexcept override { }
    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) noexcept override;
    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, UINT8 stencilValue) noexcept override;
    void setVSync(bool vsync) noexcept override;
    void setFaceWinding(bool clockwise) noexcept override;
    void enableDepthWriting(bool writing) noexcept override;
    void beginFrame() noexcept override;
    void endFrame() noexcept override;
    void swapFrame() noexcept override;

    [[nodiscard]] Ref<IVertexArrayBuilder> createVertexArray(uSys bufferCount) noexcept override;
    // [[nodiscard]] Ref<IBufferBuilder> createBuffer(uSys descriptorCount) noexcept;
    [[nodiscard]] IBufferBuilder& createBuffer() noexcept override;
    [[nodiscard]] IIndexBufferBuilder& createIndexBuffer() noexcept override;
    [[nodiscard]] IUniformBufferBuilder& createUniformBuffer() noexcept override;
    [[nodiscard]] Ref<IFrameBufferBuilder> createFrameBuffer() noexcept override { return null; }
    [[nodiscard]] Ref<ITextureBuilder> createTexture2D() noexcept override;
    [[nodiscard]] Ref<ITextureBuilder> createNullTexture() noexcept override { return null; }
    [[nodiscard]] Ref<ITextureBuilder> createTextureDepth() noexcept override { return null; }
    [[nodiscard]] Ref<ITextureCubeBuilder> createTextureCube() noexcept override { return null; }
    [[nodiscard]] ITextureSamplerBuilder& createTextureSampler() noexcept override;
    [[nodiscard]] Ref<ITextureUploaderBuilder> createTextureUploader(uSys textureCount) noexcept override;
    [[nodiscard]] Ref<ISingleTextureUploaderBuilder> createSingleTextureUploader() noexcept override;
    [[nodiscard]] Ref<IShaderBuilder> createShader() noexcept override;
protected:
    RC_IMPL(DX10RenderingContext);
};

#endif
