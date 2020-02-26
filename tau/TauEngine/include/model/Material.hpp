#pragma once

#include <Safeties.hpp>

#include "shader/IShaderProgram.hpp"
#include "texture/Texture.hpp"

#include "shader/TextureUploader.hpp"
#include "shader/Uniform.hpp"

// class MaterialUniforms final
// {
//     DEFAULT_DESTRUCT(MaterialUniforms);
//     DEFAULT_COPY(MaterialUniforms);
// private:
//     Ref<IUniform<float>> _specularExponentUni;
//     Ref<IUniform<int>> _diffuseSamplerUni;
//     Ref<IUniform<int>> _specularSamplerUni;
//     Ref<IUniform<int>> _normalSamplerUni;
// public:
//     MaterialUniforms(const Ref<IShaderProgram>& shader, const DynString& uniformPrefix) noexcept;
// private:
//     friend class Material;
// };

class Material final
{
    DEFAULT_CONSTRUCT_PU(Material);
    DEFAULT_DESTRUCT(Material);
    DEFAULT_COPY(Material);
private:
    float _specularExponent;
    Ref<ITextureUploader> _textureUploader;
private:
    inline Material(const float specularExponent, const Ref<ITextureUploader>& textureUploader) noexcept
        : _specularExponent(specularExponent), _textureUploader(textureUploader)
    { }
public:
    TextureIndices& upload(IRenderingContext& context, UniformBlockU<Material>& uniform, EShader::Stage stage, u32 uniformIndex, TextureIndices& textureIndices) const noexcept;

    TextureIndices& unbind(IRenderingContext& context, UniformBlockU<Material>& uniform, EShader::Stage stage, u32 uniformIndex, TextureIndices& textureIndices) const noexcept;
private:
    friend class MaterialBuilder;
    friend class UniformAccessor<Material>;
};

class MaterialBuilder final
{
    DEFAULT_DESTRUCT(MaterialBuilder);
    DEFAULT_COPY(MaterialBuilder);
private:
    IRenderingContext& _ctx;
    float _specularExponent;
    Ref<ITexture> _diffuseTexture;
    Ref<ITexture> _specularTexture;
    Ref<ITexture> _normalTexture;
    Ref<ITextureSampler> _textureSampler;
public:
    inline MaterialBuilder(IRenderingContext& context) noexcept
        : _ctx(context), _specularExponent(1.0f)
    { }

    inline void specularExponent(const float specularExponent) noexcept { _specularExponent = specularExponent; }
    inline void diffuseTexture(const Ref<ITexture>& diffuseTexture) noexcept { _diffuseTexture = diffuseTexture; }
    inline void specularTexture(const Ref<ITexture>& specularTexture) noexcept { _specularTexture = specularTexture; }
    inline void normalTexture(const Ref<ITexture>& normalTexture) noexcept { _normalTexture = normalTexture; }
    inline void textureSampler(const Ref<ITextureSampler>& textureSampler) noexcept { _textureSampler = textureSampler; }

    [[nodiscard]] Material build() const noexcept;

    // void set(const MaterialUniforms& uniforms, int textureBeginIndex) const noexcept;

    // void unbind(int textureBeginIndex) const noexcept;
};

template<>
class UniformAccessor<Material> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(float) * 4; }

    static inline void set(IRenderingContext& context, const Ref<IUniformBuffer>& buffer, const Material& t) noexcept
    {
        buffer->modifyBuffer(0, sizeof(float), &t._specularExponent);
    }
};
