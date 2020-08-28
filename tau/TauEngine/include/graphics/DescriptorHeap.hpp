/**
 * @file
 */
#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>
#include <NumTypes.hpp>
#include <RunTimeType.hpp>
#include <allocator/TauAllocator.hpp>

#include "DLL.hpp"
#include "GraphicsEnums.hpp"

class IResourceView;

#define DESCRIPTOR_HEAP_IMPL_BASE(_TYPE) \
    RTT_IMPL(_TYPE, IDescriptorHeap)

#define DESCRIPTOR_HEAP_IMPL(_TYPE) DESCRIPTOR_HEAP_IMPL_BASE(_TYPE)

template<typename _T, int _TypeSafety>
class TDescriptorHandle final
{
    DEFAULT_CONSTRUCT_PU(TDescriptorHandle);
    DEFAULT_DESTRUCT(TDescriptorHandle);
    DEFAULT_CM_PU(TDescriptorHandle);
public:
    _T ptr;
public:
    TDescriptorHandle(const _T _ptr) noexcept
        : ptr(_ptr)
    { }

    TDescriptorHandle(const _T _ptr, const _T offset) noexcept
        : ptr(_ptr + offset)
    { }

    TDescriptorHandle(const _T _ptr, const _T offsetCount, const _T offsetStride) noexcept
        : ptr(_ptr + offsetCount * offsetStride)
    { }

    TDescriptorHandle(const TDescriptorHandle<_T, _TypeSafety> handle, const _T offset) noexcept
        : ptr(handle.ptr + offset)
    { }

    TDescriptorHandle(const TDescriptorHandle<_T, _TypeSafety> handle, const _T offsetCount, const _T offsetStride) noexcept
        : ptr(handle.ptr + offsetCount * offsetStride)
    { }

    TDescriptorHandle<_T, _TypeSafety>& offset(const _T offset) noexcept
    {
        ptr += offset;
        return *this;
    }

    TDescriptorHandle<_T, _TypeSafety>& offset(const _T offsetCount, const _T offsetStride) noexcept
    {
        ptr += offsetCount * offsetStride;
        return *this;
    }

    [[nodiscard]] TDescriptorHandle<_T, _TypeSafety> offset(const _T offset) const noexcept
    { return TDescriptorHandle<_T, _TypeSafety>(ptr, offset); }

    [[nodiscard]] TDescriptorHandle<_T, _TypeSafety> offset(const _T offsetCount, const _T offsetStride) const noexcept
    { return TDescriptorHandle<_T, _TypeSafety>(ptr, offsetCount, offsetStride); }

    [[nodiscard]] operator _T() const noexcept { return ptr; }
    [[nodiscard]] operator bool() const noexcept { return ptr; }
    [[nodiscard]] operator void*() const noexcept { return reinterpret_cast<void*>(static_cast<uPtr>(ptr)); }

    template<typename _TT>
    [[nodiscard]] _TT* as() const noexcept
    { return reinterpret_cast<_TT*>(static_cast<uPtr>(ptr)); }

    [[nodiscard]] bool operator ==(const TDescriptorHandle<_T, _TypeSafety>& other) const noexcept { return ptr == other.ptr; }
    [[nodiscard]] bool operator !=(const TDescriptorHandle<_T, _TypeSafety>& other) const noexcept { return ptr != other.ptr; }

    [[nodiscard]] bool operator ==(const _T _ptr) const noexcept { return ptr == _ptr; }
    [[nodiscard]] bool operator !=(const _T _ptr) const noexcept { return ptr != _ptr; }

    [[nodiscard]] bool operator > (const TDescriptorHandle<_T, _TypeSafety>& other) const noexcept { return ptr >  other.ptr; }
    [[nodiscard]] bool operator < (const TDescriptorHandle<_T, _TypeSafety>& other) const noexcept { return ptr <  other.ptr; }
    [[nodiscard]] bool operator >=(const TDescriptorHandle<_T, _TypeSafety>& other) const noexcept { return ptr >= other.ptr; }
    [[nodiscard]] bool operator <=(const TDescriptorHandle<_T, _TypeSafety>& other) const noexcept { return ptr <= other.ptr; }

