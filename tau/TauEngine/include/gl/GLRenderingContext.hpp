#pragma once

#include "system/RenderingContext.hpp"
#ifdef _WIN32
#include <GL/glew.h>
#include <GL/wglew.h>
#endif

class GLGraphicsInterface;

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
class GLRasterizerState;
class GLDepthStencilStateBuilder;

struct GLRenderingContextArgs final
{
    GLGraphicsInterface& gi;
};

#if defined(_WIN32)
struct GLSystemRenderingContextArgs final
{
    HDC device;
    HGLRC context;
};
#else
struct GLSystemRenderingContextArgs final { };
#endif

class GLRenderingContext final : public IRenderingContext
{
private:
    GLGraphicsInterface& _gi;

#if defined(_WIN32)
    HDC _device;
    HGLRC _context;
#endif

    NullableRef<GLDepthStencilState> _defaultDepthStencilState;
    NullableRef<GLDepthStencilState> _currentDepthStencilState;
    NullableRef<GLRasterizerState> _defaultRasterizerState;
    NullableRef<GLRasterizerState> _currentRasterizerState;

    GLBufferBuilder* _bufferBuilder;
    GLIndexBufferBuilder* _indexBufferBuilder;
    GLUniformBufferBuilder* _uniformBufferBuilder;
    GLTextureSamplerBuilder* _textureSamplerBuilder;
    GLTexture2DBuilder* _texture2DBuilder;
    GLTextureNullBuilder* _textureNullBuilder;
    GLTextureDepthBuilder* _textureDepthBuilder;
    GLTextureCubeBuilder* _textureCubeBuilder;
public:
    GLRenderingContext(const RenderingMode& mode, const GLRenderingContextArgs& glArgs, const GLSystemRenderingContextArgs& glSysArgs) noexcept;
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
    const DepthStencilArgs& getDefaultDepthStencilArgs() noexcept override;

    NullableRef<IRasterizerState> setRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept override;
    void setDefaultRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept override;
    void resetRasterizerState() noexcept override;
    const RasterizerArgs& getDefaultRasterizerArgs() noexcept override;

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
private:
    void handleCtxError(int profileMask) const noexcept;

    void systemDestruct() noexcept;

    RC_IMPL(GLRenderingContext);
};

class TAU_DLL GLRenderingContextBuilder final : public IRenderingContextBuilder
{
    DEFAULT_DESTRUCT(GLRenderingContextBuilder);
    DELETE_COPY(GLRenderingContextBuilder);
private:
    GLGraphicsInterface& _gi;
public:
    GLRenderingContextBuilder(GLGraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] GLRenderingContext* build(const RenderingContextArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] GLRenderingContext* build(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IRenderingContext> buildCPPRef(const RenderingContextArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] NullableRef<IRenderingContext> buildTauRef(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept override;
    [[nodiscard]] NullableStrongRef<IRenderingContext> buildTauSRef(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept override;
private:
    [[nodiscard]] static bool processArgs(const RenderingContextArgs& args, [[tau::out]] GLRenderingContextArgs* glArgs, [[tau::out]] Error* error) noexcept;
    [[nodiscard]] bool processSysArgs(const RenderingContextArgs& args, const GLRenderingContextArgs& glArgs, [[tau::out]] GLSystemRenderingContextArgs* glSysArgs, [[tau::out]] Error* error) const noexcept;
    void handleCtxError(int profileMask) const noexcept;
};
