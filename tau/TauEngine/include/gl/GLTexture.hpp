#pragma once

#include <texture/Texture.hpp>
#include <GL/glew.h>

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
    GLTexture2D(u32 width, u32 height, ETexture::Format dataFormat) noexcept;

    virtual ~GLTexture2D() noexcept override;

    [[nodiscard]] inline GLuint texture() const noexcept { return _texture; }

    [[nodiscard]] inline ETexture::Type textureType() const noexcept override final { return ETexture::Type::T2D; }

    void setFilterMode(ETexture::Filter minificationFilter, ETexture::Filter magnificationFilter) noexcept override final;

    void setWrapMode(ETexture::WrapMode s, ETexture::WrapMode t) noexcept override final;

    void generateMipmaps() noexcept override;

    void set(const void* data) noexcept override;

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
    GLDepthTexture(u32 width, u32 height, ETexture::Format dataFormat) noexcept;

    void setDepthComparison(bool enableDepthTest, ETexture::DepthCompareFunc compareFunc) noexcept override final;

    void set(const void* data) noexcept override final;
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
    GLTextureCube(u32 width, u32 height, ETexture::Format dataFormat) noexcept;

    ~GLTextureCube() noexcept override;

    [[nodiscard]] inline GLuint texture() const noexcept { return _texture; }

    void setWrapModeCube(ETexture::WrapMode s, ETexture::WrapMode t, ETexture::WrapMode r) noexcept override;

    virtual void setCube(ETexture::CubeSide side, const void* data) noexcept override final;

    void setFilterMode(ETexture::Filter minificationFilter, ETexture::Filter magnificationFilter) noexcept override final;

    void setWrapMode(ETexture::WrapMode s, ETexture::WrapMode t) noexcept override final { }

    void generateMipmaps() noexcept override;

    virtual void set(const void*) noexcept override final { }

    virtual void bind(u8 textureUnit) noexcept override final;

    virtual void unbind(u8 textureUnit) noexcept override final;
};
