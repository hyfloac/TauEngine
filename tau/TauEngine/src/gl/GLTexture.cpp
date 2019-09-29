#include <texture/GLTexture.hpp>

GLTexture::GLTexture(const TextureType textureType) noexcept
    : ITexture(textureType),
    _texture(0)
{
    glGenTextures(1, &_texture);

    GLenum textureTarget = 0;

    switch(textureType)
    {
        case TextureType::TEXTURE_2D:
            textureTarget = GL_TEXTURE_2D;
            break;
        case TextureType::TEXTURE_3D:
            textureTarget = GL_TEXTURE_3D;
            break;
        case TextureType::TEXTURE_CUBE:
            textureTarget = GL_TEXTURE_CUBE_MAP;
            break;
    }

    glBindTexture(textureTarget, _texture);
}

GLTexture::~GLTexture() noexcept
{
    glDeleteTextures(1, &_texture);
}

void GLTexture::bind(u8 textureUnit) noexcept
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, _texture);
}

void GLTexture::unbind(u8 textureUnit) noexcept
{
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}
