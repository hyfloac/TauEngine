#pragma once

#include "shader/TextureUploader.hpp"

#ifdef _WIN32
#include <d3d11.h>

class DX11GraphicsInterface;
class DX11RenderingContext;
class DX11TextureSampler;

class TAU_DLL DX11SingleTextureUploader final : public ISingleTextureUploader
{
    DEFAULT_DESTRUCT(DX11SingleTextureUploader);
    DEFAULT_CM_PU(DX11SingleTextureUploader);
public:
    DX11SingleTextureUploader(ITextureView* texture, const CPPRef<DX11TextureSampler>& textureSampler) noexcept;

    TextureIndices upload(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
    TextureIndices unbind(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
};

class TAU_DLL DX11TextureUploader final : public ITextureUploader
{
    DEFAULT_DESTRUCT(DX11TextureUploader);
    DEFAULT_CM_PU(DX11TextureUploader);
private:
    RefDynArray<ID3D11ShaderResourceView*> _resources;
public:
    DX11TextureUploader(const RefDynArray<ITextureView*>& textures, const CPPRef<DX11TextureSampler>& textureSampler, const RefDynArray<ID3D11ShaderResourceView*>& resources) noexcept;

    TextureIndices upload(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
    TextureIndices unbind(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
};

class TAU_DLL DX11TextureUploaderBuilder final : public ITextureUploaderBuilder
{
    DEFAULT_CONSTRUCT_PU(DX11TextureUploaderBuilder);
    DEFAULT_DESTRUCT(DX11TextureUploaderBuilder);
    DEFAULT_CM_PU(DX11TextureUploaderBuilder);
public:
    struct DXTextureUploaderArgs final
    {
        DEFAULT_DESTRUCT(DXTextureUploaderArgs);
        DEFAULT_CM_PU(DXTextureUploaderArgs);
    public:
        RefDynArray<ID3D11ShaderResourceView*> textures;

        inline DXTextureUploaderArgs(const uSys count) noexcept
            : textures(count)
        { }
    };
public:
    [[nodiscard]] DX11TextureUploader* build(const TextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11TextureUploader* build(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureUploader> buildCPPRef(const TextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureUploader> buildTauRef(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureUploader> buildTauSRef(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX11SingleTextureUploader* build(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11SingleTextureUploader* build(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ISingleTextureUploader> buildCPPRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ISingleTextureUploader> buildTauRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ISingleTextureUploader> buildTauSRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] static bool processArgs(const TextureUploaderArgs& args, [[tau::out]] DXTextureUploaderArgs* dxArgs, [[tau::out]] Error* error) noexcept;
    [[nodiscard]] static bool processArgs(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) noexcept;
};
#endif
