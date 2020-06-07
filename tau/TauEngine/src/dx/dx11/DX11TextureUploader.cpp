#include "dx/dx11/DX11TextureUploader.hpp"

#ifdef _WIN32
#include "dx/dx11/DX11RenderingContext.hpp"
#include "dx/dx11/DX11Texture.hpp"
#include "dx/dx11/DX11TextureSampler.hpp"
#include "TauEngine.hpp"
#include "TauConfig.hpp"
#include <TUMaths.hpp>

#if TAU_RTTI_CHECK
  #define CTX() \
      if(!RTT_CHECK(context, DX11RenderingContext)) \
      { TAU_THROW(IncorrectContextException); } \
      auto& ctx = reinterpret_cast<DX11RenderingContext&>(context)
#else
  #define CTX() \
      auto& ctx = reinterpret_cast<DX11RenderingContext&>(context)
#endif

DX11SingleTextureUploader::DX11SingleTextureUploader(ITextureView* const texture, const CPPRef<DX11TextureSampler>& textureSampler) noexcept
    : ISingleTextureUploader(texture, textureSampler)
{ }

TextureIndices DX11SingleTextureUploader::upload(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    CTX();
    ID3D11ShaderResourceView* srv = reinterpret_cast<DX11TextureView*>(_texture)->d3dShaderResourceView();

    switch(stage)
    {
        case EShader::Stage::Vertex:
            ctx.d3d11DeviceContext()->VSSetShaderResources(indices.textureStartIndex, 1, &srv);
            break;
        case EShader::Stage::Hull:
            ctx.d3d11DeviceContext()->HSSetShaderResources(indices.textureStartIndex, 1, &srv);
            break;
        case EShader::Stage::Domain:
            ctx.d3d11DeviceContext()->DSSetShaderResources(indices.textureStartIndex, 1, &srv);
            break;
        case EShader::Stage::Geometry:
            ctx.d3d11DeviceContext()->GSSetShaderResources(indices.textureStartIndex, 1, &srv);
            break;
        case EShader::Stage::Pixel:
            ctx.d3d11DeviceContext()->PSSetShaderResources(indices.textureStartIndex, 1, &srv);
            break;
        default:
            break;
    }
    RefCast<DX11TextureSampler>(_textureSampler)->bind(ctx, indices.samplerStartIndex);

    return TextureIndices(indices.textureStartIndex + 1, indices.samplerStartIndex + 1, indices.uniformIndex);
}

TextureIndices DX11SingleTextureUploader::unbind(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    CTX();
    ID3D11ShaderResourceView* nullSRV = null;
    switch(stage)
    {
        case EShader::Stage::Vertex:
            ctx.d3d11DeviceContext()->VSSetShaderResources(indices.textureStartIndex, 1, &nullSRV);
            break;
        case EShader::Stage::Hull:
            ctx.d3d11DeviceContext()->HSSetShaderResources(indices.textureStartIndex, 1, &nullSRV);
            break;
        case EShader::Stage::Domain:
            ctx.d3d11DeviceContext()->DSSetShaderResources(indices.textureStartIndex, 1, &nullSRV);
            break;
        case EShader::Stage::Geometry:
            ctx.d3d11DeviceContext()->GSSetShaderResources(indices.textureStartIndex, 1, &nullSRV);
            break;
        case EShader::Stage::Pixel:
            ctx.d3d11DeviceContext()->PSSetShaderResources(indices.textureStartIndex, 1, &nullSRV);
            break;
        default:
            break;
    }
    RefCast<DX11TextureSampler>(_textureSampler)->unbind(ctx, indices.samplerStartIndex);

    return TextureIndices(indices.textureStartIndex + 1, indices.samplerStartIndex + 1, indices.uniformIndex);
}

DX11TextureUploader::DX11TextureUploader(const RefDynArray<ITextureView*>& textures, const CPPRef<DX11TextureSampler>& textureSampler, const RefDynArray<ID3D11ShaderResourceView*>& resources) noexcept
    : ITextureUploader(textures, textureSampler)
    , _resources(resources)
{ }

