#include "dx/dx12/DX12ResourceUploadHandler.hpp"

#ifdef _WIN32
#include <TUMaths.hpp>

DX12Allocation DX12BufferResourceUploadHandler::alloc(const uSys minSize) noexcept
{
    D3D12_RESOURCE_DESC desc;
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
    desc.Width = minSize;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;

    FreeBlock* freeBlock = findFreeBlock(minSize);

    const u64 offset = freeBlock->offset;

    ID3D12Resource* resource;
    const HRESULT res = _device->CreatePlacedResource(_heap, offset, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&resource));
    
    if(FAILED(res))
    {
        return { null, 0, 0 };
    }

    resizeFreeBlock(freeBlock, minSize);

    return { resource, offset, minSize };
}

NullableRef<DX12AutoAllocation> DX12BufferResourceUploadHandler::allocAuto(const uSys minSize) noexcept
{
    const DX12Allocation allocation = alloc(minSize);
    if(!allocation.resource)
    { return null; }
    return NullableRef<DX12AutoAllocation>(DefaultTauAllocator::Instance(), allocation, *this);
}

void DX12BufferResourceUploadHandler::free(DX12Allocation& resource) noexcept
{
    resource.resource->Release();
    FreeBlock* freeBlock = _freeBlockAllocator.allocateT<FreeBlock>(resource.offset, resource.size, null, null);
    mergeFreeBlock(freeBlock);
}

DX12BufferResourceUploadHandler::FreeBlock* DX12BufferResourceUploadHandler::findFreeBlock(const uSys minSize) noexcept
{
    FreeBlock* base = _beginBlock;

    while(true)
    {
        if(!base)
        {
            return null; 
        }

        if(base->count >= minSize)
        {
            if(base->offset & FreeBlock::TOMBSTONE_BIT)
            {
                base = base->next;
                continue;
            }

            return base;
        }

        base = base->next;
    }
}

void DX12BufferResourceUploadHandler::resizeFreeBlock(FreeBlock* const hostBlock, const u64 size) noexcept
{
    if(size == hostBlock->count)
    {
        if(hostBlock->prev)
        {
            hostBlock->prev->next = hostBlock->next;
            if(hostBlock->next)
            {
                hostBlock->next->prev = hostBlock->prev;
            }
        }
        else
        {
            if(hostBlock->next)
            {
                _beginBlock = hostBlock->next;
            }
            else
            {
                _beginBlock = null;
            }
        }
        _freeBlockAllocator.deallocateT(hostBlock);
    }
    else
    {
        const uSys newCount = hostBlock->count - size;
        const uSys newOffset = hostBlock->offset + size;
        const uSys alignedOffset = _alignTo(newOffset, 1 << 16);
        const uSys alignDif = alignedOffset - newOffset;
        if(alignDif >= newCount)
        {
            hostBlock->offset = newOffset | FreeBlock::TOMBSTONE_BIT;
            hostBlock->count = newCount;
        }
        else
        {
            hostBlock->offset = alignedOffset;
            hostBlock->count = newCount - alignDif;
        }

        FreeBlock* sortedParent = findFreeBlock(hostBlock->count - 1);
        if(sortedParent)
        {
            /*
             *   Free block must always become smaller. Due to sorting the
             * smallest block is always first. If a smaller block can be
             * found (sortedParent), then freeBlock must inherently have a
             * previous element.
             */
            // ReSharper disable once CppUnreachableCode
            if constexpr(true || hostBlock->prev)
            {
                hostBlock->prev->next = hostBlock->next;
                if(hostBlock->next)
                {
                    hostBlock->next->prev = hostBlock->prev;
                }
            }

            if(sortedParent->next)
            {
                sortedParent->next->prev = hostBlock;
            }
            sortedParent->next = hostBlock;
        }
        else
        {
            if(hostBlock->prev)
            {
                hostBlock->prev->next = hostBlock->next;
                if(hostBlock->next)
                {
                    hostBlock->next->prev = hostBlock->prev;
                }

                hostBlock->prev = null;
            }

            hostBlock->next = _beginBlock;
            _beginBlock->prev = hostBlock;
        }
    }
}

void DX12BufferResourceUploadHandler::mergeFreeBlock(FreeBlock* const freeBlock) noexcept
{
    if(!_beginBlock)
    {
        _beginBlock = freeBlock;
        return;
    }
    
    FreeBlock* base = _beginBlock;

    while(true)
    {
        if(!base)
        {
            return; 
        }

        if(freeBlock->offset + freeBlock->count == base->rawOffset())
        {
            freeBlock->count += base->count;
            freeBlock->prev = base->prev;
            freeBlock->next = base->next;

            if(freeBlock->prev)
            {
                freeBlock->prev->next = freeBlock;
            }
            else
            {
                _beginBlock = freeBlock;
            }

            if(freeBlock->next)
            {
                freeBlock->next->prev = freeBlock;
            }

            _freeBlockAllocator.deallocateT(base);
            return;
        }

        /*
         *   If base is a tombstone then it is smaller than the
         * alignment value. freeBlock will always be aligned. Thus we
         * need to store the old tombstone and then the new free block.
         */
        if(!base->isTombstone() && base->offset + base->count == freeBlock->offset)
        {
            base->count += freeBlock->count;
            _freeBlockAllocator.deallocateT(freeBlock);

            if(base->next && base->offset + base->count == base->next->rawOffset())
            {
                base->count += base->next->count;
                base->next = base->next->next;
                if(base->next)
                {
                    base->next->prev = base;
                }
            }
            return;
        }

        base = base->next;
    }
}

