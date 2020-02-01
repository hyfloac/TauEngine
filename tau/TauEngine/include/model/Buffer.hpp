#pragma once

#include <NumTypes.hpp>
#include <DLL.hpp>
#include <Objects.hpp>
#include <RunTimeType.hpp>
#include "model/BufferDescriptor.hpp"
#include "model/BufferEnums.hpp"

#define BUFFER_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                                RTT_IMPL(_TYPE, IBuffer)

#define BUFFER_IMPL(_TYPE) BUFFER_IMPL_BASE(_TYPE)

#define INDEX_BUFFER_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                                      RTT_IMPL(_TYPE, IIndexBuffer)

#define INDEX_BUFFER_IMPL(_TYPE) INDEX_BUFFER_IMPL_BASE(_TYPE)

#define UNIFORM_BUFFER_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                                        RTT_IMPL(_TYPE, IUniformBuffer)

#define UNIFORM_BUFFER_IMPL(_TYPE) UNIFORM_BUFFER_IMPL_BASE(_TYPE)

class TAU_DLL IBuffer
{
    DEFAULT_DESTRUCT_VI(IBuffer);
    DELETE_COPY(IBuffer);
protected:
    EBuffer::Type _type;
    EBuffer::UsageType _usage;
    uSys _bufferSize;
    bool _instanced;
    BufferDescriptor _descriptor;
protected:
    IBuffer(const EBuffer::Type type, const EBuffer::UsageType usage, const uSys bufferSize, const bool instanced, const BufferDescriptor& descriptor) noexcept
        : _type(type), _usage(usage), _bufferSize(bufferSize), _instanced(instanced), _descriptor(descriptor)
    { }
public:
    [[nodiscard]] inline EBuffer::Type type() const noexcept { return _type; }
    [[nodiscard]] inline EBuffer::UsageType usage() const noexcept { return _usage; }
    [[nodiscard]] inline uSys bufferSize() const noexcept { return _bufferSize; }
    [[nodiscard]] inline bool instanced() const noexcept { return _instanced; }
    [[nodiscard]] inline const BufferDescriptor& descriptor() const noexcept { return _descriptor; }
    [[nodiscard]] inline BufferDescriptor& descriptor() noexcept { return _descriptor; }

    virtual void bind(IRenderingContext& context) noexcept = 0;

    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void fillBuffer(IRenderingContext& context, const void* data) noexcept = 0;

    virtual void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept = 0;

    [[nodiscard]] virtual void* mapBuffer(IRenderingContext& context) noexcept = 0;
    virtual void unmapBuffer(IRenderingContext& context) noexcept = 0;

    RTT_BASE_IMPL(IBuffer);
    RTT_BASE_CHECK(IBuffer);
    RTT_BASE_CAST(IBuffer);
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

    [[nodiscard]] virtual void* mapBuffer(IRenderingContext& context) noexcept = 0;
    virtual void unmapBuffer(IRenderingContext& context) noexcept = 0;

    RTT_BASE_IMPL(IIndexBuffer);
    RTT_BASE_CHECK(IIndexBuffer);
    RTT_BASE_CAST(IIndexBuffer);
};

class TAU_DLL IUniformBuffer
{
    DEFAULT_DESTRUCT_VI(IUniformBuffer);
    DELETE_COPY(IUniformBuffer);
protected:
    EBuffer::UsageType _usage;
    uSys _bufferSize;
protected:
    IUniformBuffer(const EBuffer::UsageType usage, const uSys bufferSize) noexcept
        : _usage(usage), _bufferSize(bufferSize)
    { }
public:
    [[nodiscard]] inline EBuffer::Type type() const noexcept { return EBuffer::Type::UniformBuffer; }
    [[nodiscard]] inline EBuffer::UsageType usage() const noexcept { return _usage; }
    [[nodiscard]] inline uSys bufferSize() const noexcept { return _bufferSize; }

    virtual void bind(IRenderingContext& context) noexcept = 0;
    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void bind(IRenderingContext& context, u32 index) noexcept = 0;
    virtual void unbind(IRenderingContext& context, u32 index) noexcept = 0;

    virtual void fillBuffer(IRenderingContext& context, const void* data) noexcept = 0;
    virtual void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept = 0;

    [[nodiscard]] virtual void* mapBuffer(IRenderingContext& context) noexcept = 0;
    virtual void unmapBuffer(IRenderingContext& context) noexcept = 0;

    RTT_BASE_IMPL(IUniformBuffer);
    RTT_BASE_CHECK(IUniformBuffer);
    RTT_BASE_CAST(IUniformBuffer);
};

