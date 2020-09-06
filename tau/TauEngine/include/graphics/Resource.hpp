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
#include "_GraphicsOpaqueObjects.hpp"

class ICommandList;
class IResourceRawInterface;

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
    // Inclusive
    uSys begin;
    // Exclusive
    uSys end;
public:
    ResourceMapRange(const uSys _begin, const uSys _end) noexcept
        : begin(_begin)
        , end(_end)
    { }

    [[nodiscard]] uSys length() const noexcept { return end - begin; }
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
    [[nodiscard]] uSys size() const noexcept { return _size; }

    [[nodiscard]] virtual EResource::Type resourceType() const noexcept = 0;

    [[nodiscard]] virtual void* map(ICommandList& context, EResource::MapType mapType = EResource::MapType::Default, uSys mipLevel = 0, uSys arrayIndex = 0, const ResourceMapRange* mapReadRange = ResourceMapRange::none(), const ResourceMapRange* mapWriteRange = ResourceMapRange::all()) noexcept = 0;
    virtual void unmap(ICommandList& context, uSys mipLevel = 0, uSys arrayIndex = 0, const ResourceMapRange* mapWriteRange = null) noexcept = 0;

    template<typename _Args>
    [[nodiscard]] const _Args* getArgs() const noexcept { return null; }

    [[nodiscard]] virtual const IResourceRawInterface& _getRawHandle() const noexcept = 0;

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

struct ResourceTexture1DArgs final
{
    DEFAULT_CONSTRUCT_PU(ResourceTexture1DArgs);
    DEFAULT_DESTRUCT_VI(ResourceTexture1DArgs);
    DEFAULT_CM_PU(ResourceTexture1DArgs);
public:
    uSys width;
    u16 arrayCount;
    u16 mipLevels;
    ETexture::Format dataFormat;
    ETexture::BindFlags flags;
    /**
     * A hint on how the resource will be accessed.
     */
    EResource::UsageType usageType;
    const void* const * initialBuffers;
};

struct ResourceTexture2DArgs final
{
    DEFAULT_CONSTRUCT_PU(ResourceTexture2DArgs);
    DEFAULT_DESTRUCT_VI(ResourceTexture2DArgs);
    DEFAULT_CM_PU(ResourceTexture2DArgs);
public:
    uSys width;
    u32 height;
    u16 arrayCount;
    u16 mipLevels;
    ETexture::Format dataFormat;
    ETexture::BindFlags flags;
    /**
     * A hint on how the resource will be accessed.
     */
    EResource::UsageType usageType;
    const void* const * initialBuffers;
};

struct ResourceTexture3DArgs final
{
    DEFAULT_CONSTRUCT_PU(ResourceTexture3DArgs);
    DEFAULT_DESTRUCT_VI(ResourceTexture3DArgs);
    DEFAULT_CM_PU(ResourceTexture3DArgs);
public:
    uSys width;
    u32 height;
    u16 depth;
    u16 mipLevels;
    ETexture::Format dataFormat;
    ETexture::BindFlags flags;
    /**
     * A hint on how the resource will be accessed.
     */
    EResource::UsageType usageType;
    const void* const * initialBuffers;
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
        InvalidWidth,
        InvalidHeight,
        InvalidDepth,
        InvalidArrayCount,
        InvalidMipCount,
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
    /**
     * Gets the size required for an allocation.
     *
     * This is useful for fixed block allocators.
     *
     * @tparam _Args
     *        The ResourceXArgs type. This dictates the type of
     *      resource.
     */
    template<typename _Args>
    [[nodiscard]] uSys allocSize() const noexcept
    { return 0; }

    [[nodiscard]] virtual NullableRef<IResource> buildTauRef(const ResourceBufferArgs& args, ResourceHeap heap, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IResource> buildTauRef(const ResourceTexture1DArgs& args, ResourceHeap heap, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IResource> buildTauRef(const ResourceTexture2DArgs& args, ResourceHeap heap, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IResource> buildTauRef(const ResourceTexture3DArgs& args, ResourceHeap heap, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
protected:
    [[nodiscard]] virtual uSys _allocSize(uSys type) const noexcept = 0;
};

template<>
inline const ResourceBufferArgs* IResource::getArgs<ResourceBufferArgs>() const noexcept
{
    if(resourceType() == EResource::Type::Buffer)
    { return reinterpret_cast<const ResourceBufferArgs*>(_getArgs()); }
    return null;
}

template<>
inline const ResourceTexture1DArgs* IResource::getArgs<ResourceTexture1DArgs>() const noexcept
{
    if(resourceType() == EResource::Type::Texture1D)
    { return reinterpret_cast<const ResourceTexture1DArgs*>(_getArgs()); }
    return null;
}

template<>
inline const ResourceTexture2DArgs* IResource::getArgs<ResourceTexture2DArgs>() const noexcept
{
    if(resourceType() == EResource::Type::Texture2D)
    { return reinterpret_cast<const ResourceTexture2DArgs*>(_getArgs()); }
    return null;
}

template<>
inline const ResourceTexture3DArgs* IResource::getArgs<ResourceTexture3DArgs>() const noexcept
{
    if(resourceType() == EResource::Type::Texture3D)
    { return reinterpret_cast<const ResourceTexture3DArgs*>(_getArgs()); }
    return null;
}

#define RB_AS_BUFFER     1
#define RB_AS_TEXTURE_1D 2
#define RB_AS_TEXTURE_2D 3
#define RB_AS_TEXTURE_3D 4

template<>
inline uSys IResourceBuilder::allocSize<ResourceBufferArgs>() const noexcept
{ return _allocSize(RB_AS_BUFFER); }

template<>
inline uSys IResourceBuilder::allocSize<ResourceTexture1DArgs>() const noexcept
{ return _allocSize(RB_AS_TEXTURE_1D); }

template<>
inline uSys IResourceBuilder::allocSize<ResourceTexture2DArgs>() const noexcept
{ return _allocSize(RB_AS_TEXTURE_2D); }

template<>
inline uSys IResourceBuilder::allocSize<ResourceTexture3DArgs>() const noexcept
{ return _allocSize(RB_AS_TEXTURE_3D); }
