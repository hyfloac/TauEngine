#pragma once

#include "system/RenderingContext.hpp"

#ifdef _WIN32
#pragma warning(push, 0)
#include <d3d9.h>
#pragma warning(pop)

#include "DLL.hpp"

class TAU_DLL DX9RenderingContext final : public IRenderingContext
{
private:
    IDirect3D9* _d3d9;
    IDirect3DDevice9* _d3d9Device;

    IBufferBuilder* _bufferBuilder;
    IIndexBufferBuilder* _indexBufferBuilder;
    IUniformBufferBuilder* _uniformBufferBuilder;
    ITextureSamplerBuilder* _textureSamplerBuilder;
    IShaderBuilder* _shaderBuilder;
public:
    DX9RenderingContext(const RenderingMode& mode) noexcept;
    ~DX9RenderingContext() noexcept override final;

    [[nodiscard]] const IDirect3D9* d3d9() const noexcept { return _d3d9; }
    [[nodiscard]] IDirect3D9* d3d9() noexcept { return _d3d9; }
    [[nodiscard]] const IDirect3DDevice9* d3d9Device() const noexcept { return _d3d9Device; }
    [[nodiscard]] IDirect3DDevice9* d3d9Device() noexcept { return _d3d9Device; }

    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ = 0.0f, float maxZ = 1.0f) noexcept override final;

    [[nodiscard]] bool createContext(Window& window) noexcept override final;

    void deactivateContext() noexcept override final { }
    void activateContext() noexcept override final { }

    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, u8 stencilValue = 0) noexcept override final;

    void setVSync(bool vsync) noexcept override final { }

    void setFaceWinding(bool clockwise) noexcept override final { }

    void enableDepthWriting(bool writing) noexcept override final { }

    void beginFrame() noexcept override final;

    void endFrame() noexcept override final;

    void swapFrame() noexcept override final;

    [[nodiscard]] Ref<IVertexArrayBuilder> createVertexArray(std::size_t attribCount) noexcept override { return null; }
    [[nodiscard]] Ref<IBufferBuilder> createBuffer(std::size_t descriptorCount) noexcept;
    [[nodiscard]] IBufferBuilder& createBuffer() noexcept override { return *_bufferBuilder; }
    [[nodiscard]] IIndexBufferBuilder& createIndexBuffer() noexcept override { return *_indexBufferBuilder; }
    [[nodiscard]] IUniformBufferBuilder& createUniformBuffer() noexcept override { return *_uniformBufferBuilder; }
    [[nodiscard]] Ref<IFrameBufferBuilder> createFrameBuffer() noexcept override { return null; }
    [[nodiscard]] Ref<ITextureBuilder> createTexture2D() noexcept override { return null; }
    [[nodiscard]] Ref<ITextureBuilder> createNullTexture() noexcept override { return null; }
    [[nodiscard]] Ref<ITextureBuilder> createTextureDepth() noexcept override { return null; }
    [[nodiscard]] Ref<ITextureCubeBuilder> createTextureCube() noexcept override { return null; }
    [[nodiscard]] ITextureSamplerBuilder& createTextureSampler() noexcept override { return *_textureSamplerBuilder; }
    [[nodiscard]] Ref<ITextureUploaderBuilder> createTextureUploader(uSys textureCount) noexcept override { return null; }
    [[nodiscard]] Ref<ISingleTextureUploaderBuilder> createSingleTextureUploader() noexcept override { return null; }
    [[nodiscard]] IShaderBuilder& createShader() noexcept override { return *_shaderBuilder; }
protected:
    RC_IMPL(DX9RenderingContext);
};
#endif
