#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <Safeties.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"
#include "ResourceEnums.hpp"
#include "BufferEnums.hpp"

class IRenderingContext;

struct ResourceMapRange final
{
    DEFAULT_DESTRUCT(ResourceMapRange);
    DEFAULT_CM_PO(ResourceMapRange);
public:
    static const ResourceMapRange* all() noexcept
    { return null; }

    static const ResourceMapRange* none() noexcept
    {
        static ResourceMapRange _none(0, 0);
        return &_none;
    }
public:
    uSys begin;
    uSys end;
public:
    ResourceMapRange(const uSys _begin, const uSys _end) noexcept
        : begin(_begin)
        , end(_end)
    { }
};

#define RESOURCE_IMPL_BASE(_TYPE) \
    RTTD_IMPL(_TYPE, IResource)

#define RESOURCE_IMPL(_TYPE) RESOURCE_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE IResource
{
    DEFAULT_DESTRUCT_VI(IResource);
    DEFAULT_CM_PO(IResource);
protected:
    uSys _size;
protected:
    IResource(const uSys size) noexcept
        : _size(size)
    { }
public:
    [[nodiscard]] uSys size() const noexcept { return _size; }

    [[nodiscard]] virtual EResource::Type resourceType() const noexcept = 0;

    [[nodiscard]] virtual void* map(IRenderingContext& context, EResource::MapType mapType = EResource::MapType::Default, const ResourceMapRange* mapReadRange = ResourceMapRange::none()) noexcept = 0;
    virtual void unmap(IRenderingContext& context) noexcept = 0;

    RTTD_BASE_IMPL(IResource);
    RTTD_BASE_CHECK(IResource);
    RTTD_BASE_CAST(IResource);
};

struct ResourceBufferArgs final
{
    DEFAULT_CONSTRUCT_PU(ResourceBufferArgs);
    DEFAULT_DESTRUCT_VI(ResourceBufferArgs);
    DEFAULT_CM_PU(ResourceBufferArgs);
public:
    uSys size;
    EBuffer::Type bufferType;
    EResource::UsageType usageType;
    const void* initialBuffer;
};

class TAU_DLL TAU_NOVTABLE IResourceBuilder
{
    DEFAULT_CONSTRUCT_PO(IResourceBuilder);
    DEFAULT_DESTRUCT_VI(IResourceBuilder);
    DEFAULT_CM_PO(IResourceBuilder);
public:
    enum class Error
    {
        NoError = 0,
        InvalidSize,
        InvalidUsageType,
        InvalidBufferType,
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
        InternalError
    };
public:
    [[nodiscard]] virtual IResource* build(const ResourceBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IResource* build(const ResourceBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IResource> buildCPPRef(const ResourceBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IResource> buildTauRef(const ResourceBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IResource> buildTauSRef(const ResourceBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

};
