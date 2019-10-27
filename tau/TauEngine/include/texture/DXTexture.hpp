#pragma once
#ifdef _WIN32

#include <texture/Texture.hpp>
#include <d3d9.h>

class TAU_DLL D3D9Texture final : public ITexture
{
    DELETE_COPY(D3D9Texture);
private:
    IDirect3DTexture9* _texture;
public:
    D3D9Texture(const TextureType textureType) noexcept
        : ITexture(textureType),
          _texture(nullptr)
    {
        ((IDirect3DDevice9*) nullptr)->CreateTexture(0, 0, 0, 0, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL::D3DPOOL_DEFAULT, &_texture, nullptr);
    }

    ~D3D9Texture() noexcept override final
    {
        if(_texture)
        {
            _texture->Release();
        }
    }


    void bind(u8 textureUnit) noexcept override final
    { }

    void unbind(u8 textureUnit) noexcept override final
    { }
};
#endif
