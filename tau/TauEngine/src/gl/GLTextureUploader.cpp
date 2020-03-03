#include "gl/GLTextureUploader.hpp"
#include "gl/GLTextureSampler.hpp"
#include "gl/GLRenderingContext.hpp"

TextureIndices& GLSingleTextureUploader::upload(IRenderingContext& context, TextureIndices& textureIndices, const EShader::Stage stage) noexcept
{
    const auto glSampler = RefCast<GLTextureSampler>(_textureSampler);

    glUniform1i(textureIndices.uniformIndex, textureIndices.textureStartIndex);
    _texture->bind(textureIndices.textureStartIndex, stage);
    switch(_texture->textureType())
    {
        case ETexture::Type::T2D:
        case ETexture::Type::Depth:
            glSampler->apply(GL_TEXTURE_2D);
            break;
        case ETexture::Type::T3D:
            glSampler->apply(GL_TEXTURE_3D);
            break;
        case ETexture::Type::Cube:
            glSampler->apply(GL_TEXTURE_CUBE_MAP);
            break;
    }

    ++textureIndices.textureStartIndex;
    ++textureIndices.uniformIndex;
    return textureIndices;
}

TextureIndices& GLSingleTextureUploader::unbind(IRenderingContext& context, TextureIndices& textureIndices, const EShader::Stage stage) noexcept
{
    _texture->unbind(textureIndices.textureStartIndex, stage);

    ++textureIndices.textureStartIndex;
    ++textureIndices.uniformIndex;
    return textureIndices;
}

TextureIndices& GLTextureUploader::upload(IRenderingContext& context, TextureIndices& textureIndices, const EShader::Stage stage) noexcept
{
    const auto glSampler = RefCast<GLTextureSampler>(_textureSampler);

    for(uSys i = 0; i < _textures.size(); ++i)
    {
        auto& texture = _textures[i + textureIndices.textureStartIndex];

        glUniform1i(i + textureIndices.uniformIndex, i + textureIndices.textureStartIndex);
        texture->bind(i + textureIndices.textureStartIndex, stage);
        switch(texture->textureType())
        {
            case ETexture::Type::T2D:
            case ETexture::Type::Depth:
                glSampler->apply(GL_TEXTURE_2D);
                break;
            case ETexture::Type::T3D:
                glSampler->apply(GL_TEXTURE_3D);
                break;
            case ETexture::Type::Cube:
                glSampler->apply(GL_TEXTURE_CUBE_MAP);
                break;
        }
    }

    textureIndices.textureStartIndex += _textures.size();
    textureIndices.uniformIndex += _textures.size();
    return textureIndices;
}

TextureIndices& GLTextureUploader::unbind(IRenderingContext& context, TextureIndices& textureIndices, const EShader::Stage stage) noexcept
{
    for(uSys i = 0; i < _textures.size(); ++i)
    {
        auto& texture = _textures[i + textureIndices.samplerStartIndex];

        texture->unbind(i + textureIndices.textureStartIndex, stage);
    }

    textureIndices.textureStartIndex += _textures.size();
    textureIndices.uniformIndex += _textures.size();
    return textureIndices;
}


GLSingleTextureUploaderBuilder::GLSingleTextureUploaderBuilder(GLRenderingContext& context) noexcept
    : ISingleTextureUploaderBuilder()
{ }

void GLSingleTextureUploaderBuilder::textureSampler(const CPPRef<ITextureSampler>& textureSampler) noexcept
{
    if(RTT_CHECK(textureSampler.get(), GLTextureSampler))
    {
        ISingleTextureUploaderBuilder::textureSampler(textureSampler);
    }
}

GLSingleTextureUploader* GLSingleTextureUploaderBuilder::build(Error* error) noexcept
{
    GLSingleTextureUploader* const uploader = new(::std::nothrow) GLSingleTextureUploader(_texture, _textureSampler);
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

GLTextureUploaderBuilder::GLTextureUploaderBuilder(const uSys textureCount, GLRenderingContext& context) noexcept
    : ITextureUploaderBuilder(textureCount)
{ }

void GLTextureUploaderBuilder::textureSampler(const CPPRef<ITextureSampler>& textureSampler) noexcept
{
    if(RTT_CHECK(textureSampler.get(), GLTextureSampler))
    {
        ITextureUploaderBuilder::textureSampler(textureSampler);
    }
}

GLTextureUploader* GLTextureUploaderBuilder::build(Error* error) noexcept
{
    GLTextureUploader* const uploader = new(::std::nothrow) GLTextureUploader(_textures, _textureSampler);
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}
