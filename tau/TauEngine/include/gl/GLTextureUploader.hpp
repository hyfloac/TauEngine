#pragma once

#include "shader/TextureUploader.hpp"
#include "model/Buffer.hpp"

class GLRenderingContext;

class TAU_DLL GLSingleTextureUploader final : public ISingleTextureUploader
{
    DEFAULT_DESTRUCT(GLSingleTextureUploader);
    DELETE_COPY(GLSingleTextureUploader);
public:
    GLSingleTextureUploader(const CPPRef<ITexture>& texture, const CPPRef<ITextureSampler>& textureSampler) noexcept
        : ISingleTextureUploader(texture, textureSampler)
    { }

    TextureIndices& upload(IRenderingContext& context, TextureIndices& textureIndices, EShader::Stage stage) noexcept override;

    TextureIndices& unbind(IRenderingContext& context, TextureIndices& textureIndices, EShader::Stage stage) noexcept override;
private:
    friend class GLTextureUploaderBuilder;
};

class TAU_DLL GLTextureUploader final : public ITextureUploader
{
    DEFAULT_DESTRUCT(GLTextureUploader);
    DELETE_COPY(GLTextureUploader);
public:
    GLTextureUploader(const RefDynArray<CPPRef<ITexture>>& textures, const CPPRef<ITextureSampler>& textureSampler) noexcept
        : ITextureUploader(textures, textureSampler)
    { }

    TextureIndices& upload(IRenderingContext& context, TextureIndices& textureIndices, EShader::Stage stage) noexcept override;

    TextureIndices& unbind(IRenderingContext& context, TextureIndices& textureIndices, EShader::Stage stage) noexcept override;
private:
    friend class GLTextureUploaderBuilder;
};

class TAU_DLL GLSingleTextureUploaderBuilder final : public ISingleTextureUploaderBuilder
{
    DEFAULT_DESTRUCT(GLSingleTextureUploaderBuilder);
    DELETE_COPY(GLSingleTextureUploaderBuilder);
public:
    GLSingleTextureUploaderBuilder(GLRenderingContext& context) noexcept;

    void textureSampler(const CPPRef<ITextureSampler>& textureSampler) noexcept override;

    [[nodiscard]] GLSingleTextureUploader* build(Error* error) noexcept override;
};

class TAU_DLL GLTextureUploaderBuilder final : public ITextureUploaderBuilder
{
    DEFAULT_DESTRUCT(GLTextureUploaderBuilder);
    DELETE_COPY(GLTextureUploaderBuilder);
public:
    GLTextureUploaderBuilder(const uSys textureCount, GLRenderingContext& context) noexcept;

    void textureSampler(const CPPRef<ITextureSampler>& textureSampler) noexcept override;

    [[nodiscard]] GLTextureUploader* build(Error* error) noexcept override;
};
