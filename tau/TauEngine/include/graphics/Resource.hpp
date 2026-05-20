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
#include "ResourceHeap.hpp"
#include "Resource.debug.hpp"

class ICommandList;
class IResourceRawInterface;

namespace tau {

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
    static const ResourceMapRange* All() noexcept
    { return nullptr; }

    static const ResourceMapRange* None() noexcept
    {
        static ResourceMapRange none(0, 0);
        return &none;
    }
public:
    // Inclusive
    uSys Begin;
    // Exclusive
    uSys End;
public:
    ResourceMapRange(const uSys begin, const uSys end) noexcept
        : Begin(begin)
        , End(end)
    { }

    [[nodiscard]] uSys Length() const noexcept { return End - Begin; }

    [[nodiscard]] bool IsNone() const noexcept { return Begin >= End; }
};


#define RESOURCE_IMPL_BASE(_TYPE) \
    RTTD_IMPL(_TYPE, IResourceLegacy)

#define RESOURCE_IMPL(_TYPE) RESOURCE_IMPL_BASE(_TYPE)

/**
 * Describes a general graphics resource.
 *
 *   This is the general base for buffers and textures. Do
 * note that these types currently do not have any high level
 * specializations from IResource. Lower level APIs are free
 * to implement the various potential structures in their own
 * classes.
 */
class IResourceLegacy
{
    DELETE_CM(IResourceLegacy);
protected:
    IResourceLegacy(const uSys size, const EResource::Type resourceType, const EResource::UsageType usageType) noexcept
        : m_Size(size)
        , m_ResourceType(resourceType)
        , m_UsageType(usageType)
#if TAU_RESOURCE_DEBUG_DATA
        , _debugData(nullptr)
#endif
    { }
public:
#if TAU_RESOURCE_DEBUG_DATA
    virtual ~IResourceLegacy() noexcept
    { delete _debugData; }
#else
    virtual ~IResourceLegacy() noexcept = default;
#endif

    [[nodiscard]] uSys size() const noexcept { return m_Size; }

    [[nodiscard]] EResource::Type resourceType() const noexcept { return m_ResourceType; }
    [[nodiscard]] EResource::UsageType usageType() const noexcept { return m_UsageType; }

#if TAU_RESOURCE_DEBUG_DATA
    [[nodiscard]] const tau::debug::ResourceDebugData* debugData() const noexcept { return _debugData; }

    void attachDebugData(const tau::debug::ResourceDebugCategory& category, const WDynString& name) noexcept
    {
        delete _debugData;
        _debugData = new(::std::nothrow) tau::debug::ResourceDebugData(category, name);
    }

    void attachDebugData(const tau::debug::ResourceDebugCategory& category, WDynString&& name) noexcept
    {
        delete _debugData;
        _debugData = new(::std::nothrow) tau::debug::ResourceDebugData(category, ::std::move(name));
    }
#endif

    [[nodiscard]] virtual void* map(uSys mipLevel = 0, uSys arrayIndex = 0, const ResourceMapRange* mapReadRange = ResourceMapRange::None(), const ResourceMapRange* mapWriteRange = ResourceMapRange::All()) noexcept = 0;
    virtual void unmap(uSys mipLevel = 0, uSys arrayIndex = 0, const ResourceMapRange* mapWriteRange = ResourceMapRange::All()) noexcept = 0;

    template<typename Args>
    [[nodiscard]] const Args* getArgs() const noexcept { return nullptr; }

    [[nodiscard]] virtual const IResourceRawInterface& _getRawHandle() const noexcept = 0;

    RTTD_BASE_IMPL(IResourceLegacy);
    RTTD_BASE_CHECK(IResourceLegacy);
    RTTD_BASE_CAST(IResourceLegacy);
protected:
    [[nodiscard]] virtual const void* _getArgs() const noexcept = 0;
protected:
    uSys m_Size;
    EResource::Type m_ResourceType;
    EResource::UsageType m_UsageType;
#if TAU_RESOURCE_DEBUG_DATA
    const tau::debug::ResourceDebugData* _debugData;
#endif
};

class IResourceDebug : public tau::com::IUnknown
{
    DEFAULT_CONSTRUCT_PO(IResourceDebug);
    DEFAULT_CM_PO(IResourceDebug);
    DEFAULT_DESTRUCT_VI(IResourceDebug);
public:
    [[nodiscard]] virtual const tau::debug::ResourceDebugData* DebugData() const noexcept = 0;
    virtual void AttachDebugData(const tau::debug::ResourceDebugCategory& category, const C8DynString& name) noexcept = 0;
    virtual void AttachDebugData(const tau::debug::ResourceDebugCategory& category, C8DynString&& name) noexcept = 0;
};

struct ResourceConstructionInfo : tau::com::BaseConstructionInfo
{
    DEFAULT_CONSTRUCT_PU(ResourceConstructionInfo);
    DEFAULT_CM_PU(ResourceConstructionInfo);
    DEFAULT_DESTRUCT_VI(ResourceConstructionInfo);
public:
    /**
     * A hint on how the resource will be accessed.
     */
    EResource::UsageType UsageType;
};

