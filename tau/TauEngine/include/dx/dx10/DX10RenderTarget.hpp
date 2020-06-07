#pragma once

#include "texture/RenderTarget.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"

class TAU_DLL DX10RenderTarget final : public IRenderTarget
{
    RENDER_TARGET_IMPL(DX10RenderTarget);
private:
    ID3D10RenderTargetView* _d3dRenderTargetView;
public:
    inline DX10RenderTarget(ID3D10RenderTargetView* const d3dRenderTargetView) noexcept
        : _d3dRenderTargetView(d3dRenderTargetView)
    { }

    ~DX10RenderTarget() noexcept
    { RELEASE_DX(_d3dRenderTargetView); }

    DX10RenderTarget(const DX10RenderTarget& copy) noexcept
        : IRenderTarget(copy)
        , _d3dRenderTargetView(copy._d3dRenderTargetView)
    { _d3dRenderTargetView->AddRef(); }

    DX10RenderTarget(DX10RenderTarget&& move) noexcept
        : IRenderTarget(::std::move(move))
        , _d3dRenderTargetView(move._d3dRenderTargetView)
    { move._d3dRenderTargetView = null; }

    DX10RenderTarget& operator=(const DX10RenderTarget& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dRenderTargetView);

        IRenderTarget::operator=(copy);

        _d3dRenderTargetView = copy._d3dRenderTargetView;
        _d3dRenderTargetView->AddRef();

        return *this;
    }

    DX10RenderTarget& operator=(DX10RenderTarget&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dRenderTargetView);

        IRenderTarget::operator=(::std::move(move));

        _d3dRenderTargetView = move._d3dRenderTargetView;
        move._d3dRenderTargetView = null;

        return *this;
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

    ~DX10DepthStencilTarget() noexcept
    { RELEASE_DX(_d3dDepthStencilView); }

    DX10DepthStencilTarget(const DX10DepthStencilTarget& copy) noexcept
        : IRenderTarget(copy)
        , _d3dDepthStencilView(copy._d3dDepthStencilView)
    { _d3dDepthStencilView->AddRef(); }

    DX10DepthStencilTarget(DX10DepthStencilTarget&& move) noexcept
        : IRenderTarget(::std::move(move))
        , _d3dDepthStencilView(move._d3dDepthStencilView)
    { move._d3dDepthStencilView = null; }

    DX10DepthStencilTarget& operator=(const DX10DepthStencilTarget& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dDepthStencilView);

        IRenderTarget::operator=(copy);

        _d3dDepthStencilView = copy._d3dDepthStencilView;
        _d3dDepthStencilView->AddRef();

        return *this;
    }

    DX10DepthStencilTarget& operator=(DX10DepthStencilTarget&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dDepthStencilView);

        IRenderTarget::operator=(::std::move(move));

        _d3dDepthStencilView = move._d3dDepthStencilView;
        move._d3dDepthStencilView = null;

        return *this;
    }

    [[nodiscard]] inline const ID3D10DepthStencilView* d3dDepthStencilView() const noexcept { return _d3dDepthStencilView; }
    [[nodiscard]] inline       ID3D10DepthStencilView* d3dDepthStencilView()       noexcept { return _d3dDepthStencilView; }
};
#endif
