#pragma once

#include "system/RenderingContext.hpp"
#ifdef _WIN32
#include <GL/glew.h>
#include <GL/wglew.h>
#endif

class GLBufferBuilder;
class GLIndexBufferBuilder;
class GLUniformBufferBuilder;
class GLTextureSamplerBuilder;
class GLShaderBuilder;
class GLTexture2DBuilder;
class GLTextureNullBuilder;
class GLTextureDepthBuilder;
class GLTextureCubeBuilder;
class GLDepthStencilState;
class GLDepthStencilStateBuilder;

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

    NullableRef<GLDepthStencilState> _defaultDepthStencilState;
    NullableRef<GLDepthStencilState> _currentDepthStencilState;

    GLBufferBuilder* _bufferBuilder;
    GLIndexBufferBuilder* _indexBufferBuilder;
    GLUniformBufferBuilder* _uniformBufferBuilder;
    GLTextureSamplerBuilder* _textureSamplerBuilder;
    GLShaderBuilder* _shaderBuilder;
    GLTexture2DBuilder* _texture2DBuilder;
    GLTextureNullBuilder* _textureNullBuilder;
    GLTextureDepthBuilder* _textureDepthBuilder;
    GLTextureCubeBuilder* _textureCubeBuilder;
    GLDepthStencilStateBuilder* _depthStencilStateBuilder;
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

    NullableRef<IDepthStencilState> setDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept override;
    void setDefaultDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept override;
    void resetDepthStencilState() noexcept override;
    const DepthStencilParams& getDefaultDepthStencilStateParams() noexcept override;

    void beginFrame() noexcept override final { }
    void endFrame() noexcept override final { }

    void swapFrame() noexcept override final;

    [[nodiscard]] CPPRef<IVertexArrayBuilder> createVertexArray(uSys bufferCount) noexcept override;
    [[nodiscard]] IBufferBuilder& createBuffer() noexcept override;
    [[nodiscard]] IIndexBufferBuilder& createIndexBuffer() noexcept override;
    [[nodiscard]] IUniformBufferBuilder& createUniformBuffer() noexcept override;
    [[nodiscard]] CPPRef<IFrameBufferBuilder> createFrameBuffer() noexcept override;
    [[nodiscard]] ITextureBuilder& createTexture2D() noexcept override;
    [[nodiscard]] ITextureBuilder& createNullTexture() noexcept override;
    [[nodiscard]] ITextureBuilder& createTextureDepth() noexcept override;
    [[nodiscard]] ITextureCubeBuilder& createTextureCube() noexcept override;
    [[nodiscard]] ITextureSamplerBuilder& createTextureSampler() noexcept override;
    [[nodiscard]] CPPRef<ITextureUploaderBuilder> createTextureUploader(uSys textureCount) noexcept override;
    [[nodiscard]] CPPRef<ISingleTextureUploaderBuilder> createSingleTextureUploader() noexcept override;
    [[nodiscard]] IShaderBuilder& createShader() noexcept override;
    [[nodiscard]] IDepthStencilStateBuilder& createDepthStencilState() noexcept override;
private:
    void handleCtxError(int profileMask) const noexcept;

    RC_IMPL(GLRenderingContext);
};
