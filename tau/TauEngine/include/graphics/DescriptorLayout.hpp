#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>

#include "DLL.hpp"
#include "DescriptorHeap.hpp"
#include "_GraphicsOpaqueObjects.hpp"

struct DescriptorLayoutEntry final
{
public:
    enum class Type
    {
        UniformBufferView = 1,
        UnorderedAccessView,
        TextureView,
        TextureSampler
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
    virtual void destroyLayout(DescriptorLayout layout) const noexcept = 0;
};
