#pragma once

#include "Uniform.hpp"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

template<>
class UniformAccessor<glm::vec2> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_CM(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(float) * 4; }

    static inline void set(IRenderingContext& context, IUniformBuffer* const buffer, const glm::vec2& t) noexcept
    {
        const float x = t.x;
        const float y = t.y;

        buffer->beginModification(context);
        buffer->modifyBuffer(sizeof(float) * 0, &x);
        buffer->modifyBuffer(sizeof(float) * 1, &y);
        buffer->endModification(context);
    }
};

template<>
class UniformAccessor<glm::vec3> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_CM(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(float) * 4; }

    static inline void set(IRenderingContext& context, IUniformBuffer* const buffer, const glm::vec3& t) noexcept
    {
        const float x = t.x;
        const float y = t.y;
        const float z = t.z;

        buffer->beginModification(context);
        buffer->modifyBuffer(sizeof(float) * 0, &x);
        buffer->modifyBuffer(sizeof(float) * 1, &y);
        buffer->modifyBuffer(sizeof(float) * 2, &z);
        buffer->endModification(context);
    }
};

template<>
class UniformAccessor<glm::vec4> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_CM(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(float) * 4; }

    static inline void set(IRenderingContext& context, IUniformBuffer* const buffer, const glm::vec4& t) noexcept
    {
        const float x = t.x;
        const float y = t.y;
        const float z = t.z;
        const float w = t.w;

        buffer->beginModification(context);
        buffer->modifyBuffer(sizeof(float) * 0, &x);
        buffer->modifyBuffer(sizeof(float) * 1, &y);
        buffer->modifyBuffer(sizeof(float) * 2, &z);
        buffer->modifyBuffer(sizeof(float) * 3, &w);
        buffer->endModification(context);
    }
};

template<>
class UniformAccessor<glm::mat4> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_CM(UniformAccessor);
private:
    static constexpr uSys MATRIX_SIZE = sizeof(float) * 4 * 4;
public:
    [[nodiscard]] static inline uSys size() noexcept { return MATRIX_SIZE; }

    static inline void set(IRenderingContext& context, IUniformBuffer* const buffer, const glm::mat4& t) noexcept
    {
        buffer->fillBuffer(context, glm::value_ptr(t));
    }
};
