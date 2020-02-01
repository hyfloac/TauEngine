#include "shader/PointLight.hpp"

// PointLightUniforms::PointLightUniforms(const Ref<IShaderProgram>& shader, const DynString& uniformPrefix) noexcept
// {
//     _positionUni = shader->getUniformVector<Vector3f>(uniformPrefix.concat("position"));
//     _ambientUni = shader->getUniformVector<Vector3f>(uniformPrefix.concat("ambient"));
//     _diffuseUni = shader->getUniformVector<Vector3f>(uniformPrefix.concat("diffuse"));
//     _specularUni = shader->getUniformVector<Vector3f>(uniformPrefix.concat("specular"));
//     _constantUni = shader->getUniform<float>(uniformPrefix.concat("constant"));
//     _linearUni = shader->getUniform<float>(uniformPrefix.concat("linear"));
//     _quadraticUni = shader->getUniform<float>(uniformPrefix.concat("quadratic"));
// }

static Vector3f colorCast(const RGBColor c) noexcept
{
    return Vector3f(static_cast<float>(c.r) / 255.0f, static_cast<float>(c.g) / 255.0f, static_cast<float>(c.b) / 255.0f);
}

void PointLight::ambient(RGBColor color) noexcept
{
    _ambientColor = color;
    _ambientCache = colorCast(color);
}

void PointLight::diffuse(RGBColor color) noexcept
{
    _diffuseColor = color;
    _diffuseCache = colorCast(color);
}

void PointLight::specular(RGBColor color) noexcept
{
    _specularColor = color;
    _specularCache = colorCast(color);
}

// void PointLight::set(const PointLightUniforms& uniforms) const noexcept
// {
//     uniforms._positionUni->set(_position);
//     uniforms._ambientUni->set(_ambientCache);
//     uniforms._diffuseUni->set(_diffuseCache);
//     uniforms._specularUni->set(_specularCache);
//     uniforms._constantUni->set(_constant);
//     uniforms._linearUni->set(_linear);
//     uniforms._quadraticUni->set(_quadratic);
// }
