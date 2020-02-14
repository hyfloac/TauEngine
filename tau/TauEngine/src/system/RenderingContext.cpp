#include "system/RenderingContext.hpp"
#include "gl/GLRenderingContext.hpp"
#include "Timings.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"

#ifdef _WIN32
#include "dx/dx9/DX9RenderingContext.hpp"
#endif

IRenderingContext* IRenderingContext::create(const RenderingMode& mode) noexcept
{
    switch(mode.currentMode())
    {
    #ifdef _WIN32
        case RenderingMode::Mode::DirectX9:
            return new(std::nothrow) DX9RenderingContext(mode);
        case RenderingMode::Mode::DirectX10:
            return new(std::nothrow) DX10RenderingContext(mode);
        case RenderingMode::Mode::DirectX11:
        case RenderingMode::Mode::DirectX12:
        case RenderingMode::Mode::DirectX12_1:
            return null;
    #else
        case RenderingMode::Mode::DirectX9:
        case RenderingMode::Mode::DirectX10:
        case RenderingMode::Mode::DirectX11:
        case RenderingMode::Mode::DirectX12:
        case RenderingMode::Mode::DirectX12_1:
        return null;
    #endif
        case RenderingMode::Mode::Vulkan:
            return null;
        case RenderingMode::Mode::OpenGL2:
            return new(std::nothrow) GLRenderingContext(mode, 2, 0, GLRenderingContext::GLProfile::Neither, false);
        case RenderingMode::Mode::OpenGL3:
            return new(std::nothrow) GLRenderingContext(mode, 3, 0, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL3_1:
            return new(std::nothrow) GLRenderingContext(mode, 3, 1, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL3_2:
            return new(std::nothrow) GLRenderingContext(mode, 3, 2, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL3_3:
            return new(std::nothrow) GLRenderingContext(mode, 3, 3, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL4:
            return new(std::nothrow) GLRenderingContext(mode, 4, 0, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL4_2:
            return new(std::nothrow) GLRenderingContext(mode, 4, 2, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL4_3:
            return new(std::nothrow) GLRenderingContext(mode, 4, 3, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL4_4:
            return new(std::nothrow) GLRenderingContext(mode, 4, 4, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL4_5:
            return new(std::nothrow) GLRenderingContext(mode, 4, 5, GLRenderingContext::GLProfile::Compat, true);
        case RenderingMode::Mode::OpenGL4_6:
            return new(std::nothrow) GLRenderingContext(mode, 4, 6, GLRenderingContext::GLProfile::Compat, true);
        default: return null;
    }
}
