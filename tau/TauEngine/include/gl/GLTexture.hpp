#pragma once

#include "texture/Texture.hpp"
#include "GLRenderTarget.hpp"
#include "GLTextureView.hpp"

class TAU_DLL GLTexture2D final : public ITexture2D
{
    TEXTURE_IMPL(GLTexture2D);
public:
    static GLint glInternalFormat(ETexture::Format format) noexcept;
    static GLint glFilterType(ETexture::Filter filterType) noexcept;
    static GLint glWrapMode(ETexture::WrapMode wrapMode) noexcept;
    static GLint glDepthCompareFunc(ETexture::CompareFunc func) noexcept;
    static GLenum glInputFormat(ETexture::Format format) noexcept;
    static GLenum glInputDataType(ETexture::Format format) noexcept;
private:
    GLuint _texture;
    GLRenderTarget _renderTarget;
    GLTextureView _textureView;
public:
    inline GLTexture2D(const u32 width, const u32 height, const ETexture::Format dataFormat, const GLuint texture) noexcept
        : ITexture2D(width, height, dataFormat)
        , _texture(texture)
        , _renderTarget(texture, GL_TEXTURE_2D)
        , _textureView(texture, GL_TEXTURE_2D)
    { }

    ~GLTexture2D() noexcept;

    [[nodiscard]] inline GLuint texture() const noexcept { return _texture; }

    [[nodiscard]] const GLRenderTarget* renderTarget() const noexcept override { return &_renderTarget; }
    [[nodiscard]]       GLRenderTarget* renderTarget()       noexcept override { return &_renderTarget; }
    [[nodiscard]] const GLTextureView*   textureView() const noexcept override { return &_textureView;  }
    [[nodiscard]]       GLTextureView*   textureView()       noexcept override { return &_textureView;  }

    [[nodiscard]] u64 _getHandle() const noexcept override { return _texture; }

    void set(u32 mipLevel, const void* data) const noexcept;
    void set(IRenderingContext&, u32 mipLevel, const void* data) noexcept override
    { set(mipLevel, data); }
};

class TAU_DLL GLTexture3D final : public ITexture3D
{
    TEXTURE_IMPL(GLTexture3D);
private:
    GLuint _texture;
    GLRenderTarget _renderTarget;
    GLTextureView _textureView;
public:
    inline GLTexture3D(const u32 width, const u32 height, const u32 depth, const ETexture::Format dataFormat, const GLuint texture) noexcept
        : ITexture3D(width, height, depth, dataFormat)
        , _texture(texture)
        , _renderTarget(texture, GL_TEXTURE_3D)
        , _textureView(texture, GL_TEXTURE_3D)
    { }

    ~GLTexture3D() noexcept;

    [[nodiscard]] inline GLuint texture() const noexcept { return _texture; }

    [[nodiscard]] const GLRenderTarget* renderTarget() const noexcept override { return &_renderTarget; }
    [[nodiscard]]       GLRenderTarget* renderTarget()       noexcept override { return &_renderTarget; }
    [[nodiscard]] const GLTextureView*   textureView() const noexcept override { return &_textureView;  }
    [[nodiscard]]       GLTextureView*   textureView()       noexcept override { return &_textureView;  }

    [[nodiscard]] u64 _getHandle() const noexcept override { return _texture; }

    void set(u32 depthLevel, u32 mipLevel, const void* data) const noexcept;
    void set(IRenderingContext&, u32 depthLevel, u32 mipLevel, const void* data) noexcept override
    { set(depthLevel, mipLevel, data); }
};

class TAU_DLL GLTextureCube final : public ITextureCube
{
    TEXTURE_IMPL(GLTextureCube);
public:
    static GLenum glCubeMapFace(ETexture::CubeSide cubeSide) noexcept;
private:
    GLuint _texture;
    GLRenderTarget _renderTarget;
    GLTextureView _textureView;
public:
    GLTextureCube(const u32 width, const u32 height, const ETexture::Format dataFormat, const GLuint texture) noexcept
        : ITextureCube(width, height, dataFormat)
        , _texture(texture)
        , _renderTarget(texture, GL_TEXTURE_CUBE_MAP)
        , _textureView(texture, GL_TEXTURE_CUBE_MAP)
    { }

    ~GLTextureCube() noexcept override;

    [[nodiscard]] inline GLuint texture() const noexcept { return _texture; }

    [[nodiscard]] const GLRenderTarget* renderTarget() const noexcept override { return &_renderTarget; }
    [[nodiscard]]       GLRenderTarget* renderTarget()       noexcept override { return &_renderTarget; }
    [[nodiscard]] const GLTextureView*   textureView() const noexcept override { return &_textureView;  }
    [[nodiscard]]       GLTextureView*   textureView()       noexcept override { return &_textureView;  }

