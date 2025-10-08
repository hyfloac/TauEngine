#include "MemoryStream.hpp"
#include <allocator/PageAllocator.hpp>
#include <allocator/FixedBlockAllocator.hpp>
#include <TUMaths.hpp>

#pragma warning(push, 0)
#ifdef _WIN32
  #include <intrin.h>
  #include <Windows.h>
#else
  #include <sys/time.h>
  #include <ctime>
#endif
#pragma warning(pop)

namespace tau {

#ifdef _DEBUG
constexpr AllocationTracking MemoryStreamAllocationTracking = AllocationTracking::DoubleDeleteCount;
#else
constexpr AllocationTracking MemoryStreamAllocationTracking = AllocationTracking::None;
#endif

static FixedBlockAllocator<MemoryStreamAllocationTracking> s_MemoryStreamAllocator(maxT(sizeof(DynamicMemoryStream), sizeof(MemoryStream)));

void DynamicMemoryStream::DestroySelf() noexcept
{
    s_MemoryStreamAllocator.DeallocateT(this);
}

void MemoryStream::DestroySelf() noexcept
{
    s_MemoryStreamAllocator.DeallocateT(this);
}

com::EResultCode DynamicMemoryStream::Factory(
    const com::UUID& iid,
    void** const pInterface,
    const com::BaseConstructionInfo* const pConstructionInfo
) noexcept
{
    using namespace com;

    if(!pInterface)
    {
        return RC_NullParam;
    }

    if(iid != iid_of<DynamicMemoryStream>)
    {
        return RC_InterfaceNotFound;
    }

    if(pConstructionInfo)
    {
        if(pConstructionInfo->Iid != iid_of<DynamicMemoryStream>)
        {
            return RC_InterfaceNotFound;
        }

        const ConstructionInfo* const constructionInfo = static_cast<const ConstructionInfo*>(pConstructionInfo);

        *pInterface = s_MemoryStreamAllocator.AllocateT<DynamicMemoryStream>(
            constructionInfo->Capacity
        );
    }
    else
    {
        *pInterface = s_MemoryStreamAllocator.AllocateT<DynamicMemoryStream>();
    }

    if(!*pInterface)
    {
        return RC_OutOfMemory;
    }

    return RC_Success;
}

com::EResultCode MemoryStream::Factory(
    const com::UUID& iid,
    void** const pInterface,
    const com::BaseConstructionInfo* const pConstructionInfo
) noexcept
{
    using namespace com;

    if(!pInterface)
    {
        return RC_NullParam;
    }

    if(iid != iid_of<MemoryStream>)
    {
        return RC_InterfaceNotFound;
    }

    if(pConstructionInfo)
    {
        if(pConstructionInfo->Iid != iid_of<MemoryStream>)
        {
            return RC_InterfaceNotFound;
        }

        const ConstructionInfo* const constructionInfo = static_cast<const ConstructionInfo*>(pConstructionInfo);

        *pInterface = s_MemoryStreamAllocator.AllocateT<MemoryStream>(
            constructionInfo->Buffer,
            constructionInfo->Length,
            constructionInfo->Writable
        );
    }
    else
    {
        return RC_NullParam;
    }

    if(!*pInterface)
    {
        return RC_OutOfMemory;
    }

    return RC_Success;
}

}
