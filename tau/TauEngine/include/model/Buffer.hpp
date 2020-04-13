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
  #ifndef TAU_BUFFER_SAFETY_DOUBLE_BIND
    #define TAU_BUFFER_SAFETY_DOUBLE_BIND 1
  #endif

  #ifndef TAU_BUFFER_SAFETY_DOUBLE_UNBIND
    #define TAU_BUFFER_SAFETY_DOUBLE_UNBIND 1
  #endif

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

  #ifndef TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_BOUND
    #define TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_BOUND 1
  #endif

  #ifndef TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_UNIFORM_BOUND
    #define TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_UNIFORM_BOUND 1
  #endif

  #ifndef TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_MODIFYING
    #define TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_MODIFYING 1
  #endif
#else
  #define TAU_BUFFER_SAFETY_DOUBLE_BIND            0
  #define TAU_BUFFER_SAFETY_DOUBLE_UNBIND          0
  #define TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_BIND    0
  #define TAU_BUFFER_SAFETY_UNIFORM_DOUBLE_UNBIND  0
  #define TAU_BUFFER_SAFETY_MODIFY_WITHOUT_BEGIN   0
  #define TAU_BUFFER_SAFETY_DOUBLE_MODIFY_BEGIN    0
  #define TAU_BUFFER_SAFETY_DOUBLE_MODIFY_END      0
  #define TAU_BUFFER_SAFETY_MODIFIED_STATIC_BUFFER 0
  #define TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_BOUND 0
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
        DoubleBufferBind,
        DoubleBufferUnbind,
        DoubleUniformBufferBind,
        DoubleUniformBufferUnbind,
        ModifiedWithoutBegin,
        DoubleModifyBegin,
        DoubleModifyEnd,
        ModifiedStaticBuffer,
        DestructedWhileBound,
        DestructedWhileUniformBound,
        DestructedWhileModifying
    };
private:
    Type _type;
public:
    inline BufferSafetyException(Type type) noexcept
        : _type(type)
    { }

    ~BufferSafetyException() noexcept override = default;

    [[nodiscard]] inline Type type() const noexcept { return _type; }

    EXCEPTION_IMPL(BufferSafetyException);
};

#define BUFFER_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                                RTT_IMPL(_TYPE, IBuffer)

#define BUFFER_IMPL(_TYPE) BUFFER_IMPL_BASE(_TYPE)

#define INDEX_BUFFER_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                                      RTT_IMPL(_TYPE, IIndexBuffer)

#define INDEX_BUFFER_IMPL(_TYPE) INDEX_BUFFER_IMPL_BASE(_TYPE)

#define UNIFORM_BUFFER_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                                        RTT_IMPL(_TYPE, IUniformBuffer)

#define UNIFORM_BUFFER_IMPL(_TYPE) UNIFORM_BUFFER_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE IBuffer
{
    DELETE_COPY(IBuffer);
protected:
    EBuffer::Type _type;
    EBuffer::UsageType _usage;
    uSys _bufferSize;
    bool _instanced;
    BufferDescriptor _descriptor;

#if TAU_BUFFER_SAFETY
    iSys _modificationLockCount;
    iSys _bindLockCount;
#endif
protected:
    IBuffer(const EBuffer::Type type, const EBuffer::UsageType usage, const uSys bufferSize, const bool instanced, const BufferDescriptor& descriptor) noexcept
        : _type(type), _usage(usage), _bufferSize(bufferSize), _instanced(instanced), _descriptor(descriptor)
#if TAU_BUFFER_SAFETY
        , _modificationLockCount(0), _bindLockCount(0)
#endif
    { }
public:
    virtual ~IBuffer() noexcept
    {
#if TAU_BUFFER_SAFETY
  #if TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_MODIFYING
        if(_modificationLockCount != 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DestructedWhileModifying);
        }
  #endif
  #if TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_BOUND
        if(_bindLockCount != 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DestructedWhileBound);
        }
  #endif
