#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "system/RenderingContext.hpp"

class GLGraphicsInterface;

class GLDepthStencilState;
class GLRasterizerState;
class GLBlendingState;

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

    GLintptr* _iaOffsets;
    GLsizei* _iaStrides;

    NullableRef<GLDepthStencilState> _defaultDepthStencilState;
    NullableRef<GLDepthStencilState> _currentDepthStencilState;
    NullableRef<GLRasterizerState> _defaultRasterizerState;
    NullableRef<GLRasterizerState> _currentRasterizerState;
    NullableRef<GLBlendingState> _defaultBlendingState;
    NullableRef<GLBlendingState> _currentBlendingState;
public:
    GLRenderingContext(const RenderingMode& mode, const GLRenderingContextArgs& glArgs, const GLSystemRenderingContextArgs& glSysArgs) noexcept;
    ~GLRenderingContext() noexcept override final;

    [[nodiscard]] HDC getHDC() const noexcept { return _device; }

    void setBufferData(GLintptr* const offsets, GLsizei* const strides) noexcept
    {
        _iaOffsets = offsets;
        _iaStrides = strides;
    }

    void setBuffers(uSys count, const GLuint* buffers) const noexcept;

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

    NullableRef<IBlendingState> setBlendingState(const NullableRef<IBlendingState>& bsState, const float color[4]) noexcept override;
    void setDefaultBlendingState(const NullableRef<IBlendingState>& bsState) noexcept override;
    void resetBlendingState(const float color[4]) noexcept override;
    const BlendingArgs& getDefaultBlendingArgs() noexcept override;
    [[nodiscard]] NullableRef<IBlendingState> getDefaultBlendingState() noexcept override;

    void beginFrame() noexcept override { }
    void endFrame() noexcept override { }

    void swapFrame() noexcept override;

    void resizeSwapChain(uSys width, uSys height) noexcept override { }
private:
    void handleCtxError(int profileMask) const noexcept;

    void systemDestruct() noexcept;

    RC_IMPL(GLRenderingContext);
};

class TAU_DLL GLRenderingContextBuilder final : public IRenderingContextBuilder
{
    DEFAULT_DESTRUCT(GLRenderingContextBuilder);
    DEFAULT_CM_PU(GLRenderingContextBuilder);
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
