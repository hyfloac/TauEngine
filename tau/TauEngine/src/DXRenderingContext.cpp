#include <dx/DXRenderingContext.hpp>
#include <Utils.hpp>

#ifdef _WIN32

DXRenderingContext::DXRenderingContext() noexcept
    : _d3d(null), _dx9Device(null)
{ }

void DXRenderingContext::updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ)
{
    D3DVIEWPORT9 viewport { x, y, width, height, minZ, maxZ };

    _dx9Device->SetViewport(&viewport);
}

struct D3D9ContextSettings
{
    HWND hwnd;
};

void DXRenderingContext::createContext(void* param)
{
    D3D9ContextSettings* settings = reinterpret_cast<D3D9ContextSettings*>(param);

    _d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dPP;

    ZeroMemory(&d3dPP, sizeof(d3dPP));

    d3dPP.Windowed = TRUE;
    d3dPP.SwapEffect = D3DSWAPEFFECT::D3DSWAPEFFECT_DISCARD;
    d3dPP.EnableAutoDepthStencil = TRUE;
    d3dPP.AutoDepthStencilFormat = D3DFMT_D16;
    d3dPP.hDeviceWindow = settings->hwnd;

    _d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, settings->hwnd,
                       D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                       &d3dPP, &_dx9Device);
}

void DXRenderingContext::clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, int stencilValue)
{
    DWORD flags = 0;
    if(clearColorBuffer)   { flags  = D3DCLEAR_TARGET;  }
    if(clearDepthBuffer)   { flags |= D3DCLEAR_ZBUFFER; }
    if(clearStencilBuffer) { flags |= D3DCLEAR_STENCIL; }

    _dx9Device->Clear(0, null, flags, D3DCOLOR_RGBA(color.r, color.g, color.b, color.a), depthValue, stencilValue);
}

// IRenderingContext* createDXContext() noexcept
// {
//     return new DXRenderingContext;
// }
#else
// IRenderingContext* createDXContext() noexcept
// {
//     return null;
// }
#endif
