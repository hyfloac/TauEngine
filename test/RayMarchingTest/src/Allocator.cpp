#include "Allocator.hpp"
#include <d3dx12.h>

[[nodiscard]] static UINT64 roundTo64k(UINT64 x) noexcept
{
    static constexpr UINT64 _64k = 64 * 1024 - 1;
    static constexpr UINT64 inv64k = ~_64k;

    return (x + _64k) & inv64k;
}

Allocation BufferAllocator::alloc(const UINT size) noexcept
{
    if(size > BlockSize)
    {
        return Allocation(nullptr, 0, 0);
    }

    D3D12_RESOURCE_STATES resState = D3D12_RESOURCE_STATE_COMMON;
    if(_type == Type::Upload)
    {
        resState = D3D12_RESOURCE_STATE_GENERIC_READ;
    }

    if(::std::empty(_blocks))
    {
        Block& block = initBlock();
        const CD3DX12_RESOURCE_DESC resourceArgs = CD3DX12_RESOURCE_DESC::Buffer(size);

        winrt::com_ptr<ID3D12Resource> resource;
        _device->CreatePlacedResource(block.heap.get(), 0, &resourceArgs, resState, NULL, IID_PPV_ARGS(&resource));

        const UINT64 slackSpace = roundTo64k(size) - size;
        block.freeBlocks.emplace_back(size, block.size - size, slackSpace);
        return Allocation(resource, 0, size);
    }

    for(auto& block : _blocks)
    {
        for(auto& freeBlock : block.freeBlocks)
        {
            if(freeBlock.size >= size)
            {
                const CD3DX12_RESOURCE_DESC resourceArgs = CD3DX12_RESOURCE_DESC::Buffer(size);

                winrt::com_ptr<ID3D12Resource> resource;
                _device->CreatePlacedResource(block.heap.get(), freeBlock.offset + freeBlock.slackSpace, &resourceArgs, resState, NULL, IID_PPV_ARGS(&resource));

                freeBlock.size -= size;
                freeBlock.offset += size + freeBlock.slackSpace;
                freeBlock.slackSpace = roundTo64k(freeBlock.offset) - freeBlock.offset;
                if(freeBlock.size == 0)
                {
                    block.freeBlocks.remove(freeBlock);
                }
                return Allocation(resource, 0, size);
            }
        }
    }

    Block& block = initBlock();
    const CD3DX12_RESOURCE_DESC resourceArgs = CD3DX12_RESOURCE_DESC::Buffer(size);

    winrt::com_ptr<ID3D12Resource> resource;
    _device->CreatePlacedResource(block.heap.get(), 0, &resourceArgs, resState, NULL, IID_PPV_ARGS(&resource));

    const UINT64 slackSpace = roundTo64k(size) - size;
    block.freeBlocks.emplace_back(size, block.size - size, slackSpace);
    return Allocation(resource, 0, size);
}

BufferAllocator::Block& BufferAllocator::initBlock() noexcept
{
    D3D12_HEAP_DESC heapArgs;
    heapArgs.SizeInBytes = BlockSize;
    if(_type == Type::Default)
    {
        heapArgs.Properties.Type = D3D12_HEAP_TYPE_DEFAULT;
    }
    else if(_type == Type::Upload)
    {
        heapArgs.Properties.Type = D3D12_HEAP_TYPE_UPLOAD;
    }
    heapArgs.Properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapArgs.Properties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapArgs.Properties.CreationNodeMask = 0;
    heapArgs.Properties.VisibleNodeMask = 0;
    heapArgs.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
    heapArgs.Flags = D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES | D3D12_HEAP_FLAG_DENY_NON_RT_DS_TEXTURES;

    winrt::com_ptr<ID3D12Heap> heap;
    _device->CreateHeap(&heapArgs, IID_PPV_ARGS(&heap));
    return _blocks.emplace_back(heap, heapArgs.SizeInBytes);
}

void BufferAllocator::pruneFreeBlocks() noexcept
{
}
