#pragma once

#include <Objects.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"
#include "texture/TextureEnums.hpp"

#ifndef TAU_RTV_DEBUG
  #if defined(TAU_PRODUCTION)
    #define TAU_RTV_DEBUG 0
  #else
    #define TAU_RTV_DEBUG 1
  #endif
#endif

#if TAU_RTV_DEBUG
#include <String.hpp>
#endif

#define RTV_IMPL(_TYPE) RTT_IMPL(_TYPE, IRenderTargetView)
#define DSV_IMPL(_TYPE) RTT_IMPL(_TYPE, IDepthStencilView)

#if TAU_RTV_DEBUG
class TAU_DLL TAU_NOVTABLE IRenderTargetView
{
    DEFAULT_CONSTRUCT_PO(IRenderTargetView);
    DEFAULT_DESTRUCT_VI(IRenderTargetView);
    DEFAULT_CM_PO(IRenderTargetView);
protected:
    WDynString _targetName;
protected:
    IRenderTargetView(const WDynString& targetName) noexcept
        : _targetName(targetName)
    { }

    IRenderTargetView(WDynString&& targetName) noexcept
        : _targetName(::std::move(targetName))
    { }
public:
    [[nodiscard]] const WDynString& targetName() const noexcept { return _targetName; }

    RTT_BASE_IMPL(IRenderTargetView);
    RTT_BASE_CHECK(IRenderTargetView);
    RTT_BASE_CAST(IRenderTargetView);
};
#else
class TAU_DLL TAU_NOVTABLE IRenderTargetView
{
    DEFAULT_CONSTRUCT_PO(IRenderTargetView);
    DEFAULT_DESTRUCT_VI(IRenderTargetView);
    DEFAULT_CM_PO(IRenderTargetView);
public:
    RTT_BASE_IMPL(IRenderTargetView);
    RTT_BASE_CHECK(IRenderTargetView);
    RTT_BASE_CAST(IRenderTargetView);
};
#endif

class TAU_DLL TAU_NOVTABLE IDepthStencilView
{
    DEFAULT_CONSTRUCT_PO(IDepthStencilView);
    DEFAULT_DESTRUCT_VI(IDepthStencilView);
    DEFAULT_CM_PO(IDepthStencilView);
public:
    RTT_BASE_IMPL(IDepthStencilView);
    RTT_BASE_CHECK(IDepthStencilView);
    RTT_BASE_CAST(IDepthStencilView);
};

class IResource;

template<int _TypeSafety>
struct _RTVArgs final
{
    NullableRef<IResource> resource;
    ETexture::Format format;
};

using RenderTargetViewArgs = _RTVArgs<0>;
using DepthStencilViewArgs = _RTVArgs<1>;

class TAU_DLL TAU_NOVTABLE IRenderTargetViewBuilder
{
    DEFAULT_CONSTRUCT_PO(IRenderTargetViewBuilder);
    DEFAULT_DESTRUCT_VI(IRenderTargetViewBuilder);
    DEFAULT_CM_PO(IRenderTargetViewBuilder);
public:
    enum class Error
    {
        NoError = 0,
        NullResource,
        ResourceIsNotTexture,
        InvalidFormat,
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
     *        The XViewArgs type. This dictates the type of
     *      resource.
     */
    template<typename _Args>
    [[nodiscard]] uSys allocSize() const noexcept
    { return 0; }

    [[nodiscard]] virtual NullableRef<IRenderTargetView> createRenderTarget(const RenderTargetViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IDepthStencilView> createDepthStencil(const DepthStencilViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
protected:
    [[nodiscard]] virtual uSys _allocSize(uSys type) const noexcept = 0;
};

#define RTV_AS_RENDER_TARGET 1
#define RTV_AS_DEPTH_STENCIL 2

template<>
inline uSys IRenderTargetViewBuilder::allocSize<RenderTargetViewArgs>() const noexcept
{ return _allocSize(RTV_AS_RENDER_TARGET); }

template<>
inline uSys IRenderTargetViewBuilder::allocSize<DepthStencilViewArgs>() const noexcept
{ return _allocSize(RTV_AS_DEPTH_STENCIL); }
