#include "system/SystemInterface.hpp"

#ifdef _WIN32
#include <Windows.h>
#include "dx/dx10/DX10GraphicsInterface.hpp"

SystemInterface::SystemInterface() noexcept
    : _sysContainer({ NULL })
{
    _sysContainer.programHandle = GetModuleHandleA(NULL);
}

Ref<IGraphicsInterface> SystemInterface::createGraphicsInterface(const RenderingMode& renderingMode) noexcept
{
    switch(renderingMode.currentMode())
    {
        case RenderingMode::Mode::DirectX9:
            return null;
        case RenderingMode::Mode::DirectX10:
            return Ref<IGraphicsInterface>(new(std::nothrow) DX10GraphicsInterface);
        case RenderingMode::Mode::DirectX11:
        case RenderingMode::Mode::DirectX12:
        case RenderingMode::Mode::DirectX12_1:
            return null;
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
        default: return null;
    }
}

#endif