class TAU_DLL IBufferBuilder
{
    DEFAULT_DESTRUCT_VI(IBufferBuilder);
    DELETE_COPY(IBufferBuilder);
public:
    enum Error : u8
    {
        NoError = 0,
        /**
         * Attempted to allocate an index buffer.
         *
         *   One should use IIndexBufferBuilder and IIndexBuffer. This
         * is because there are special optimization that have been made
         * based on the usage case.
         */
        BufferCannotBeIndexBuffer,
        /**
         * Attempted to allocate a uniform buffer.
         *
         *   One should use IUniformBufferBuilder and IUniformBuffer. This
         * is because there are special optimization that have been made
         * based on the usage case.
         */
         BufferCannotBeUniformBuffer,
        /**
         * The buffer type is unset.
         *
         *   This needs to be set for TauEngine, independent of whether
         * or not it actually makes a difference in the driver.
         */
        TypeIsUnset,
        /**
         * The usage type is unset.
         *
         *   This needs to be set for TauEngine, independent of whether
         * or not it actually makes a difference in the driver.
         */
        UsageIsUnset,
        /**
         * The buffer size was zero.
         *
         *   While a driver may support allocating a zero byte buffer,
         * this serves little practical purpose AFAIK, as such is
         * disallowed by TauEngine.
         */
        BufferSizeIsZero,
        /**
         * Failed to allocate system memory.
         *
         *   This error is produced when a call to the system memory
         * allocator (malloc, new, new[], HeapAlloc, LocalAlloc,
         * GlobalAlloc) fails (returns null). This generally occurs
         * when the system has run out of memory. It may also mean
         * that the process itself has exceed its maximum allotted
         * memory. It may also occur when allocating a buffer that
         * is too large.
         *
         *   If this occurs its probably safe to assume that the
         * program or system will crash shortly. As such I prescribe to
         * the philosophy of simply exiting, or outright ignoring the
         * error, given that it likely won't matter in a couple of seconds
         * at most.
         */
        SystemMemoryAllocationFailure,
        /**
         * The driver failed to allocate system memory.
         *
         *   This occurs when the driver failed to allocate memory on
         * the system. This is likely caused by the same reasons as
         * Error::SystemMemoryAllocationFailure.
         */
        DriverMemoryAllocationFailure,
        /**
         * The driver failed to allocate memory on the GPU.
         *
         *   This occurs when the GPU runs out of memory. It may also mean
         * that there isn't a GPU installed, or at the very least, one that
         * isn't suitable. This may be more resolvable by scaling back
         * texture sizes and model sizes. As well as deallocating unnecessary
         * objects. Depending on the backing API it may also be possible to
         * move certain allocation from the GPU back into system memory.
         */
        GPUMemoryAllocationFailure,
        /**
         * TauEngine screwed up somehow.
         *
         * I fucked up. Tell me about.
         *
         *   This shouldn't ever occur. If it does I must have done something
         * very wrong. Please, tell me about it, including any relevant
         * information - things like the call stack (I mostly care about within
         * TauEngine), the model (or at the very least, the model size), etc.
         */
        LibImplementationError,
        /**
         * An unknown error occured.
         *
         *   This is also likely my fault. It will generally occur if a driver
         * returns an undocumented error code. If you can debug the function
         * I would like to know what the exact error code given by the driver
         * is, if you are also able to identify what the name of the error is
         * (like the macro value) that may also be useful.
         */
        UnknownError,
    };
protected:
    EBuffer::Type _type;
    EBuffer::UsageType _usage;
    // uSys _bufferSize;
    uSys _elementCount;
    void* _initialBuffer;
    bool _instanced;
    BufferDescriptorBuilder _descriptor;
public:
    inline IBufferBuilder(const uSys descriptorCount) noexcept
        : _type(static_cast<EBuffer::Type>(0)),
          _usage(static_cast<EBuffer::UsageType>(0)),
          _elementCount(0), _initialBuffer(null),
          _instanced(false),
          _descriptor(descriptorCount)
    { }

    virtual void type(const EBuffer::Type type) noexcept { _type = type; }
    virtual void usage(const EBuffer::UsageType usage) noexcept { _usage = usage; }
    // inline void bufferSize(const uSys bufferSize) noexcept { _bufferSize = bufferSize; }
    inline void elementCount(const uSys elementCount) noexcept { _elementCount = elementCount; }
    inline void initialBuffer(void* const initialBuffer) noexcept { _initialBuffer = initialBuffer; }
    inline void instanced(const bool instanced) noexcept { _instanced = instanced; }
    [[nodiscard]] inline BufferDescriptorBuilder& descriptor() noexcept { return _descriptor; }

    [[nodiscard]] inline uSys bufferSize() const noexcept { return _elementCount * _descriptor.stride(); }

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
    void* _initialBuffer;
public:
    inline IIndexBufferBuilder() noexcept
        : _usage(static_cast<EBuffer::UsageType>(0)),
          _bufferSize(0), _initialBuffer(null)
    { }

    virtual void usage(const EBuffer::UsageType usage) noexcept { _usage = usage; }
    // inline void bufferSize(const uSys bufferSize) noexcept { _bufferSize = bufferSize; }
    inline void elementCount(const uSys elementCount) noexcept { _bufferSize = elementCount * sizeof(u32); }
    inline void initialBuffer(void* const initialBuffer) noexcept { _initialBuffer = initialBuffer; }

    [[nodiscard]] virtual IIndexBuffer* build([[tau::out]] Error* error) const noexcept = 0;
};

class TAU_DLL IUniformBufferBuilder
{
    DEFAULT_DESTRUCT_VI(IUniformBufferBuilder);
    DELETE_COPY(IUniformBufferBuilder);
public:
    using Error = IBufferBuilder::Error;
protected:
    EBuffer::UsageType _usage;
    uSys _bufferSize;
    void* _initialBuffer;
public:
    inline IUniformBufferBuilder() noexcept
        : _usage(static_cast<EBuffer::UsageType>(0)),
        _bufferSize(0), _initialBuffer(null)
    { }

    virtual void usage(const EBuffer::UsageType usage) noexcept { _usage = usage; }
    inline void bufferSize(const uSys bufferSize) noexcept { _bufferSize = bufferSize; }
    inline void initialBuffer(void* const initialBuffer) noexcept { _initialBuffer = initialBuffer; }

    [[nodiscard]] virtual IUniformBuffer* build([[tau::out]] Error* error) const noexcept = 0;
};
