#pragma once

#include "texture/TextureView.hpp"

class GLResourceTexture;
class GLResource;

class TAU_DLL GLTextureViewBuilder final : public ITextureViewBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureViewBuilder);
    DEFAULT_DESTRUCT(GLTextureViewBuilder);
    DEFAULT_CM_PU(GLTextureViewBuilder);
public:
    struct GLTextureViewArgs final
    {
        GLResourceTexture* texture;
    };
public:
    [[nodiscard]] TextureView build(const TextureViewArgs& args, DescriptorTable table, uSys tableIndex, Error* error = null) const noexcept override;
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
