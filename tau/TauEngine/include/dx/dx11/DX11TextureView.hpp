#pragma once

#include "texture/TextureView.hpp"

#ifdef _WIN32
#include <d3d11.h>
#include "dx/DXUtils.hpp"

class TAU_DLL DX11TextureView : public ITextureView
{
    TEXTURE_VIEW_IMPL(DX11TextureView);
private:
    ID3D11ShaderResourceView* _d3dShaderResourceView;
public:
    DX11TextureView(ID3D11ShaderResourceView* const d3dShaderResourceView) noexcept
        : _d3dShaderResourceView(d3dShaderResourceView)
    { }

    ~DX11TextureView() noexcept
    { RELEASE_DX(_d3dShaderResourceView); }

    DX11TextureView(const DX11TextureView& copy) noexcept
        : ITextureView(copy)
        , _d3dShaderResourceView(copy._d3dShaderResourceView)
    { _d3dShaderResourceView->AddRef(); }

    DX11TextureView(DX11TextureView&& move) noexcept
        : ITextureView(::std::move(move))
        , _d3dShaderResourceView(move._d3dShaderResourceView)
    { move._d3dShaderResourceView = null; }

    DX11TextureView& operator=(const DX11TextureView& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        ITextureView::operator=(copy);

        _d3dShaderResourceView = copy._d3dShaderResourceView;
        _d3dShaderResourceView->AddRef();

        return *this;
    }

    DX11TextureView& operator=(DX11TextureView&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        ITextureView::operator=(::std::move(move));

        _d3dShaderResourceView = move._d3dShaderResourceView;
        move._d3dShaderResourceView = null;

        return *this;
    }

    [[nodiscard]]       ID3D11ShaderResourceView* d3dShaderResourceView()       noexcept { return _d3dShaderResourceView; }
    [[nodiscard]] const ID3D11ShaderResourceView* d3dShaderResourceView() const noexcept { return _d3dShaderResourceView; }

    void generateMipmaps(IRenderingContext& context) noexcept override;
};

class TAU_DLL DX11NoMipmapTextureView final : public DX11TextureView
{
    DEFAULT_DESTRUCT(DX11NoMipmapTextureView);
    DEFAULT_CM_PU(DX11NoMipmapTextureView);
public:
    DX11NoMipmapTextureView(ID3D11ShaderResourceView* const d3dShaderResourceView) noexcept
        : DX11TextureView(d3dShaderResourceView)
    { }

    void generateMipmaps(IRenderingContext&) noexcept override { }
};
#endif
