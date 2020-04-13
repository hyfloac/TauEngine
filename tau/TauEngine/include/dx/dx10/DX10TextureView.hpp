#pragma once

#include "texture/TextureView.hpp"

#ifdef _WIN32
#include <d3d10.h>

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

    inline ~DX10TextureView() noexcept
    {
        if(_d3dShaderResourceView)
        {
            _d3dShaderResourceView->Release();
            _d3dShaderResourceView = null;
        }
    }

    [[nodiscard]] inline const ID3D10ShaderResourceView* d3dShaderResourceView() const noexcept { return _d3dShaderResourceView; }
    [[nodiscard]] inline       ID3D10ShaderResourceView* d3dShaderResourceView()       noexcept { return _d3dShaderResourceView; }

    void generateMipmaps(IRenderingContext& context) noexcept override;
};

class TAU_DLL DX10NoMipmapTextureView final : public DX10TextureView
{
    DEFAULT_DESTRUCT(DX10NoMipmapTextureView);
    DELETE_COPY(DX10NoMipmapTextureView);
public:
    inline DX10NoMipmapTextureView(ID3D10ShaderResourceView* const d3dShaderResourceView) noexcept
        : DX10TextureView(d3dShaderResourceView)
    { }

    void generateMipmaps(IRenderingContext&) noexcept override { }
};
#endif
