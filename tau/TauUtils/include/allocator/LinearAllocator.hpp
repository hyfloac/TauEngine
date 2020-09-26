#pragma once

#include "Objects.hpp"
#include "TauAllocator.hpp"
#include "PageAllocator.hpp"
#include "FixedBlockAllocator.hpp"

template<AllocationTracking _AllocTracking = AllocationTracking::None>
class LinearAllocatorHelper final
{
public:
    using FBAllocator = FixedBlockAllocator<_AllocTracking>;

    struct FreeBlockTree final
    {
        uSys offset;
        uSys count;
        FreeBlockTree* left;
        FreeBlockTree* right;

        FreeBlock(const uSys _offset, const uSys _count, FreeBlockTree* const _left, FreeBlockTree* const _next) noexcept
            : offset(_offset)
            , count(_count)
            , prev(_prev)
            , next(_next)
        { }
    };
private:
    FBAllocator _freeBlockAllocator;
    FreeBlock* _beginBlock;
    FreeBlock* _endBlock;
private:
    [[nodiscard]] FreeBlock* findFreeBlock(uSys minSize) noexcept
    {
        FreeBlock* base = _beginBlock;

        while(true)
        {
            if(!base)
            {
                return nullptr; 
            }

            if(base->count >= minSize)
            {
                return base;
            }

            base = base->next;
        }
    }

    void resizeFreeBlock(FreeBlock* hostBlock, uSys size) noexcept;
    void mergeFreeBlock(FreeBlock* freeBlock) noexcept;
};
