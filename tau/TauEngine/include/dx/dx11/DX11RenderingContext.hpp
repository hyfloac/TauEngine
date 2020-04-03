#pragma once

#include "system/RenderingContext.hpp"

#ifdef _WIN32
#pragma warning(push, 0)
#include <d3d11.h>
#pragma warning(pop)

#include "DLL.hpp"

class DX11DepthStencilState;
class DX11RasterizerState;
class DX11GraphicsInterface;

struct DX11RenderingContextArgs final
{
    ID3D11DeviceContext* d3d11DeviceContext;
    ID3D11RenderTargetView* renderTargetView;
    ID3D11Texture2D* depthStencilBuffer;
    ID3D11DepthStencilView* depthStencilView;
    IDXGISwapChain* swapChain;
};

class TAU_DLL DX11RenderingContext final : public IRenderingContext
{
private:
    DX11GraphicsInterface& _gi;

    ID3D11DeviceContext* _d3d11DeviceContext;
    ID3D11RenderTargetView* _renderTargetView;
    ID3D11Texture2D* _depthStencilBuffer;
    ID3D11DepthStencilView* _depthStencilView;
    IDXGISwapChain* _swapChain;

    bool _vsync;

    NullableRef<DX11DepthStencilState> _defaultDepthStencilState;
    NullableRef<DX11DepthStencilState> _currentDepthStencilState;
    NullableRef<DX11RasterizerState> _defaultRasterizerState;
    NullableRef<DX11RasterizerState> _currentRasterizerState;
public:
    DX11RenderingContext(DX11GraphicsInterface& gi, const DX11RenderingContextArgs& args) noexcept;

    ~DX11RenderingContext() noexcept override;

    [[nodiscard]] const ID3D11Device* d3d11Device() const noexcept;
    [[nodiscard]] ID3D11Device* d3d11Device() noexcept;

    [[nodiscard]] const ID3D11DeviceContext* d3d11DeviceContext() const noexcept { return _d3d11DeviceContext; }
    [[nodiscard]] ID3D11DeviceContext* d3d11DeviceContext() noexcept { return _d3d11DeviceContext; }

    void deactivateContext() noexcept override { }
    void activateContext() noexcept override { }
    void updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) noexcept override;
    void clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, u8 stencilValue) noexcept override;
    void setVSync(bool vsync) noexcept override;

    NullableRef<IDepthStencilState> setDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept override;
    void setDefaultDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept override;
    void resetDepthStencilState() noexcept override;
    const DepthStencilArgs& getDefaultDepthStencilArgs() noexcept override;
    [[nodiscard]] NullableRef<IDepthStencilState> getDefaultDepthStencilState() noexcept override;

    NullableRef<IRasterizerState> setRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept override;
    void setDefaultRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept override;
    void resetRasterizerState() noexcept override;
    const RasterizerArgs& getDefaultRasterizerArgs() noexcept override;
    [[nodiscard]] NullableRef<IRasterizerState> getDefaultRasterizerState() noexcept override;

    void beginFrame() noexcept override;
    void endFrame() noexcept override;
    void swapFrame() noexcept override;

    [[nodiscard]] CPPRef<IFrameBufferBuilder> createFrameBuffer() noexcept override { return null; }
protected:
    RC_IMPL(DX11RenderingContext);
};

class TAU_DLL DX11RenderingContextBuilder final : public IRenderingContextBuilder
{
    DEFAULT_DESTRUCT(DX11RenderingContextBuilder);
    DELETE_COPY(DX11RenderingContextBuilder);
private:
    DX11GraphicsInterface& _gi;
public:
    DX11RenderingContextBuilder(DX11GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX11RenderingContext* build(const RenderingContextArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] DX11RenderingContext* build(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IRenderingContext> buildCPPRef(const RenderingContextArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] NullableRef<IRenderingContext> buildTauRef(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept override;
    [[nodiscard]] NullableStrongRef<IRenderingContext> buildTauSRef(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept override;
private:
    [[nodiscard]] bool processArgs(const RenderingContextArgs& args, [[tau::out]] DX11RenderingContextArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
