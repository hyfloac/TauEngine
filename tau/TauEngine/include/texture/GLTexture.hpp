#pragma once

#include <texture/Texture.hpp>
#include <GL/glew.h>

class TAU_DLL GLTexture final : public ITexture
{
    DELETE_COPY(GLTexture);
private:
    GLuint _texture;
public:
    GLTexture(const TextureType textureType) noexcept;

    ~GLTexture() noexcept override final;

    void bind(u8 textureUnit) noexcept override final;

    void unbind(u8 textureUnit) noexcept override final;
};
