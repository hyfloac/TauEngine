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
#include "_GraphicsOpaqueObjects.hpp"
#include "GraphicsEnums.hpp"

class IResourceView;

#define DESCRIPTOR_HEAP_IMPL_BASE(_TYPE) \
    RTT_IMPL(_TYPE, IDescriptorHeap)

#define DESCRIPTOR_SAMPLER_HEAP_IMPL_BASE(_TYPE) \
    RTT_IMPL(_TYPE, IDescriptorSamplerHeap)

#define DESCRIPTOR_HEAP_IMPL(_TYPE) DESCRIPTOR_HEAP_IMPL_BASE(_TYPE)
#define DESCRIPTOR_SAMPLER_HEAP_IMPL(_TYPE) DESCRIPTOR_SAMPLER_HEAP_IMPL_BASE(_TYPE)

class CPUDescriptorHandle final
{
    DEFAULT_CONSTRUCT_PU(CPUDescriptorHandle);
    DEFAULT_DESTRUCT(CPUDescriptorHandle);
    DEFAULT_CM_PU(CPUDescriptorHandle);
public:
    uSys ptr;
public:
    CPUDescriptorHandle(const uSys _ptr) noexcept
        : ptr(_ptr)
    { }

    CPUDescriptorHandle(const uSys _ptr, const uSys offset) noexcept
        : ptr(_ptr + offset)
    { }

    CPUDescriptorHandle(const uSys _ptr, const uSys offsetCount, const uSys offsetStride) noexcept
        : ptr(_ptr + offsetCount * offsetStride)
    { }

    CPUDescriptorHandle(const CPUDescriptorHandle handle, const uSys offset) noexcept
        : ptr(handle.ptr + offset)
    { }

    CPUDescriptorHandle(const CPUDescriptorHandle handle, const uSys offsetCount, const uSys offsetStride) noexcept
        : ptr(handle.ptr + offsetCount * offsetStride)
    { }

    CPUDescriptorHandle& offset(const uSys offset) noexcept
    {
        ptr += offset;
        return *this;
    }

    CPUDescriptorHandle& offset(const uSys offsetCount, const uSys offsetStride) noexcept
    {
        ptr += offsetCount * offsetStride;
        return *this;
    }

    [[nodiscard]] CPUDescriptorHandle offset(const uSys offset) const noexcept
    { return CPUDescriptorHandle(ptr, offset); }

    [[nodiscard]] CPUDescriptorHandle offset(const uSys offsetCount, const uSys offsetStride) const noexcept
    { return CPUDescriptorHandle(ptr, offsetCount, offsetStride); }

    [[nodiscard]] operator uSys() const noexcept { return ptr; }
    [[nodiscard]] operator bool() const noexcept { return ptr; }
    [[nodiscard]] operator void*() const noexcept { return reinterpret_cast<void*>(static_cast<uPtr>(ptr)); }

    [[nodiscard]] bool operator ==(const CPUDescriptorHandle& other) const noexcept { return ptr == other.ptr; }
    [[nodiscard]] bool operator !=(const CPUDescriptorHandle& other) const noexcept { return ptr != other.ptr; }

    [[nodiscard]] bool operator ==(const uSys _ptr) const noexcept { return ptr == _ptr; }
    [[nodiscard]] bool operator !=(const uSys _ptr) const noexcept { return ptr != _ptr; }

    [[nodiscard]] bool operator > (const CPUDescriptorHandle& other) const noexcept { return ptr >  other.ptr; }
    [[nodiscard]] bool operator < (const CPUDescriptorHandle& other) const noexcept { return ptr <  other.ptr; }
    [[nodiscard]] bool operator >=(const CPUDescriptorHandle& other) const noexcept { return ptr >= other.ptr; }
    [[nodiscard]] bool operator <=(const CPUDescriptorHandle& other) const noexcept { return ptr <= other.ptr; }

    [[nodiscard]] bool operator > (const uSys _ptr) const noexcept { return ptr >  _ptr; }
    [[nodiscard]] bool operator < (const uSys _ptr) const noexcept { return ptr <  _ptr; }
    [[nodiscard]] bool operator >=(const uSys _ptr) const noexcept { return ptr >= _ptr; }
    [[nodiscard]] bool operator <=(const uSys _ptr) const noexcept { return ptr <= _ptr; }
};

