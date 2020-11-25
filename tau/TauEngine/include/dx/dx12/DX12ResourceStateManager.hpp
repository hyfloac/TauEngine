#pragma once
#include "DX12Resource.hpp"

#ifdef _WIN32
#include <d3d12.h>
#include "d3dx12.h"

#include <unordered_map>
#include <vector>
#include <NumTypes.hpp>
#include <DynArray.hpp>
#include <BitSet.hpp>

#include "DLL.hpp"
#include "system/Mutex.hpp"
#include "TauConfig.hpp"

class DX12Resource;

class TAU_DLL DX12ResourceStateManager final
{
public:
    struct UsageIndices final
    {
        DEFAULT_CONSTRUCT_PU(UsageIndices);
        DEFAULT_DESTRUCT(UsageIndices);
        DEFAULT_CM_PU(UsageIndices);
    public:
        uSys subresourceCount;
        DynArray<uSys> _usageSet;

        UsageIndices(const uSys _subresourceCount)
            : subresourceCount(_subresourceCount)
            , _usageSet((_subresourceCount + 1) * 3)
        { ::std::memset(_usageSet.arr(), 0xFF, _usageSet.count() * sizeof(uSys)); }

        [[nodiscard]] uSys& _getUsageSet(const uSys setIndex, const uSys subresource) noexcept
        {
            uSys* const subArr = _usageSet.arr() + setIndex * (subresourceCount + 1);

            if(subresource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES)
            {
                return subArr[subresourceCount];
            }
            else
            {
#if TAU_GENERAL_SAFETY_CHECK
                static uSys NULL_VAL = static_cast<uSys>(-1);

                if(subresource >= subresourceCount)
                {
                    NULL_VAL = -1;
                    return NULL_VAL;
                }
#endif
                return subArr[subresource];
            }
        }

        [[nodiscard]] uSys& firstRead(const uSys subresource) noexcept
        { return _getUsageSet(0, subresource); }

        [[nodiscard]] uSys& lastRead(const uSys subresource) noexcept
        { return _getUsageSet(1, subresource); }

        [[nodiscard]] uSys& lastUsage(const uSys subresource) noexcept
        { return _getUsageSet(2, subresource); }

        [[nodiscard]] uSys firstRead(const uSys subresource) const noexcept
        { return const_cast<UsageIndices*>(this)->_getUsageSet(0, subresource); }

        [[nodiscard]] uSys lastRead(const uSys subresource) const noexcept
        { return const_cast<UsageIndices*>(this)->_getUsageSet(1, subresource); }

        [[nodiscard]] uSys lastUsage(const uSys subresource) const noexcept
        { return const_cast<UsageIndices*>(this)->_getUsageSet(2, subresource); }
    };

    struct ResourceState final
    {
        DEFAULT_CONSTRUCT_PU(ResourceState);
        DEFAULT_DESTRUCT(ResourceState);
        DEFAULT_CM_PU(ResourceState);
    public:
        u32 perResource : 1;
        D3D12_RESOURCE_STATES resourceState;
        bool hasBegin;
        DynArray<D3D12_RESOURCE_STATES> subresourceStates;
        BitSet subresourceHasBegin;

        ResourceState(const uSys subresourceCount, const D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COMMON)
            : perResource(true)
            , resourceState(initialState)
            , hasBegin(false)
            , subresourceStates(subresourceCount)
            , subresourceHasBegin(subresourceCount)
        { }

        void setSubresourceState(const uSys subresource, const D3D12_RESOURCE_STATES state) noexcept
        {
            if(subresource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES)
            {
                perResource = true;
                resourceState = state;
            }
            else
            {
#if TAU_GENERAL_SAFETY_CHECK
                if(subresource >= subresourceStates.count())
                { return; }
#endif

                if(perResource)
                {
                    if(resourceState == state)
                    { return; }

                    perResource = false;
                    for(uSys i = 0; i < subresourceStates; ++i)
                    { subresourceStates[i] = resourceState; }
                }

                subresourceStates[subresource] = state;

                bool isUnified = true;
                for(uSys i = 0, count = subresourceStates.count(); i < count; ++i)
                {
                    if(subresourceStates[i] != state)
                    {
                        isUnified = false;
                        break;
                    }
                }
                if(isUnified)
                {
                    perResource = true;
                    resourceState = state;
                }
            }
        }

