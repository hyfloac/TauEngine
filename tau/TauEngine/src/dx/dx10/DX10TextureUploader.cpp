#include "dx/dx10/DX10TextureUploader.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10Texture.hpp"
#include "dx/dx10/DX10TextureSampler.hpp"
#include "TauEngine.hpp"

TextureIndices& DX10SingleTextureUploader::upload(IRenderingContext& context, TextureIndices& textureIndices, const EShader::Stage stage) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return textureIndices;
    }

    _texture->bind(context, textureIndices.textureStartIndex, stage);
    RefCast<DX10TextureSampler>(_textureSampler)->bind(textureIndices.samplerStartIndex);

    ++textureIndices.textureStartIndex;
    ++textureIndices.samplerStartIndex;
    return textureIndices;
}

TextureIndices& DX10SingleTextureUploader::unbind(IRenderingContext& context, TextureIndices& textureIndices, const EShader::Stage stage) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return textureIndices;
    }

    _texture->unbind(context, textureIndices.textureStartIndex, stage);
    RefCast<DX10TextureSampler>(_textureSampler)->unbind(textureIndices.samplerStartIndex);

    ++textureIndices.textureStartIndex;
    ++textureIndices.samplerStartIndex;
    return textureIndices;
}

TextureIndices& DX10TextureUploader::upload(IRenderingContext& context, TextureIndices& textureIndices, const EShader::Stage stage) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return textureIndices;
    }

    for(uSys i = 0; i < _textures.size(); ++i)
    {
        _textures[i]->bind(context, i + textureIndices.textureStartIndex, stage);
    }
    RefCast<DX10TextureSampler>(_textureSampler)->bind(textureIndices.samplerStartIndex);

    textureIndices.textureStartIndex += _textures.size();
    ++textureIndices.samplerStartIndex;
    return textureIndices;
}

TextureIndices& DX10TextureUploader::unbind(IRenderingContext& context, TextureIndices& textureIndices, const EShader::Stage stage) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return textureIndices;
    }

    for(uSys i = 0; i < _textures.size(); ++i)
    {
        _textures[i]->unbind(context, i + textureIndices.textureStartIndex, stage);
    }
    RefCast<DX10TextureSampler>(_textureSampler)->unbind(textureIndices.samplerStartIndex);

    textureIndices.textureStartIndex += _textures.size();
    ++textureIndices.samplerStartIndex;
    return textureIndices;
}

void DX10SingleTextureUploaderBuilder::textureSampler(const CPPRef<ITextureSampler>& textureSampler) noexcept
{
    if(RTT_CHECK(textureSampler.get(), DX10TextureSampler))
    {
        ISingleTextureUploaderBuilder::textureSampler(textureSampler);
    }
}

ISingleTextureUploader* DX10SingleTextureUploaderBuilder::build(Error* error) noexcept
{
    DX10SingleTextureUploader* const uploader = new(::std::nothrow) DX10SingleTextureUploader(_texture, _textureSampler);
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

void DX10TextureUploaderBuilder::textureSampler(const CPPRef<ITextureSampler>& textureSampler) noexcept
{
    if(RTT_CHECK(textureSampler.get(), DX10TextureSampler))
    {
        ITextureUploaderBuilder::textureSampler(textureSampler);
    }
}

ITextureUploader* DX10TextureUploaderBuilder::build(Error* error) noexcept
{
    DX10TextureUploader* const uploader = new(::std::nothrow) DX10TextureUploader(_textures, _textureSampler);
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}
#endif
