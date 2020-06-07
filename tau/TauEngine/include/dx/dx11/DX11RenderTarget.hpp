#pragma once

#include "texture/RenderTarget.hpp"

#ifdef _WIN32
#include <d3d11.h>
#include "dx/DXUtils.hpp"

class TAU_DLL DX11RenderTarget final : public IRenderTarget
{
    RENDER_TARGET_IMPL(DX11RenderTarget);
private:
    ID3D11RenderTargetView* _d3dRenderTargetView;
public:
    inline DX11RenderTarget(ID3D11RenderTargetView* const d3dRenderTargetView) noexcept
        : _d3dRenderTargetView(d3dRenderTargetView)
    { }

    ~DX11RenderTarget() noexcept
    { RELEASE_DX(_d3dRenderTargetView); }

    DX11RenderTarget(const DX11RenderTarget& copy) noexcept
        : IRenderTarget(copy)
        , _d3dRenderTargetView(copy._d3dRenderTargetView)
    { _d3dRenderTargetView->AddRef(); }

    DX11RenderTarget(DX11RenderTarget&& move) noexcept
        : IRenderTarget(::std::move(move))
        , _d3dRenderTargetView(move._d3dRenderTargetView)
    { move._d3dRenderTargetView = null; }

    DX11RenderTarget& operator=(const DX11RenderTarget& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dRenderTargetView);

        IRenderTarget::operator=(copy);

        _d3dRenderTargetView = copy._d3dRenderTargetView;
        _d3dRenderTargetView->AddRef();

        return *this;
    }

    DX11RenderTarget& operator=(DX11RenderTarget&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dRenderTargetView);

        IRenderTarget::operator=(::std::move(move));

        _d3dRenderTargetView = move._d3dRenderTargetView;
        move._d3dRenderTargetView = null;

        return *this;
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

    ~DX11DepthStencilTarget() noexcept
    { RELEASE_DX(_d3dDepthStencilView); }

    DX11DepthStencilTarget(const DX11DepthStencilTarget& copy) noexcept
        : IRenderTarget(copy)
        , _d3dDepthStencilView(copy._d3dDepthStencilView)
    { _d3dDepthStencilView->AddRef(); }

    DX11DepthStencilTarget(DX11DepthStencilTarget&& move) noexcept
        : IRenderTarget(::std::move(move))
        , _d3dDepthStencilView(move._d3dDepthStencilView)
    { move._d3dDepthStencilView = null; }

    DX11DepthStencilTarget& operator=(const DX11DepthStencilTarget& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dDepthStencilView);

        IRenderTarget::operator=(copy);

        _d3dDepthStencilView = copy._d3dDepthStencilView;
        _d3dDepthStencilView->AddRef();

        return *this;
    }

    DX11DepthStencilTarget& operator=(DX11DepthStencilTarget&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dDepthStencilView);

        IRenderTarget::operator=(::std::move(move));

        _d3dDepthStencilView = move._d3dDepthStencilView;
        move._d3dDepthStencilView = null;

        return *this;
    }

    [[nodiscard]] inline const ID3D11DepthStencilView* d3dDepthStencilView() const noexcept { return _d3dDepthStencilView; }
    [[nodiscard]] inline       ID3D11DepthStencilView* d3dDepthStencilView()       noexcept { return _d3dDepthStencilView; }
};
#endif
