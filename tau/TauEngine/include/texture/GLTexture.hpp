#pragma once

#include <texture/Texture.hpp>
#include <GL/glew.h>

class TAU_DLL GLTexture final
    : public ITexture
{
private:
    GLuint _texture;
public:
    GLTexture(const TextureType textureType) noexcept;

    ~GLTexture() noexcept override final;

    GLTexture(const GLTexture& copy) noexcept = delete;
    GLTexture(GLTexture&& move) noexcept = delete;

    GLTexture& operator=(const GLTexture& copy) noexcept = delete;
    GLTexture& operator=(GLTexture&& move) noexcept = delete;

    void bind(u8 textureUnit) noexcept override final;

    void unbind(u8 textureUnit) noexcept override final;
};
