#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <TauCOM.hpp>
#include "DLL.hpp"

#include "GraphicsEnums.hpp"

namespace tau {

struct ResourceHeapArgs final
{
    DEFAULT_CONSTRUCT_PUC(ResourceHeapArgs);
    DEFAULT_CM_PU(ResourceHeapArgs);
    DEFAULT_DESTRUCT(ResourceHeapArgs);
public:
    u64 Size;
    EGraphics::ResourceAlignment Alignment;
    EGraphics::ResourceHeapDataType ResourceTypes;
    EGraphics::ResourceHeapUsageType Usage;
};

class IResourceHeap : public com::IUnknown
{
    DEFAULT_CONSTRUCT_PO(IResourceHeap);
    DEFAULT_CM_PO(IResourceHeap);
    DEFAULT_DESTRUCT_VI(IResourceHeap);
public:
    struct ConstructionInfo final : com::BaseConstructionInfo
    {
        DEFAULT_CONSTRUCT_PU(ConstructionInfo);
        DEFAULT_CM_PU(ConstructionInfo);
        DEFAULT_DESTRUCT(ConstructionInfo);
    public:
        ResourceHeapArgs HeapArgs;
    };
public:
    [[nodiscard]] virtual const ResourceHeapArgs& HeapArgs() const noexcept = 0;
};

}

TAU_DECL_UUID(IResourceHeap, 0x87FD36AC4EAD43EDull, 0xB16925A3E6076983ull);

// class TAU_DLL TAU_NOVTABLE IResourceHeapBuilder
// {
//     DEFAULT_CONSTRUCT_PO(IResourceHeapBuilder);
//     DEFAULT_DESTRUCT_VI(IResourceHeapBuilder);
//     DEFAULT_CM_PO(IResourceHeapBuilder);
// public:
//     enum class Error
//     {
//         NoError = 0,
//         SystemMemoryAllocationFailure,
//         DriverMemoryAllocationFailure,
//         /**
//          *   This error is used to signal that the backing API does not
//          * support user resource heaps. This error is safe to ignore.
//          */
//         Unsupported
//     };
// public:
//     [[nodiscard]] virtual ResourceHeap build(const ResourceHeapArgs& args, [[tau::out]] Error* error) noexcept = 0;
// };
