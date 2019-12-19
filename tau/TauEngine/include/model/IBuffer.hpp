#pragma once

#include <NumTypes.hpp>
#include <DLL.hpp>
#include <Objects.hpp>
// #include "system/RenderingContext.hpp"
#include "model/BufferDescriptor.hpp"
#include "model/BufferEnums.hpp"
#include "Safeties.hpp"

class TAU_DLL IBuffer
{
    DEFAULT_DESTRUCT_VI(IBuffer);
    DELETE_COPY(IBuffer);
protected:
    EBuffer::Type _type;
    EBuffer::UsageType _usage;
    uSys _bufferSize;
    BufferDescriptor _descriptor;
protected:
    IBuffer(const EBuffer::Type type, const EBuffer::UsageType usage, const uSys bufferSize, const BufferDescriptor& descriptor) noexcept
        : _type(type), _usage(usage), _bufferSize(bufferSize), _descriptor(descriptor)
    { }
public:
    [[nodiscard]] inline EBuffer::Type type() const noexcept { return _type; }
    [[nodiscard]] inline EBuffer::UsageType usage() const noexcept { return _usage; }
    [[nodiscard]] inline uSys bufferSize() const noexcept { return _bufferSize; }
    [[nodiscard]] inline const BufferDescriptor& descriptor() const noexcept { return _descriptor; }
    // [[nodiscard]] inline BufferDescriptor& descriptor() noexcept { return _descriptor; }

    virtual void bind(IRenderingContext& context) noexcept = 0;

    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void fillBuffer(IRenderingContext& context, const void* data) noexcept = 0;

    virtual void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept = 0;
};

class TAU_DLL IIndexBuffer
{
    DEFAULT_DESTRUCT_VI(IIndexBuffer);
    DELETE_COPY(IIndexBuffer);
protected:
    EBuffer::UsageType _usage;
    uSys _bufferSize;
protected:
    IIndexBuffer(const EBuffer::UsageType usage, const uSys bufferSize) noexcept
        : _usage(usage), _bufferSize(bufferSize)
    { }
public:
    [[nodiscard]] inline EBuffer::Type type() const noexcept { return EBuffer::Type::IndexBuffer; }
    [[nodiscard]] inline EBuffer::UsageType usage() const noexcept { return _usage; }
    [[nodiscard]] inline uSys bufferSize() const noexcept { return _bufferSize; }

    virtual void bind(IRenderingContext& context) noexcept = 0;

    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void fillBuffer(IRenderingContext& context, const void* data) noexcept = 0;

    virtual void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept = 0;
};

class TAU_DLL IBufferBuilder
{
    DEFAULT_DESTRUCT_VI(IBufferBuilder);
    DELETE_COPY(IBufferBuilder);
public:
    enum Error : u8
    {
        NoError = 0,
        BufferCannotBeIndexBuffer,
        TypeIsUnset,
        UsageIsUnset,
        BufferSizeIsZero,
        MemoryAllocationFailure
    };
protected:
    EBuffer::Type _type;
    EBuffer::UsageType _usage;
    uSys _bufferSize;
    BufferDescriptor _descriptor;
public:
    inline IBufferBuilder(const uSys descriptorCount) noexcept
        : _type(static_cast<EBuffer::Type>(0)),
          _usage(static_cast<EBuffer::UsageType>(0)),
          _bufferSize(0),
          _descriptor(descriptorCount)
    { }

    virtual void type(const EBuffer::Type type) noexcept { _type = type; }
    virtual void usage(const EBuffer::UsageType usage) noexcept { _usage = usage; }
    inline void bufferSize(const uSys bufferSize) noexcept { _bufferSize = bufferSize; }
    [[nodiscard]] inline BufferDescriptor& descriptor() noexcept { return _descriptor; }

    [[nodiscard]] virtual IBuffer* build([[tau::out]] Error* error) const noexcept = 0;
};

class TAU_DLL IIndexBufferBuilder
{
    DEFAULT_DESTRUCT_VI(IIndexBufferBuilder);
    DELETE_COPY(IIndexBufferBuilder);
public:
    using Error = IBufferBuilder::Error;
protected:
    EBuffer::UsageType _usage;
    uSys _bufferSize;
public:
    inline IIndexBufferBuilder() noexcept
        : _usage(static_cast<EBuffer::UsageType>(0)), _bufferSize(0)
    { }

    virtual void usage(const EBuffer::UsageType usage) noexcept { _usage = usage; }
    inline void bufferSize(const uSys bufferSize) noexcept { _bufferSize = bufferSize; }

    [[nodiscard]] virtual IIndexBuffer* build([[tau::out]] Error* error) const noexcept = 0;
};
