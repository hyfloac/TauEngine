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
    DELETE_COPY(DX11SingleTextureUploader);
public:
    DX11SingleTextureUploader(ITextureView* texture, const CPPRef<DX11TextureSampler>& textureSampler) noexcept;

    TextureIndices upload(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
    TextureIndices unbind(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
};

class TAU_DLL DX11TextureUploader final : public ITextureUploader
{
    DEFAULT_DESTRUCT(DX11TextureUploader);
    DELETE_COPY(DX11TextureUploader);
private:
    RefDynArray<ID3D11ShaderResourceView*> _resources;
public:
    DX11TextureUploader(const RefDynArray<ITextureView*>& textures, const CPPRef<DX11TextureSampler>& textureSampler, const RefDynArray<ID3D11ShaderResourceView*>& resources) noexcept;

    TextureIndices upload(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
    TextureIndices unbind(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
};

class TAU_DLL DX11SingleTextureUploaderBuilder final : public ISingleTextureUploaderBuilder
{
    DEFAULT_DESTRUCT(DX11SingleTextureUploaderBuilder);
    DELETE_COPY(DX11SingleTextureUploaderBuilder);
private:
    DX11GraphicsInterface& _gi;
public:
    DX11SingleTextureUploaderBuilder(DX11GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX11SingleTextureUploader* build(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11SingleTextureUploader* build(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ISingleTextureUploader> buildCPPRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ISingleTextureUploader> buildTauRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ISingleTextureUploader> buildTauSRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] static bool processArgs(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) noexcept;
};

class TAU_DLL DX11TextureUploaderBuilder final : public ITextureUploaderBuilder
{
    DEFAULT_DESTRUCT(DX11TextureUploaderBuilder);
    DELETE_COPY(DX11TextureUploaderBuilder);
public:
    struct DXTextureUploaderArgs final
    {
        DEFAULT_DESTRUCT(DXTextureUploaderArgs);
        DEFAULT_COPY(DXTextureUploaderArgs);
    public:
        RefDynArray<ID3D11ShaderResourceView*> textures;

        inline DXTextureUploaderArgs(const uSys count) noexcept
            : textures(count)
        { }
    };
private:
    DX11GraphicsInterface& _gi;
public:
    DX11TextureUploaderBuilder(DX11GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX11TextureUploader* build(const TextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11TextureUploader* build(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureUploader> buildCPPRef(const TextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureUploader> buildTauRef(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureUploader> buildTauSRef(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] static bool processArgs(const TextureUploaderArgs& args, [[tau::out]] DXTextureUploaderArgs* dxArgs, [[tau::out]] Error* error) noexcept;
};
#endif
