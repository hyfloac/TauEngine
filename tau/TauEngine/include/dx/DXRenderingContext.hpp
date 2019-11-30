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

    [[nodiscard]] Ref<IVertexArray> createVertexArray(std::size_t attribCount, DrawType drawType) noexcept override final { return null; }

    [[nodiscard]] void* getVertexArrayHandle(IVertexArray* vertexArray) noexcept override final { return null; }

    void destroyVA(IVertexArray* vertexArray) noexcept override final { }

    void clearVAs() noexcept override final { }

    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue = 1.0f, int stencilValue = 0) noexcept override final;

    void setVSync(bool vsync) noexcept override final { }

    void beginFrame() noexcept override final;

    void endFrame() noexcept override final;

    void swapFrame() noexcept override final;

    Ref<IBuffer> createBuffer(std::size_t descriptorCount, IBuffer::Type type, IBuffer::UsageType usage = IBuffer::UsageType::StaticDraw) noexcept override final { return nullptr; }

    Ref<IIndexBuffer> createIndexBuffer(IBuffer::UsageType usage) noexcept override { return nullptr; }
protected:
    bool createContextsShared(void* param, IRenderingContext** sharers, std::size_t count) noexcept override final { return false; }

    RC_IMPL(DXRenderingContext);
};
#endif
