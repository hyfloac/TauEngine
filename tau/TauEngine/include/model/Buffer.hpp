#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <Safeties.hpp>
#include <RunTimeType.hpp>
#include <allocator/TauAllocator.hpp>
#include <ReferenceCountingPointer.hpp>

#include "DLL.hpp"
#include "shader/EShader.hpp"
#include "model/BufferEnums.hpp"
#include "model/BufferDescriptor.hpp"
#include "TauEngine.hpp"
#include "TauConfig.hpp"

#if TAU_BUFFER_SAFETY
  #ifndef TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_BIND
    #define TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_BIND 1
  #endif

  #ifndef TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_UNBIND
    #define TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_UNBIND 1
  #endif

  #ifndef TAU_BUFFER_SAFETY_MODIFY_WITHOUT_BEGIN
    #define TAU_BUFFER_SAFETY_MODIFY_WITHOUT_BEGIN 1
  #endif

  #ifndef TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN
    #define TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN 1
  #endif

  #ifndef TAU_BUFFER_SAFETY_DOUBLE_MODIFY_END
    #define TAU_BUFFER_SAFETY_DOUBLE_MODIFY_END 1
  #endif

  #ifndef TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER
    #define TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER 1
  #endif

  #ifndef TAU_BUFFER_SAFETY_FILLED_WHILE_MODIFYING
    #define TAU_BUFFER_SAFETY_FILLED_WHILE_MODIFYING 1
  #endif

  #ifndef TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_UNIFORM_BOUND
    #define TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_UNIFORM_BOUND 1
  #endif

  #ifndef TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_MODIFYING
    #define TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_MODIFYING 1
  #endif
#else
  #define TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_BIND    0
  #define TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_UNBIND  0
  #define TAU_BUFFER_SAFETY_MODIFY_WITHOUT_BEGIN   0
  #define TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN    0
  #define TAU_BUFFER_SAFETY_DOUBLE_MODIFY_END      0
  #define TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER 0
  #define TAU_BUFFER_SAFETY_FILLED_WHILE_MODIFYING 0
  #define TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_UNIFORM_BOUND 0
  #define TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_MODIFYING     0
#endif

#include "events/Exception.hpp"

class BufferSafetyException final : public Exception
{
public:
    enum Type
    {
        Unknown = 0,
        DoubleUniformBufferBind,
        DoubleUniformBufferUnbind,
        ModifiedWithoutBegin,
        DoubleModifyBegin,
        DoubleModifyEnd,
        ModifiedStaticBuffer,
        FilledWhileModifying,
        DestructedWhileUniformBound,
        DestructedWhileModifying
    };
private:
    Type _type;
public:
    inline BufferSafetyException(const Type type) noexcept
        : _type(type)
    { }

    ~BufferSafetyException() noexcept override = default;

    [[nodiscard]] inline Type type() const noexcept { return _type; }

    EXCEPTION_IMPL(BufferSafetyException);
};

#define VERTEX_BUFFER_IMPL_BASE(_TYPE) \
    DELETE_COPY(_TYPE); \
    RTT_IMPL(_TYPE, IVertexBuffer)

#define VERTEX_BUFFER_IMPL(_TYPE) VERTEX_BUFFER_IMPL_BASE(_TYPE)

#define INDEX_BUFFER_IMPL_BASE(_TYPE) \
    DELETE_COPY(_TYPE); \
    RTT_IMPL(_TYPE, IIndexBuffer)

#define INDEX_BUFFER_IMPL(_TYPE) INDEX_BUFFER_IMPL_BASE(_TYPE)

#define UNIFORM_BUFFER_IMPL_BASE(_TYPE) \
    DELETE_COPY(_TYPE); \
    RTT_IMPL(_TYPE, IUniformBuffer)

