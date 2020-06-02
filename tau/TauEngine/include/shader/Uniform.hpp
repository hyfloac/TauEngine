#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>

#include "model/Buffer.hpp"

enum UniformBlockBinding
{
    UBB_Upload = 1,
    UBB_Store,
    UBB_Bind
};

template<typename _T, UniformBlockBinding _Binding>
class UniformBlock final { };

template<typename _T>
class UniformAccessor final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
public:
    [[nodiscard]] static inline uSys size() noexcept { return sizeof(_T); }

    static inline void set(IRenderingContext& context, IUniformBuffer* const buffer, const _T& t) noexcept
    { buffer->fillBuffer(context, reinterpret_cast<const void*>(&t)); }
};

template<typename _T>
using UniformBlockU = UniformBlock<_T, UniformBlockBinding::UBB_Upload>;
template<typename _T>
using UniformBlockS = UniformBlock<_T, UniformBlockBinding::UBB_Store>;
template<typename _T>
using UniformBlockB = UniformBlock<_T, UniformBlockBinding::UBB_Bind>;

template<typename _T>
class UniformBlock<_T, UniformBlockBinding::UBB_Upload> final
{
    DEFAULT_DESTRUCT(UniformBlock);
    DEFAULT_COPY(UniformBlock);
private:
    CPPRef<IUniformBuffer> _buffer;
public:
    inline UniformBlock(const IBufferBuilder& builder) noexcept
        : _buffer(null)
    {
        UniformBufferArgs args;
        args.usage = EBuffer::UsageType::DynamicDraw;
        args.bufferSize = UniformAccessor<_T>::size();
        args.initialBuffer = null;
        _buffer = builder.buildCPPRef(args, null);
    }

    void set(IRenderingContext& context, const _T& t) noexcept
    {
        UniformAccessor<_T>::set(context, _buffer.get(), t);
    }

    void upload(IRenderingContext& context, const EShader::Stage stage, const u32 index) const noexcept
    {
        _buffer->bind(context, stage, index);
    }

    void unbind(IRenderingContext& context, const EShader::Stage stage, const u32 index) const noexcept
    {
        _buffer->unbind(context, stage, index);
    }
};

template<typename _T>
class UniformBlock<_T, UniformBlockBinding::UBB_Store> final
{
private:
    CPPRef<IUniformBuffer> _buffer;
    _T _t;
public:
    template<typename... _Args>
    UniformBlock(const IBufferBuilder& builder, _Args&&... args) noexcept
        : _buffer(null), _t(std::forward<_Args>(args)...)
    {
        UniformBufferArgs bufArgs;
        bufArgs.usage = EBuffer::UsageType::DynamicDraw;
        bufArgs.bufferSize = UniformAccessor<_T>::size();
        bufArgs.initialBuffer = null;
        _buffer = builder.buildCPPRef(bufArgs, null);
    }

    [[nodiscard]] _T& data() noexcept { return _t; }

    void upload(IRenderingContext& context, const EShader::Stage stage, const u32 index) const noexcept
    {
        UniformAccessor<_T>::set(context, _buffer.get(), _t);
        _buffer->bind(context, stage, index);
    }

    void unbind(IRenderingContext& context, const EShader::Stage stage, const u32 index) const noexcept
    {
        _buffer->unbind(context, stage, index);
    }
};

template<typename _T>
class UniformBlock<_T, UniformBlockBinding::UBB_Bind> final
{
private:
    CPPRef<IUniformBuffer> _buffer;
    const _T* _t;
public:
    UniformBlock(const IBufferBuilder& builder, const _T* t) noexcept
        : _buffer(null), _t(t)
    {
        UniformBufferArgs args;
        args.usage = EBuffer::UsageType::DynamicDraw;
        args.bufferSize = UniformAccessor<_T>::size();
        args.initialBuffer = null;
        _buffer = builder.buildCPPRef(args, null);
    }

    void upload(IRenderingContext& context, const EShader::Stage stage, const u32 index) const noexcept
    {
        UniformAccessor<_T>::set(context, _buffer.get(), *_t);
        _buffer->bind(context, stage, index);
    }

    void unbind(IRenderingContext& context, const EShader::Stage stage, const u32 index) const noexcept
    {
        _buffer->unbind(context, stage, index);
    }
};

#if 0
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 *   An example structure to demonstrate how to upload uniforms
 * that can't simply be copied by address.
 */
struct _ExampleUniform final
{
    glm::mat4x4 projectionMatrix;
    glm::mat4x4 viewMatrix;
    glm::vec3 cameraPos;
};

template<>
class UniformAccessor<_ExampleUniform> final
{
    DELETE_CONSTRUCT(UniformAccessor);
    DELETE_DESTRUCT(UniformAccessor);
    DELETE_COPY(UniformAccessor);
private:
    static constexpr uSys MATRIX_SIZE = 4 * 4 * 4;
public:
    /**
     *   Vector3f's never work well with uniform buffers, as such we just
     * pad them to the size of a Vector4f.
     */
    [[nodiscard]] static inline uSys size() noexcept { return MATRIX_SIZE * 2 + (4 * 4); }

    static inline void set(IRenderingContext& context, IUniformBuffer* const buffer, const _ExampleUniform& t) noexcept
    {
        buffer->beginModification(context);
        buffer->modifyBuffer(0, MATRIX_SIZE, glm::value_ptr(t.projectionMatrix));
        buffer->modifyBuffer(MATRIX_SIZE, MATRIX_SIZE, glm::value_ptr(t.viewMatrix));

        const float x = t.cameraPos.x;
        const float y = t.cameraPos.y;
        const float z = t.cameraPos.z;
        buffer->modifyBuffer(MATRIX_SIZE * 2, 4, &x);
        buffer->modifyBuffer(MATRIX_SIZE * 2 + 4, 4, &y);
        buffer->modifyBuffer(MATRIX_SIZE * 2 + 8, 4, &z);

        //   We don't need to modify the buffer for the final
        // virtual element (w). The buffer has already been
        // padded, and the shader doesn't care about the data.

        buffer->endModification(context);
    }  
};
#endif
