#pragma once

#include <Safeties.hpp>

#include "shader/ShaderProgram.hpp"
#include "graphics/Resource.hpp"
#include "texture/TextureSampler.hpp"

// TODO: port to ICommandQueue upload path
// #include "shader/TextureUploader.hpp"
#include "shader/Uniform.hpp"

class IGraphicsInterface;

class Material final
{
    DEFAULT_CONSTRUCT_PU(Material);
    DEFAULT_DESTRUCT(Material);
    DEFAULT_COPY(Material);
private:
    float _specularExponent;
    NullableRef<tau::IResource> _diffuseTexture;
    NullableRef<tau::IResource> _specularTexture;
    NullableRef<tau::IResource> _normalTexture;
#if 0 // TODO: port to ICommandQueue upload path
    CPPRef<ITextureUploader> _textureUploader;
#endif
private:
    inline Material(const float specularExponent, const NullableRef<tau::IResource>& diffuseTexture, const NullableRef<tau::IResource>& specularTexture, const NullableRef<tau::IResource>& normalTexture) noexcept
        : _specularExponent(specularExponent)
        , _diffuseTexture(diffuseTexture)
        , _specularTexture(specularTexture)
        , _normalTexture(normalTexture)
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
    NullableRef<tau::IResource> _diffuseTexture;
    NullableRef<tau::IResource> _specularTexture;
    NullableRef<tau::IResource> _normalTexture;
    TextureSamplerArgs _textureSampler;
public:
    inline MaterialBuilder(IGraphicsInterface& gi) noexcept
        : _gi(gi), _specularExponent(1.0f)
    { }

    inline void specularExponent(const float specularExponent) noexcept { _specularExponent = specularExponent; }
    inline void diffuseTexture(const NullableRef<tau::IResource>& diffuseTexture) noexcept { _diffuseTexture = diffuseTexture; }
    inline void specularTexture(const NullableRef<tau::IResource>& specularTexture) noexcept { _specularTexture = specularTexture; }
    inline void normalTexture(const NullableRef<tau::IResource>& normalTexture) noexcept { _normalTexture = normalTexture; }
    inline void textureSampler(const TextureSamplerArgs& textureSampler) noexcept { _textureSampler = textureSampler; }

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

    static inline void set(IRenderingContext& context, IUniformBuffer* const buffer, const Material& t) noexcept
    {
        buffer->beginModification(context);
        buffer->modifyBuffer(0, sizeof(float), &t._specularExponent);
        buffer->endModification(context);
    }
};