DX12Allocation DX12BufferResourceUploadHandler2::alloc(const uSys minSize) noexcept
{
    FreeBlock* freeBlock = findFreeBlock(minSize);

    const u64 offset = freeBlock->offset;

    resizeFreeBlock(freeBlock, minSize);

    return { _heap, offset, minSize };
}

NullableRef<DX12AutoAllocation2> DX12BufferResourceUploadHandler2::allocAuto(const uSys minSize) noexcept
{
    const DX12Allocation allocation = alloc(minSize);
    if(!allocation.resource)
    { return null; }
    return NullableRef<DX12AutoAllocation2>(DefaultTauAllocator::Instance(), allocation, *this);
}

void DX12BufferResourceUploadHandler2::free(DX12Allocation& resource) noexcept
{
    FreeBlock* freeBlock = _freeBlockAllocator.allocateT<FreeBlock>(resource.offset, resource.size, null, null);
    mergeFreeBlock(freeBlock);
}

DX12BufferResourceUploadHandler2::FreeBlock* DX12BufferResourceUploadHandler2::findFreeBlock(const uSys minSize) noexcept
{
    FreeBlock* base = _beginBlock;

    while(true)
    {
        if(!base)
        {
            return null; 
        }

        if(base->count >= minSize)
        {
            return base;
        }

        base = base->next;
    }
}

void DX12BufferResourceUploadHandler2::resizeFreeBlock(FreeBlock* const hostBlock, const u64 size) noexcept
{
    if(size == hostBlock->count)
    {
        if(hostBlock->prev)
        {
            hostBlock->prev->next = hostBlock->next;
            if(hostBlock->next)
            {
                hostBlock->next->prev = hostBlock->prev;
            }
        }
        else
        {
            if(hostBlock->next)
            {
                _beginBlock = hostBlock->next;
            }
            else
            {
                _beginBlock = null;
            }
        }
        _freeBlockAllocator.deallocateT(hostBlock);
    }
    else
    {
        hostBlock->offset -= size;
        hostBlock->count += size;

        FreeBlock* sortedParent = findFreeBlock(hostBlock->count - 1);
        if(sortedParent)
        {
            /*
             *   Free block must always become smaller. Due to sorting the
             * smallest block is always first. If a smaller block can be
             * found (sortedParent), then freeBlock must inherently have a
             * previous element.
             */
            // ReSharper disable once CppUnreachableCode
            if constexpr(true || hostBlock->prev)
            {
                hostBlock->prev->next = hostBlock->next;
                if(hostBlock->next)
                {
                    hostBlock->next->prev = hostBlock->prev;
                }
            }

            if(sortedParent->next)
            {
                sortedParent->next->prev = hostBlock;
            }
            sortedParent->next = hostBlock;
        }
        else
        {
            if(hostBlock->prev)
            {
                hostBlock->prev->next = hostBlock->next;
                if(hostBlock->next)
                {
                    hostBlock->next->prev = hostBlock->prev;
                }

                hostBlock->prev = null;
            }

            hostBlock->next = _beginBlock;
            _beginBlock->prev = hostBlock;
        }
    }
}

void DX12BufferResourceUploadHandler2::mergeFreeBlock(FreeBlock* const freeBlock) noexcept
{
    if(!_beginBlock)
    {
        _beginBlock = freeBlock;
        return;
    }
    
    FreeBlock* base = _beginBlock;

    while(true)
    {
        if(!base)
        {
            return; 
        }

        if(freeBlock->offset + freeBlock->count == base->offset)
        {
            freeBlock->count += base->count;
            freeBlock->prev = base->prev;
            freeBlock->next = base->next;

            if(freeBlock->prev)
            {
                freeBlock->prev->next = freeBlock;
            }
            else
            {
                _beginBlock = freeBlock;
            }

            if(freeBlock->next)
            {
                freeBlock->next->prev = freeBlock;
            }

            _freeBlockAllocator.deallocateT(base);
            return;
        }

        if(base->offset + base->count == freeBlock->offset)
        {
            base->count += freeBlock->count;
            _freeBlockAllocator.deallocateT(freeBlock);

            if(base->next && base->offset + base->count == base->next->offset)
            {
                base->count += base->next->count;
                base->next = base->next->next;
                if(base->next)
                {
                    base->next->prev = base;
                }
            }
            return;
        }

        base = base->next;
    }
}
#endif