struct ResourceBufferConstructionInfo final : ResourceConstructionInfo
{
    DEFAULT_CONSTRUCT_PU(ResourceBufferConstructionInfo);
    DEFAULT_CM_PU(ResourceBufferConstructionInfo);
    DEFAULT_DESTRUCT_VI(ResourceBufferConstructionInfo);
public:
    uSys Size;
    EBuffer::Type BufferType;
    const void* InitialBuffer;
};

struct ResourceTexture1DConstructionInfo final : ResourceConstructionInfo
{
    DEFAULT_CONSTRUCT_PU(ResourceTexture1DConstructionInfo);
    DEFAULT_CM_PU(ResourceTexture1DConstructionInfo);
    DEFAULT_DESTRUCT_VI(ResourceTexture1DConstructionInfo);
public:
    uSys Width;
    u16 ArrayCount;
    u16 MipLevels;
    ETexture::Format DataFormat;
    ETexture::BindFlags Flags;
    const void* const* InitialBuffers;
};

struct ResourceTexture2DConstructionInfo final : ResourceConstructionInfo
{
    DEFAULT_CONSTRUCT_PU(ResourceTexture2DConstructionInfo);
    DEFAULT_CM_PU(ResourceTexture2DConstructionInfo);
    DEFAULT_DESTRUCT_VI(ResourceTexture2DConstructionInfo);
public:
    uSys Width;
    u32 Height;
    u16 ArrayCount;
    u16 MipLevels;
    ETexture::Format DataFormat;
    ETexture::BindFlags Flags;
    const void* const* InitialBuffers;
};

struct ResourceTexture3DConstructionInfo final : ResourceConstructionInfo
{
    DEFAULT_CONSTRUCT_PU(ResourceTexture3DConstructionInfo);
    DEFAULT_CM_PU(ResourceTexture3DConstructionInfo);
    DEFAULT_DESTRUCT_VI(ResourceTexture3DConstructionInfo);
public:
    uSys Width;
    u32 Height;
    u16 Depth;
    u16 ArrayCount;
    u16 MipLevels;
    ETexture::Format DataFormat;
    ETexture::BindFlags Flags;
    const void* const* InitialBuffers;
};

class IResource : public tau::com::IUnknown
{
    DEFAULT_CONSTRUCT_PO(IResource);
    DEFAULT_CM_PO(IResource);
    DEFAULT_DESTRUCT_VI(IResource);
public:

    virtual u64 GetHandle() const noexcept = 0;
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
    const void* const* initialBuffers;
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
    const void* const* initialBuffers;
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
    const void* const* initialBuffers;
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
         * GlobalAlloc) fails (returns nullptr). This generally occurs
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
     * @tparam Args
     *        The ResourceXArgs type. This dictates the type of
     *      resource.
     */
    template<typename Args>
    [[nodiscard]] uSys allocSize() const noexcept
    { return 0; }

    [[nodiscard]] virtual NullableRef<IResource> buildTauRef(const ResourceBufferArgs& args, tau::IResourceHeap* heap, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IResource> buildTauRef(const ResourceTexture1DArgs& args, tau::IResourceHeap* heap, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IResource> buildTauRef(const ResourceTexture2DArgs& args, tau::IResourceHeap* heap, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IResource> buildTauRef(const ResourceTexture3DArgs& args, tau::IResourceHeap* heap, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
protected:
    [[nodiscard]] virtual uSys _allocSize(uSys type) const noexcept = 0;
};

template<>
inline const ResourceBufferArgs* IResourceLegacy::getArgs<ResourceBufferArgs>() const noexcept
{
    if(m_ResourceType == EResource::Type::Buffer)
    { return reinterpret_cast<const ResourceBufferArgs*>(_getArgs()); }
    return nullptr;
}

template<>
inline const ResourceTexture1DArgs* IResourceLegacy::getArgs<ResourceTexture1DArgs>() const noexcept
{
    if(m_ResourceType == EResource::Type::Texture1D)
    { return reinterpret_cast<const ResourceTexture1DArgs*>(_getArgs()); }
    return nullptr;
}

template<>
inline const ResourceTexture2DArgs* IResourceLegacy::getArgs<ResourceTexture2DArgs>() const noexcept
{
    if(m_ResourceType == EResource::Type::Texture2D)
    { return reinterpret_cast<const ResourceTexture2DArgs*>(_getArgs()); }
    return nullptr;
}

template<>
inline const ResourceTexture3DArgs* IResourceLegacy::getArgs<ResourceTexture3DArgs>() const noexcept
{
    if(m_ResourceType == EResource::Type::Texture3D)
    { return reinterpret_cast<const ResourceTexture3DArgs*>(_getArgs()); }
    return nullptr;
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

}
