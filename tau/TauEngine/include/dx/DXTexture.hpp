#pragma once
#ifdef _WIN32

#include <texture/Texture.hpp>
#include <d3d9.h>

class TAU_DLL D3D9Texture2D final : public ITexture
{
    DELETE_COPY(D3D9Texture2D);
private:
    IDirect3DTexture9* _texture;
public:
    D3D9Texture2D(const u32 width, const u32 height, const ETexture::Format dataFormat) noexcept
        : ITexture(width, height, dataFormat),
          _texture(nullptr)
    {
        ((IDirect3DDevice9*) nullptr)->CreateTexture(0, 0, 0, 0, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL::D3DPOOL_DEFAULT, &_texture, nullptr);
    }

    ~D3D9Texture2D() noexcept override final
    {
        if(_texture)
        {
            _texture->Release();
        }
    }

    [[nodiscard]] inline ETexture::Type textureType() const noexcept override { return ETexture::Type::TEXTURE_2D; }


    void set(const void* data) noexcept override final { }

    void bind(u8 textureUnit) noexcept override final
    { }

    void unbind(u8 textureUnit) noexcept override final
    { }

    void setFilterMode(ETexture::Filter minificationFilter, ETexture::Filter magnificationFilter) noexcept override { }
};
#endif
