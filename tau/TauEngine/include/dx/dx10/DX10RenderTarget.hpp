#pragma once

#include "texture/RenderTarget.hpp"

#ifdef _WIN32
#include <d3d10.h>

class TAU_DLL DX10RenderTarget final : public IRenderTarget
{
    RENDER_TARGET_IMPL(DX10RenderTarget);
private:
    ID3D10RenderTargetView* _d3dRenderTargetView;
public:
    inline DX10RenderTarget(ID3D10RenderTargetView* const d3dRenderTargetView) noexcept
        : _d3dRenderTargetView(d3dRenderTargetView)
    { }

    inline ~DX10RenderTarget() noexcept
    {
        if(_d3dRenderTargetView)
        {
            _d3dRenderTargetView->Release();
            _d3dRenderTargetView = null;
        }
    }

    [[nodiscard]] inline const ID3D10RenderTargetView* d3dRenderTargetView() const noexcept { return _d3dRenderTargetView; }
    [[nodiscard]] inline       ID3D10RenderTargetView* d3dRenderTargetView()       noexcept { return _d3dRenderTargetView; }
};

class TAU_DLL DX10DepthStencilTarget final : public IRenderTarget
{
    RENDER_TARGET_IMPL(DX10DepthStencilTarget);
private:
    ID3D10DepthStencilView* _d3dDepthStencilView;
public:
    inline DX10DepthStencilTarget(ID3D10DepthStencilView* const d3dDepthStencilView) noexcept
        : _d3dDepthStencilView(d3dDepthStencilView)
    { }

    inline ~DX10DepthStencilTarget() noexcept
    {
        if(_d3dDepthStencilView)
        {
            _d3dDepthStencilView->Release();
            _d3dDepthStencilView = null;
        }
    }

    [[nodiscard]] inline const ID3D10DepthStencilView* d3dDepthStencilView() const noexcept { return _d3dDepthStencilView; }
    [[nodiscard]] inline       ID3D10DepthStencilView* d3dDepthStencilView()       noexcept { return _d3dDepthStencilView; }
};
#endif
