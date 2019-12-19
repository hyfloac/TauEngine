#pragma once

#include "shader/IShaderProgram.hpp"
#include "texture/Texture.hpp"
#include <Safeties.hpp>
#include <String.hpp>

class MaterialUniforms final
{
    DEFAULT_DESTRUCT(MaterialUniforms);
    DEFAULT_COPY(MaterialUniforms);
private:
    Ref<IUniform<float>> _specularExponentUni;
    Ref<IUniform<int>> _diffuseSamplerUni;
    Ref<IUniform<int>> _specularSamplerUni;
public:
    MaterialUniforms(const Ref<IShaderProgram>& shader, const DynString& uniformPrefix) noexcept;
private:
    friend class Material;
};

class Material final
{
    DEFAULT_CONSTRUCT_PU(Material);
    DEFAULT_DESTRUCT(Material);
    DEFAULT_COPY(Material);
private:
    float _specularExponent;
    Ref<ITexture> _diffuseTexture;
    Ref<ITexture> _specularTexture;
public:
    [[nodiscard]] inline float specularExponent() const noexcept { return _specularExponent; }
    [[nodiscard]] inline float& specularExponent() noexcept { return _specularExponent; }

    [[nodiscard]] inline const Ref<ITexture>& diffuseTexture() const noexcept { return _diffuseTexture; }
    [[nodiscard]] inline Ref<ITexture>& diffuseTexture() noexcept { return _diffuseTexture; }

    [[nodiscard]] inline const Ref<ITexture>& specularTexture() const noexcept { return _specularTexture; }
    [[nodiscard]] inline Ref<ITexture>& specularTexture() noexcept { return _specularTexture; }

    void set(const MaterialUniforms& uniforms, int textureBeginIndex) const noexcept;

    void unbind(int textureBeginIndex) const noexcept;
};
