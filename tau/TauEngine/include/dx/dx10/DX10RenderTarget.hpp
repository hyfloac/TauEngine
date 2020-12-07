#pragma once

#include "graphics/RenderTarget.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"

class TAU_DLL DX10RenderTargetView final : public IRenderTargetView
{
    RTV_IMPL(DX10RenderTargetView);
private:
    ID3D10RenderTargetView* _d3dRenderTargetView;
public:
    DX10RenderTargetView(ID3D10RenderTargetView* const d3dRenderTargetView) noexcept
        : _d3dRenderTargetView(d3dRenderTargetView)
    { }

    ~DX10RenderTargetView() noexcept override
    { RELEASE_DX(_d3dRenderTargetView); }

    DX10RenderTargetView(const DX10RenderTargetView& copy) noexcept
        : IRenderTargetView(copy)
        , _d3dRenderTargetView(copy._d3dRenderTargetView)
    { _d3dRenderTargetView->AddRef(); }

    DX10RenderTargetView(DX10RenderTargetView&& move) noexcept
        : IRenderTargetView(::std::move(move))
        , _d3dRenderTargetView(move._d3dRenderTargetView)
    { move._d3dRenderTargetView = null; }

    DX10RenderTargetView& operator=(const DX10RenderTargetView& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dRenderTargetView);

        IRenderTargetView::operator=(copy);

        _d3dRenderTargetView = copy._d3dRenderTargetView;
        _d3dRenderTargetView->AddRef();

        return *this;
    }

    DX10RenderTargetView& operator=(DX10RenderTargetView&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dRenderTargetView);

        IRenderTargetView::operator=(::std::move(move));

        _d3dRenderTargetView = move._d3dRenderTargetView;
        move._d3dRenderTargetView = null;

        return *this;
    }

    [[nodiscard]] ID3D10RenderTargetView* d3dRenderTargetView() const noexcept { return _d3dRenderTargetView; }
};

class TAU_DLL DX10DepthStencilView final : public IDepthStencilView
{
    DSV_IMPL(DX10DepthStencilView);
private:
    ID3D10DepthStencilView* _d3dDepthStencilView;
public:
    DX10DepthStencilView(ID3D10DepthStencilView* const d3dDepthStencilView) noexcept
        : _d3dDepthStencilView(d3dDepthStencilView)
    { }

    ~DX10DepthStencilView() noexcept override
    { RELEASE_DX(_d3dDepthStencilView); }

    DX10DepthStencilView(const DX10DepthStencilView& copy) noexcept
        : IDepthStencilView(copy)
        , _d3dDepthStencilView(copy._d3dDepthStencilView)
    { _d3dDepthStencilView->AddRef(); }

    DX10DepthStencilView(DX10DepthStencilView&& move) noexcept
        : IDepthStencilView(::std::move(move))
        , _d3dDepthStencilView(move._d3dDepthStencilView)
    { move._d3dDepthStencilView = null; }

    DX10DepthStencilView& operator=(const DX10DepthStencilView& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dDepthStencilView);

        IDepthStencilView::operator=(copy);

        _d3dDepthStencilView = copy._d3dDepthStencilView;
        _d3dDepthStencilView->AddRef();

        return *this;
    }

    DX10DepthStencilView& operator=(DX10DepthStencilView&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dDepthStencilView);

        IDepthStencilView::operator=(::std::move(move));

        _d3dDepthStencilView = move._d3dDepthStencilView;
        move._d3dDepthStencilView = null;

        return *this;
    }

    [[nodiscard]] ID3D10DepthStencilView* d3dDepthStencilView() const noexcept { return _d3dDepthStencilView; }
};
#endif
