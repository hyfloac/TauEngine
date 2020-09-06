#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include "DLL.hpp"

#include "_GraphicsOpaqueObjects.hpp"
#include "GraphicsEnums.hpp"

struct ResourceHeapArgs final
{
    u64 size;
    EGraphics::ResourceAlignment alignment;
    EGraphics::ResourceHeapDataType resourceTypes;
    EGraphics::ResourceHeapUsageType usage;
};

class TAU_DLL TAU_NOVTABLE IResourceHeapBuilder
{
    DEFAULT_CONSTRUCT_PO(IResourceHeapBuilder);
    DEFAULT_DESTRUCT_VI(IResourceHeapBuilder);
    DEFAULT_CM_PO(IResourceHeapBuilder);
public:
    enum class Error
    {
        NoError = 0,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        /**
         *   This error is used to signal that the backing API does not
         * support user resource heaps. This error is safe to ignore.
         */
        Unsupported
    };
public:
    [[nodiscard]] virtual ResourceHeap build(const ResourceHeapArgs& args, [[tau::out]] Error* error) noexcept = 0;
};
