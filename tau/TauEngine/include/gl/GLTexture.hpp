#pragma once

#include <texture/Texture.hpp>
#include <GL/glew.h>

class TAU_DLL GLTexture2D final : public ITexture
{
    DELETE_COPY(GLTexture2D);
private:
    GLuint _texture;
    GLint _minFilter;
    GLint _magFilter;
public:
    GLTexture2D(u32 width, u32 height, ETexture::Format dataFormat) noexcept;

    ~GLTexture2D() noexcept override final;

    [[nodiscard]] inline ETexture::Type textureType() const noexcept override final { return ETexture::Type::TEXTURE_2D; }

    void setFilterMode(ETexture::Filter minificationFilter, ETexture::Filter magnificationFilter) noexcept override final;

    void set(const void* data) noexcept override final;

    void bind(u8 textureUnit) noexcept override final;

    void unbind(u8 textureUnit) noexcept override final;
};
