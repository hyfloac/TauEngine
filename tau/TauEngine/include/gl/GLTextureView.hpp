#pragma once

#include "texture/TextureView.hpp"

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

class TAU_DLL GLTextureView : public ITextureView
{
    DEFAULT_DESTRUCT(GLTextureView);
    TEXTURE_VIEW_IMPL(GLTextureView);
private:
    GLuint _texture;
    GLenum _target;
public:
    inline GLTextureView(const GLuint texture, const GLenum target) noexcept
        : _texture(texture)
        , _target(target)
    { }

    [[nodiscard]] inline GLuint texture() const noexcept { return _texture; }
    [[nodiscard]] inline GLenum  glType() const noexcept { return _target;  }

    void generateMipmaps(IRenderingContext&) noexcept override
    {
        glBindTexture(_target, _texture);
        glGenerateMipmap(_target);
        glBindTexture(_target, 0);
    }
};

class TAU_DLL GLNoMipmapTextureView : public GLTextureView
{
    DEFAULT_DESTRUCT(GLNoMipmapTextureView);
    DELETE_COPY(GLNoMipmapTextureView);
public:
    inline GLNoMipmapTextureView(const GLuint texture, const GLenum target) noexcept
        : GLTextureView(texture, target)
    { }

    void generateMipmaps(IRenderingContext&) noexcept override { }
};
