#pragma once

#include "texture/TextureView.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"

class TAU_DLL DX10TextureView : public ITextureView
{
    TEXTURE_VIEW_IMPL(DX10TextureView);
private:
    ID3D10ShaderResourceView* _d3dShaderResourceView;
public:
    inline DX10TextureView(ID3D10ShaderResourceView* const d3dShaderResourceView) noexcept
        : ITextureView()
        , _d3dShaderResourceView(d3dShaderResourceView)
    { }

    ~DX10TextureView() noexcept
    { RELEASE_DX(_d3dShaderResourceView); }

    DX10TextureView(const DX10TextureView& copy) noexcept
        : ITextureView(copy)
        , _d3dShaderResourceView(copy._d3dShaderResourceView)
    { _d3dShaderResourceView->AddRef(); }

    DX10TextureView(DX10TextureView&& move) noexcept
        : ITextureView(::std::move(move))
        , _d3dShaderResourceView(move._d3dShaderResourceView)
    { move._d3dShaderResourceView = null; }

    DX10TextureView& operator=(const DX10TextureView& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        ITextureView::operator=(copy);

        _d3dShaderResourceView = copy._d3dShaderResourceView;
        _d3dShaderResourceView->AddRef();

        return *this;
    }

    DX10TextureView& operator=(DX10TextureView&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        ITextureView::operator=(::std::move(move));

        _d3dShaderResourceView = move._d3dShaderResourceView;
        move._d3dShaderResourceView = null;

        return *this;
    }

    [[nodiscard]]       ID3D10ShaderResourceView* d3dShaderResourceView()       noexcept { return _d3dShaderResourceView; }
    [[nodiscard]] const ID3D10ShaderResourceView* d3dShaderResourceView() const noexcept { return _d3dShaderResourceView; }

    void generateMipmaps(IRenderingContext& context) noexcept override;
};

class TAU_DLL DX10NoMipmapTextureView final : public DX10TextureView
{
    DEFAULT_DESTRUCT(DX10NoMipmapTextureView);
    DEFAULT_CM_PU(DX10NoMipmapTextureView);
public:
    inline DX10NoMipmapTextureView(ID3D10ShaderResourceView* const d3dShaderResourceView) noexcept
        : DX10TextureView(d3dShaderResourceView)
    { }

    void generateMipmaps(IRenderingContext&) noexcept override { }
};
#endif
