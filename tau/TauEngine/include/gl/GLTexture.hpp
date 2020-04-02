#pragma once

#include "texture/Texture.hpp"
#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

class TAU_DLL GLNullTexture final : public ITexture
{
    DEFAULT_DESTRUCT(GLNullTexture);
    TEXTURE_IMPL(GLNullTexture);
public:
    GLNullTexture() noexcept
        : ITexture(0, 0, ETexture::Format::Red8UnsignedInt)
    { }

    [[nodiscard]] inline ETexture::Type textureType() const noexcept override { return ETexture::Type::T2D; }

    void set(IRenderingContext&, u32 level, const void* data) noexcept override { }

    void bind(IRenderingContext&, u8 textureUnit, EShader::Stage) noexcept override
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void unbind(IRenderingContext&, u8 textureUnit, EShader::Stage) noexcept override { }
    void generateMipmaps(IRenderingContext&) noexcept override { }
};

class TAU_DLL GLTexture2D : public ITexture
{
    TEXTURE_IMPL(GLTexture2D);
public:
    static GLint glInternalFormat(ETexture::Format format) noexcept;
    static GLint glFilterType(ETexture::Filter filterType) noexcept;
    static GLint glWrapMode(ETexture::WrapMode wrapMode) noexcept;
    static GLint glDepthCompareFunc(ETexture::DepthCompareFunc func) noexcept;
    static GLenum glInputFormat(ETexture::Format format) noexcept;
    static GLenum glInputDataType(ETexture::Format format) noexcept;
protected:
    GLuint _texture;
public:
    GLTexture2D(u32 width, u32 height, ETexture::Format dataFormat, GLuint texture) noexcept;

    virtual ~GLTexture2D() noexcept override;

    [[nodiscard]] inline GLuint texture() const noexcept { return _texture; }

    [[nodiscard]] inline ETexture::Type textureType() const noexcept override { return ETexture::Type::T2D; }

    void generateMipmaps(IRenderingContext&) noexcept override;

    void set(IRenderingContext&, u32 level, const void* data) noexcept override;
    virtual void set(u32 level, const void* data) const noexcept;

    void bind(IRenderingContext&, u8 textureUnit, EShader::Stage) noexcept override final;
    void unbind(IRenderingContext&, u8 textureUnit, EShader::Stage) noexcept override final;

    [[nodiscard]] u64 _getHandle() const noexcept override { return _texture; }
};

class TAU_DLL GLDepthTexture final : public GLTexture2D
{
    DEFAULT_DESTRUCT(GLDepthTexture);
    DELETE_COPY(GLDepthTexture);
public:
    GLDepthTexture(u32 width, u32 height, ETexture::Format dataFormat, GLuint texture) noexcept;

    [[nodiscard]] inline ETexture::Type textureType() const noexcept override { return ETexture::Type::Depth; }

    void set(IRenderingContext&, u32 level, const void* data) noexcept override;
    void set(u32 level, const void* data) const noexcept;
};

class TAU_DLL GLTextureCube final : public ITextureCube
{
    TEXTURE_IMPL(GLTextureCube);
public:
    static GLenum glCubeMapFace(ETexture::CubeSide cubeSide) noexcept;
private:
    GLuint _texture;
public:
    GLTextureCube(u32 width, u32 height, ETexture::Format dataFormat, GLuint texture) noexcept;

    ~GLTextureCube() noexcept override;

    [[nodiscard]] inline GLuint texture() const noexcept { return _texture; }

    [[nodiscard]] u64 _getHandle() const noexcept override { return _texture; }

    void setCube(IRenderingContext&, u32 level, ETexture::CubeSide side, const void* data) noexcept override;
    void setCube(u32 level, ETexture::CubeSide side, const void* data) const noexcept;

    void generateMipmaps(IRenderingContext&) noexcept override;

    void bind(IRenderingContext&, u8 textureUnit, EShader::Stage stage) noexcept override;
    void unbind(IRenderingContext&, u8 textureUnit, EShader::Stage stage) noexcept override;
};

class TAU_DLL GLTexture2DBuilder final : public ITextureBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTexture2DBuilder);
    DEFAULT_DESTRUCT(GLTexture2DBuilder);
    DELETE_COPY(GLTexture2DBuilder);
public:
    struct GLTextureArgs final
    {
        GLuint textureHandle;
    };
public:
    [[nodiscard]] GLTexture2D* build(const TextureArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLTexture2D* build(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITexture> buildCPPRef(const TextureArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITexture> buildTauRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITexture> buildTauSRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
public:
    [[nodiscard]] static bool processArgs(const TextureArgs& args, [[tau::out]] GLTextureArgs* glArgs, [[tau::out]] Error* error) noexcept;
};

class TAU_DLL GLTextureNullBuilder final : public ITextureBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureNullBuilder);
    DEFAULT_DESTRUCT(GLTextureNullBuilder);
    DELETE_COPY(GLTextureNullBuilder);
public:
    [[nodiscard]] GLNullTexture* build(const TextureArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLNullTexture* build(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITexture> buildCPPRef(const TextureArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITexture> buildTauRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITexture> buildTauSRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
};

class TAU_DLL GLTextureDepthBuilder final : public ITextureBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureDepthBuilder);
    DEFAULT_DESTRUCT(GLTextureDepthBuilder);
    DELETE_COPY(GLTextureDepthBuilder);
public:
    using GLTextureArgs = GLTexture2DBuilder::GLTextureArgs;
public:
    [[nodiscard]] GLDepthTexture* build(const TextureArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLDepthTexture* build(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITexture> buildCPPRef(const TextureArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITexture> buildTauRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITexture> buildTauSRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
};

class TAU_DLL GLTextureCubeBuilder final : public ITextureCubeBuilder
{
    DEFAULT_CONSTRUCT_PU(GLTextureCubeBuilder);
    DEFAULT_DESTRUCT(GLTextureCubeBuilder);
    DELETE_COPY(GLTextureCubeBuilder);
public:
    using GLTextureArgs = GLTexture2DBuilder::GLTextureArgs;
public:
    [[nodiscard]] GLTextureCube* build(const TextureCubeArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLTextureCube* build(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureCube> buildCPPRef(const TextureCubeArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureCube> buildTauRef(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureCube> buildTauSRef(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] static bool processArgs(const TextureCubeArgs& args, [[tau::out]] GLTextureArgs* glArgs, [[tau::out]] Error* error) noexcept;

    static void setupInitial(GLTextureCube& texture, const void* initialBuffers[6]);
};