#define UNIFORM_BUFFER_IMPL(_TYPE) UNIFORM_BUFFER_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE IVertexBuffer
{
    DELETE_COPY(IVertexBuffer);
protected:
    EBuffer::UsageType _usage;
    uSys _bufferSize;
    BufferDescriptor _descriptor;

#if TAU_BUFFER_SAFETY
    iSys _modificationLockCount;
#endif
protected:
    IVertexBuffer(const EBuffer::UsageType usage, const uSys bufferSize, const BufferDescriptor& descriptor) noexcept
        : _usage(usage)
        , _bufferSize(bufferSize)
        , _descriptor(descriptor)
#if TAU_BUFFER_SAFETY
        , _modificationLockCount(0)
#endif
    { }
public:
    virtual ~IVertexBuffer() noexcept
    {
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_MODIFYING
        if(_modificationLockCount != 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DestructedWhileModifying);
        }
  #endif
#endif
    }

    [[nodiscard]] inline EBuffer::UsageType usage() const noexcept { return _usage; }
    [[nodiscard]] inline uSys bufferSize() const noexcept { return _bufferSize; }
    [[nodiscard]] inline uSys elementCount() const noexcept { return _bufferSize / _descriptor.stride(); }
    [[nodiscard]] inline const BufferDescriptor& descriptor() const noexcept { return _descriptor; }
    [[nodiscard]] inline BufferDescriptor& descriptor() noexcept { return _descriptor; }

    virtual bool beginModification(IRenderingContext& context) noexcept = 0;
    virtual void endModification(IRenderingContext& context) noexcept = 0;

    virtual void modifyBuffer(uSys offset, uSys size, const void* data) noexcept = 0;
    virtual void fillBuffer(IRenderingContext& context, const void* data) noexcept = 0;

	template<typename _T>
    uSys modifyBuffer(const uSys offset, const _T& data) noexcept
	{
	    modifyBuffer(offset, sizeof(_T), &data);
        return offset + sizeof(_T);
	}
	
    RTT_BASE_IMPL(IVertexBuffer);
    RTT_BASE_CHECK(IVertexBuffer);
    RTT_BASE_CAST(IVertexBuffer);
};

class TAU_DLL TAU_NOVTABLE IIndexBuffer
{
    DELETE_COPY(IIndexBuffer);
protected:
    EBuffer::UsageType _usage;
    EBuffer::IndexSize _indexSize;
    uSys _bufferSize;

#if TAU_BUFFER_SAFETY
    iSys _modificationLockCount;
#endif
protected:
    IIndexBuffer(const EBuffer::UsageType usage, const EBuffer::IndexSize indexSize, const uSys bufferSize) noexcept
        : _usage(usage)
        , _indexSize(indexSize)
        , _bufferSize(bufferSize)
#if TAU_BUFFER_SAFETY
        , _modificationLockCount(0)
#endif
    { }
public:
    virtual ~IIndexBuffer() noexcept
    {
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_MODIFYING
        if(_modificationLockCount != 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DestructedWhileModifying);
        }
  #endif
#endif
    }

    [[nodiscard]] inline EBuffer::UsageType usage() const noexcept { return _usage; }
    [[nodiscard]] inline EBuffer::IndexSize indexSize() const noexcept { return _indexSize; }
    [[nodiscard]] inline uSys bufferSize() const noexcept { return _bufferSize; }

    virtual bool beginModification(IRenderingContext& context) noexcept = 0;
    virtual void endModification(IRenderingContext& context) noexcept = 0;

    virtual void modifyBuffer(uSys offset, uSys size, const void* data) noexcept = 0;
    virtual void fillBuffer(IRenderingContext& context, const void* data) noexcept = 0;

	template<typename _T>
	uSys modifyBuffer(const uSys offset, const _T& data) noexcept
	{
	    modifyBuffer(offset, sizeof(_T), &data);
        return offset + sizeof(_T);
	}
	
    RTT_BASE_IMPL(IIndexBuffer);
    RTT_BASE_CHECK(IIndexBuffer);
    RTT_BASE_CAST(IIndexBuffer);
};

class TAU_DLL TAU_NOVTABLE IUniformBuffer
{
    DELETE_COPY(IUniformBuffer);
protected:
    EBuffer::UsageType _usage;
    uSys _bufferSize;

#if TAU_BUFFER_SAFETY
    iSys _modificationLockCount;
    iSys _uniformBindLockCount;
#endif
protected:
    IUniformBuffer(const EBuffer::UsageType usage, const uSys bufferSize) noexcept
        : _usage(usage)
        , _bufferSize(bufferSize)
#if TAU_BUFFER_SAFETY
        , _modificationLockCount(0)
        , _uniformBindLockCount(0)
#endif
    { }
public:
    virtual ~IUniformBuffer() noexcept
    {
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_MODIFYING
        if(_modificationLockCount != 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DestructedWhileModifying);
        }
  #endif
  #if TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_UNIFORM_BOUND
        if(_uniformBindLockCount != 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DestructedWhileUniformBound);
        }
  #endif