        D3D12_RESOURCE_STATES getSubresourceState(const uSys subresource) noexcept
        {
            if(subresource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES && perResource)
            { return resourceState; }
            
#if TAU_GENERAL_SAFETY_CHECK
            if(subresource >= subresourceStates.count())
            { return static_cast<D3D12_RESOURCE_STATES>(-1); }
#endif

            return subresourceStates[subresource];
        }
    };

    struct InitialStateChange final
    {
        ID3D12Resource* resource;
        uSys subresource;
        D3D12_RESOURCE_STATES state;


        InitialStateChange(ID3D12Resource* const _resource, const uSys _subresource, const D3D12_RESOURCE_STATES _state) noexcept
            : resource(_resource)
            , subresource(_subresource)
            , state(_state)
        { }
    };

    using StateMap = ::std::unordered_map<ID3D12Resource*, ResourceState>;
    using UsageMap = ::std::unordered_map<ID3D12Resource*, UsageIndices>;
    using InitialStateList = ::std::vector<InitialStateChange>;
    using StateList = ::std::vector<D3D12_RESOURCE_BARRIER>;
private:
    static StateMap  _globalStateMap;
    static SRWMutex _globalMutex;
public:
    static void trackResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES initialState) noexcept;

    static void releaseResource(ID3D12Resource* resource) noexcept;
private:
    static ResourceState* getGlobalResourceState(ID3D12Resource* resource) noexcept;
private:
    StateMap _localStateMap;
    UsageMap _usageMap;
    InitialStateList _initialStateList;
    StateList _deltaStateList;
public:
    void resourceBarrier(const D3D12_RESOURCE_BARRIER& barrier) noexcept;
    void resourceBarrierBegin(const D3D12_RESOURCE_BARRIER& barrier) noexcept;

    void transitionResource(ID3D12Resource* const resource, const D3D12_RESOURCE_STATES stateAfter, const UINT subresource) noexcept
    {
        resourceBarrier(CD3DX12_RESOURCE_BARRIER::Transition(resource, D3D12_RESOURCE_STATE_COMMON, stateAfter, subresource));
    }

    void transitionResource(const DX12Resource* const resource, const D3D12_RESOURCE_STATES stateAfter, const uSys subresource) noexcept
    {
        resourceBarrier(CD3DX12_RESOURCE_BARRIER::Transition(resource->d3dResource(), D3D12_RESOURCE_STATE_COMMON, stateAfter, static_cast<UINT>(subresource)));
    }

    void transitionResourceBegin(ID3D12Resource* const resource, const D3D12_RESOURCE_STATES stateAfter, const UINT subresource) noexcept
    {
        resourceBarrierBegin(CD3DX12_RESOURCE_BARRIER::Transition(resource, D3D12_RESOURCE_STATE_COMMON, stateAfter, subresource));
    }

    void transitionResourceBegin(const DX12Resource* const resource, const D3D12_RESOURCE_STATES stateAfter, const uSys subresource) noexcept
    {
        resourceBarrierBegin(CD3DX12_RESOURCE_BARRIER::Transition(resource->d3dResource(), D3D12_RESOURCE_STATE_COMMON, stateAfter, static_cast<UINT>(subresource)));
    }

    void uavBarrier(ID3D12Resource* const resource) noexcept
    {
        resourceBarrier(CD3DX12_RESOURCE_BARRIER::UAV(resource));
    }

    void uavBarrier(const DX12Resource* const resource) noexcept
    {
        resourceBarrier(CD3DX12_RESOURCE_BARRIER::UAV(resource ? resource->d3dResource() : nullptr));
    }

    void aliasBarrier(ID3D12Resource* const resourceBefore, ID3D12Resource* const resourceAfter) noexcept
    {
        resourceBarrier(CD3DX12_RESOURCE_BARRIER::Aliasing(resourceBefore, resourceAfter));
    }

    void aliasBarrier(const DX12Resource* const resourceBefore, const DX12Resource* const resourceAfter) noexcept
    {
        resourceBarrier(CD3DX12_RESOURCE_BARRIER::Aliasing(resourceBefore ? resourceBefore->d3dResource() : nullptr, resourceAfter ? resourceAfter->d3dResource() : nullptr));
    }

    UsageIndices* getUsageIndices(ID3D12Resource* resource) noexcept;

    void flushResourceBarriers(ID3D12GraphicsCommandList* cmdList) noexcept;

    void commitFinalState() noexcept;
};

#endif
