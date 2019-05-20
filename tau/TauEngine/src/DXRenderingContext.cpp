#include <dx/DXRenderingContext.hpp>
#include <Utils.hpp>

#ifdef _WIN32

DXRenderingContext::DXRenderingContext() noexcept
    : _dx9Device(null)
{ }

void DXRenderingContext::updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ)
{
    D3DVIEWPORT9 viewport { x, y, width, height, minZ, maxZ };

    _dx9Device->SetViewport(&viewport);
}

void DXRenderingContext::createContext(void* param)
{
    
}

IRenderingContext* createDXContext() noexcept
{
    return new DXRenderingContext;
}
#else
IRenderingContext* createDXContext() noexcept
{
    return null;
}
#endif
