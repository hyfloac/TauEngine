#pragma once

#include "texture/RenderTarget.hpp"

#ifdef _WIN32
#include <d3d11.h>

class TAU_DLL DX11RenderTarget final : public IRenderTarget
{
    RENDER_TARGET_IMPL(DX11RenderTarget);
private:
    ID3D11RenderTargetView* _d3dRenderTargetView;
public:
    inline DX11RenderTarget(ID3D11RenderTargetView* const d3dRenderTargetView) noexcept
        : _d3dRenderTargetView(d3dRenderTargetView)
    { }

    inline ~DX11RenderTarget() noexcept
    {
        if(_d3dRenderTargetView)
        {
            _d3dRenderTargetView->Release();
            _d3dRenderTargetView = null;
        }
    }

    [[nodiscard]] inline const ID3D11RenderTargetView* d3dRenderTargetView() const noexcept { return _d3dRenderTargetView; }
    [[nodiscard]] inline       ID3D11RenderTargetView* d3dRenderTargetView()       noexcept { return _d3dRenderTargetView; }
};

class TAU_DLL DX11DepthStencilTarget final : public IRenderTarget
{
    RENDER_TARGET_IMPL(DX11DepthStencilTarget);
private:
    ID3D11DepthStencilView* _d3dDepthStencilView;
public:
    inline DX11DepthStencilTarget(ID3D11DepthStencilView* const d3dDepthStencilView) noexcept
        : _d3dDepthStencilView(d3dDepthStencilView)
    { }

    inline ~DX11DepthStencilTarget() noexcept
    {
        if(_d3dDepthStencilView)
        {
            _d3dDepthStencilView->Release();
            _d3dDepthStencilView = null;
        }
    }

    [[nodiscard]] inline const ID3D11DepthStencilView* d3dDepthStencilView() const noexcept { return _d3dDepthStencilView; }
    [[nodiscard]] inline       ID3D11DepthStencilView* d3dDepthStencilView()       noexcept { return _d3dDepthStencilView; }
};
#endif
