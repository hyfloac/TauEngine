#pragma once

#include <unordered_map>
#include <system/RenderingContext.hpp>
#include <GL/glew.h>
#ifdef _WIN32
#include <GL/wglew.h>
#endif
#include "GLBuffer.hpp"

class GLRenderingContext final : public IRenderingContext
{
public:
    enum class GLProfile
    {
        Core = 0,
        Compat,
        Neither
    };
private:
#if defined(_WIN32)
    HDC _device;
    HGLRC _context;
#endif
    int _majorVersion;
    int _minorVersion;
    GLProfile _compat;
    bool _forwardCompatible;

    GLBufferBuilder* _bufferBuilder;
    GLIndexBufferBuilder* _indexBufferBuilder;
    GLUniformBufferBuilder* _uniformBufferBuilder;
public:
    GLRenderingContext(const RenderingMode& mode, int majorVersion, int minorVersion, GLProfile core, bool forwardCompatible) noexcept;
    ~GLRenderingContext() noexcept override final;

    [[nodiscard]] bool createContext(Window& window) noexcept override final;

    void deactivateContext() noexcept override final;
    void activateContext() noexcept override final;

    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ = 0.0f, float maxZ = 1.0f) noexcept override final;

    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, u8 stencilValue = 0) noexcept override final;

    void setVSync(bool vsync) noexcept override final;

    void setFaceWinding(bool clockwise) noexcept override final;

    void enableDepthWriting(bool writing) noexcept override final;

    void beginFrame() noexcept override final { }

    void endFrame() noexcept override final { }

    void swapFrame() noexcept override final;

    [[nodiscard]] Ref<IVertexArrayBuilder> createVertexArray(uSys bufferCount) noexcept override;
    [[nodiscard]] GLBufferBuilder& createBuffer() noexcept override { return *_bufferBuilder; }
    [[nodiscard]] IIndexBufferBuilder& createIndexBuffer() noexcept override { return *_indexBufferBuilder; }
    [[nodiscard]] IUniformBufferBuilder& createUniformBuffer() noexcept override { return *_uniformBufferBuilder; }
    [[nodiscard]] Ref<IFrameBufferBuilder> createFrameBuffer() noexcept override;
    [[nodiscard]] Ref<ITextureBuilder> createTexture2D() noexcept override;
    [[nodiscard]] Ref<ITextureBuilder> createNullTexture() noexcept override;
    [[nodiscard]] Ref<ITextureBuilder> createTextureDepth() noexcept override;
    [[nodiscard]] Ref<ITextureCubeBuilder> createTextureCube() noexcept override;
    [[nodiscard]] Ref<ITextureSamplerBuilder> createTextureSampler() noexcept override;
    [[nodiscard]] Ref<ITextureUploaderBuilder> createTextureUploader(uSys textureCount) noexcept override;
    [[nodiscard]] Ref<ISingleTextureUploaderBuilder> createSingleTextureUploader() noexcept override;
    [[nodiscard]] Ref<IShaderBuilder> createShader() noexcept override;
private:
    void handleCtxError(int profileMask) const noexcept;

    RC_IMPL(GLRenderingContext);
};