#endif
    }

    [[nodiscard]] inline EBuffer::Type type() const noexcept { return _type; }
    [[nodiscard]] inline EBuffer::UsageType usage() const noexcept { return _usage; }
    [[nodiscard]] inline uSys bufferSize() const noexcept { return _bufferSize; }
    [[nodiscard]] inline uSys elementCount() const noexcept { return _bufferSize / _descriptor.stride(); }
    [[nodiscard]] inline bool instanced() const noexcept { return _instanced; }
    [[nodiscard]] inline const BufferDescriptor& descriptor() const noexcept { return _descriptor; }
    [[nodiscard]] inline BufferDescriptor& descriptor() noexcept { return _descriptor; }

    virtual void bind(IRenderingContext& context) noexcept = 0;
    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual bool beginModification(IRenderingContext& context) noexcept = 0;
    virtual void endModification(IRenderingContext& context) noexcept = 0;

    virtual void modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept = 0;
    virtual void fillBuffer(IRenderingContext& context, const void* data) noexcept = 0;

	template<typename _T>
	::std::intptr_t modifyBuffer(::std::intptr_t offset, const _T& data) noexcept
	{
	    modifyBuffer(offset, sizeof(_T), &data);
        return offset + sizeof(_T);
	}
	
    RTT_BASE_IMPL(IBuffer);
    RTT_BASE_CHECK(IBuffer);
    RTT_BASE_CAST(IBuffer);
};

class TAU_DLL TAU_NOVTABLE IIndexBuffer
{
    DELETE_COPY(IIndexBuffer);
protected:
    EBuffer::UsageType _usage;
    uSys _bufferSize;

#if TAU_BUFFER_SAFETY
    iSys _modificationLockCount;
    iSys _bindLockCount;
#endif
protected:
    IIndexBuffer(const EBuffer::UsageType usage, const uSys bufferSize) noexcept
        : _usage(usage), _bufferSize(bufferSize)
#if TAU_BUFFER_SAFETY
        , _modificationLockCount(0), _bindLockCount(0)
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
  #if TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_BOUND
        if(_bindLockCount != 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DestructedWhileBound);
        }
  #endif
#endif
    }

    [[nodiscard]] inline EBuffer::Type type() const noexcept { return EBuffer::Type::IndexBuffer; }
    [[nodiscard]] inline EBuffer::UsageType usage() const noexcept { return _usage; }
    [[nodiscard]] inline uSys bufferSize() const noexcept { return _bufferSize; }

    virtual void bind(IRenderingContext& context) noexcept = 0;
    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual bool beginModification(IRenderingContext& context) noexcept = 0;
    virtual void endModification(IRenderingContext& context) noexcept = 0;

    virtual void modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept = 0;
    virtual void fillBuffer(IRenderingContext& context, const void* data) noexcept = 0;

	template<typename _T>
	void modifyBuffer(::std::intptr_t offset, const _T& data) noexcept
    { modifyBuffer(offset, sizeof(_T), &data); }
	
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
    iSys _bindLockCount;
    iSys _uniformBindLockCount;
#endif
protected:
    IUniformBuffer(const EBuffer::UsageType usage, const uSys bufferSize) noexcept
        : _usage(usage), _bufferSize(bufferSize)
#if TAU_BUFFER_SAFETY
        , _modificationLockCount(0), _bindLockCount(0), _uniformBindLockCount(0)
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
  #if TAU_BUFFER_SAFETY_DESTRUCTED_WHILE_BOUND
        if(_bindLockCount != 0)
        {
            TAU_THROW(BufferSafetyException, BufferSafetyException::DestructedWhileBound);
        }
  #endif
