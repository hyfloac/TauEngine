#pragma once

#include "texture/TextureView.hpp"

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

class GLResourceTexture;
class GLResource;

struct GLTextureViewArgs final
{
    ETexture::Format dataFormat;
    ETexture::Type type;
    u32 width;
    u32 height;
    union
    {
        u32 depth;
        u32 arrayCount;
    };
    u32 mipLevels;
    GLenum target;
    GLuint texture;
    bool genMipmaps;
};

class TAU_DLL TAU_NOVTABLE GLTextureView : public ITextureView
{
    DEFAULT_DESTRUCT_VI(GLTextureView);
    DELETE_CM(GLTextureView);
    TEXTURE_VIEW_IMPL(GLTextureView);
protected:
    ETexture::Format _dataFormat;
    ETexture::Type _type;
    u32 _width;
    u32 _height;
    union
    {
        u32 _depth;
        u32 _arrayCount;
    };
    u32 _mipLevels;
    GLenum _target;
    GLuint _texture;
public:
    inline GLTextureView(const GLTextureViewArgs& args) noexcept  // NOLINT(cppcoreguidelines-pro-type-member-init)
        : _dataFormat(args.dataFormat)
        , _type(args.type)
        , _width(args.width)
        , _height(args.height)
        , _depth(args.depth)
        , _mipLevels(args.mipLevels)
        , _target(args.target)
        , _texture(args.texture)
    { }

    [[nodiscard]] ETexture::Format dataFormat() const noexcept override { return _dataFormat; }
    [[nodiscard]] ETexture::Type  textureType() const noexcept override { return _type;       }

    [[nodiscard]] u32      width() const noexcept override { return _width;      }
    [[nodiscard]] u32     height() const noexcept override { return _height;     }
    [[nodiscard]] u32      depth() const noexcept override { return _depth;      }
    [[nodiscard]] u32 arrayCount() const noexcept override { return _arrayCount; }
    [[nodiscard]] u32  mipLevels() const noexcept override { return _mipLevels;  }

    [[nodiscard]] inline GLuint texture() const noexcept { return _texture; }
    [[nodiscard]] inline GLenum  glType() const noexcept { return _target;  }

    void generateMipmaps(IRenderingContext&) noexcept override;
};

class TAU_DLL GLNoMipmapTextureView final : public GLTextureView
{
    DEFAULT_DESTRUCT(GLNoMipmapTextureView);
    DELETE_CM(GLNoMipmapTextureView);
public:
    GLNoMipmapTextureView(const GLTextureViewArgs& args) noexcept : GLTextureView(args) { }
    void generateMipmaps(IRenderingContext&) noexcept override { }
};

class TAU_DLL GLTextureViewBuilder final : public ITextureViewBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureViewBuilder);
    DEFAULT_DESTRUCT(GLTextureViewBuilder);
    DEFAULT_CM_PU(GLTextureViewBuilder);
public:
    [[nodiscard]] GLTextureView* build(const TextureViewArgs& args, Error* error, DescriptorTable table, uSys tableIndex) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs1D(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, GLResourceTexture* texture, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs1DArray(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, GLResourceTexture* texture, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs2D(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, GLResourceTexture* texture, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs2DArray(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, GLResourceTexture* texture, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs3D(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, GLResourceTexture* texture, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgsCube(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, GLResourceTexture* texture, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgsCubeArray(const TextureViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, GLResourceTexture* texture, [[tau::out]] Error* error) const noexcept;
};
