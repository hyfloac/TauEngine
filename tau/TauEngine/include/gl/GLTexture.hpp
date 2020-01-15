#pragma once

#include <texture/Texture.hpp>
#include <GL/glew.h>

class TAU_DLL GLNullTexture final : public ITexture
{
    DEFAULT_DESTRUCT(GLNullTexture);
    DELETE_COPY(GLNullTexture);
public:
    GLNullTexture() noexcept
        : ITexture(0, 0, ETexture::Format::Red8UnsignedInt)
    { }

    [[nodiscard]] inline ETexture::Type textureType() const noexcept override { return ETexture::Type::T2D; }
    void setFilterMode(ETexture::Filter minificationFilter, ETexture::Filter magnificationFilter) noexcept override { }
    void setWrapMode(ETexture::WrapMode s, ETexture::WrapMode t) noexcept override { }
    void setDepthComparison(bool enableDepthTest, ETexture::DepthCompareFunc compareFunc) noexcept override { }
    void set(u32 level, const void* data) noexcept override { }

    void bind(u8 textureUnit) noexcept override
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void unbind(u8 textureUnit) noexcept override { }
    void generateMipmaps() noexcept override { }
};

class TAU_DLL GLTexture2D : public ITexture
{
    DELETE_COPY(GLTexture2D);
public:
    static GLint glInternalFormat(ETexture::Format format) noexcept;
    static GLint glFilterType(ETexture::Filter filterType) noexcept;
    static GLint glWrapMode(ETexture::WrapMode wrapMode) noexcept;
    static GLint glDepthCompareFunc(ETexture::DepthCompareFunc func) noexcept;
    static GLenum glInputFormat(ETexture::Format format) noexcept;
    static GLenum glInputDataType(ETexture::Format format) noexcept;
protected:
    GLuint _texture;
    GLint _minFilter;
    GLint _magFilter;
    GLint _wrapS;
    GLint _wrapT;
public:
    GLTexture2D(u32 width, u32 height, ETexture::Format dataFormat, GLuint texture) noexcept;

    virtual ~GLTexture2D() noexcept override;

    [[nodiscard]] inline GLuint texture() const noexcept { return _texture; }

    [[nodiscard]] inline ETexture::Type textureType() const noexcept override final { return ETexture::Type::T2D; }

    void setFilterMode(ETexture::Filter minificationFilter, ETexture::Filter magnificationFilter) noexcept override final;

    void setWrapMode(ETexture::WrapMode s, ETexture::WrapMode t) noexcept override final;

    void generateMipmaps() noexcept override;

    void set(u32 level, const void* data) noexcept override;

    void bind(u8 textureUnit) noexcept override final;

    void unbind(u8 textureUnit) noexcept override final;
};

class TAU_DLL GLDepthTexture final : public GLTexture2D
{
    DEFAULT_DESTRUCT(GLDepthTexture);
    DELETE_COPY(GLDepthTexture);
private:
    GLint _depthCompareMode;
    GLint _depthCompareFunc;
public:
    GLDepthTexture(u32 width, u32 height, ETexture::Format dataFormat, GLuint texture) noexcept;

    void setDepthComparison(bool enableDepthTest, ETexture::DepthCompareFunc compareFunc) noexcept override final;

    void set(u32 level, const void* data) noexcept override final;
};

class TAU_DLL GLTextureCube final : public ITextureCube
{
    DELETE_COPY(GLTextureCube);
public:
    static GLenum glCubeMapFace(ETexture::CubeSide cubeSide) noexcept;
private:
    GLuint _texture;
    GLint _minFilter;
    GLint _magFilter;
    GLint _wrapS;
    GLint _wrapT;
    GLint _wrapR;
public:
    GLTextureCube(u32 width, u32 height, ETexture::Format dataFormat, GLuint texture) noexcept;

    ~GLTextureCube() noexcept override;

    [[nodiscard]] inline GLuint texture() const noexcept { return _texture; }

    void setWrapModeCube(ETexture::WrapMode s, ETexture::WrapMode t, ETexture::WrapMode r) noexcept override;

    virtual void setCube(u32 level, ETexture::CubeSide side, const void* data) noexcept override final;

    void setFilterMode(ETexture::Filter minificationFilter, ETexture::Filter magnificationFilter) noexcept override final;

    void setWrapMode(ETexture::WrapMode s, ETexture::WrapMode t) noexcept override final { }

    void generateMipmaps() noexcept override;

    virtual void set(u32 level, const void*) noexcept override final { }

    virtual void bind(u8 textureUnit) noexcept override final;

    virtual void unbind(u8 textureUnit) noexcept override final;
};

class TAU_DLL GLTexture2DBuilder final : public ITextureBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTexture2DBuilder);
    DEFAULT_DESTRUCT(GLTexture2DBuilder);
    DELETE_COPY(GLTexture2DBuilder);
public:
    [[nodiscard]] ITexture* build([[tau::out]] Error* error) const noexcept override;
};

class TAU_DLL GLTextureNullBuilder final : public ITextureBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureNullBuilder);
    DEFAULT_DESTRUCT(GLTextureNullBuilder);
    DELETE_COPY(GLTextureNullBuilder);
public:
    [[nodiscard]] ITexture* build([[tau::out]] Error* error) const noexcept override;
};

class TAU_DLL GLTextureDepthBuilder final : public ITextureBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureDepthBuilder);
    DEFAULT_DESTRUCT(GLTextureDepthBuilder);
    DELETE_COPY(GLTextureDepthBuilder);
public:
    [[nodiscard]] ITexture* build([[tau::out]] Error* error) const noexcept override;
};

class TAU_DLL GLTextureCubeBuilder final : public ITextureCubeBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureCubeBuilder);
    DEFAULT_DESTRUCT(GLTextureCubeBuilder);
    DELETE_COPY(GLTextureCubeBuilder);
public:
    [[nodiscard]] ITextureCube* build([[tau::out]] Error* error) const noexcept override;
};
