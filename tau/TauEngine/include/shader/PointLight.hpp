#pragma once

#include "shader/IShaderProgram.hpp"
#include "maths/Vector3f.hpp"
#include "Color.hpp"
#include <Safeties.hpp>
#include <String.hpp>

class PointLightUniforms final
{
    DEFAULT_DESTRUCT(PointLightUniforms);
    DEFAULT_COPY(PointLightUniforms);
private:
    Ref<IUniform<const Vector3f&>> _positionUni;
    Ref<IUniform<const Vector3f&>> _ambientUni;
    Ref<IUniform<const Vector3f&>> _diffuseUni;
    Ref<IUniform<const Vector3f&>> _specularUni;
    Ref<IUniform<float>> _constantUni;
    Ref<IUniform<float>> _linearUni;
    Ref<IUniform<float>> _quadraticUni;
public:
    PointLightUniforms(const Ref<IShaderProgram>& shader, const DynString& uniformPrefix) noexcept;
private:
    friend class PointLight;
};

class PointLight final
{
    DEFAULT_CONSTRUCT_PU(PointLight);
    DEFAULT_DESTRUCT(PointLight);
    DEFAULT_COPY(PointLight);
private:
    Vector3f _position;
    RGBColor _ambientColor;
    RGBColor _diffuseColor;
    RGBColor _specularColor;
    Vector3f _ambientCache;
    Vector3f _diffuseCache;
    Vector3f _specularCache;
    float _constant;
    float _linear;
    float _quadratic;
public:
    [[nodiscard]] inline Vector3f position() const noexcept { return _position; }
    [[nodiscard]] inline Vector3f& position() noexcept { return _position; }

    [[nodiscard]] inline RGBColor ambient() const noexcept { return _ambientColor; }
    [[nodiscard]] inline RGBColor diffuse() const noexcept { return _diffuseColor; }
    [[nodiscard]] inline RGBColor specular() const noexcept { return _specularColor; }

    void ambient(RGBColor color) noexcept;
    void diffuse(RGBColor color) noexcept;
    void specular(RGBColor color) noexcept;

    [[nodiscard]] inline float constant() const noexcept { return _constant; }
    [[nodiscard]] inline float linear() const noexcept { return _linear; }
    [[nodiscard]] inline float quadratic() const noexcept { return _quadratic; }

    [[nodiscard]] inline float& constant() noexcept { return _constant; }
    [[nodiscard]] inline float& linear() noexcept { return _linear; }
    [[nodiscard]] inline float& quadratic() noexcept { return _quadratic; }

    void set(const PointLightUniforms& uniforms) const noexcept;
};
