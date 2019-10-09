#include "system/RenderingContext.hpp"
#include "gl/GLRenderingContext.hpp"

#ifdef _WIN32
#include "dx/DXRenderingContext.hpp"
#endif

IRenderingContext* IRenderingContext::create(const RenderingMode& mode, GLContextSettings settings) noexcept
{
    switch(mode.currentMode())
    {
        case RenderingMode::Mode::DirectX9:
#ifdef _WIN32
            return new(std::nothrow) DXRenderingContext;
#else
            return null;
#endif
        case RenderingMode::Mode::DirectX11:
        case RenderingMode::Mode::DirectX12:
        case RenderingMode::Mode::DirectX12_1:
#ifdef _WIN32
            return null;
#else
            return null;
#endif
        case RenderingMode::Mode::Vulkan:
            return null;
        case RenderingMode::Mode::OpenGL2:
        case RenderingMode::Mode::OpenGL3:
        case RenderingMode::Mode::OpenGL3_1:
        case RenderingMode::Mode::OpenGL3_2:
        case RenderingMode::Mode::OpenGL3_3:
        case RenderingMode::Mode::OpenGL4:
        case RenderingMode::Mode::OpenGL4_2:
        case RenderingMode::Mode::OpenGL4_3:
        case RenderingMode::Mode::OpenGL4_4:
        case RenderingMode::Mode::OpenGL4_5:
        case RenderingMode::Mode::OpenGL4_6:
            return new(std::nothrow) GLRenderingContext(settings);
        default: return null;
    }
}
