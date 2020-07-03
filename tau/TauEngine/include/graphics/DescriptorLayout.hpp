#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <allocator/TauAllocator.hpp>

#include "DLL.hpp"
#include "DescriptorHeap.hpp"

/**
 * An opaque descriptor layout.
 *
 *   This is used during resource binding to know where each
 * descriptor should be bound to within the pipeline.
 */
DECL_OPAQUE_TYPE(DescriptorLayout);

struct DescriptorLayoutEntry final
{
public:
    enum class Type
    {
        Constant = 1,
        DescriptorTable,
        DescriptorSamplerTable,
        UniformBufferView,
        UnorderedAccessView,
        TextureView
    };
public:
    Type type;
    /**
     * The beginning index for binding.
     */
    uSys begin;
    /**
     * The number of descriptors within the table to bind.
     */
    uSys count;
    union
    {
        DescriptorTable table;
        DescriptorSamplerTable samplerTable;
    };
};

class TAU_DLL TAU_NOVTABLE IDescriptorLayoutBuilder
{
    DEFAULT_CONSTRUCT_PO(IDescriptorLayoutBuilder);
    DEFAULT_DESTRUCT_VI(IDescriptorLayoutBuilder);
    DEFAULT_CM_PO(IDescriptorLayoutBuilder);
public:
    enum class Error
    {
        NoError = 0,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        InternalError
    };
public:
    [[nodiscard]] virtual DescriptorLayout build(uSys entryCount, DescriptorLayoutEntry* entries, Error* error = null) const noexcept = 0;
};