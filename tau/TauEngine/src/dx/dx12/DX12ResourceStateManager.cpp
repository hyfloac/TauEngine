#include "dx/dx12/DX12ResourceStateManager.hpp"

#ifdef _WIN32
DX12ResourceStateManager::StateMap DX12ResourceStateManager::_globalStateMap;
SRWMutex DX12ResourceStateManager::_globalMutex;

void DX12ResourceStateManager::trackResource(ID3D12Resource* const resource, const D3D12_RESOURCE_STATES initialState) noexcept
{
#if TAU_NULL_CHECK
    if(!resource)
    { return; }
#endif

    Lock<SRWMutex> lock(_globalMutex);

    _globalStateMap[resource] = initialState;
}

void DX12ResourceStateManager::releaseResource(ID3D12Resource* const resource) noexcept
{
#if TAU_NULL_CHECK
    if(!resource)
    { return; }
#endif

    Lock<SRWMutex> lock(_globalMutex);

    _globalStateMap.erase(resource);
}

DX12ResourceStateManager::ResourceState* DX12ResourceStateManager::getGlobalResourceState(ID3D12Resource* const resource) noexcept
{
#if TAU_NULL_CHECK
    if(!resource)
    { return nullptr; }
#endif

    if(!_globalStateMap.count(resource))
    { return nullptr; }

    return &_globalStateMap[resource];
}

void DX12ResourceStateManager::resourceBarrier(const D3D12_RESOURCE_BARRIER& barrier) noexcept
{
    if(barrier.Type == D3D12_RESOURCE_BARRIER_TYPE_TRANSITION)
    {
        // Has the resource tracked by this command list yet?
        if(_localStateMap.count(barrier.Transition.pResource) > 0)
        {
            ResourceState& localState = _localStateMap[barrier.Transition.pResource];

            // Are we setting all subresources, and is the resource in a per subresource state?
            if(barrier.Transition.Subresource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES && !localState.perResource)
            {
                //   Iterate through each individual subresource state and issue its
                // own transition.
                for(uSys i = 0, count = localState.subresourceStates.count(); i < count; ++i)
                {
                    // Is the initial state of this subresource unknown?
                    if(localState.subresourceStates[i] == static_cast<D3D12_RESOURCE_STATES>(-1))
                    {
                        _initialStateList.emplace_back(barrier.Transition.pResource, barrier.Transition.Subresource, barrier.Transition.StateAfter);
                    }
                    else if((localState.subresourceStates[i] & barrier.Transition.StateAfter) != barrier.Transition.StateAfter)
                    {
                        D3D12_RESOURCE_BARRIER newBarrier = barrier;
                        newBarrier.Transition.Subresource = static_cast<UINT>(i);
                        newBarrier.Transition.StateBefore = localState.subresourceStates[i];

                        if(localState.hasBegin)
                        {
                            newBarrier.Flags |= D3D12_RESOURCE_BARRIER_FLAG_END_ONLY;
                        }

                        _deltaStateList.emplace_back(newBarrier);
                    }
                }
            }
            else
            {
                const D3D12_RESOURCE_STATES finalState = localState.getSubresourceState(barrier.Transition.Subresource);

                // Is the initial state of this subresource unknown?
                if(finalState == static_cast<D3D12_RESOURCE_STATES>(-1))
                {
                    _initialStateList.emplace_back(barrier.Transition.pResource, barrier.Transition.Subresource, barrier.Transition.StateAfter);
                }
                else if((finalState & barrier.Transition.StateAfter) != barrier.Transition.StateAfter)
                {
                    D3D12_RESOURCE_BARRIER newBarrier = barrier;
                    newBarrier.Transition.StateBefore = finalState;

                    if(localState.subresourceHasBegin[barrier.Transition.Subresource])
                    {
                        newBarrier.Flags |= D3D12_RESOURCE_BARRIER_FLAG_END_ONLY;
                    }

                    _deltaStateList.emplace_back(newBarrier);
                }
            }

            // Update the local state.
            localState.setSubresourceState(barrier.Transition.Subresource, barrier.Transition.StateAfter);
        }
        else
        {
            _initialStateList.emplace_back(barrier.Transition.pResource, barrier.Transition.Subresource, barrier.Transition.StateAfter);
            ResourceState* globalState;
            // Get the global state so we know how many subresources there are.
            {
                ReadLock readLock(_globalMutex);
                globalState = getGlobalResourceState(barrier.Transition.pResource);
                if(!globalState)
                { return; }
            }
            ResourceState localState(globalState->subresourceStates.count(), static_cast<D3D12_RESOURCE_STATES>(-1));
            localState.setSubresourceState(barrier.Transition.Subresource, barrier.Transition.StateAfter);
            _localStateMap[barrier.Transition.pResource] = ::std::move(localState);
        }
    }
    // Nothing special for non-transition barriers.
    else
    {
        _deltaStateList.emplace_back(barrier);
    }
}

