#pragma once

#include "texture/RenderTarget.hpp"

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

class TAU_DLL GLRenderTarget final : public IRenderTarget
{
    DEFAULT_DESTRUCT(GLRenderTarget);
    RENDER_TARGET_IMPL(GLRenderTarget);
private:
    GLuint _texture;
    GLenum _glType;
public:
    inline GLRenderTarget(const GLuint texture, const GLenum glType) noexcept
        : _texture(texture)
        , _glType(glType)
    { }

    [[nodiscard]] inline GLuint texture() const noexcept { return _texture; }
    [[nodiscard]] inline GLenum  glType() const noexcept { return _glType;  }
};
