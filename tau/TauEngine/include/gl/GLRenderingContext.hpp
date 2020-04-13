#pragma once

#include "system/RenderingContext.hpp"
#ifdef _WIN32
#include <GL/glew.h>
#include <GL/wglew.h>
#endif

class GLGraphicsInterface;

class GLDepthStencilState;
class GLRasterizerState;

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
public:
    GLRenderingContext(const RenderingMode& mode, const GLRenderingContextArgs& glArgs, const GLSystemRenderingContextArgs& glSysArgs) noexcept;
    ~GLRenderingContext() noexcept override final;

    HDC getHDC() const noexcept { return _device; }

    void deactivateContext() noexcept override final;
    void activateContext() noexcept override final;

    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ = 0.0f, float maxZ = 1.0f) noexcept override final;

    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, u8 stencilValue = 0) noexcept override final;

    void setVSync(bool vsync) noexcept override final;

    NullableRef<IDepthStencilState> setDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept override;
    void setDefaultDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept override;
    void resetDepthStencilState() noexcept override;
    const DepthStencilArgs& getDefaultDepthStencilArgs() noexcept override;
    [[nodiscard]] NullableRef<IDepthStencilState> getDefaultDepthStencilState() noexcept override;

    NullableRef<IRasterizerState> setRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept override;
    void setDefaultRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept override;
    void resetRasterizerState() noexcept override;
    const RasterizerArgs& getDefaultRasterizerArgs() noexcept override;
    [[nodiscard]] NullableRef<IRasterizerState> getDefaultRasterizerState() noexcept override;

    void beginFrame() noexcept override final { }
    void endFrame() noexcept override final { }

    void swapFrame() noexcept override final;
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