#endif
    }

    [[nodiscard]] inline EBuffer::Type type() const noexcept { return EBuffer::Type::UniformBuffer; }
    [[nodiscard]] inline EBuffer::UsageType usage() const noexcept { return _usage; }
    [[nodiscard]] inline uSys bufferSize() const noexcept { return _bufferSize; }

    virtual void bind(IRenderingContext& context) noexcept = 0;
    virtual void unbind(IRenderingContext& context) noexcept = 0;

    virtual void bind(IRenderingContext& context, EShader::Stage stage, u32 index) noexcept = 0;
    virtual void unbind(IRenderingContext& context, EShader::Stage stage, u32 index) noexcept = 0;

    virtual bool beginModification(IRenderingContext& context) noexcept = 0;
    virtual void endModification(IRenderingContext& context) noexcept = 0;

    virtual void modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept = 0;
    virtual void fillBuffer(IRenderingContext& context, const void* data) noexcept = 0;

	template<typename _T>
	void modifyBuffer(::std::intptr_t offset, const _T& data) noexcept
    { modifyBuffer(offset, sizeof(_T), &data); }
	
    RTT_BASE_IMPL(IUniformBuffer);
    RTT_BASE_CHECK(IUniformBuffer);
    RTT_BASE_CAST(IUniformBuffer);
};

struct BufferArgs final
{
    DEFAULT_DESTRUCT(BufferArgs);
    DEFAULT_COPY(BufferArgs);
public:
    EBuffer::Type type;
    EBuffer::UsageType usage;
    uSys elementCount;
    const void* initialBuffer;
    bool instanced;
    BufferDescriptorBuilder descriptor;
public:
    inline BufferArgs(const uSys descriptorCount) noexcept
        : type(static_cast<EBuffer::Type>(0)),
          usage(static_cast<EBuffer::UsageType>(0)),
          elementCount(0), initialBuffer(null),
          instanced(false),
          descriptor(descriptorCount)
    { }

    [[nodiscard]] inline uSys bufferSize() const noexcept { return elementCount * descriptor.stride(); }
};

struct IndexBufferArgs final
{
    DEFAULT_DESTRUCT(IndexBufferArgs);
    DEFAULT_COPY(IndexBufferArgs);
public:
    EBuffer::UsageType usage;
    uSys elementCount;
    const void* initialBuffer;
public:
    inline IndexBufferArgs() noexcept
        : usage(static_cast<EBuffer::UsageType>(0)),
          elementCount(0), initialBuffer(null)
    { }

    [[nodiscard]] inline uSys bufferSize() const noexcept { return elementCount * sizeof(u32); }
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
        : usage(static_cast<EBuffer::UsageType>(0)),
          bufferSize(0), initialBuffer(null)
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
public:
    [[nodiscard]] virtual IBuffer* build(const BufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IBuffer* build(const BufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IBuffer> buildCPPRef(const BufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IBuffer> buildTauRef(const BufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IBuffer> buildTauSRef(const BufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};

class TAU_DLL TAU_NOVTABLE IIndexBufferBuilder
{
    DEFAULT_CONSTRUCT_PO(IIndexBufferBuilder);
    DEFAULT_DESTRUCT_VI(IIndexBufferBuilder);
    DELETE_COPY(IIndexBufferBuilder);
public:
    using Error = IBufferBuilder::Error;
public:
    [[nodiscard]] virtual IIndexBuffer* build(const IndexBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IIndexBuffer* build(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IIndexBuffer> buildCPPRef(const IndexBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IIndexBuffer> buildTauRef(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IIndexBuffer> buildTauSRef(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};

class TAU_DLL TAU_NOVTABLE IUniformBufferBuilder
{
    DEFAULT_CONSTRUCT_PO(IUniformBufferBuilder);
    DEFAULT_DESTRUCT_VI(IUniformBufferBuilder);
    DELETE_COPY(IUniformBufferBuilder);
public:
    using Error = IBufferBuilder::Error;
public:
    [[nodiscard]] virtual IUniformBuffer* build(const UniformBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IUniformBuffer* build(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IUniformBuffer> buildCPPRef(const UniformBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IUniformBuffer> buildTauRef(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IUniformBuffer> buildTauSRef(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
