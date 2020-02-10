#include "model/Material.hpp"
#include "system/RenderingContext.hpp"

template<>
class UniformAccessor<Material> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(float); }

    static inline void set(IRenderingContext& context, const Ref<IUniformBuffer>& buffer, const Material& t) noexcept
    {
        buffer->fillBuffer(context, reinterpret_cast<const void*>(&t._specularExponent));
    }
};

// MaterialUniforms::MaterialUniforms(const Ref<IShaderProgram>& shader, const DynString& uniformPrefix) noexcept
// {
//     _specularExponentUni = shader->getUniform<float>(uniformPrefix.concat("specularExponent"));
//     _diffuseSamplerUni = shader->getUniform<int>(uniformPrefix.concat("diffuseSampler"));
//     _specularSamplerUni = shader->getUniform<int>(uniformPrefix.concat("specularSampler"));
//     _normalSamplerUni = shader->getUniform<int>(uniformPrefix.concat("normalSampler"));
// }


TextureIndices& Material::upload(IRenderingContext& context, UniformBlockU<Material>& uniform, const EShader::Stage stage, const u32 uniformIndex, TextureIndices& textureIndices) const noexcept
{
    uniform.set(context, *this);
    uniform.upload(context, stage, uniformIndex);
    return _textureUploader->upload(context, textureIndices);
}


TextureIndices& Material::unbind(IRenderingContext& context, UniformBlockU<Material>& uniform, const EShader::Stage stage, const u32 uniformIndex, TextureIndices& textureIndices) const noexcept
{
    uniform.unbind(context, stage, uniformIndex);
    return _textureUploader->unbind(context, textureIndices);
}

Material MaterialBuilder::build() const noexcept
{
    Ref<ITextureUploaderBuilder> uploaderBuilder = _ctx.createTextureUploader(3);
    uploaderBuilder->setTexture(0, _diffuseTexture);
    uploaderBuilder->setTexture(1, _specularTexture);
    uploaderBuilder->setTexture(2, _normalTexture);
    uploaderBuilder->textureSampler(_textureSampler);

    return Material(_specularExponent, Ref<ITextureUploader>(uploaderBuilder->build()));
}

// void Material::set(const MaterialUniforms& uniforms, const int textureBeginIndex) const noexcept
// {
//     uniforms._specularExponentUni->set(_specularExponent);
//     uniforms._diffuseSamplerUni->set(textureBeginIndex);
//     uniforms._specularSamplerUni->set(textureBeginIndex + 1);
//     uniforms._normalSamplerUni->set(textureBeginIndex + 2);
//
//     _diffuseTexture->bind(textureBeginIndex);
//     _specularTexture->bind(textureBeginIndex + 1);
//     _normalTexture->bind(textureBeginIndex + 2);
// }
//
// void Material::unbind(int textureBeginIndex) const noexcept
// {
//     _normalTexture->unbind(textureBeginIndex + 2);
//     _specularTexture->unbind(textureBeginIndex + 1);
//     _diffuseTexture->unbind(textureBeginIndex);
// }
