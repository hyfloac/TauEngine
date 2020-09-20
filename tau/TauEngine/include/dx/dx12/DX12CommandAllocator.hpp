#pragma once

#include "graphics/CommandAllocator.hpp"

#ifdef _WIN32
#include <d3d12.h>
#include "allocator/FreeListAllocator.hpp"

class TAU_DLL DX12CommandAllocator final : public ICommandAllocator
{
    DELETE_CM(DX12CommandAllocator);
    COMMAND_ALLOCATOR_IMPL(DX12CommandAllocator);
private:
    ID3D12CommandAllocator* _allocator;
    FreeListAllocator _freeList;
public:
    DX12CommandAllocator(ID3D12CommandAllocator* const allocator, const uSys maxTotalCommands) noexcept
        : _allocator(allocator)
        , _freeList(maxTotalCommands)
    { }

    ~DX12CommandAllocator() noexcept override
    {
        if(_allocator) 
        { _allocator->Release(); }
    }

    [[nodiscard]] ID3D12CommandAllocator* allocator() const noexcept { return _allocator; }
    [[nodiscard]] FreeListAllocator& freeList() noexcept { return _freeList; }

    void reset(bool releasePages = false) noexcept override
    {
        _allocator->Reset();
        _freeList.reset(releasePages);
    }

#if TAU_CA_EXPOSE_MEM_STAT
    [[nodiscard]] iSys  reservedMemory() const noexcept override { return -1; }
    [[nodiscard]] iSys committedMemory() const noexcept override { return -1; }
    [[nodiscard]] iSys allocatedMemory() const noexcept override { return -1; }
#endif
};
#endif
