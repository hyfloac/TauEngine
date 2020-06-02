#pragma once

#include "system/RenderingContext.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10DepthStencilState;
class DX10RasterizerState;
class DX10BlendingState;
class DX10GraphicsInterface;

struct DX10RenderingContextArgs final
{
    ID3D10RenderTargetView* renderTargetView;
    ID3D10Texture2D* depthStencilBuffer;
    ID3D10DepthStencilView* depthStencilView;
    ID3D10BlendState* blendState;
    IDXGISwapChain* swapChain;
};

class TAU_DLL DX10RenderingContext final : public IRenderingContext
{
private:
    DX10GraphicsInterface& _gi;

    ID3D10RenderTargetView* _renderTargetView;
    ID3D10Texture2D* _depthStencilBuffer;
    ID3D10DepthStencilView* _depthStencilView;
    ID3D10BlendState* _blendState;
    IDXGISwapChain* _swapChain;

    bool _vsync;

    UINT* _iaStrides;
    UINT* _iaOffsets;

    NullableRef<DX10DepthStencilState> _defaultDepthStencilState;
    NullableRef<DX10DepthStencilState> _currentDepthStencilState;
    NullableRef<DX10RasterizerState> _defaultRasterizerState;
    NullableRef<DX10RasterizerState> _currentRasterizerState;
    NullableRef<DX10BlendingState> _defaultBlendingState;
    NullableRef<DX10BlendingState> _currentBlendingState;
public:
    DX10RenderingContext(DX10GraphicsInterface& gi, const DX10RenderingContextArgs& args) noexcept;

    ~DX10RenderingContext() noexcept override;

    [[nodiscard]] const ID3D10Device* d3dDevice() const noexcept;
    [[nodiscard]] ID3D10Device* d3dDevice() noexcept;

    void resetFrameBuffer() const noexcept;

    void setBufferData(UINT* const strides, UINT* const offsets) noexcept
    {
        _iaStrides = strides;
        _iaOffsets = offsets;
    }

    void setBuffers(uSys count, ID3D10Buffer** buffers) const noexcept;

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

    NullableRef<IBlendingState> setBlendingState(const NullableRef<IBlendingState>& bsState, const float color[4]) noexcept override;
    void setDefaultBlendingState(const NullableRef<IBlendingState>& bsState) noexcept override;
    void resetBlendingState(const float color[4]) noexcept override;
    const BlendingArgs& getDefaultBlendingArgs() noexcept override;
    [[nodiscard]] NullableRef<IBlendingState> getDefaultBlendingState() noexcept override;

    void beginFrame() noexcept override;
    void endFrame() noexcept override;
    void swapFrame() noexcept override;

    void resizeSwapChain(uSys width, uSys height) noexcept override;
protected:
    RC_IMPL(DX10RenderingContext);
};

class TAU_DLL DX10RenderingContextBuilder final : public IRenderingContextBuilder
{
    DEFAULT_DESTRUCT(DX10RenderingContextBuilder);
    DELETE_COPY(DX10RenderingContextBuilder);
private:
    DX10GraphicsInterface& _gi;
public:
    DX10RenderingContextBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10RenderingContext* build(const RenderingContextArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] DX10RenderingContext* build(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IRenderingContext> buildCPPRef(const RenderingContextArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] NullableRef<IRenderingContext> buildTauRef(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept override;
    [[nodiscard]] NullableStrongRef<IRenderingContext> buildTauSRef(const RenderingContextArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept override;
private:
    [[nodiscard]] bool processArgs(const RenderingContextArgs& args, [[tau::out]] DX10RenderingContextArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
