/**
 * @file
 */
#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <Safeties.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"
#include "ResourceEnums.hpp"
#include "BufferEnums.hpp"
#include "texture/TextureEnums.hpp"

class IRenderingContext;

struct ResourceBufferArgs;
struct ResourceTextureArgs;

/**
 * Describes a range of a resource to access.
 *
 *   This allows for protection and potential optimizations for
 * certain memory operations.
 */
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

/**
 * Describes a general graphics resource.
 *
 *   This is the general base for buffers and textures. Do
 * note that these types currently do not have any high level
 * specializations from IResource. Lower level API's are free
 * to implement the various potential structures in their own
 * classes.
 */
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
    /**
     * The size in bytes of the resource allocation.
     */
    [[nodiscard]] uSys size() const noexcept { return _size; }

    [[nodiscard]] virtual EResource::Type resourceType() const noexcept = 0;

    [[nodiscard]] virtual void* map(IRenderingContext& context, EResource::MapType mapType = EResource::MapType::Default, uSys subResource = 0, const ResourceMapRange* mapReadRange = ResourceMapRange::none()) noexcept = 0;
    virtual void unmap(IRenderingContext& context, uSys subResource = 0) noexcept = 0;

    template<typename _Args>
    [[nodiscard]] const _Args* getArgs() const noexcept { return null; }

    RTTD_BASE_IMPL(IResource);
    RTTD_BASE_CHECK(IResource);
    RTTD_BASE_CAST(IResource);
protected:
    [[nodiscard]] virtual const void* _getArgs() const noexcept = 0;
};

struct ResourceBufferArgs final
{
    DEFAULT_CONSTRUCT_PU(ResourceBufferArgs);
    DEFAULT_DESTRUCT_VI(ResourceBufferArgs);
    DEFAULT_CM_PU(ResourceBufferArgs);
public:
    uSys size;
    EBuffer::Type bufferType;
    /**
     * A hint on how the resource will be accessed.
     */
    EResource::UsageType usageType;
    const void* initialBuffer;
};

struct ResourceTextureArgs final
{
    DEFAULT_CONSTRUCT_PU(ResourceTextureArgs);
    DEFAULT_DESTRUCT_VI(ResourceTextureArgs);
    DEFAULT_CM_PU(ResourceTextureArgs);
public:
    uSys size;
    ETexture::Type type;
    ETexture::Format dataFormat;
    ETexture::BindFlags flags;
    /**
     * A hint on how the resource will be accessed.
     */
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
        InvalidTextureType,
        InvalidTextureFormat,
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

    [[nodiscard]] virtual IResource* build(const ResourceTextureArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IResource* build(const ResourceTextureArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IResource> buildCPPRef(const ResourceTextureArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IResource> buildTauRef(const ResourceTextureArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IResource> buildTauSRef(const ResourceTextureArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};

template<>
inline const ResourceBufferArgs* IResource::getArgs<ResourceBufferArgs>() const noexcept
{
    switch(resourceType())
    {
        case EResource::Type::Buffer: return reinterpret_cast<const ResourceBufferArgs*>(_getArgs());
        case EResource::Type::Texture1D:
        case EResource::Type::Texture2D:
        case EResource::Type::Texture3D: return null;
        default: return null;
    }
}

template<>
inline const ResourceTextureArgs* IResource::getArgs<ResourceTextureArgs>() const noexcept
{
    switch(resourceType())
    {
        case EResource::Type::Buffer: return null;
        case EResource::Type::Texture1D:
        case EResource::Type::Texture2D:
        case EResource::Type::Texture3D: return reinterpret_cast<const ResourceTextureArgs*>(_getArgs());
        default: return null;
    }
}
