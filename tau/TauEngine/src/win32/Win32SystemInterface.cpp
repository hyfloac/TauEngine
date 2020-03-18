#include "gl/GLGraphicsInterface.hpp"
#include "system/SystemInterface.hpp"

#ifdef _WIN32
#include <Windows.h>
#include "dx/dx10/DX10GraphicsInterface.hpp"

SystemInterface::SystemInterface() noexcept
    : _sysContainer({ NULL })
{
    _sysContainer.programHandle = GetModuleHandleA(NULL);
}

static void getGLArgs(const RenderingMode& mode, GLGraphicsInterfaceArgs& args) noexcept;

NullableRef<IGraphicsInterface> SystemInterface::createGraphicsInterface(const RenderingMode& renderingMode) const noexcept
{
    switch(renderingMode.currentMode())
    {
        case RenderingMode::Mode::DirectX9:
            return null;
        case RenderingMode::Mode::DirectX10:
        {
            const DX10GraphicsInterfaceArgs dx10Params {
                renderingMode
            };

            return RefCast<IGraphicsInterface>(DX10GraphicsInterfaceBuilder::build(dx10Params));
        }
        case RenderingMode::Mode::DirectX11:
        {
            const DX10GraphicsInterfaceArgs dx11Params {
                renderingMode
            };

            return RefCast<IGraphicsInterface>(DX10GraphicsInterfaceBuilder::build(dx11Params));
        }
        case RenderingMode::Mode::DirectX12:
        case RenderingMode::Mode::DirectX12_1:
            return null;
        case RenderingMode::Mode::Vulkan:
            return null;
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
        {
            GLGraphicsInterfaceArgs glArgs{ renderingMode, 0, 0, static_cast<GLGraphicsInterface::GLProfile>(0), false };
            getGLArgs(renderingMode, glArgs);
            return RefCast<IGraphicsInterface>(GLGraphicsInterfaceBuilder::build(glArgs));
        }
        default: return null;
    }
}

static void getGLArgs(const RenderingMode& mode, GLGraphicsInterfaceArgs& args) noexcept
{
    switch(mode)
    {
        case RenderingMode::Mode::OpenGL3:
            args.majorVersion = 3;
            args.minorVersion = 0;
            break;
        case RenderingMode::Mode::OpenGL3_1:
            args.majorVersion = 3;
            args.minorVersion = 1;
            break;
        case RenderingMode::Mode::OpenGL3_2:
            args.majorVersion = 3;
            args.minorVersion = 2;
            break;
        case RenderingMode::Mode::OpenGL3_3:
            args.majorVersion = 3;
            args.minorVersion = 3;
            break;
        case RenderingMode::Mode::OpenGL4:
            args.majorVersion = 4;
            args.minorVersion = 0;
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
