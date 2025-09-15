#include "dx/dx10/DX10ResourceHeap.hpp"

using namespace tau;
using namespace com;

tau::com::ResultCode DX10ResourceHeap::QueryInterface(const tau::com::UUID& iid, void** const pInterface) noexcept
{
    if(!pInterface)
    {
        return RC_NullParam;
    }

    if(iid == iid_of<IUnknown>)
    {
        *pInterface = static_cast<IUnknown*>(this);
        AddReference();
    }
    else if(iid == iid_of<IResourceHeap>)
    {
        *pInterface = this;
        AddReference();
    }
    else
    {
        return RC_InterfaceNotFound;
    }

    return RC_Success;
}

ResultCode DX10ResourceHeap::Factory(const UUID& iid, void** const pInterface, const BaseConstructionInfo* const pConstructionInfo) noexcept
{
    if(!pInterface)
    {
        return RC_NullParam;
    }

    if(!pConstructionInfo)
    {
        return RC_NullParam;
    }

    if(iid != iid_of<IResourceHeap>)
    {
        return RC_InterfaceNotFound;
    }

    if(pConstructionInfo->Iid != iid_of<IResourceHeap>)
    {
        return RC_InterfaceNotFound;
    }

    const ConstructionInfo* constructionInfo = reinterpret_cast<const ConstructionInfo*>(pConstructionInfo);

    *pInterface = BasicTauAllocator<AllocationTracking::None>::Instance().AllocateT<DX10ResourceHeap>(constructionInfo->HeapArgs);

    return RC_Success;
}
