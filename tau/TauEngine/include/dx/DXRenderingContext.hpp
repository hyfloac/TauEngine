#pragma once

#include <system/RenderingContext.hpp>

#ifdef _WIN32
#pragma warning(push, 0)
#include <d3d9.h>
#pragma warning(pop)

#include <DLL.hpp>

class TAU_DLL DXRenderingContext final : public IRenderingContext
{
private:
    IDirect3D9* _d3d;
    IDirect3DDevice9* _dx9Device;
public:
    DXRenderingContext() noexcept;
    ~DXRenderingContext() noexcept override final = default;

    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) override final;

    void createContext(void* param) override final;

    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, int stencilValue = 0) override final;

    RC_IMPL(DXRenderingContext);
};
#endif
