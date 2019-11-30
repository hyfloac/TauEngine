#include <dx/DXRenderingContext.hpp>
#include <Utils.hpp>

#ifdef _WIN32

DXRenderingContext::DXRenderingContext(const RenderingMode& mode, const bool debug) noexcept
    : IRenderingContext(mode, debug),
      _d3d(null), _dx9Device(null)
{ }

void DXRenderingContext::updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) noexcept
{
    D3DVIEWPORT9 viewport { x, y, width, height, minZ, maxZ };

    _dx9Device->SetViewport(&viewport);
}

struct D3D9ContextSettings
{
    HWND hwnd;
};

bool DXRenderingContext::createContext(void* param) noexcept
{
    HWND hWnd = *reinterpret_cast<HWND*>(param);

    _d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dPP;

    ZeroMemory(&d3dPP, sizeof(d3dPP));

    d3dPP.Windowed = TRUE;
    d3dPP.SwapEffect = D3DSWAPEFFECT::D3DSWAPEFFECT_DISCARD;
    d3dPP.EnableAutoDepthStencil = TRUE;
    d3dPP.AutoDepthStencilFormat = D3DFMT_D16;
    d3dPP.hDeviceWindow = hWnd;

    _d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                       D3DCREATE_HARDWARE_VERTEXPROCESSING,
                       &d3dPP, &_dx9Device);
    return true;
}

void DXRenderingContext::clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, int stencilValue) noexcept
{
    DWORD flags = 0;
    if(clearColorBuffer)   { flags  = D3DCLEAR_TARGET;  }
    if(clearDepthBuffer)   { flags |= D3DCLEAR_ZBUFFER; }
    if(clearStencilBuffer) { flags |= D3DCLEAR_STENCIL; }

    _dx9Device->Clear(0, null, flags, D3DCOLOR_RGBA(color.r, color.g, color.b, color.a), depthValue, stencilValue);
}

void DXRenderingContext::beginFrame() noexcept
{
    _dx9Device->BeginScene();
}

void DXRenderingContext::endFrame() noexcept
{
    _dx9Device->EndScene();
}

void DXRenderingContext::swapFrame() noexcept
{
    _dx9Device->Present(NULL, NULL, NULL, NULL);
}
#endif
