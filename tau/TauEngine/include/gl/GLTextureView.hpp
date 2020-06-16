#pragma once

#include "texture/TextureView.hpp"

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

struct GLTextureViewArgs final
{
    ETexture::Format dataFormat;
    GLenum target;
    GLuint texture;
    u32 width;
    u32 height;
    u32 depth;
    u32 mipLevels;
    u32 arrayCount;
};

class TAU_DLL TAU_NOVTABLE GLTextureView : public ITextureView
{
    DEFAULT_DESTRUCT_VI(GLTextureView);
    DELETE_CM(GLTextureView);
    TEXTURE_VIEW_IMPL(GLTextureView);
protected:
    ETexture::Format _dataFormat;
    u32 _mipCount;
    GLenum _target;
    GLuint _texture;
protected:
    inline GLTextureView(const GLTextureViewArgs& args) noexcept
        : _dataFormat(args.dataFormat)
        , _mipCount(args.mipLevels)
        , _target(args.target)
        , _texture(args.texture)
    { }
public:
    [[nodiscard]] ETexture::Format dataFormat() const noexcept override { return _dataFormat; }

    [[nodiscard]] u32 mipCount() const noexcept override { return _mipCount; }

    [[nodiscard]] inline GLuint texture() const noexcept { return _texture; }
    [[nodiscard]] inline GLenum  glType() const noexcept { return _target;  }

    void generateMipmaps(IRenderingContext&) noexcept override;
};

class TAU_DLL GLTexture1DView : public GLTextureView
{
    DEFAULT_DESTRUCT_VI(GLTexture1DView);
    DELETE_CM(GLTexture1DView);
protected:
    u32 _width;
public:
    GLTexture1DView(const GLTextureViewArgs& args) noexcept
        : GLTextureView(args)
        , _width(args.width)
    { }

    [[nodiscard]] ETexture::Type textureType() const noexcept override { return ETexture::Type::Texture1D; }

    [[nodiscard]] u32 width() const noexcept override { return _width; }
};

class TAU_DLL GLTexture1DArrayView : public GLTextureView
{
    DEFAULT_DESTRUCT_VI(GLTexture1DArrayView);
    DELETE_CM(GLTexture1DArrayView);
protected:
    u32 _width;
    u32 _arrayCount;
public:
    GLTexture1DArrayView(const GLTextureViewArgs& args) noexcept
        : GLTextureView(args)
        , _width(args.width)
        , _arrayCount(args.arrayCount)
    { }

    [[nodiscard]] ETexture::Type textureType() const noexcept override { return ETexture::Type::Texture1DArray; }

    [[nodiscard]] u32      width() const noexcept override { return _width;      }
    [[nodiscard]] u32 arrayCount() const noexcept override { return _arrayCount; }
};

class TAU_DLL GLTexture2DView : public GLTextureView
{
    DEFAULT_DESTRUCT_VI(GLTexture2DView);
    DELETE_CM(GLTexture2DView);
protected:
    u32 _width;
    u32 _height;
public:
    GLTexture2DView(const GLTextureViewArgs& args) noexcept
        : GLTextureView(args)
        , _width(args.width)
        , _height(args.height)
    { }

    [[nodiscard]] ETexture::Type textureType() const noexcept override { return ETexture::Type::Texture2D; }

    [[nodiscard]] u32  width() const noexcept override { return _width;  }
    [[nodiscard]] u32 height() const noexcept override { return _height; }
};

class TAU_DLL GLTexture2DArrayView : public GLTextureView
{
    DEFAULT_DESTRUCT_VI(GLTexture2DArrayView);
    DELETE_CM(GLTexture2DArrayView);
protected:
    u32 _width;
    u32 _height;
    u32 _arrayCount;
public:
    GLTexture2DArrayView(const GLTextureViewArgs& args) noexcept
        : GLTextureView(args)
        , _width(args.width)
        , _height(args.height)
        , _arrayCount(args.arrayCount)
    { }

    [[nodiscard]] ETexture::Type textureType() const noexcept override { return ETexture::Type::Texture2DArray; }

    [[nodiscard]] u32      width() const noexcept override { return _width;      }
    [[nodiscard]] u32     height() const noexcept override { return _height;     }
    [[nodiscard]] u32 arrayCount() const noexcept override { return _arrayCount; }
};

class TAU_DLL GLTexture3DView : public GLTextureView
{
    DEFAULT_DESTRUCT_VI(GLTexture3DView);
    DELETE_CM(GLTexture3DView);
protected:
    u32 _width;
    u32 _height;
    u32 _depth;
public:
    GLTexture3DView(const GLTextureViewArgs& args) noexcept
        : GLTextureView(args)
        , _width(args.width)
        , _height(args.height)
        , _depth(args.depth)
    { }

    [[nodiscard]] ETexture::Type textureType() const noexcept override { return ETexture::Type::Texture3D; }

    [[nodiscard]] u32  width() const noexcept override { return _width;  }
    [[nodiscard]] u32 height() const noexcept override { return _height; }
    [[nodiscard]] u32  depth() const noexcept override { return _depth;  }
};

