#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>

#include "model/Buffer.hpp"

template<typename _T>
class IUniform : ShaderDataType::Typed
{
    DEFAULT_CONSTRUCT_PO(IUniform);
    DEFAULT_DESTRUCT_VI(IUniform);
    DELETE_COPY(IUniform);
public:
    virtual void set(_T value) noexcept = 0;

    [[nodiscard]] virtual ShaderDataType::Type dataType() noexcept override
    { return ShaderDataType::Unknown; }
};

enum UniformBlockBinding
{
    UBB_Upload,
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

    static inline void set(IRenderingContext& context, const Ref<IUniformBuffer>& buffer, const _T& t) noexcept
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
    Ref<IUniformBuffer> _buffer;
public:
    inline UniformBlock(const IUniformBufferBuilder& builder) noexcept
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
        _buffer->bind(context);
        UniformAccessor<_T>::set(context, _buffer, t);
        _buffer->unbind(context);
    }

    void upload(IRenderingContext& context, const u32 index) const noexcept
    {
        _buffer->bind(context, index);
    }

    void unbind(IRenderingContext& context, const u32 index) const noexcept
    {
        _buffer->unbind(context, index);
    }
};

template<typename _T>
class UniformBlock<_T, UniformBlockBinding::UBB_Store> final
{
private:
    Ref<IUniformBuffer> _buffer;
    _T _t;
public:
    template<typename... _Args>
    UniformBlock(const IUniformBufferBuilder& builder, _Args&&... args) noexcept
        : _buffer(null), _t(std::forward<_Args>(args)...)
    {
        UniformBufferArgs bufArgs;
        bufArgs.usage = EBuffer::UsageType::DynamicDraw;
        bufArgs.bufferSize = UniformAccessor<_T>::size();
        bufArgs.initialBuffer = null;
        _buffer = builder.buildCPPRef(bufArgs, null);
    }

    [[nodiscard]] _T& data() noexcept { return _t; }

    void upload(IRenderingContext& context, const u32 index) const noexcept
    {
        _buffer->bind(context);
        UniformAccessor<_T>::set(context, _buffer, _t);
        _buffer->unbind(context);
        _buffer->bind(context, index);
    }

    void unbind(IRenderingContext& context, const u32 index) const noexcept
    {
        _buffer->unbind(context, index);
    }
};

template<typename _T>
class UniformBlock<_T, UniformBlockBinding::UBB_Bind> final
{
private:
    Ref<IUniformBuffer> _buffer;
    const _T* _t;
public:
    UniformBlock(const IUniformBufferBuilder& builder, const _T* t) noexcept
        : _buffer(null), _t(t)
    {
        UniformBufferArgs args;
        args.usage = EBuffer::UsageType::DynamicDraw;
        args.bufferSize = UniformAccessor<_T>::size();
        args.initialBuffer = null;
        _buffer = builder.buildCPPRef(args, null);
    }

    void upload(IRenderingContext& context, const u32 index) const noexcept
    {
        _buffer->bind(context);
        UniformAccessor<_T>::set(context, _buffer, *_t);
        _buffer->unbind(context);
        _buffer->bind(context, index);
    }

    void unbind(IRenderingContext& context, const u32 index) const noexcept
    {
        _buffer->unbind(context, index);
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

    static inline void set(IRenderingContext& context, const Ref<IUniformBuffer>& buffer, const _ExampleUniform& t) noexcept
    {
        buffer->modifyBuffer(context, 0, MATRIX_SIZE, glm::value_ptr(t.projectionMatrix));
        buffer->modifyBuffer(context, MATRIX_SIZE, MATRIX_SIZE, glm::value_ptr(t.viewMatrix));

        const float x = t.cameraPos.x;
        const float y = t.cameraPos.y;
        const float z = t.cameraPos.z;
        buffer->modifyBuffer(context, MATRIX_SIZE * 2, 4, &x);
        buffer->modifyBuffer(context, MATRIX_SIZE * 2 + 4, 4, &y);
        buffer->modifyBuffer(context, MATRIX_SIZE * 2 + 8, 4, &z);
    }  
};
#endif
