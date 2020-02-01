#pragma once

#include <Safeties.hpp>

#include "shader/Uniform.hpp"
#include "maths/Vector3f.hpp"
#include "Color.hpp"

// class PointLightUniforms final
// {
//     DEFAULT_DESTRUCT(PointLightUniforms);
//     DEFAULT_COPY(PointLightUniforms);
// private:
//     Ref<IUniform<const Vector3f&>> _positionUni;
//     Ref<IUniform<const Vector3f&>> _ambientUni;
//     Ref<IUniform<const Vector3f&>> _diffuseUni;
//     Ref<IUniform<const Vector3f&>> _specularUni;
//     Ref<IUniform<float>> _constantUni;
//     Ref<IUniform<float>> _linearUni;
//     Ref<IUniform<float>> _quadraticUni;
// public:
//     PointLightUniforms(const Ref<IShaderProgram>& shader, const DynString& uniformPrefix) noexcept;
// private:
//     friend class PointLight;
// };

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

    // void set(const PointLightUniforms& uniforms) const noexcept;
private:
    friend class UniformAccessor<PointLight>;
};

template<>
class UniformAccessor<PointLight> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept
    {
        // 4xVector4F + 3xFloat
        return 4 * sizeof(float) * 4 + 3 * sizeof(float);
    }

    static inline void set(IRenderingContext& context, const Ref<IUniformBuffer>& buffer, const PointLight& t) noexcept
    {
        void* const buf = buffer->mapBuffer(context);

    #define SP_PASTE2(_A, _B) _A ## _B
    #define SP_PASTE(_A, _B) SP_PASTE2(_A, _B)
    #define SP_UNIQUE(_X) SP_PASTE(_X, __LINE__)
    #define STORE(_OFFSET, _VAR) ::std::memcpy(reinterpret_cast<void*>(reinterpret_cast<u8*>(buf) + (_OFFSET)), &(_VAR), sizeof((_VAR)))
    #define COPY_VEC(_VEC, _OFFSET) const __m128 SP_UNIQUE(_vec_) = (_VEC).data().vec; \
                                    STORE(_OFFSET, SP_UNIQUE(_vec_))

        COPY_VEC(t._position, 0);
        COPY_VEC(t._ambientCache, 16);
        COPY_VEC(t._diffuseCache, 32);
        COPY_VEC(t._specularCache, 48);

        STORE(64, t._constant);
        STORE(68, t._linear);
        STORE(72, t._quadratic);

    #undef COPY_VEC
    #undef STORE
    #undef SP_UNIQUE
    #undef SP_PASTE
    #undef SP_PASTE2

        buffer->unmapBuffer(context);
    }
};