class TAU_DLL GLTextureCubeView : public GLTextureView
{
    DEFAULT_DESTRUCT_VI(GLTextureCubeView);
    DELETE_CM(GLTextureCubeView);
protected:
    u32 _width;
    u32 _height;
public:
    GLTextureCubeView(const GLTextureViewArgs& args) noexcept
        : GLTextureView(args)
        , _width(args.width)
        , _height(args.height)
    { }

    [[nodiscard]] ETexture::Type textureType() const noexcept override { return ETexture::Type::TextureCube; }

    [[nodiscard]] u32  width() const noexcept override { return _width;  }
    [[nodiscard]] u32 height() const noexcept override { return _height; }
};

class TAU_DLL GLTextureCubeArrayView : public GLTextureView
{
    DEFAULT_DESTRUCT_VI(GLTextureCubeArrayView);
    DELETE_CM(GLTextureCubeArrayView);
protected:
    u32 _width;
    u32 _height;
    u32 _arrayCount;
public:
    GLTextureCubeArrayView(const GLTextureViewArgs& args) noexcept
        : GLTextureView(args)
        , _width(args.width)
        , _height(args.height)
        , _arrayCount(args.arrayCount)
    { }

    [[nodiscard]] ETexture::Type textureType() const noexcept override { return ETexture::Type::TextureCubeArray; }

    [[nodiscard]] u32      width() const noexcept override { return _width;      }
    [[nodiscard]] u32     height() const noexcept override { return _height;     }
    [[nodiscard]] u32 arrayCount() const noexcept override { return _arrayCount; }
};

#define GL_NO_MIPMAP_TEXTURE_VIEW(_BASE, _TYPE) \
    class TAU_DLL _TYPE final : public _BASE { \
        DEFAULT_DESTRUCT(_TYPE); \
        DELETE_CM(_TYPE); \
    public: \
        _TYPE(const GLTextureViewArgs& args) noexcept : _BASE(args) { } \
        void generateMipmaps(IRenderingContext&) noexcept override { } \
    }

GL_NO_MIPMAP_TEXTURE_VIEW(GLTexture1DView, GLNoMipmapTexture1DView);
GL_NO_MIPMAP_TEXTURE_VIEW(GLTexture1DArrayView, GLNoMipmapTexture1DArrayView);
GL_NO_MIPMAP_TEXTURE_VIEW(GLTexture2DView, GLNoMipmapTexture2DView);
GL_NO_MIPMAP_TEXTURE_VIEW(GLTexture2DArrayView, GLNoMipmapTexture2DArrayView);
GL_NO_MIPMAP_TEXTURE_VIEW(GLTexture3DView, GLNoMipmapTexture3DView);
GL_NO_MIPMAP_TEXTURE_VIEW(GLTextureCubeView, GLNoMipmapTextureCubeDView);
GL_NO_MIPMAP_TEXTURE_VIEW(GLTextureCubeArrayView, GLNoMipmapTextureCubeArrayView);

class TAU_DLL GLTextureViewBuilder final : public ITextureViewBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureViewBuilder);
    DEFAULT_DESTRUCT(GLTextureViewBuilder);
    DEFAULT_CM_PU(GLTextureViewBuilder);
public:
    [[nodiscard]] GLTexture1DView* build(const Texture1DViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLTexture1DView* build(const Texture1DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureView> buildCPPRef(const Texture1DViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureView> buildTauRef(const Texture1DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureView> buildTauSRef(const Texture1DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] GLTexture1DArrayView* build(const Texture1DArrayViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLTexture1DArrayView* build(const Texture1DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureView> buildCPPRef(const Texture1DArrayViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureView> buildTauRef(const Texture1DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureView> buildTauSRef(const Texture1DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] GLTexture2DView* build(const Texture2DViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLTexture2DView* build(const Texture2DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureView> buildCPPRef(const Texture2DViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureView> buildTauRef(const Texture2DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureView> buildTauSRef(const Texture2DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] GLTexture2DArrayView* build(const Texture2DArrayViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLTexture2DArrayView* build(const Texture2DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureView> buildCPPRef(const Texture2DArrayViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureView> buildTauRef(const Texture2DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureView> buildTauSRef(const Texture2DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] GLTexture3DView* build(const Texture3DViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLTexture3DView* build(const Texture3DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureView> buildCPPRef(const Texture3DViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureView> buildTauRef(const Texture3DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureView> buildTauSRef(const Texture3DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] GLTextureCubeView* build(const TextureCubeViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLTextureCubeView* build(const TextureCubeViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureView> buildCPPRef(const TextureCubeViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureView> buildTauRef(const TextureCubeViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureView> buildTauSRef(const TextureCubeViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] GLTextureCubeArrayView* build(const TextureCubeArrayViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLTextureCubeArrayView* build(const TextureCubeArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureView> buildCPPRef(const TextureCubeArrayViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureView> buildTauRef(const TextureCubeArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureView> buildTauSRef(const TextureCubeArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const Texture1DViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const Texture1DArrayViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const Texture2DViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const Texture2DArrayViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const Texture3DViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const TextureCubeViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const TextureCubeArrayViewArgs& args, [[tau::out]] GLTextureViewArgs* glArgs, [[tau::out]] Error* error) const noexcept;
};
