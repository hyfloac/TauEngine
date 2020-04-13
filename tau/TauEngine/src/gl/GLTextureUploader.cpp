#include "gl/GLTextureUploader.hpp"
#include "gl/GLTextureSampler.hpp"
#include "gl/GLTexture.hpp"

GLSingleTextureUploader::GLSingleTextureUploader(ITextureView* const texture, const CPPRef<GLTextureSampler>& textureSampler) noexcept
    : ISingleTextureUploader(texture, textureSampler)
{ }

TextureIndices GLSingleTextureUploader::upload(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    const auto glSampler = RefCast<GLTextureSampler>(_textureSampler);

    const GLuint textureHandle = reinterpret_cast<GLTextureView*>(_texture)->texture();
    const GLenum glType = reinterpret_cast<GLTextureView*>(_texture)->glType();

    glUniform1i(indices.uniformIndex, indices.textureStartIndex);
    glActiveTexture(GL_TEXTURE0 + indices.textureStartIndex);
    glBindTexture(glType, textureHandle);
    glSampler->apply(glType);

    return TextureIndices(indices.textureStartIndex + 1, indices.samplerStartIndex, indices.uniformIndex + 1);
}

TextureIndices GLSingleTextureUploader::unbind(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    const GLenum glType = reinterpret_cast<GLTextureView*>(_texture)->glType();

    glActiveTexture(GL_TEXTURE0 + indices.textureStartIndex);
    glBindTexture(glType, 0);
    return TextureIndices(indices.textureStartIndex + 1, indices.samplerStartIndex, indices.uniformIndex + 1);
}

GLTextureUploader::GLTextureUploader(const RefDynArray<ITextureView*>& textures, const CPPRef<GLTextureSampler>& textureSampler) noexcept
    : ITextureUploader(textures, textureSampler)
{ }

TextureIndices GLTextureUploader::upload(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    const auto glSampler = RefCast<GLTextureSampler>(_textureSampler);

    for(uSys i = 0; i < _textures.size(); ++i)
    {
        auto& texture = _textures[i + indices.textureStartIndex];

        const GLuint textureHandle = reinterpret_cast<GLTextureView*>(texture)->texture();
        const GLenum glType = reinterpret_cast<GLTextureView*>(texture)->glType();

        glUniform1i(i + indices.uniformIndex, i + indices.textureStartIndex);
        glActiveTexture(GL_TEXTURE0 + i + indices.textureStartIndex);
        glBindTexture(glType, textureHandle);
        glSampler->apply(glType);
    }

    return TextureIndices(indices.textureStartIndex + _textures.size(), indices.samplerStartIndex, indices.uniformIndex + _textures.size());
}

TextureIndices GLTextureUploader::unbind(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    for(uSys i = 0; i < _textures.size(); ++i)
    {
        const GLenum glType = reinterpret_cast<GLTextureView*>(_textures[i])->glType();
        glActiveTexture(GL_TEXTURE0 + i + indices.textureStartIndex);
        glBindTexture(glType, 0);
    }

    return TextureIndices(indices.textureStartIndex + _textures.size(), indices.samplerStartIndex, indices.uniformIndex + _textures.size());
}

GLSingleTextureUploader* GLSingleTextureUploaderBuilder::build(const SingleTextureUploaderArgs& args, Error* const error) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    GLSingleTextureUploader* const uploader = new(::std::nothrow) GLSingleTextureUploader(args.texture, RefCast<GLTextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

GLSingleTextureUploader* GLSingleTextureUploaderBuilder::build(const SingleTextureUploaderArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    GLSingleTextureUploader* const uploader = allocator.allocateT<GLSingleTextureUploader>(args.texture, RefCast<GLTextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

CPPRef<ISingleTextureUploader> GLSingleTextureUploaderBuilder::buildCPPRef(const SingleTextureUploaderArgs& args, Error* const error) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const CPPRef<GLSingleTextureUploader> uploader = CPPRef<GLSingleTextureUploader>(new(::std::nothrow) GLSingleTextureUploader(args.texture, RefCast<GLTextureSampler>(args.textureSampler)));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

NullableRef<ISingleTextureUploader> GLSingleTextureUploaderBuilder::buildTauRef(const SingleTextureUploaderArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const NullableRef<GLSingleTextureUploader> uploader(allocator, args.texture, RefCast<GLTextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

NullableStrongRef<ISingleTextureUploader> GLSingleTextureUploaderBuilder::buildTauSRef(const SingleTextureUploaderArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const NullableStrongRef<GLSingleTextureUploader> uploader(allocator, args.texture, RefCast<GLTextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

bool GLSingleTextureUploaderBuilder::processArgs(const SingleTextureUploaderArgs& args, Error* const error) noexcept
{
    ERROR_CODE_COND_F(!args.textureSampler, Error::TextureSamplerNotSet);
    ERROR_CODE_COND_F(!RTT_CHECK(args.textureSampler.get(), GLTextureSampler), Error::CrossAPIFailure);
    ERROR_CODE_COND_F(!args.texture, Error::TextureNotSet);

    ERROR_CODE_COND_F(!RTT_CHECK(args.texture, GLTextureView), Error::CrossAPIFailure);

    return true;
}

GLTextureUploader* GLTextureUploaderBuilder::build(const TextureUploaderArgs& args, Error* const error) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    GLTextureUploader* const uploader = new(::std::nothrow) GLTextureUploader(args.textures, RefCast<GLTextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

GLTextureUploader* GLTextureUploaderBuilder::build(const TextureUploaderArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    GLTextureUploader* const uploader = allocator.allocateT<GLTextureUploader>(args.textures, RefCast<GLTextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

CPPRef<ITextureUploader> GLTextureUploaderBuilder::buildCPPRef(const TextureUploaderArgs& args, Error* const error) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const CPPRef<GLTextureUploader> uploader = CPPRef<GLTextureUploader>(new(::std::nothrow) GLTextureUploader(args.textures, RefCast<GLTextureSampler>(args.textureSampler)));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

NullableRef<ITextureUploader> GLTextureUploaderBuilder::buildTauRef(const TextureUploaderArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const NullableRef<GLTextureUploader> uploader(allocator, args.textures, RefCast<GLTextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

NullableStrongRef<ITextureUploader> GLTextureUploaderBuilder::buildTauSRef(const TextureUploaderArgs& args, Error* const error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const NullableStrongRef<GLTextureUploader> uploader(allocator, args.textures, RefCast<GLTextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

bool GLTextureUploaderBuilder::processArgs(const TextureUploaderArgs& args, Error* const error) noexcept
{
    ERROR_CODE_COND_F(!args.textureSampler, Error::TextureSamplerNotSet);
    ERROR_CODE_COND_F(!RTT_CHECK(args.textureSampler.get(), GLTextureSampler), Error::CrossAPIFailure);

    ERROR_CODE_COND_F(args.textures.size() == 0, Error::ZeroTextures);

    for(const auto texture : args.textures)
    {
        ERROR_CODE_COND_F(!texture, Error::TextureNotSet);
        ERROR_CODE_COND_F(!RTT_CHECK(texture, GLTextureView), Error::CrossAPIFailure);
    }

    return true;
}
