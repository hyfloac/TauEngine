#pragma once

#ifdef _WIN32
#include "texture/TextureRawInterface.hpp"

class TAU_DLL DX11TextureRawInterface final : public ITextureRawInterface
{
    DEFAULT_DESTRUCT(DX11TextureRawInterface);
    DEFAULT_CM_PU(DX11TextureRawInterface);
private:
    union
    {
        ID3D11Texture1D* _tex1D;
        ID3D11Texture2D* _tex2D;
        ID3D11Texture3D* _tex3D;
    };
public:
    DX11TextureRawInterface(ID3D11Texture1D* const tex1D) noexcept
        : _tex1D(tex1D)
    { }

    DX11TextureRawInterface(ID3D11Texture2D* const tex2D) noexcept
        : _tex2D(tex2D)
    { }

    DX11TextureRawInterface(ID3D11Texture3D* const tex3D) noexcept
        : _tex3D(tex3D)
    { }

    DX11TextureRawInterface& operator=(ID3D11Texture1D* const tex1D) noexcept
    {
        _tex1D = tex1D;
        return *this;
    }

    DX11TextureRawInterface& operator=(ID3D11Texture2D* const tex2D) noexcept
    {
        _tex2D = tex2D;
        return *this;
    }

    DX11TextureRawInterface& operator=(ID3D11Texture3D* const tex3D) noexcept
    {
        _tex3D = tex3D;
        return *this;
    }

    [[nodiscard]] ID3D11Texture1D* dx11Texture1D() const noexcept override { return _tex1D; }
    [[nodiscard]] ID3D11Texture2D* dx11Texture2D() const noexcept override { return _tex2D; }
    [[nodiscard]] ID3D11Texture3D* dx11Texture3D() const noexcept override { return _tex3D; }
};
#endif