void DX12ResourceStateManager::resourceBarrierBegin(const D3D12_RESOURCE_BARRIER& barrier) noexcept
{
    if(barrier.Type == D3D12_RESOURCE_BARRIER_TYPE_TRANSITION)
    {
        bool usingSplitBarrier = false;

        // Has the resource tracked by this command list yet?
        if(_localStateMap.count(barrier.Transition.pResource) > 0)
        {
            ResourceState& localState = _localStateMap[barrier.Transition.pResource];

            // Are we setting all subresources, and is the resource in a per subresource state?
            if(barrier.Transition.Subresource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES && !localState.perResource)
            {
                //   Iterate through each individual subresource state and issue its
                // own transition.
                for(uSys i = 0, count = localState.subresourceStates.count(); i < count; ++i)
                {
                    // Is the initial state of this subresource unknown?
                    if(localState.subresourceStates[i] == static_cast<D3D12_RESOURCE_STATES>(-1))
                    {
                        _initialStateList.emplace_back(barrier.Transition.pResource, barrier.Transition.Subresource, barrier.Transition.StateAfter);
                    }
                    else if((localState.subresourceStates[i] & barrier.Transition.StateAfter) != barrier.Transition.StateAfter)
                    {
                        D3D12_RESOURCE_BARRIER newBarrier = barrier;
                        newBarrier.Transition.Subresource = static_cast<UINT>(i);
                        newBarrier.Transition.StateBefore = localState.subresourceStates[i];

                        newBarrier.Flags |= D3D12_RESOURCE_BARRIER_FLAG_BEGIN_ONLY;

                        localState.subresourceHasBegin[barrier.Transition.Subresource] = true;
                        usingSplitBarrier = true;

                        _deltaStateList.emplace_back(newBarrier);
                    }
                }
            }
            else
            {
                const D3D12_RESOURCE_STATES finalState = localState.getSubresourceState(barrier.Transition.Subresource);

                // Is the initial state of this subresource unknown?
                if(finalState == static_cast<D3D12_RESOURCE_STATES>(-1))
                {
                    _initialStateList.emplace_back(barrier.Transition.pResource, barrier.Transition.Subresource, barrier.Transition.StateAfter);
                }
                else if((finalState & barrier.Transition.StateAfter) != barrier.Transition.StateAfter)
                {
                    D3D12_RESOURCE_BARRIER newBarrier = barrier;
                    newBarrier.Transition.StateBefore = finalState;

                    newBarrier.Flags |= D3D12_RESOURCE_BARRIER_FLAG_BEGIN_ONLY;

                    if(barrier.Transition.Subresource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES)
                    { localState.hasBegin = true; }
                    else
                    { localState.subresourceHasBegin[barrier.Transition.Subresource] = true; }
                    usingSplitBarrier = true;

                    _deltaStateList.emplace_back(newBarrier);
                }
            }

            if(!usingSplitBarrier)
            {
                // Update the local state.
                localState.setSubresourceState(barrier.Transition.Subresource, barrier.Transition.StateAfter);
            }
        }
        else
        {
            _initialStateList.emplace_back(barrier.Transition.pResource, barrier.Transition.Subresource, barrier.Transition.StateAfter);
            ResourceState* globalState;
            // Get the global state so we know how many subresources there are.
            {
                ReadLock readLock(_globalMutex);
                globalState = getGlobalResourceState(barrier.Transition.pResource);
                if(!globalState)
                { return; }
            }
            ResourceState localState(globalState->subresourceStates.count(), static_cast<D3D12_RESOURCE_STATES>(-1));
            localState.setSubresourceState(barrier.Transition.Subresource, barrier.Transition.StateAfter);
            _localStateMap[barrier.Transition.pResource] = ::std::move(localState);
        }
    }
    // Nothing special for non-transition barriers.
    else
    {
        _deltaStateList.emplace_back(barrier);
    }
}

DX12ResourceStateManager::UsageIndices* DX12ResourceStateManager::getUsageIndices(ID3D12Resource* const resource) noexcept
{
#if TAU_NULL_CHECK
    if(!resource)
    { return nullptr; }
#endif

    if(_usageMap.count(resource) == 0)
    {
        ResourceState* globalState;
        // Get the global state so we know how many subresources there are.
        {
            ReadLock readLock(_globalMutex);
            globalState = getGlobalResourceState(resource);
            if(!globalState)
            { return nullptr; }
        }

        _usageMap[resource] = UsageIndices(globalState->subresourceStates.count());
    }

    return &_usageMap[resource];
}

void DX12ResourceStateManager::flushResourceBarriers(ID3D12GraphicsCommandList* const cmdList) noexcept
{
    if(_deltaStateList.size() > 0)
    {
        cmdList->ResourceBarrier(static_cast<UINT>(_deltaStateList.size()), &_deltaStateList.front());
        _deltaStateList.clear();
    }
}

void DX12ResourceStateManager::commitFinalState() noexcept
{
    Lock<SRWMutex> lock(_globalMutex);
    for(const auto& state : _localStateMap)
    {
        _globalStateMap[state.first] = state.second;
    }
    _localStateMap.clear();
}
#endif
