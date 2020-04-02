#pragma once

#include "shader/TextureUploader.hpp"

#ifdef _WIN32
class DX10GraphicsInterface;
class DX10RenderingContext;
class DX10TextureSampler;

class TAU_DLL DX10SingleTextureUploader final : public ISingleTextureUploader
{
    DEFAULT_DESTRUCT(DX10SingleTextureUploader);
    DELETE_COPY(DX10SingleTextureUploader);
public:
    DX10SingleTextureUploader(const CPPRef<ITexture>& texture, const CPPRef<DX10TextureSampler>& textureSampler) noexcept;

    TextureIndices upload(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
    TextureIndices unbind(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
};

class TAU_DLL DX10TextureUploader final : public ITextureUploader
{
    DEFAULT_DESTRUCT(DX10TextureUploader);
    DELETE_COPY(DX10TextureUploader);
public:
    DX10TextureUploader(const RefDynArray<CPPRef<ITexture>>& textures, const CPPRef<DX10TextureSampler>& textureSampler) noexcept;

    TextureIndices upload(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
    TextureIndices unbind(IRenderingContext& context, const TextureIndices& indices, EShader::Stage stage) noexcept override;
};

class TAU_DLL DX10SingleTextureUploaderBuilder final : public ISingleTextureUploaderBuilder
{
    DEFAULT_DESTRUCT(DX10SingleTextureUploaderBuilder);
    DELETE_COPY(DX10SingleTextureUploaderBuilder);
private:
    DX10GraphicsInterface& _gi;
public:
    DX10SingleTextureUploaderBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10SingleTextureUploader* build(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10SingleTextureUploader* build(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ISingleTextureUploader> buildCPPRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ISingleTextureUploader> buildTauRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ISingleTextureUploader> buildTauSRef(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] static bool processArgs(const SingleTextureUploaderArgs& args, [[tau::out]] Error* error) noexcept;
};

class TAU_DLL DX10TextureUploaderBuilder final : public ITextureUploaderBuilder
{
    DEFAULT_DESTRUCT(DX10TextureUploaderBuilder);
    DELETE_COPY(DX10TextureUploaderBuilder);
private:
    DX10GraphicsInterface& _gi;
public:
    DX10TextureUploaderBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10TextureUploader* build(const TextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10TextureUploader* build(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureUploader> buildCPPRef(const TextureUploaderArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureUploader> buildTauRef(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureUploader> buildTauSRef(const TextureUploaderArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] static bool processArgs(const TextureUploaderArgs& args, [[tau::out]] Error* error) noexcept;
};
#endif