#endif
    }

    [[nodiscard]] inline EBuffer::UsageType usage() const noexcept { return _usage; }
    [[nodiscard]] inline uSys bufferSize() const noexcept { return _bufferSize; }

    virtual void bind(IRenderingContext& context, EShader::Stage stage, u32 index) noexcept = 0;
    virtual void unbind(IRenderingContext& context, EShader::Stage stage, u32 index) noexcept = 0;

    /**
     * A stateless unbind used purely for buffer bind safety tracking.
     */
    virtual void fastUnbind() noexcept = 0;

    virtual bool beginModification(IRenderingContext& context) noexcept = 0;
    virtual void endModification(IRenderingContext& context) noexcept = 0;

    virtual void modifyBuffer(uSys offset, uSys size, const void* data) noexcept = 0;
    virtual void fillBuffer(IRenderingContext& context, const void* data) noexcept = 0;

	template<typename _T>
	uSys modifyBuffer(const uSys offset, const _T& data) noexcept
    {
	    modifyBuffer(offset, sizeof(_T), &data);
        return offset + sizeof(_T);
    }
	
    RTT_BASE_IMPL(IUniformBuffer);
    RTT_BASE_CHECK(IUniformBuffer);
    RTT_BASE_CAST(IUniformBuffer);
};

struct VertexBufferArgs final
{
    DEFAULT_DESTRUCT(VertexBufferArgs);
    DEFAULT_COPY(VertexBufferArgs);
public:
    EBuffer::UsageType usage;
    uSys elementCount;
    const void* initialBuffer;
    BufferDescriptorBuilder descriptor;
public:
    inline VertexBufferArgs(const uSys descriptorCount, const bool instanced) noexcept
        : usage(static_cast<EBuffer::UsageType>(0))
        , elementCount(0)
        , initialBuffer(null)
        , descriptor(descriptorCount, instanced)
    { }

    [[nodiscard]] inline uSys bufferSize() const noexcept { return elementCount * descriptor.stride(); }
};

struct IndexBufferArgs final
{
    DEFAULT_DESTRUCT(IndexBufferArgs);
    DEFAULT_COPY(IndexBufferArgs);
public:
    EBuffer::UsageType usage;
    EBuffer::IndexSize indexSize;
    uSys elementCount;
    const void* initialBuffer;
public:
    inline IndexBufferArgs() noexcept
        : usage(static_cast<EBuffer::UsageType>(0))
        , indexSize(EBuffer::IndexSize::Uint32)
        , elementCount(0)
        , initialBuffer(null)
    { }

    [[nodiscard]] inline uSys bufferSize() const noexcept { return elementCount * EBuffer::indexSize(indexSize); }
};

struct UniformBufferArgs final
{
    DEFAULT_DESTRUCT(UniformBufferArgs);
    DEFAULT_COPY(UniformBufferArgs);
public:
    EBuffer::UsageType usage;
    uSys bufferSize;
    const void* initialBuffer;
public:
    inline UniformBufferArgs() noexcept
        : usage(static_cast<EBuffer::UsageType>(0))
        , bufferSize(0)
        , initialBuffer(null)
    { }
};

class TAU_DLL TAU_NOVTABLE IBufferBuilder
{
    DEFAULT_CONSTRUCT_PO(IBufferBuilder);
    DEFAULT_DESTRUCT_VI(IBufferBuilder);
    DELETE_COPY(IBufferBuilder);
public:
    enum class Error
    {
        NoError = 0,
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
         * An internal error occured.
         *
         * This is most likely caused by some failure in cross API types.
         */
        InternalError,
        /**
         * An unknown error occured.
         *
         *   This is also likely my fault. It will generally occur if a driver
         * returns an undocumented error code. If you can debug the function
         * I would like to know what the exact error code given by the driver
         * is, if you are also able to identify what the name of the error is
         * (like the macro value) that may also be useful.
         */
        UnknownError
    };
public:
    [[nodiscard]] virtual IVertexBuffer* build(const VertexBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IVertexBuffer* build(const VertexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IVertexBuffer> buildCPPRef(const VertexBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IVertexBuffer> buildTauRef(const VertexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IVertexBuffer> buildTauSRef(const VertexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual IIndexBuffer* build(const IndexBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IIndexBuffer* build(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IIndexBuffer> buildCPPRef(const IndexBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IIndexBuffer> buildTauRef(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IIndexBuffer> buildTauSRef(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual IUniformBuffer* build(const UniformBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IUniformBuffer* build(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IUniformBuffer> buildCPPRef(const UniformBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IUniformBuffer> buildTauRef(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IUniformBuffer> buildTauSRef(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