    [[nodiscard]] bool operator > (const _T _ptr) const noexcept { return ptr >  _ptr; }
    [[nodiscard]] bool operator < (const _T _ptr) const noexcept { return ptr <  _ptr; }
    [[nodiscard]] bool operator >=(const _T _ptr) const noexcept { return ptr >= _ptr; }
    [[nodiscard]] bool operator <=(const _T _ptr) const noexcept { return ptr <= _ptr; }
};

using CPUDescriptorHandle = TDescriptorHandle<uSys, 0>;
using GPUDescriptorHandle = TDescriptorHandle<u64, 1>;

/**
 * A heap of memory intended to hold descriptors.
 *
 *   This is intended to map to DirectX12 descriptor heaps. The
 * exact implementation may vary heavily though, due to the
 * drastic differences between conventional graphics API's, and
 * the more modern low-overhead API's.
 *
 *   Unlike DirectX12 you are expected to destroy tables. This
 * is because the descriptor table is intended to contain
 * allocations for all of the Tau descriptor objects.
 */
class TAU_DLL TAU_NOVTABLE IDescriptorHeap
{
    DEFAULT_CONSTRUCT_PO(IDescriptorHeap);
    DEFAULT_DESTRUCT_VI(IDescriptorHeap);
    DEFAULT_CM_PO(IDescriptorHeap);
public:
    [[nodiscard]] virtual EGraphics::DescriptorType type() const noexcept = 0;

    [[nodiscard]] virtual CPUDescriptorHandle getBaseCPUHandle() const noexcept = 0;
    [[nodiscard]] virtual GPUDescriptorHandle getBaseGPUHandle() const noexcept = 0;

    [[nodiscard]] virtual uSys getOffsetStride() const noexcept = 0;

    /**
     * Allocates a descriptor table on the heap.
     *
     *   This function takes in an optional allocator. This
     * allocator cannot be a fixed block allocator unless all
     * tables are the same size. This is because in the backend the
     * API is intended to allocate a large placement buffer to fit
     * all of the Tau descriptor objects, and then use placement
     * new to create the actual objects within that predefined
     * space.
     * 
     * @param[in] numDescriptors
     *      The number of descriptors the table will contain.
     */
    // [[nodiscard]] virtual DescriptorTable allocateTable(uSys numDescriptors) noexcept = 0;

    /**
     * Deallocates a descriptor table.
     *
     *   It is important to destroy the descriptor tables manually
     * as they contain an allocation for all of the Tau descriptors.
     *
     * @param[in] table
     *      The descriptor table to destroy.
     */
    // [[nodiscard]] virtual void destroyTable(DescriptorTable table) noexcept = 0;

    RTT_BASE_IMPL(IDescriptorHeap);
    RTT_BASE_CHECK(IDescriptorHeap);
    RTT_BASE_CAST(IDescriptorHeap);
};

struct DescriptorHeapArgs
{
    EGraphics::DescriptorType type;
    uSys numDescriptors;
    EGraphics::DescriptorHeapFlags flags;
};

class TAU_DLL TAU_NOVTABLE IDescriptorHeapBuilder
{
    DEFAULT_CONSTRUCT_PO(IDescriptorHeapBuilder);
    DEFAULT_DESTRUCT_VI(IDescriptorHeapBuilder);
    DEFAULT_CM_PO(IDescriptorHeapBuilder);
public:
    enum class Error
    {
        NoError = 0,
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
     * @tparam _T
     *        The pointer type. This is going to be an
     *      IDescriptorHeap*, a NullableRef<IDescriptorHeap>, or a
     *      NullableStrongRef<IDescriptorHeap>. The same would hold
     *      true for IDescriptorSamplerHeap.
     */
    template<typename _T, EGraphics::DescriptorType _DescriptorType>
    [[nodiscard]] uSys allocSize() const noexcept
    { return 0; }

