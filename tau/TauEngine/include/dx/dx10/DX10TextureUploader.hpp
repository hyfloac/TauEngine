#pragma once

#include "shader/TextureUploader.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10GraphicsInterface;
class DX10RenderingContext;
class DX10TextureSampler;

class TAU_DLL DX10SingleTextureUploader final : public ISingleTextureUploader
{
    DEFAULT_DESTRUCT(DX10SingleTextureUploader);
    DEFAULT_CM_PU(DX10SingleTextureUploader);
public:
    DX10SingleTextureUploader(ITextureView* texture, const CPPRef<DX10TextureSampler>& textureSampler) noexcept;

    TextureIndices upload(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
    TextureIndices unbind(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
};

class TAU_DLL DX10TextureUploader final : public ITextureUploader
{
    DEFAULT_DESTRUCT(DX10TextureUploader);
    DEFAULT_CM_PU(DX10TextureUploader);
private:
    RefDynArray<ID3D10ShaderResourceView*> _resources;
public:
    DX10TextureUploader(const RefDynArray<ITextureView*>& textures, const CPPRef<DX10TextureSampler>& textureSampler, const RefDynArray<ID3D10ShaderResourceView*>& resources) noexcept;

    TextureIndices upload(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
    TextureIndices unbind(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
};

class TAU_DLL DX10TextureUploaderBuilder final : public ITextureUploaderBuilder
{
    DEFAULT_CONSTRUCT_PU(DX10TextureUploaderBuilder);
    DEFAULT_DESTRUCT(DX10TextureUploaderBuilder);
    DEFAULT_CM_PU(DX10TextureUploaderBuilder);
public:
    struct DXTextureUploaderArgs final
    {
        DEFAULT_DESTRUCT(DXTextureUploaderArgs);
        DEFAULT_CM_PU(DXTextureUploaderArgs);
    public:
        RefDynArray<ID3D10ShaderResourceView*> textures;

        inline DXTextureUploaderArgs(const uSys count) noexcept
            : textures(count)
        { }
    };
public:
    [[nodiscard]] DX10TextureUploader* build(const TextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10TextureUploader* build(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureUploader> buildCPPRef(const TextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureUploader> buildTauRef(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureUploader> buildTauSRef(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX10SingleTextureUploader* build(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10SingleTextureUploader* build(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ISingleTextureUploader> buildCPPRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ISingleTextureUploader> buildTauRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ISingleTextureUploader> buildTauSRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] static bool processArgs(const TextureUploaderArgs& args, [[tau::out]] DXTextureUploaderArgs* dxArgs, [[tau::out]] Error* error) noexcept;
    [[nodiscard]] static bool processArgs(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) noexcept;
};
#endif
