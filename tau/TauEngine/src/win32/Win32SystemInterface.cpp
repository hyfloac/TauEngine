#include "system/SystemInterface.hpp"

#ifdef _WIN32
#include <Windows.h>
#include "gl/GLGraphicsInterface.hpp"
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx11/DX11GraphicsInterface.hpp"

_SysContainer SystemInterface::_sysContainer;

const _SysContainer& SystemInterface::sysContainer() noexcept
{
    if(_sysContainer.programHandle == NULL)
    {
        _sysContainer.programHandle = GetModuleHandleA(NULL);
    }
    return _sysContainer;
}

static void getGLArgs(const RenderingMode& mode, GLGraphicsInterfaceArgs& args) noexcept;

NullableRef<IGraphicsInterface> SystemInterface::createGraphicsInterface(const RenderingMode& renderingMode) noexcept
{
    switch(renderingMode.currentMode())
    {
        case RenderingMode::Mode::DirectX10:
        {
            const DX10GraphicsInterfaceArgs dx10Params {
                renderingMode
            };

            return RefCast<IGraphicsInterface>(DX10GraphicsInterfaceBuilder::build(dx10Params));
        }
        case RenderingMode::Mode::DirectX11:
        {
            const DX11GraphicsInterfaceArgs dx11Params {
                renderingMode
            };

            return RefCast<IGraphicsInterface>(DX11GraphicsInterfaceBuilder::build(dx11Params));
        }
        case RenderingMode::Mode::DirectX12:
        case RenderingMode::Mode::DirectX12_1:
            return null;
        case RenderingMode::Mode::Vulkan:
            return null;
        case RenderingMode::Mode::OpenGL4_1:
        case RenderingMode::Mode::OpenGL4_2:
        case RenderingMode::Mode::OpenGL4_3:
        case RenderingMode::Mode::OpenGL4_4:
        case RenderingMode::Mode::OpenGL4_5:
        case RenderingMode::Mode::OpenGL4_6:
        {
            GLGraphicsInterfaceArgs glArgs{ renderingMode, 0, 0, static_cast<GLGraphicsInterface::GLProfile>(0), false };
            getGLArgs(renderingMode, glArgs);
            return RefCast<IGraphicsInterface>(GLGraphicsInterfaceBuilder::build(glArgs));
        }
        default: return null;
    }
}

void SystemInterface::createAlert(const char* const title, const char* const message) noexcept
{
    (void) MessageBoxA(NULL, message, title, MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
}

static void getGLArgs(const RenderingMode& mode, GLGraphicsInterfaceArgs& args) noexcept
{
    switch(mode)
    {
        case RenderingMode::Mode::OpenGL4_1:
            args.majorVersion = 4;
            args.minorVersion = 1;
            break;
        case RenderingMode::Mode::OpenGL4_2:
            args.majorVersion = 4;
            args.minorVersion = 2;
            break;
        case RenderingMode::Mode::OpenGL4_3:
            args.majorVersion = 4;
            args.minorVersion = 3;
            break;
        case RenderingMode::Mode::OpenGL4_4:
            args.majorVersion = 4;
            args.minorVersion = 4;
            break;
        case RenderingMode::Mode::OpenGL4_5:
            args.majorVersion = 4;
            args.minorVersion = 5;
            break;
        case RenderingMode::Mode::OpenGL4_6:
            args.majorVersion = 4;
            args.minorVersion = 6;
            break;
        default: return;
    }
    args.compat = GLGraphicsInterface::GLProfile::Compat;
    args.forwardCompatible = true;
}

#endif
