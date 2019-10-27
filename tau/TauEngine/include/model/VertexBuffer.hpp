#pragma once

#include <NumTypes.hpp>
#include <DLL.hpp>
#include "system/RenderingContext.hpp"
#include <Objects.hpp>

class TAU_DLL IVertexBuffer
{
    DEFAULT_DESTRUCT_VI(IVertexBuffer);
    DELETE_COPY(IVertexBuffer);
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
        PixelPackBuffer,
        PixelUnpackBuffer,
        QueryBuffer,
        ShaderStorageBuffer,
        TextureBuffer,
        TransformFeedbackBuffer,
        UniformBuffer
    };
public:
    static Ref<IVertexBuffer> create(IRenderingContext& context, Type type, UsageType usage = UsageType::StaticDraw) noexcept;
protected:
    Type _type;
    UsageType _usage;
protected:
    IVertexBuffer(Type type, UsageType usage) noexcept
        : _type(type), _usage(usage)
    { }
public:
    [[nodiscard]] inline Type type() const noexcept { return _type; }
    [[nodiscard]] inline UsageType usage() const noexcept { return _usage; }

    virtual void bind(IRenderingContext& context) noexcept = 0;

    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void fillBuffer(IRenderingContext& context, std::size_t renderCount, std::ptrdiff_t size, const void* data) noexcept = 0;
    virtual void modifyBuffer(IRenderingContext& context, std::size_t renderCount, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept = 0;

    virtual void draw(IRenderingContext& context) noexcept = 0;

    virtual void drawIndexed(IRenderingContext& context) noexcept = 0;
};
