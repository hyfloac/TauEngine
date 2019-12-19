#pragma once

#include "shader/IShaderProgram.hpp"
#include "maths/Vector3f.hpp"
#include "Color.hpp"
#include <Safeties.hpp>
#include <String.hpp>

class SpotLightUniforms final
{
    DEFAULT_DESTRUCT(SpotLightUniforms);
    DEFAULT_COPY(SpotLightUniforms);
private:
    Ref<IUniform<const Vector3f&>> _positionUni;
    Ref<IUniform<const Vector3f&>> _directionUni;
    Ref<IUniform<float>> _cutOffUni;
    Ref<IUniform<float>> _outerCutOffUni;
    Ref<IUniform<const Vector3f&>> _ambientUni;
    Ref<IUniform<const Vector3f&>> _diffuseUni;
    Ref<IUniform<const Vector3f&>> _specularUni;
    Ref<IUniform<float>> _constantUni;
    Ref<IUniform<float>> _linearUni;
    Ref<IUniform<float>> _quadraticUni;
public:
    SpotLightUniforms(const Ref<IShaderProgram>& shader, const DynString& uniformPrefix) noexcept;
private:
    friend class SpotLight;
};

class SpotLight final
{
    DEFAULT_CONSTRUCT_PU(SpotLight);
    DEFAULT_DESTRUCT(SpotLight);
    DEFAULT_COPY(SpotLight);
private:
    Vector3f _position;
    Vector3f _direction;
    Vector3f _ambientCache;
    Vector3f _diffuseCache;
    Vector3f _specularCache;
    RGBColor _ambientColor;
    RGBColor _diffuseColor;
    RGBColor _specularColor;
    float _constant;
    float _linear;
    float _quadratic;
    float _cutOff;
    float _outerCutOff;
public:
    [[nodiscard]] inline Vector3f position() const noexcept { return _position; }
    [[nodiscard]] inline Vector3f& position() noexcept { return _position; }

    [[nodiscard]] inline Vector3f direction() const noexcept { return _direction; }
    [[nodiscard]] inline Vector3f& direction() noexcept { return _direction; }

    [[nodiscard]] inline RGBColor ambient() const noexcept { return _ambientColor; }
    [[nodiscard]] inline RGBColor diffuse() const noexcept { return _diffuseColor; }
    [[nodiscard]] inline RGBColor specular() const noexcept { return _specularColor; }

    void ambient(RGBColor color) noexcept;
    void diffuse(RGBColor color) noexcept;
    void specular(RGBColor color) noexcept;

    [[nodiscard]] inline float constant() const noexcept { return _constant; }
    [[nodiscard]] inline float linear() const noexcept { return _linear; }
    [[nodiscard]] inline float quadratic() const noexcept { return _quadratic; }
    [[nodiscard]] inline float cutOff() const noexcept { return _cutOff; }
    [[nodiscard]] inline float outerCutOff() const noexcept { return _outerCutOff; }

    [[nodiscard]] inline float& constant() noexcept { return _constant; }
    [[nodiscard]] inline float& linear() noexcept { return _linear; }
    [[nodiscard]] inline float& quadratic() noexcept { return _quadratic; }
    [[nodiscard]] inline float& cutOff() noexcept { return _cutOff; }
    [[nodiscard]] inline float& outerCutOff() noexcept { return _outerCutOff; }

    void set(const SpotLightUniforms& uniforms) const noexcept;
};
