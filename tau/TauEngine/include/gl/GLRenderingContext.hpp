#pragma once

#include <system/RenderingContext.hpp>
#ifdef _WIN32
#include <GL/wglew.h>
#endif

class GLRenderingContext : public IRenderingContext
{
private:
#ifdef _WIN32
    HGLRC _context;
#endif
    GLContextSettings _contextSettings;
public:
    GLRenderingContext(GLContextSettings contextSettings) noexcept;

    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ = 0, float maxZ = 0) override;

    void createContext(void* param) override;
};
