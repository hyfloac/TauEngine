#pragma once

#include <Safeties.hpp>

#include "shader/IShaderProgram.hpp"
#include "texture/Texture.hpp"

#include "shader/TextureUploader.hpp"
#include "shader/Uniform.hpp"

class IGraphicsInterface;

class Material final
{
    DEFAULT_CONSTRUCT_PU(Material);
    DEFAULT_DESTRUCT(Material);
    DEFAULT_COPY(Material);
private:
    float _specularExponent;
    CPPRef<ITexture2D> _diffuseTexture;
    CPPRef<ITexture2D> _specularTexture;
    CPPRef<ITexture2D> _normalTexture;
    CPPRef<ITextureUploader> _textureUploader;
private:
    inline Material(const float specularExponent, const CPPRef<ITexture2D>& diffuseTexture, const CPPRef<ITexture2D>& specularTexture, const CPPRef<ITexture2D>& normalTexture, const CPPRef<ITextureUploader>& textureUploader) noexcept
        : _specularExponent(specularExponent)
        , _diffuseTexture(diffuseTexture)
        , _specularTexture(specularTexture)
        , _normalTexture(normalTexture)
        , _textureUploader(textureUploader)
    { }
public:
    TextureIndices upload(IRenderingContext& context, UniformBlockU<Material>& uniform, EShader::Stage stage, u32 uniformIndex, const TextureIndices& textureIndices) const noexcept;
    TextureIndices unbind(IRenderingContext& context, UniformBlockU<Material>& uniform, EShader::Stage stage, u32 uniformIndex, const TextureIndices& textureIndices) const noexcept;
private:
    friend class MaterialBuilder;
    friend class UniformAccessor<Material>;
};

class MaterialBuilder final
{
    DEFAULT_DESTRUCT(MaterialBuilder);
    DEFAULT_COPY(MaterialBuilder);
private:
    IGraphicsInterface& _gi;
    float _specularExponent;
    CPPRef<ITexture2D> _diffuseTexture;
    CPPRef<ITexture2D> _specularTexture;
    CPPRef<ITexture2D> _normalTexture;
    CPPRef<ITextureSampler> _textureSampler;
public:
    inline MaterialBuilder(IGraphicsInterface& gi) noexcept
        : _gi(gi), _specularExponent(1.0f)
    { }

    inline void specularExponent(const float specularExponent) noexcept { _specularExponent = specularExponent; }
    inline void diffuseTexture(const CPPRef<ITexture2D>& diffuseTexture) noexcept { _diffuseTexture = diffuseTexture; }
    inline void specularTexture(const CPPRef<ITexture2D>& specularTexture) noexcept { _specularTexture = specularTexture; }
    inline void normalTexture(const CPPRef<ITexture2D>& normalTexture) noexcept { _normalTexture = normalTexture; }
    inline void textureSampler(const CPPRef<ITextureSampler>& textureSampler) noexcept { _textureSampler = textureSampler; }

    [[nodiscard]] Material build() const noexcept;
};

template<>
class UniformAccessor<Material> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(float) * 4; }

    static inline void set(IRenderingContext& context, const CPPRef<IUniformBuffer>& buffer, const Material& t) noexcept
    {
        buffer->beginModification(context);
        buffer->modifyBuffer(0, sizeof(float), &t._specularExponent);
        buffer->endModification(context);
    }
};
