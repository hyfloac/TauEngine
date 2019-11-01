#pragma once

#include <DLL.hpp>
#include <NumTypes.hpp>
#include <Objects.hpp>
#include <DynArray.hpp>
#include <Safeties.hpp>

class IRenderingContext;
class IBuffer;

enum class DataType : u8
{
    Byte = 1,
    UByte,
    Short,
    UShort,
    Int,
    UInt,
    HalfFloat,
    Float,
    Double,
    Fixed,
    Int2_10_10_10_Rev,
    UInt2_10_10_10_Rev,
    UInt10F_11F_11F_Rev
};

class TAU_DLL IBufferDescriptor
{
    DEFAULT_DESTRUCT_VI(IBufferDescriptor);
    DELETE_COPY(IBufferDescriptor);
public:
    struct AttributeDescriptor final
    {
        Ref<IBuffer> buffer;
        u32 size;
        DataType type;
        bool normalized;
        i32 stride;
        const void* pointer;
    };
protected:
    u64 _uid;
    DynArray<AttributeDescriptor> _attribs;
protected:
    IBufferDescriptor(const u64 uid, const std::size_t attribCount) noexcept
        : _uid(uid), _attribs(attribCount)
    { }
public:
    virtual void addAttribute(Ref<IBuffer> buffer, u32 size, DataType type, bool normalized, i32 stride, const void* pointer) noexcept = 0;

    virtual void bind(IRenderingContext& context) noexcept = 0;

    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void enableAttributes(IRenderingContext& context) noexcept = 0;

    virtual void disableAttributes(IRenderingContext& context) noexcept = 0;

    [[nodiscard]] u64 uid() const noexcept { return _uid; }
    [[nodiscard]] DynArray<AttributeDescriptor>& attribs() noexcept { return _attribs; }
};
