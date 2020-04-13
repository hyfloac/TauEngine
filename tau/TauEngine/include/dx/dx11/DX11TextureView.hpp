#pragma once

#include "texture/TextureView.hpp"

#ifdef _WIN32
#include <d3d11.h>

class TAU_DLL DX11TextureView : public ITextureView
{
    TEXTURE_VIEW_IMPL(DX11TextureView);
private:
    ID3D11ShaderResourceView* _d3dShaderResourceView;
public:
    inline DX11TextureView(ID3D11ShaderResourceView* const d3dShaderResourceView) noexcept
        : ITextureView()
        , _d3dShaderResourceView(d3dShaderResourceView)
    { }

    inline ~DX11TextureView() noexcept
    {
        if(_d3dShaderResourceView)
        {
            _d3dShaderResourceView->Release();
            _d3dShaderResourceView = null;
        }
    }

    [[nodiscard]] inline const ID3D11ShaderResourceView* d3dShaderResourceView() const noexcept { return _d3dShaderResourceView; }
    [[nodiscard]] inline       ID3D11ShaderResourceView* d3dShaderResourceView()       noexcept { return _d3dShaderResourceView; }

    void generateMipmaps(IRenderingContext& context) noexcept override;
};

class TAU_DLL DX11NoMipmapTextureView final : public DX11TextureView
{
    DEFAULT_DESTRUCT(DX11NoMipmapTextureView);
    DELETE_COPY(DX11NoMipmapTextureView);
public:
    inline DX11NoMipmapTextureView(ID3D11ShaderResourceView* const d3dShaderResourceView) noexcept
        : DX11TextureView(d3dShaderResourceView)
    { }

    void generateMipmaps(IRenderingContext&) noexcept override { }
};
#endif
