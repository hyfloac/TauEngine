#pragma once

#include <system/RenderingContext.hpp>

#ifdef _WIN32
#pragma warning(push, 0)
#include <d3d9.h>
#pragma warning(pop)

#include <DLL.hpp>

class TAU_DLL DXRenderingContext : public IRenderingContext
{
private:
    IDirect3D9* _d3d;
    IDirect3DDevice9* _dx9Device;
public:
    DXRenderingContext() noexcept;

    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) override;

    void createContext(void* param) override;

    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, int stencilValue = 0) override;
};
#endif