    [[nodiscard]] u64 _getHandle() const noexcept override { return _texture; }

    void set(u32 mipLevel, ETexture::CubeSide side, const void* data) const noexcept;
    void set(IRenderingContext&, u32 mipLevel, ETexture::CubeSide side, const void* data) noexcept override
    { set(mipLevel, side, data); }
};

class TAU_DLL GLTextureDepthStencil final : public ITextureDepthStencil
{
    TEXTURE_IMPL(GLTextureDepthStencil);
private:
    GLuint _texture;
    GLRenderTarget _renderTarget;
    GLNoMipmapTextureView _depthView;
    GLNoMipmapTextureView _stencilView;
public:
    GLTextureDepthStencil(const u32 width, const u32 height, const GLuint texture) noexcept
        : ITextureDepthStencil(width, height)
        , _texture(texture)
        , _renderTarget(texture, GL_TEXTURE_2D)
        , _depthView(texture, GL_TEXTURE_2D)
        , _stencilView(texture, GL_TEXTURE_2D)
    { }

    ~GLTextureDepthStencil() noexcept;

    [[nodiscard]] inline GLuint texture() const noexcept { return _texture; }

    [[nodiscard]] const GLRenderTarget*       renderTarget() const noexcept override { return &_renderTarget; }
    [[nodiscard]]       GLRenderTarget*       renderTarget()       noexcept override { return &_renderTarget; }
    [[nodiscard]] const GLNoMipmapTextureView*   depthView() const noexcept override { return &_depthView;    }
    [[nodiscard]]       GLNoMipmapTextureView*   depthView()       noexcept override { return &_depthView;    }
    [[nodiscard]] const GLNoMipmapTextureView* stencilView() const noexcept override { return &_stencilView;  }
    [[nodiscard]]       GLNoMipmapTextureView* stencilView()       noexcept override { return &_stencilView;  }

    [[nodiscard]] u64 _getHandle() const noexcept override { return _texture; }

    void set(const void* data) const noexcept;
    void set(IRenderingContext&, const void* data) noexcept override
    { set(data); }
};

class TAU_DLL GLTextureBuilder final : public ITextureBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureBuilder);
    DEFAULT_DESTRUCT(GLTextureBuilder);
    DELETE_COPY(GLTextureBuilder);
public:
    struct GLTexture2DArgs final
    {
        GLuint texture;
    };

    using GLTexture3DArgs = GLTexture2DArgs;
    using GLTextureCubeArgs = GLTexture2DArgs;
    using GLTextureDepthStencilArgs = GLTexture2DArgs;
public:
    [[nodiscard]] GLTexture2D* build(const Texture2DArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLTexture2D* build(const Texture2DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITexture2D> buildCPPRef(const Texture2DArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITexture2D> buildTauRef(const Texture2DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITexture2D> buildTauSRef(const Texture2DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] GLTexture3D* build(const Texture3DArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLTexture3D* build(const Texture3DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITexture3D> buildCPPRef(const Texture3DArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITexture3D> buildTauRef(const Texture3DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITexture3D> buildTauSRef(const Texture3DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] GLTextureCube* build(const TextureCubeArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLTextureCube* build(const TextureCubeArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureCube> buildCPPRef(const TextureCubeArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureCube> buildTauRef(const TextureCubeArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureCube> buildTauSRef(const TextureCubeArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] GLTextureDepthStencil* build(const TextureDepthStencilArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLTextureDepthStencil* build(const TextureDepthStencilArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureDepthStencil> buildCPPRef(const TextureDepthStencilArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureDepthStencil> buildTauRef(const TextureDepthStencilArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureDepthStencil> buildTauSRef(const TextureDepthStencilArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] static bool processArgs(const Texture2DArgs& args, [[tau::out]] GLTexture2DArgs* glArgs, [[tau::out]] Error* error) noexcept;
    [[nodiscard]] static bool processArgs(const Texture3DArgs& args, [[tau::out]] GLTexture3DArgs* glArgs, [[tau::out]] Error* error) noexcept;
    [[nodiscard]] static bool processArgs(const TextureCubeArgs& args, [[tau::out]] GLTextureCubeArgs* glArgs, [[tau::out]] Error* error) noexcept;
    [[nodiscard]] static bool processArgs(const TextureDepthStencilArgs& args, [[tau::out]] GLTextureDepthStencilArgs* glArgs, [[tau::out]] Error* error) noexcept;
};