    [[nodiscard]] virtual IDescriptorHeap* build(const DescriptorHeapArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IDescriptorHeap* build(const DescriptorHeapArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IDescriptorHeap> buildCPPRef(const DescriptorHeapArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IDescriptorHeap> buildTauRef(const DescriptorHeapArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IDescriptorHeap> buildTauSRef(const DescriptorHeapArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
protected:
    [[nodiscard]] virtual uSys _allocSize(uSys type) const noexcept = 0;
};

#define _DHB_AS_RAW_TV   1
#define _DHB_AS_NR_TV    2
#define _DHB_AS_NSR_TV   3
#define _DHB_AS_RAW_RTV  4
#define _DHB_AS_NR_RTV   5
#define _DHB_AS_NSR_RTV  6
#define _DHB_AS_RAW_DSV  7
#define _DHB_AS_NR_DSV   8
#define _DHB_AS_NSR_DSV  9
#define _DHB_AS_RAW_UBV 10
#define _DHB_AS_NR_UBV  11
#define _DHB_AS_NSR_UBV 12
#define _DHB_AS_RAW_UAV 13
#define _DHB_AS_NR_UAV  14
#define _DHB_AS_NSR_UAV 15
#define _DHB_AS_RAW_S   16
#define _DHB_AS_NR_S    17
#define _DHB_AS_NSR_S   18

template<>
inline uSys IDescriptorHeapBuilder::allocSize<IDescriptorHeap, EGraphics::DescriptorType::TextureView>() const noexcept
{ return _allocSize(_DHB_AS_RAW_TV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<NullableRef<IDescriptorHeap>, EGraphics::DescriptorType::TextureView>() const noexcept
{ return _allocSize(_DHB_AS_NR_TV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<NullableStrongRef<IDescriptorHeap>, EGraphics::DescriptorType::TextureView>() const noexcept
{ return _allocSize(_DHB_AS_NSR_TV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<IDescriptorHeap, EGraphics::DescriptorType::RenderTargetView>() const noexcept
{ return _allocSize(_DHB_AS_RAW_RTV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<NullableRef<IDescriptorHeap>, EGraphics::DescriptorType::RenderTargetView>() const noexcept
{ return _allocSize(_DHB_AS_NR_RTV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<NullableStrongRef<IDescriptorHeap>, EGraphics::DescriptorType::RenderTargetView>() const noexcept
{ return _allocSize(_DHB_AS_NSR_RTV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<IDescriptorHeap, EGraphics::DescriptorType::DepthStencilView>() const noexcept
{ return _allocSize(_DHB_AS_RAW_DSV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<NullableRef<IDescriptorHeap>, EGraphics::DescriptorType::DepthStencilView>() const noexcept
{ return _allocSize(_DHB_AS_NR_DSV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<NullableStrongRef<IDescriptorHeap>, EGraphics::DescriptorType::DepthStencilView>() const noexcept
{ return _allocSize(_DHB_AS_NSR_DSV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<IDescriptorHeap, EGraphics::DescriptorType::UniformBufferView>() const noexcept
{ return _allocSize(_DHB_AS_RAW_UBV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<NullableRef<IDescriptorHeap>, EGraphics::DescriptorType::UniformBufferView>() const noexcept
{ return _allocSize(_DHB_AS_NR_UBV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<NullableStrongRef<IDescriptorHeap>, EGraphics::DescriptorType::UniformBufferView>() const noexcept
{ return _allocSize(_DHB_AS_NSR_UBV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<IDescriptorHeap, EGraphics::DescriptorType::UnorderedAccessView>() const noexcept
{ return _allocSize(_DHB_AS_RAW_UAV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<NullableRef<IDescriptorHeap>, EGraphics::DescriptorType::UnorderedAccessView>() const noexcept
{ return _allocSize(_DHB_AS_NR_UAV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<NullableStrongRef<IDescriptorHeap>, EGraphics::DescriptorType::UnorderedAccessView>() const noexcept
{ return _allocSize(_DHB_AS_NSR_UAV); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<IDescriptorHeap, EGraphics::DescriptorType::Sampler>() const noexcept
{ return _allocSize(_DHB_AS_RAW_S); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<NullableRef<IDescriptorHeap>, EGraphics::DescriptorType::Sampler>() const noexcept
{ return _allocSize(_DHB_AS_NR_S); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<NullableStrongRef<IDescriptorHeap>, EGraphics::DescriptorType::Sampler>() const noexcept
{ return _allocSize(_DHB_AS_NSR_S); }
