#include "model/Material.hpp"

MaterialUniforms::MaterialUniforms(const Ref<IShaderProgram>& shader, const DynString& uniformPrefix) noexcept
{
    _specularExponentUni = shader->getUniform<float>(uniformPrefix.concat("specularExponent"));
    _diffuseSamplerUni = shader->getUniform<int>(uniformPrefix.concat("diffuseSampler"));
    _specularSamplerUni = shader->getUniform<int>(uniformPrefix.concat("specularSampler"));
}

void Material::set(const MaterialUniforms& uniforms, const int textureBeginIndex) const noexcept
{
    uniforms._specularExponentUni->set(_specularExponent);
    uniforms._diffuseSamplerUni->set(textureBeginIndex);
    uniforms._specularSamplerUni->set(textureBeginIndex + 1);

    _diffuseTexture->bind(textureBeginIndex);
    _specularTexture->bind(textureBeginIndex + 1);
}

void Material::unbind(int textureBeginIndex) const noexcept
{
    _specularTexture->unbind(textureBeginIndex + 1);
    _diffuseTexture->unbind(textureBeginIndex);
}
