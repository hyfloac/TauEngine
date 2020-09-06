#include "system/SystemInterface.hpp"

#ifdef _WIN32
#include <Windows.h>
#include "gl/GLGraphicsInterface.hpp"
#include "dx/dx10/DX10GraphicsInterface.hpp"
#include "dx/dx11/DX11GraphicsInterface.hpp"
#include "dx/dx12/DX12GraphicsInterface.hpp"
#include "dx/dxgi/DXGI11GraphicsAccelerator.hpp"
#include "dx/dxgi/DXGI13GraphicsAccelerator.hpp"
#include "dx/dxgi/DXGI16GraphicsAccelerator.hpp"

_SysContainer SystemInterface::_sysContainer;

IGraphicsInterfaceBuilder* SystemInterface::_giBuilders[RenderingMode::_MAX_VALUE];

const _SysContainer& SystemInterface::sysContainer() noexcept
{
    if(!_sysContainer.programHandle)
    {
        _sysContainer.programHandle = GetModuleHandleW(NULL);
    }
    return _sysContainer;
}

static void getGLArgs(const RenderingMode& mode, GLGraphicsInterfaceArgs& args) noexcept;

SystemInterface::GAList SystemInterface::graphicsAccelerators(const RenderingMode& renderingMode) noexcept
{
    switch(renderingMode.currentMode())
    {
        case RenderingMode::Mode::DirectX10:
            return DXGI11GraphicsAccelerator::graphicsAccelerators();
        case RenderingMode::Mode::DirectX11:
            return DXGI13GraphicsAccelerator::graphicsAccelerators(renderingMode.debugMode());
        case RenderingMode::Mode::DirectX12:
        case RenderingMode::Mode::DirectX12_1:
            return DXGI16GraphicsAccelerator::graphicsAccelerators(renderingMode.debugMode());
        default: return GAList();
    }
}

IGraphicsInterfaceBuilder* SystemInterface::createGraphicsInterface(const RenderingMode::Mode mode) noexcept
{
    if(mode > RenderingMode::_MAX_VALUE)
    { return null; }

    return _giBuilders[mode];
}


NullableRef<IGraphicsInterface> SystemInterface::createGraphicsInterface(const GraphicsInterfaceArgs& args) noexcept
{
    switch(args.renderingMode.currentMode())
    {
        case RenderingMode::Mode::DirectX10: return DX10GraphicsInterfaceBuilder::build(args);
        case RenderingMode::Mode::DirectX11: return DX11GraphicsInterfaceBuilder::build(args);
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
            GLGraphicsInterfaceArgs glArgs{ args.renderingMode, 0, 0, static_cast<GLGraphicsInterface::GLProfile>(0), false };
            getGLArgs(args.renderingMode, glArgs);
            return GLGraphicsInterfaceBuilder::build(glArgs);
        }
        default: return null;
    }
}

void SystemInterface::registerGraphicsInterface(const RenderingMode::Mode mode, IGraphicsInterfaceBuilder* const builder) noexcept
{
    if(mode > RenderingMode::_MAX_VALUE)
    { return; }

    if(_giBuilders[mode])
    { delete _giBuilders[mode]; }

    _giBuilders[mode] = builder;
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
