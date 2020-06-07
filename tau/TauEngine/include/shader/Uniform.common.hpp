#pragma once

#include "Uniform.hpp"
#include "maths/Vector2f.hpp"
#include "maths/Vector3f.hpp"
#include "maths/Vector4f.hpp"

template<>
class UniformAccessor<float> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_CM(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(float) * 4; }

    static inline void set(IRenderingContext& context, IUniformBuffer* const buffer, const float& t) noexcept
    {
        buffer->beginModification(context);
        buffer->modifyBuffer(0, sizeof(float), &t);
        buffer->endModification(context);
    }
};

template<>
class UniformAccessor<Vector2f> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_CM(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(float) * 4; }

    static inline void set(IRenderingContext& context, IUniformBuffer* const buffer, const Vector2f& t) noexcept
    {
        const float x = t.x();
        const float y = t.y();

        buffer->beginModification(context);
        buffer->modifyBuffer(sizeof(float) * 0, &x);
        buffer->modifyBuffer(sizeof(float) * 1, &y);
        buffer->endModification(context);
    }
};

template<>
class UniformAccessor<Vector3f> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_CM(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(float) * 4; }

    static inline void set(IRenderingContext& context, IUniformBuffer* const buffer, const Vector3f& t) noexcept
    {
        const float x = t.x();
        const float y = t.y();
        const float z = t.z();

        buffer->beginModification(context);
        buffer->modifyBuffer(sizeof(float) * 0, &x);
        buffer->modifyBuffer(sizeof(float) * 1, &y);
        buffer->modifyBuffer(sizeof(float) * 2, &z);
        buffer->endModification(context);
    }
};

template<>
class UniformAccessor<Vector4f> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_CM(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(float) * 4; }

    static inline void set(IRenderingContext& context, IUniformBuffer* const buffer, const Vector4f& t) noexcept
    {
        const __m128 vec = t.data().vec;
        buffer->fillBuffer(context, &vec);
    }
};
