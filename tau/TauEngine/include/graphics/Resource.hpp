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

    [[nodiscard]] virtual void* map(IRenderingContext& context, EResource::MapType mapType = EResource::MapType::Default, uSys mipLevel = 0, uSys arrayIndex = 0, const ResourceMapRange* mapReadRange = ResourceMapRange::none()) noexcept = 0;
    virtual void unmap(IRenderingContext& context, uSys mipLevel = 0, uSys arrayIndex = 0) noexcept = 0;

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
     * @tparam _Ptr
     *        The pointer type. This is going to be an IResource*,
     *      a NullableRef<IResource>, or a
     *      NullableStrongRef<IResource>.
     */
    template<typename _Args, typename _Ptr>
    [[nodiscard]] uSys allocSize() const noexcept
    { return 0; }

    [[nodiscard]] virtual IResource* build(const ResourceBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IResource* build(const ResourceBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IResource> buildCPPRef(const ResourceBufferArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IResource> buildTauRef(const ResourceBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IResource> buildTauSRef(const ResourceBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual IResource* build(const ResourceTexture1DArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IResource* build(const ResourceTexture1DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IResource> buildCPPRef(const ResourceTexture1DArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IResource> buildTauRef(const ResourceTexture1DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IResource> buildTauSRef(const ResourceTexture1DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual IResource* build(const ResourceTexture2DArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IResource* build(const ResourceTexture2DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IResource> buildCPPRef(const ResourceTexture2DArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IResource> buildTauRef(const ResourceTexture2DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IResource> buildTauSRef(const ResourceTexture2DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual IResource* build(const ResourceTexture3DArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IResource* build(const ResourceTexture3DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IResource> buildCPPRef(const ResourceTexture3DArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IResource> buildTauRef(const ResourceTexture3DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IResource> buildTauSRef(const ResourceTexture3DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
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

template<>
inline uSys IResourceBuilder::allocSize<ResourceBufferArgs, IResource*>() const noexcept
{ return _allocSize(1); }

template<>
inline uSys IResourceBuilder::allocSize<ResourceBufferArgs, NullableRef<IResource>>() const noexcept
{ return _allocSize(2); }

template<>
inline uSys IResourceBuilder::allocSize<ResourceBufferArgs, NullableStrongRef<IResource>>() const noexcept
{ return _allocSize(3); }

template<>
inline uSys IResourceBuilder::allocSize<ResourceTexture1DArgs, IResource*>() const noexcept
{ return _allocSize(4); }

template<>
inline uSys IResourceBuilder::allocSize<ResourceTexture1DArgs, NullableRef<IResource>>() const noexcept
{ return _allocSize(5); }

template<>
inline uSys IResourceBuilder::allocSize<ResourceTexture1DArgs, NullableStrongRef<IResource>>() const noexcept
{ return _allocSize(6); }

template<>
inline uSys IResourceBuilder::allocSize<ResourceTexture2DArgs, IResource*>() const noexcept
{ return _allocSize(7); }

template<>
inline uSys IResourceBuilder::allocSize<ResourceTexture2DArgs, NullableRef<IResource>>() const noexcept
{ return _allocSize(8); }

template<>
inline uSys IResourceBuilder::allocSize<ResourceTexture2DArgs, NullableStrongRef<IResource>>() const noexcept
{ return _allocSize(9); }

template<>
inline uSys IResourceBuilder::allocSize<ResourceTexture3DArgs, IResource*>() const noexcept
{ return _allocSize(10); }

template<>
inline uSys IResourceBuilder::allocSize<ResourceTexture3DArgs, NullableRef<IResource>>() const noexcept
{ return _allocSize(11); }

template<>
inline uSys IResourceBuilder::allocSize<ResourceTexture3DArgs, NullableStrongRef<IResource>>() const noexcept
{ return _allocSize(12); }
