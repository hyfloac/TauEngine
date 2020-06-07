#pragma once

#ifdef _WIN32
#include "texture/TextureRawInterface.hpp"

class TAU_DLL DX10TextureRawInterface final : public ITextureRawInterface
{
    DEFAULT_DESTRUCT(DX10TextureRawInterface);
    DEFAULT_CM_PU(DX10TextureRawInterface);
private:
    union
    {
        ID3D10Texture1D* _tex1D;
        ID3D10Texture2D* _tex2D;
        ID3D10Texture3D* _tex3D;
    };
public:
    DX10TextureRawInterface(ID3D10Texture1D* const tex1D) noexcept
        : _tex1D(tex1D)
    { }

    DX10TextureRawInterface(ID3D10Texture2D* const tex2D) noexcept
        : _tex2D(tex2D)
    { }

    DX10TextureRawInterface(ID3D10Texture3D* const tex3D) noexcept
        : _tex3D(tex3D)
    { }

    DX10TextureRawInterface& operator=(ID3D10Texture1D* const tex1D) noexcept
    {
        _tex1D = tex1D;
        return *this;
    }

    DX10TextureRawInterface& operator=(ID3D10Texture2D* const tex2D) noexcept
    {
        _tex2D = tex2D;
        return *this;
    }

    DX10TextureRawInterface& operator=(ID3D10Texture3D* const tex3D) noexcept
    {
        _tex3D = tex3D;
        return *this;
    }

    [[nodiscard]] void* rawHandle() const noexcept override { return _tex1D; }

    [[nodiscard]] ID3D10Texture1D* dx10Texture1D() const noexcept override { return _tex1D; }
    [[nodiscard]] ID3D10Texture2D* dx10Texture2D() const noexcept override { return _tex2D; }
    [[nodiscard]] ID3D10Texture3D* dx10Texture3D() const noexcept override { return _tex3D; }
};
#endif