TextureIndices DX11TextureUploader::upload(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    CTX();
    switch(stage)
    {
        case EShader::Stage::Vertex:
            ctx.d3d11DeviceContext()->VSSetShaderResources(indices.textureStartIndex, _resources.count(), _resources.arr());
            break;
        case EShader::Stage::Hull:
            ctx.d3d11DeviceContext()->HSSetShaderResources(indices.textureStartIndex, _resources.count(), _resources.arr());
            break;
        case EShader::Stage::Domain:
            ctx.d3d11DeviceContext()->DSSetShaderResources(indices.textureStartIndex, _resources.count(), _resources.arr());
            break;
        case EShader::Stage::Geometry:
            ctx.d3d11DeviceContext()->GSSetShaderResources(indices.textureStartIndex, _resources.count(), _resources.arr());
            break;
        case EShader::Stage::Pixel:
            ctx.d3d11DeviceContext()->PSSetShaderResources(indices.textureStartIndex, _resources.count(), _resources.arr());
            break;
        default:
            break;
    }
    RefCast<DX11TextureSampler>(_textureSampler)->bind(ctx, indices.samplerStartIndex);

    return TextureIndices(indices.textureStartIndex + _textures.size(), indices.samplerStartIndex + 1, indices.uniformIndex);
}

TextureIndices DX11TextureUploader::unbind(IRenderingContext& context, const TextureIndices& indices, const EShader::Stage stage) noexcept
{
    CTX();
    ID3D11ShaderResourceView* nullSRV[16] = {
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
                ctx.d3d11DeviceContext()->VSSetShaderResources(i + indices.textureStartIndex, minT(16ull, _textures.count() - i), nullSRV);
                break;
            case EShader::Stage::Hull:
                ctx.d3d11DeviceContext()->HSSetShaderResources(i + indices.textureStartIndex, minT(16ull, _textures.count() - i), nullSRV);
                break;
            case EShader::Stage::Domain:
                ctx.d3d11DeviceContext()->DSSetShaderResources(i + indices.textureStartIndex, minT(16ull, _textures.count() - i), nullSRV);
                break;
            case EShader::Stage::Geometry:
                ctx.d3d11DeviceContext()->GSSetShaderResources(i + indices.textureStartIndex, minT(16ull, _textures.count() - i), nullSRV);
                break;
            case EShader::Stage::Pixel:
                ctx.d3d11DeviceContext()->PSSetShaderResources(i + indices.textureStartIndex, minT(16ull, _textures.count() - i), nullSRV);
                break;
            default:
                break;
        }
    }
    RefCast<DX11TextureSampler>(_textureSampler)->unbind(ctx, indices.samplerStartIndex);

    return TextureIndices(indices.textureStartIndex + _textures.size(), indices.samplerStartIndex + 1, indices.uniformIndex);
}

