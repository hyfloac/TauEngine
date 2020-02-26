#pragma once

#include "shader/TextureUploader.hpp"

#ifdef _WIN32
class DX10RenderingContext;

class TAU_DLL DX10SingleTextureUploader final : public ISingleTextureUploader
{
    DEFAULT_DESTRUCT(DX10SingleTextureUploader);
    DELETE_COPY(DX10SingleTextureUploader);
public:
    DX10SingleTextureUploader(const Ref<ITexture>& texture, const Ref<ITextureSampler>& textureSampler) noexcept
        : ISingleTextureUploader(texture, textureSampler)
    { }

    TextureIndices& upload(IRenderingContext& context, TextureIndices& textureIndices, EShader::Stage stage) noexcept override;
    TextureIndices& unbind(IRenderingContext& context, TextureIndices& textureIndices, EShader::Stage stage) noexcept override;
};

class TAU_DLL DX10TextureUploader final : public ITextureUploader
{
    DEFAULT_DESTRUCT(DX10TextureUploader);
    DELETE_COPY(DX10TextureUploader);
public:
    DX10TextureUploader(const RefDynArray<Ref<ITexture>>& textures, const Ref<ITextureSampler>& textureSampler) noexcept
        : ITextureUploader(textures, textureSampler) 
    { }

    TextureIndices& upload(IRenderingContext& context, TextureIndices& textureIndices, EShader::Stage stage) noexcept override;
    TextureIndices& unbind(IRenderingContext& context, TextureIndices& textureIndices, EShader::Stage stage) noexcept override;
};

class TAU_DLL DX10SingleTextureUploaderBuilder final : public ISingleTextureUploaderBuilder
{
    DEFAULT_DESTRUCT(DX10SingleTextureUploaderBuilder);
    DELETE_COPY(DX10SingleTextureUploaderBuilder);
private:
    DX10RenderingContext& _ctx;
public:
    DX10SingleTextureUploaderBuilder(DX10RenderingContext& ctx) noexcept
        : ISingleTextureUploaderBuilder(), _ctx(ctx)
    { }

    void textureSampler(const Ref<ITextureSampler>& textureSampler) noexcept override;

    [[nodiscard]] ISingleTextureUploader* build(Error* error) noexcept override;
};

class TAU_DLL DX10TextureUploaderBuilder final : public ITextureUploaderBuilder
{
    DEFAULT_DESTRUCT(DX10TextureUploaderBuilder);
    DELETE_COPY(DX10TextureUploaderBuilder);
private:
    DX10RenderingContext& _ctx;
public:
    DX10TextureUploaderBuilder(const uSys textureCount, DX10RenderingContext& ctx) noexcept
        : ITextureUploaderBuilder(textureCount), _ctx(ctx)
    { }

    void textureSampler(const Ref<ITextureSampler>& textureSampler) noexcept override;

    [[nodiscard]] ITextureUploader* build(Error* error) noexcept override;
};
#endif
