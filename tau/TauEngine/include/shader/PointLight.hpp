#pragma once

#include "shader/Uniform.hpp"
#include "maths/Vector3f.hpp"
#include "Color.hpp"

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
        // 4xVector4F + 3xFloat (pad 1xFloat)
        return 4 * sizeof(float) * 4 + 4 * sizeof(float);
    }

    static inline void set(IRenderingContext& context, IUniformBuffer* const buffer, const PointLight& t) noexcept
    {
        buffer->beginModification(context);

    #define SP_PASTE2(_A, _B) _A ## _B
    #define SP_PASTE(_A, _B) SP_PASTE2(_A, _B)
    #define SP_UNIQUE(_X) SP_PASTE(_X, __LINE__)
    #define COPY_VEC(_VEC, _OFFSET) const __m128 SP_UNIQUE(_vec_) = (_VEC).data().vec; \
                                    buffer->modifyBuffer(_OFFSET, SP_UNIQUE(_vec_))

        COPY_VEC(t._position, 0);
        COPY_VEC(t._ambientCache, 16);
        COPY_VEC(t._diffuseCache, 32);
        COPY_VEC(t._specularCache, 48);

        buffer->modifyBuffer(64, t._constant);
        buffer->modifyBuffer(68, t._linear);
        buffer->modifyBuffer(72, t._quadratic);

    #undef COPY_VEC
    #undef SP_UNIQUE
    #undef SP_PASTE
    #undef SP_PASTE2

        buffer->endModification(context);
    }
};