class GPUDescriptorHandle final
{
    DEFAULT_CONSTRUCT_PU(GPUDescriptorHandle);
    DEFAULT_DESTRUCT(GPUDescriptorHandle);
    DEFAULT_CM_PU(GPUDescriptorHandle);
public:
    u64 ptr;
public:
    GPUDescriptorHandle(const u64 _ptr) noexcept
        : ptr(_ptr)
    { }

    GPUDescriptorHandle(const u64 _ptr, const u64 offset) noexcept
        : ptr(_ptr + offset)
    { }

    GPUDescriptorHandle(const u64 _ptr, const u64 offsetCount, const u64 offsetStride) noexcept
        : ptr(_ptr + offsetCount * offsetStride)
    { }

    GPUDescriptorHandle(const GPUDescriptorHandle handle, const u64 offset) noexcept
        : ptr(handle.ptr + offset)
    { }

    GPUDescriptorHandle(const GPUDescriptorHandle handle, const u64 offsetCount, const u64 offsetStride) noexcept
        : ptr(handle.ptr + offsetCount * offsetStride)
    { }

    GPUDescriptorHandle& offset(const u64 offset) noexcept
    {
        ptr += offset;
        return *this;
    }

    GPUDescriptorHandle& offset(const u64 offsetCount, const u64 offsetStride) noexcept
    {
        ptr += offsetCount * offsetStride;
        return *this;
    }

    [[nodiscard]] GPUDescriptorHandle offset(const u64 offset) const noexcept
    { return GPUDescriptorHandle(ptr, offset); }

    [[nodiscard]] GPUDescriptorHandle offset(const u64 offsetCount, const u64 offsetStride) const noexcept
    { return GPUDescriptorHandle(ptr, offsetCount, offsetStride); }

    [[nodiscard]] operator u64() const noexcept { return ptr; }
    [[nodiscard]] operator bool() const noexcept { return ptr; }
    [[nodiscard]] operator void*() const noexcept { return reinterpret_cast<void*>(static_cast<uPtr>(ptr)); }

    [[nodiscard]] bool operator ==(const GPUDescriptorHandle& other) const noexcept { return ptr == other.ptr; }
    [[nodiscard]] bool operator !=(const GPUDescriptorHandle& other) const noexcept { return ptr != other.ptr; }

    [[nodiscard]] bool operator ==(const u64 _ptr) const noexcept { return ptr == _ptr; }
    [[nodiscard]] bool operator !=(const u64 _ptr) const noexcept { return ptr != _ptr; }

    [[nodiscard]] bool operator > (const GPUDescriptorHandle& other) const noexcept { return ptr > other.ptr; }
    [[nodiscard]] bool operator < (const GPUDescriptorHandle& other) const noexcept { return ptr < other.ptr; }
    [[nodiscard]] bool operator >=(const GPUDescriptorHandle& other) const noexcept { return ptr >= other.ptr; }
    [[nodiscard]] bool operator <=(const GPUDescriptorHandle& other) const noexcept { return ptr <= other.ptr; }

    [[nodiscard]] bool operator > (const u64 _ptr) const noexcept { return ptr > _ptr; }
    [[nodiscard]] bool operator < (const u64 _ptr) const noexcept { return ptr < _ptr; }
    [[nodiscard]] bool operator >=(const u64 _ptr) const noexcept { return ptr >= _ptr; }
    [[nodiscard]] bool operator <=(const u64 _ptr) const noexcept { return ptr <= _ptr; }
};

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
    template<typename _T>
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

template<>
inline uSys IDescriptorHeapBuilder::allocSize<IDescriptorHeap>() const noexcept
{ return _allocSize(1); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<NullableRef<IDescriptorHeap>>() const noexcept
{ return _allocSize(2); }

template<>
inline uSys IDescriptorHeapBuilder::allocSize<NullableStrongRef<IDescriptorHeap>>() const noexcept
{ return _allocSize(3); }
