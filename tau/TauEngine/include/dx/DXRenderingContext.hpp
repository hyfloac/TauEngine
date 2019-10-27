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
    DXRenderingContext(const RenderingMode& mode, const bool debug) noexcept;
    ~DXRenderingContext() noexcept override final = default;

    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) noexcept override final;

    [[nodiscard]] bool createContext(void* param) noexcept override final;

    void createFromShared(void* param) noexcept override final { }

    void deactivateContext() noexcept override final { }
    void activateContext() noexcept override final { }

    // [[nodiscard]] void* getBDHandle(IBufferDescriptor* bufferDescriptor) noexcept override { return null; }

    [[nodiscard]] Ref<IBufferDescriptor> createBufferDescriptor(std::size_t attribCount) noexcept override final { return null; }

    [[nodiscard]] void* getBufferDescriptorHandle(IBufferDescriptor* bufferDescriptor) noexcept override { return null; }


    // void bindBD(IBufferDescriptor* bufferDescriptor) noexcept override final { }
    // void unbindBD(IBufferDescriptor* bufferDescriptor) noexcept override final { }

    void destroyBD(IBufferDescriptor* bufferDescriptor) noexcept override final { }

    void clearBDs() noexcept override final { }

    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, int stencilValue = 0) noexcept override final;
protected:
    bool createContextsShared(void* param, IRenderingContext * *sharers, std::size_t count) noexcept override final { return false; }

    // void initBufferDescriptor(IBufferDescriptor* bufferDescriptor) noexcept override final { }

    RC_IMPL(DXRenderingContext);
};
#endif
