#pragma once

#include <system/RenderingContext.hpp>
#include <GL/glew.h>
#ifdef _WIN32
#include <GL/wglew.h>
#endif

class GLRenderingContext final : public IRenderingContext
{
private:
#ifdef _WIN32
    HGLRC _context;
#endif
    GLContextSettings _contextSettings;
public:
    GLRenderingContext(GLContextSettings contextSettings) noexcept;
    ~GLRenderingContext() noexcept override final = default;

    void createContext(void* param) override final;

    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ = 0, float maxZ = 0) override final;

    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, int stencilValue = 0) override final;

    RC_IMPL(GLRenderingContext);
};
