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

class IResourceView;

DECL_OPAQUE_TYPE(DescriptorTable);

enum class DescriptorType
{
    TextureView = 1,
    UniformBufferView,
    UnorderedAccessView
};

#define DESCRIPTOR_HEAP_IMPL_BASE(_TYPE) \
    RTT_IMPL(_TYPE, IDescriptorHeap)

#define DESCRIPTOR_HEAP_IMPL(_TYPE) DESCRIPTOR_HEAP_IMPL_BASE(_TYPE)

/**
 * A heap of memory intended to hold descriptors.
 *
 *   This is intended to map to DirectX12 descriptor heaps. The
 * exact implementation may vary heavily, though due to the
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
     * @param[in] descriptors
     *      The number of descriptors the table will contain.
     * @param[in] type
     *      The type of the descriptors.
     * @param[in] allocator
     *        An optional allocator for the Tau descriptors. This
     *      allocator cannot be a fixed block allocator, unless all
     *      tables are the same size.
     */
    [[nodiscard]] virtual DescriptorTable allocateTable(uSys descriptors, DescriptorType type, [[tau::nullable]] TauAllocator* allocator = null) noexcept = 0;

    /**
     * Deallocates a descriptor table.
     *
     *   It is important to destroy the descriptor tables manually
     * as they contain an allocation for all of the Tau descriptors.
     *
     * @param[in] table
     *      The descriptor table to destroy.
     */
    [[nodiscard]] virtual void destroyTable(DescriptorTable table) noexcept = 0;

    RTT_BASE_IMPL(IDescriptorHeap);
    RTT_BASE_CHECK(IDescriptorHeap);
    RTT_BASE_CAST(IDescriptorHeap);
};

struct DescriptorHeapArgs
{
    uSys maxTables;
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
    [[nodiscard]] virtual IDescriptorHeap* build(const DescriptorHeapArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IDescriptorHeap* build(const DescriptorHeapArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IDescriptorHeap> buildCPPRef(const DescriptorHeapArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IDescriptorHeap> buildTauRef(const DescriptorHeapArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IDescriptorHeap> buildTauSRef(const DescriptorHeapArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