DX11TextureUploader* DX11TextureUploaderBuilder::build(const TextureUploaderArgs& args, Error* error) const noexcept
{
    DXTextureUploaderArgs dxArgs(args.textures.count());
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX11TextureUploader* const uploader = new(::std::nothrow) DX11TextureUploader(args.textures, RefCast<DX11TextureSampler>(args.textureSampler), dxArgs.textures);
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

DX11TextureUploader* DX11TextureUploaderBuilder::build(const TextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureUploaderArgs dxArgs(args.textures.count());
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX11TextureUploader* const uploader = allocator.allocateT<DX11TextureUploader>(args.textures, RefCast<DX11TextureSampler>(args.textureSampler), dxArgs.textures);
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

CPPRef<ITextureUploader> DX11TextureUploaderBuilder::buildCPPRef(const TextureUploaderArgs& args, Error* error) const noexcept
{
    DXTextureUploaderArgs dxArgs(args.textures.count());
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX11TextureUploader> uploader = CPPRef<DX11TextureUploader>(new(::std::nothrow) DX11TextureUploader(args.textures, RefCast<DX11TextureSampler>(args.textureSampler), dxArgs.textures));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

NullableRef<ITextureUploader> DX11TextureUploaderBuilder::buildTauRef(const TextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureUploaderArgs dxArgs(args.textures.count());
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX11TextureUploader> uploader(allocator, args.textures, RefCast<DX11TextureSampler>(args.textureSampler), dxArgs.textures);
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

NullableStrongRef<ITextureUploader> DX11TextureUploaderBuilder::buildTauSRef(const TextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    DXTextureUploaderArgs dxArgs(args.textures.count());
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX11TextureUploader> uploader(allocator, args.textures, RefCast<DX11TextureSampler>(args.textureSampler), dxArgs.textures);
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

bool DX11TextureUploaderBuilder::processArgs(const TextureUploaderArgs& args, DXTextureUploaderArgs* dxArgs, Error* error) noexcept
{
    ERROR_CODE_COND_F(!args.textureSampler, Error::TextureSamplerNotSet);
    ERROR_CODE_COND_F(!RTT_CHECK(args.textureSampler.get(), DX11TextureSampler), Error::CrossAPIFailure);
    ERROR_CODE_COND_F(args.textures.size() == 0, Error::ZeroTextures);

    for(uSys i = 0; i < args.textures.count(); ++i)
    {
        ITextureView* const texture = args.textures[i];

        ERROR_CODE_COND_F(!texture, Error::TextureNotSet);
        ERROR_CODE_COND_F(!RTT_CHECK(texture, DX11TextureView), Error::CrossAPIFailure);
        dxArgs->textures[i] = reinterpret_cast<DX11TextureView*>(texture)->d3dShaderResourceView();
    }

    return true;
}

DX11SingleTextureUploader* DX11TextureUploaderBuilder::build(const SingleTextureUploaderArgs& args, Error* error) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    DX11SingleTextureUploader* const uploader = new(::std::nothrow) DX11SingleTextureUploader(args.texture, RefCast<DX11TextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

DX11SingleTextureUploader* DX11TextureUploaderBuilder::build(const SingleTextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    DX11SingleTextureUploader* const uploader = allocator.allocateT<DX11SingleTextureUploader>(args.texture, RefCast<DX11TextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

CPPRef<ISingleTextureUploader> DX11TextureUploaderBuilder::buildCPPRef(const SingleTextureUploaderArgs& args, Error* error) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const CPPRef<DX11SingleTextureUploader> uploader = CPPRef<DX11SingleTextureUploader>(new(::std::nothrow) DX11SingleTextureUploader(args.texture, RefCast<DX11TextureSampler>(args.textureSampler)));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

NullableRef<ISingleTextureUploader> DX11TextureUploaderBuilder::buildTauRef(const SingleTextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const NullableRef<DX11SingleTextureUploader> uploader(allocator, args.texture, RefCast<DX11TextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

NullableStrongRef<ISingleTextureUploader> DX11TextureUploaderBuilder::buildTauSRef(const SingleTextureUploaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    if(!processArgs(args, error))
    { return null; }

    const NullableStrongRef<DX11SingleTextureUploader> uploader(allocator, args.texture, RefCast<DX11TextureSampler>(args.textureSampler));
    ERROR_CODE_COND_N(!uploader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, uploader);
}

bool DX11TextureUploaderBuilder::processArgs(const SingleTextureUploaderArgs& args, Error* error) noexcept
{
    ERROR_CODE_COND_F(!args.textureSampler, Error::TextureSamplerNotSet);
    ERROR_CODE_COND_F(!RTT_CHECK(args.textureSampler.get(), DX11TextureSampler), Error::CrossAPIFailure);
    ERROR_CODE_COND_F(!args.texture, Error::TextureNotSet);
    ERROR_CODE_COND_F(!RTT_CHECK(args.texture, DX11TextureView), Error::CrossAPIFailure);

    return true;
}
#endif
