#include "dx/dx10/DX10TextureUploader.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10Texture.hpp"
#include "dx/dx10/DX10TextureSampler.hpp"
#include "TauEngine.hpp"
#include "TauConfig.hpp"
#include <TUMaths.hpp>

#if TAU_RTTI_CHECK
  #define CTX() \
      if(!RTT_CHECK(context, DX10RenderingContext)) \
      { TAU_THROW(IncorrectContextException); } \
      auto& ctx = reinterpret_cast<DX10RenderingContext&>(context)
#else
  #define CTX() \
      auto& ctx = reinterpret_cast<DX10RenderingContext&>(context)
#endif

DX10SingleTextureUploader::DX10SingleTextureUploader(ITextureView* const texture, const CPPRef<DX10TextureSampler>& textureSampler) noexcept
    : ISingleTextureUploader(texture, textureSampler)
{ }

TextureIndices DX10SingleTextureUploader::upload(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    CTX();
    ID3D10ShaderResourceView* srv = reinterpret_cast<DX10TextureView*>(_texture)->d3dShaderResourceView();

    switch(stage)
    {
        case EShader::Stage::Vertex:
            ctx.d3dDevice()->VSSetShaderResources(indices.textureStartIndex, 1, &srv);
            break;
        case EShader::Stage::Geometry:
            ctx.d3dDevice()->GSSetShaderResources(indices.textureStartIndex, 1, &srv);
            break;
        case EShader::Stage::Pixel:
            ctx.d3dDevice()->PSSetShaderResources(indices.textureStartIndex, 1, &srv);
            break;
        default:
            break;
    }
    RefCast<DX10TextureSampler>(_textureSampler)->bind(ctx, indices.samplerStartIndex);

    return TextureIndices(indices.textureStartIndex + 1, indices.samplerStartIndex + 1, indices.uniformIndex);
}

TextureIndices DX10SingleTextureUploader::unbind(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    CTX();
    ID3D10ShaderResourceView* nullSRV = null;
    switch(stage)
    {
        case EShader::Stage::Vertex:
            ctx.d3dDevice()->VSSetShaderResources(indices.textureStartIndex, 1, &nullSRV);
            break;
        case EShader::Stage::Geometry:
            ctx.d3dDevice()->GSSetShaderResources(indices.textureStartIndex, 1, &nullSRV);
            break;
        case EShader::Stage::Pixel:
            ctx.d3dDevice()->PSSetShaderResources(indices.textureStartIndex, 1, &nullSRV);
            break;
        default:
            break;
    }
    RefCast<DX10TextureSampler>(_textureSampler)->unbind(ctx, indices.samplerStartIndex);

    return TextureIndices(indices.textureStartIndex + 1, indices.samplerStartIndex + 1, indices.uniformIndex);
}

DX10TextureUploader::DX10TextureUploader(const RefDynArray<ITextureView*>& textures, const CPPRef<DX10TextureSampler>& textureSampler, const RefDynArray<ID3D10ShaderResourceView*>& resources) noexcept
    : ITextureUploader(textures, textureSampler)
    , _resources(resources)
{ }

TextureIndices DX10TextureUploader::upload(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    CTX();
    switch(stage)
    {
        case EShader::Stage::Vertex:
            ctx.d3dDevice()->VSSetShaderResources(indices.textureStartIndex, _resources.count(), _resources.arr());
            break;
        case EShader::Stage::Geometry:
            ctx.d3dDevice()->GSSetShaderResources(indices.textureStartIndex, _resources.count(), _resources.arr());
            break;
        case EShader::Stage::Pixel:
            ctx.d3dDevice()->PSSetShaderResources(indices.textureStartIndex, _resources.count(), _resources.arr());
            break;
        default:
            break;
    }
    RefCast<DX10TextureSampler>(_textureSampler)->bind(ctx, indices.samplerStartIndex);

    return TextureIndices(indices.textureStartIndex + _textures.size(), indices.samplerStartIndex + 1, indices.uniformIndex);
}

