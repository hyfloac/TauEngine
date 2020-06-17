#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <allocator/TauAllocator.hpp>

#include "DLL.hpp"

class IResourceView;

struct DescriptorTable final
{
    void* raw;
};

enum class DescriptorType
{
    TextureView = 1,
    UniformBufferView,
    UnorderedAccessView
};

class TAU_DLL TAU_NOVTABLE IDescriptorHeap
{
    DEFAULT_CONSTRUCT_PO(IDescriptorHeap);
    DEFAULT_CM_PO(IDescriptorHeap);
    DEFAULT_DESTRUCT_VI(IDescriptorHeap);
public:
    [[nodiscard]] virtual DescriptorTable allocateTable(uSys descriptors, DescriptorType type, TauAllocator* allocator = null) noexcept = 0;
    [[nodiscard]] virtual void destroyTable(DescriptorTable table) noexcept = 0;
};
