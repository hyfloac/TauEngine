#pragma once

#include <NumTypes.hpp>
#include <DLL.hpp>
#include <Objects.hpp>
#include "system/RenderingContext.hpp"
#include "model/BufferDescriptor.hpp"

class TAU_DLL IBuffer
{
    DEFAULT_DESTRUCT_VI(IBuffer);
    DELETE_COPY(IBuffer);
public:
    enum class UsageType : u8
    {
        StreamDraw = 0,
        StreamRead,
        StreamCopy,
        StaticDraw,
        StaticRead,
        StaticCopy,
        DynamicDraw,
        DynamicRead,
        DynamicCopy
    };

    enum class Type : u8
    {
        ArrayBuffer = 0,
        AtomicCounterBuffer,
        CopyReadBuffer,
        CopyWriteBuffer,
        DispatchIndirectBuffer,
        DrawIndirectBuffer,
        ElementArrayBuffer,
        IndexBuffer = ElementArrayBuffer,
        PixelPackBuffer,
        PixelUnpackBuffer,
        QueryBuffer,
        ShaderStorageBuffer,
        TextureBuffer,
        TransformFeedbackBuffer,
        UniformBuffer
    };
public:
    static Ref<IBuffer> create(IRenderingContext& context, std::size_t descriptorCount, Type type, UsageType usage = UsageType::StaticDraw) noexcept;
protected:
    Type _type;
    UsageType _usage;
    BufferDescriptor _descriptor;
protected:
    IBuffer(Type type, UsageType usage, const std::size_t descriptorCount) noexcept
        : _type(type), _usage(usage), _descriptor(descriptorCount)
    { }
public:
    [[nodiscard]] inline Type type() const noexcept { return _type; }
    [[nodiscard]] inline UsageType usage() const noexcept { return _usage; }
    [[nodiscard]] inline const BufferDescriptor& descriptor() const noexcept { return _descriptor; }
    [[nodiscard]] inline BufferDescriptor& descriptor() noexcept { return _descriptor; }

    virtual void bind(IRenderingContext& context) noexcept = 0;

    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void fillBuffer(IRenderingContext& context, std::ptrdiff_t size, const void* data) noexcept = 0;

    virtual void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept = 0;
};

class TAU_DLL IIndexBuffer
{
    DEFAULT_DESTRUCT_VI(IIndexBuffer);
    DELETE_COPY(IIndexBuffer);
public:
    static Ref<IIndexBuffer> create(IRenderingContext& context, IBuffer::UsageType usage = IBuffer::UsageType::StaticDraw) noexcept;
protected:
    IBuffer::UsageType _usage;
protected:
    IIndexBuffer(IBuffer::UsageType usage) noexcept
        : _usage(usage)
    { }
public:
    [[nodiscard]] inline IBuffer::Type type() const noexcept { return IBuffer::Type::ElementArrayBuffer; }
    [[nodiscard]] inline IBuffer::UsageType usage() const noexcept { return _usage; }

    virtual void bind(IRenderingContext& context) noexcept = 0;

    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void fillBuffer(IRenderingContext& context, std::ptrdiff_t size, const void* data) noexcept = 0;

    virtual void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept = 0;
};