TextureIndices DX10TextureUploader::unbind(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    CTX();
    ID3D10ShaderResourceView* nullSRV[16] = {
        null, null, null, null,
        null, null, null, null,
        null, null, null, null,
        null, null, null, null
    };

    for(uSys i = 0; i < _textures.size(); i += 16)
    {
        switch(stage)
        {
            case EShader::Stage::Vertex:
                ctx.d3dDevice()->VSSetShaderResources(i + indices.textureStartIndex, minT(16ull, _textures.count() - i), nullSRV);
                break;
            case EShader::Stage::Geometry:
                ctx.d3dDevice()->GSSetShaderResources(i + indices.textureStartIndex, minT(16ull, _textures.count() - i), nullSRV);
                break;
            case EShader::Stage::Pixel:
                ctx.d3dDevice()->PSSetShaderResources(i + indices.textureStartIndex, minT(16ull, _textures.count() - i), nullSRV);
                break;
            default:
                break;
        }
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
    ERROR_CODE_COND_F(!RTT_CHECK(args.texture, DX10TextureView), Error::CrossAPIFailure);

    return true;
}

DX10TextureUploader* DX10TextureUploaderBuilder::build(const TextureUploaderArgs& args, Error* error) const noexcept
{
    DXTextureUploaderArgs dxArgs(args.textures.count());
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10TextureUploader* const uploader = new(::std::nothrow) DX10TextureUploader(args.textures, RefCast<DX10TextureSampler>(args.textureSampler), dxArgs.textures);
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

DX10TextureUploader* DX10TextureUploaderBuilder::build(const TextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureUploaderArgs dxArgs(args.textures.count());
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10TextureUploader* const uploader = allocator.allocateT<DX10TextureUploader>(args.textures, RefCast<DX10TextureSampler>(args.textureSampler), dxArgs.textures);
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

CPPRef<ITextureUploader> DX10TextureUploaderBuilder::buildCPPRef(const TextureUploaderArgs& args, Error* error) const noexcept
{
    DXTextureUploaderArgs dxArgs(args.textures.count());
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10TextureUploader> uploader = CPPRef<DX10TextureUploader>(new(::std::nothrow) DX10TextureUploader(args.textures, RefCast<DX10TextureSampler>(args.textureSampler), dxArgs.textures));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

NullableRef<ITextureUploader> DX10TextureUploaderBuilder::buildTauRef(const TextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureUploaderArgs dxArgs(args.textures.count());
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10TextureUploader> uploader(allocator, args.textures, RefCast<DX10TextureSampler>(args.textureSampler), dxArgs.textures);
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

NullableStrongRef<ITextureUploader> DX10TextureUploaderBuilder::buildTauSRef(const TextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureUploaderArgs dxArgs(args.textures.count());
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10TextureUploader> uploader(allocator, args.textures, RefCast<DX10TextureSampler>(args.textureSampler), dxArgs.textures);
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

bool DX10TextureUploaderBuilder::processArgs(const TextureUploaderArgs& args, DXTextureUploaderArgs* dxArgs, Error* error) noexcept
{
    ERROR_CODE_COND_F(!args.textureSampler, Error::TextureSamplerNotSet);
    ERROR_CODE_COND_F(!RTT_CHECK(args.textureSampler.get(), DX10TextureSampler), Error::CrossAPIFailure);
    ERROR_CODE_COND_F(args.textures.size() == 0, Error::ZeroTextures);

    for(uSys i = 0; i < args.textures.count(); ++i)
    {
        ITextureView* const texture = args.textures[i];

        ERROR_CODE_COND_F(!texture, Error::TextureNotSet);
        ERROR_CODE_COND_F(!RTT_CHECK(texture, DX10TextureView), Error::CrossAPIFailure);
        dxArgs->textures[i] = reinterpret_cast<DX10TextureView*>(texture)->d3dShaderResourceView();
    }

    return true;
}
#endif
