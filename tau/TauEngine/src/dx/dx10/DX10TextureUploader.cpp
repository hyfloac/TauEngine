#include "dx/dx10/DX10TextureUploader.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10Texture.hpp"
#include "dx/dx10/DX10TextureSampler.hpp"
#include "TauEngine.hpp"

DX10SingleTextureUploader::DX10SingleTextureUploader(const CPPRef<ITexture>& texture, const CPPRef<DX10TextureSampler>& textureSampler) noexcept
    : ISingleTextureUploader(texture, textureSampler)
{ }

TextureIndices DX10SingleTextureUploader::upload(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return indices;
    }

    DX10RenderingContext& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    _texture->bind(context, indices.textureStartIndex, stage);
    RefCast<DX10TextureSampler>(_textureSampler)->bind(ctx, indices.samplerStartIndex);

    return TextureIndices(indices.textureStartIndex + 1, indices.samplerStartIndex + 1, indices.uniformIndex);
}

TextureIndices DX10SingleTextureUploader::unbind(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return indices;
    }

    DX10RenderingContext& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    _texture->unbind(context, indices.textureStartIndex, stage);
    RefCast<DX10TextureSampler>(_textureSampler)->unbind(ctx, indices.samplerStartIndex);

    return TextureIndices(indices.textureStartIndex + 1, indices.samplerStartIndex + 1, indices.uniformIndex);
}

DX10TextureUploader::DX10TextureUploader(const RefDynArray<CPPRef<ITexture>>& textures, const CPPRef<DX10TextureSampler>& textureSampler) noexcept
    : ITextureUploader(textures, textureSampler)
{ }

TextureIndices DX10TextureUploader::upload(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return indices;
    }

    DX10RenderingContext& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    for(uSys i = 0; i < _textures.size(); ++i)
    {
        _textures[i]->bind(context, i + indices.textureStartIndex, stage);
    }
    RefCast<DX10TextureSampler>(_textureSampler)->bind(ctx, indices.samplerStartIndex);

    return TextureIndices(indices.textureStartIndex + _textures.size(), indices.samplerStartIndex + 1, indices.uniformIndex);
}

TextureIndices DX10TextureUploader::unbind(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    if(!RTT_CHECK(context, DX10RenderingContext))
    {
        TAU_THROW(IncorrectContextException);
        return indices;
    }

    DX10RenderingContext& ctx = reinterpret_cast<DX10RenderingContext&>(context);

    for(uSys i = 0; i < _textures.size(); ++i)
    {
        _textures[i]->unbind(context, i + indices.textureStartIndex, stage);
    }
    RefCast<DX10TextureSampler>(_textureSampler)->unbind(ctx, indices.samplerStartIndex);

    return TextureIndices(indices.textureStartIndex + _textures.size(), indices.samplerStartIndex + 1, indices.uniformIndex);
}

DX10SingleTextureUploader* DX10SingleTextureUploaderBuilder::build(const SingleTextureUploaderArgs& args, Error* error) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    DX10SingleTextureUploader* const uploader = new(::std::nothrow) DX10SingleTextureUploader(args.texture, RefCast<DX10TextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

DX10SingleTextureUploader* DX10SingleTextureUploaderBuilder::build(const SingleTextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    DX10SingleTextureUploader* const uploader = allocator.allocateT<DX10SingleTextureUploader>(args.texture, RefCast<DX10TextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

CPPRef<ISingleTextureUploader> DX10SingleTextureUploaderBuilder::buildCPPRef(const SingleTextureUploaderArgs& args, Error* error) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const CPPRef<DX10SingleTextureUploader> uploader = CPPRef<DX10SingleTextureUploader>(new(::std::nothrow) DX10SingleTextureUploader(args.texture, RefCast<DX10TextureSampler>(args.textureSampler)));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

NullableRef<ISingleTextureUploader> DX10SingleTextureUploaderBuilder::buildTauRef(const SingleTextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const NullableRef<DX10SingleTextureUploader> uploader(allocator, args.texture, RefCast<DX10TextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

NullableStrongRef<ISingleTextureUploader> DX10SingleTextureUploaderBuilder::buildTauSRef(const SingleTextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const NullableStrongRef<DX10SingleTextureUploader> uploader(allocator, args.texture, RefCast<DX10TextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

bool DX10SingleTextureUploaderBuilder::processArgs(const SingleTextureUploaderArgs& args, Error* error) noexcept
{
    ERROR_CODE_COND_F(!args.textureSampler, Error::TextureSamplerNotSet);
    ERROR_CODE_COND_F(!RTT_CHECK(args.textureSampler.get(), DX10TextureSampler), Error::CrossAPIFailure);
    ERROR_CODE_COND_F(!args.texture, Error::TextureNotSet);

    if(!RTT_CHECK(args.texture.get(), DX10Texture2D) && 
       !RTT_CHECK(args.texture.get(), DX10NullTexture) && 
       !RTT_CHECK(args.texture.get(), DX10TextureCube))
    {
        ERROR_CODE_F(Error::CrossAPIFailure);
    }

    return true;
}

DX10TextureUploader* DX10TextureUploaderBuilder::build(const TextureUploaderArgs& args, Error* error) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    DX10TextureUploader* const uploader = new(::std::nothrow) DX10TextureUploader(args.textures, RefCast<DX10TextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

DX10TextureUploader* DX10TextureUploaderBuilder::build(const TextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    DX10TextureUploader* const uploader = allocator.allocateT<DX10TextureUploader>(args.textures, RefCast<DX10TextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

CPPRef<ITextureUploader> DX10TextureUploaderBuilder::buildCPPRef(const TextureUploaderArgs& args, Error* error) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const CPPRef<DX10TextureUploader> uploader = CPPRef<DX10TextureUploader>(new(::std::nothrow) DX10TextureUploader(args.textures, RefCast<DX10TextureSampler>(args.textureSampler)));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

NullableRef<ITextureUploader> DX10TextureUploaderBuilder::buildTauRef(const TextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const NullableRef<DX10TextureUploader> uploader(allocator, args.textures, RefCast<DX10TextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

NullableStrongRef<ITextureUploader> DX10TextureUploaderBuilder::buildTauSRef(const TextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const NullableStrongRef<DX10TextureUploader> uploader(allocator, args.textures, RefCast<DX10TextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

bool DX10TextureUploaderBuilder::processArgs(const TextureUploaderArgs& args, Error* error) noexcept
{
    ERROR_CODE_COND_F(!args.textureSampler, Error::TextureSamplerNotSet);
    ERROR_CODE_COND_F(!RTT_CHECK(args.textureSampler.get(), DX10TextureSampler), Error::CrossAPIFailure);

    ERROR_CODE_COND_F(args.textures.size() == 0, Error::ZeroTextures);

    for(const auto& texture : args.textures)
    {
        ERROR_CODE_COND_F(!texture, Error::TextureNotSet);

        if(!RTT_CHECK(texture.get(), DX10Texture2D) &&
           !RTT_CHECK(texture.get(), DX10NullTexture) &&
           !RTT_CHECK(texture.get(), DX10TextureCube))
        {
            ERROR_CODE_F(Error::CrossAPIFailure);
        }
    }

    return true;
}
#endif
