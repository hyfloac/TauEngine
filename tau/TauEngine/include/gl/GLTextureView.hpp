#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "texture/TextureView.hpp"

class GLResourceTexture;
class GLResource;

class TAU_DLL GLTextureView final
{
    DEFAULT_DESTRUCT(GLTextureView);
    DEFAULT_CM_PU(GLTextureView);
private:
    GLenum _target;
    GLResourceTexture* _texture;
public:
    GLTextureView(const GLenum target, GLResourceTexture* const texture) noexcept
        : _target(target)
        , _texture(texture)
    { }

    [[nodiscard]] GLenum target() const noexcept { return _target; }

    [[nodiscard]]       GLResourceTexture* texture()       noexcept { return _texture; }
    [[nodiscard]] const GLResourceTexture* texture() const noexcept { return _texture; }
};

class TAU_DLL GLTextureViewBuilder final : public ITextureViewBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureViewBuilder);
    DEFAULT_DESTRUCT(GLTextureViewBuilder);
    DEFAULT_CM_PU(GLTextureViewBuilder);
public:
    struct GLTextureViewArgs final
    {
        GLenum target;
        GLResourceTexture* texture;
    };
public:
    [[nodiscard]] TextureView build(const TextureViewArgs& args, CPUDescriptorHandle handle, Error* error) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs1D(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs1DArray(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs2D(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs2DArray(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs3D(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgsCube(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgsCubeArray(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
};
