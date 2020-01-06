#include "model/Material.hpp"

MaterialUniforms::MaterialUniforms(const Ref<IShaderProgram>& shader, const DynString& uniformPrefix) noexcept
{
    _specularExponentUni = shader->getUniform<float>(uniformPrefix.concat("specularExponent"));
    _diffuseSamplerUni = shader->getUniform<int>(uniformPrefix.concat("diffuseSampler"));
    _specularSamplerUni = shader->getUniform<int>(uniformPrefix.concat("specularSampler"));
    _normalSamplerUni = shader->getUniform<int>(uniformPrefix.concat("normalSampler"));
}

void Material::set(const MaterialUniforms& uniforms, const int textureBeginIndex) const noexcept
{
    uniforms._specularExponentUni->set(_specularExponent);
    uniforms._diffuseSamplerUni->set(textureBeginIndex);
    uniforms._specularSamplerUni->set(textureBeginIndex + 1);
    uniforms._normalSamplerUni->set(textureBeginIndex + 2);

    _diffuseTexture->bind(textureBeginIndex);
    _specularTexture->bind(textureBeginIndex + 1);
    _normalTexture->bind(textureBeginIndex + 2);
}

void Material::unbind(int textureBeginIndex) const noexcept
{
    _normalTexture->unbind(textureBeginIndex + 2);
    _specularTexture->unbind(textureBeginIndex + 1);
    _diffuseTexture->unbind(textureBeginIndex);
}
