#include "dx/DX9RenderingContext.hpp"

#ifdef _WIN32
#include <Utils.hpp>
#include "dx/DXBuffer.hpp"

DX9RenderingContext::DX9RenderingContext(const RenderingMode& mode, const bool debug) noexcept
    : IRenderingContext(mode, debug),
      _d3d9(null), _d3d9Device(null)
{ }

DX9RenderingContext::~DX9RenderingContext() noexcept
{
    if(_d3d9)
    {
        _d3d9->Release();
        _d3d9 = null;
    }
    if(_d3d9Device)
    {
        _d3d9Device->Release();
        _d3d9Device = null;
    }
}

void DX9RenderingContext::updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) noexcept
{
    D3DVIEWPORT9 viewport { x, y, width, height, minZ, maxZ };

    _d3d9Device->SetViewport(&viewport);
}

struct D3D9ContextSettings
{
    HWND hwnd;
};

bool DX9RenderingContext::createContext(void* param) noexcept
{
    HWND hWnd = *reinterpret_cast<HWND*>(param);

    _d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dPP;

    ZeroMemory(&d3dPP, sizeof(d3dPP));

    d3dPP.Windowed = TRUE;
    d3dPP.SwapEffect = D3DSWAPEFFECT::D3DSWAPEFFECT_DISCARD;
    d3dPP.EnableAutoDepthStencil = TRUE;
    d3dPP.AutoDepthStencilFormat = D3DFMT_D16;
    d3dPP.hDeviceWindow = hWnd;

    _d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                       D3DCREATE_HARDWARE_VERTEXPROCESSING,
                       &d3dPP, &_d3d9Device);
    return true;
}

void DX9RenderingContext::clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, int stencilValue) noexcept
{
    DWORD flags = 0;
    if(clearColorBuffer)   { flags  = D3DCLEAR_TARGET;  }
    if(clearDepthBuffer)   { flags |= D3DCLEAR_ZBUFFER; }
    if(clearStencilBuffer) { flags |= D3DCLEAR_STENCIL; }

    _d3d9Device->Clear(0, null, flags, D3DCOLOR_RGBA(color.r, color.g, color.b, color.a), depthValue, stencilValue);
}

void DX9RenderingContext::beginFrame() noexcept
{
    _d3d9Device->BeginScene();
}

void DX9RenderingContext::endFrame() noexcept
{
    _d3d9Device->EndScene();
}

void DX9RenderingContext::swapFrame() noexcept
{
    _d3d9Device->Present(NULL, NULL, NULL, NULL);
}

Ref<IBufferBuilder> DX9RenderingContext::createBuffer(const std::size_t descriptorCount) noexcept
{
    return Ref<DXBufferBuilder>(new(std::nothrow) DXBufferBuilder(descriptorCount, *this));
}
#endif
