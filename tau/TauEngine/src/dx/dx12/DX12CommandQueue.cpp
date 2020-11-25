#include "dx/dx12/DX12CommandQueue.hpp"


#ifdef _WIN32
#include "dx/dx12/DX12CommandList.hpp"
#include <DynArray.hpp>
#include "TauConfig.hpp"

void DX12CommandQueue::executeCommandLists(const uSys count, const ICommandList* const* const lists) noexcept
{
#if TAU_NULL_CHECK
    if(!lists)
    { return; }
#endif

    UINT realCount = 0;
    DynArray<ID3D12CommandList*> d3dLists(count);
    
    for(uSys i = 0; i < count; ++i)
    {
#if TAU_NULL_CHECK
        if(!lists[i])
        { continue; }
#endif

#if TAU_RTTI_CHECK
        if(!RTT_CHECK(lists[i], DX12CommandList))
        { continue; }
#endif

        d3dLists[realCount] = static_cast<const DX12CommandList*>(lists[i])->cmdList();
        ++realCount;
    }

    _d3dQueue->ExecuteCommandLists(realCount, d3dLists);
}


void DX12CommandQueue::executeCommandLists(const UINT count, ID3D12CommandList* const* const lists) noexcept
{
#if TAU_NULL_CHECK
    if(!lists)
    { return; }

    for(uSys i = 0; i < count; ++i)
    {
        if(!lists[i])
        { return; }
    }
#endif

    _d3dQueue->ExecuteCommandLists(count, lists);
}
#endif
