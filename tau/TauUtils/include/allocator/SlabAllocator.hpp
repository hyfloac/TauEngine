#pragma once

#include "NumTypes.hpp"
#include "DynArray.hpp"
#include "TUMaths.hpp"
#include "ArrayList.hpp"
#include "allocator/FixedBlockAllocator.hpp"

class SlabAllocatorHelper
{
    DEFAULT_DESTRUCT_VI(SlabAllocatorHelper);
public:
    struct AllocPoint final
    {
        uSys block;
        uSys offset;
        AllocPoint* next;

        AllocPoint(const uSys _block, const uSys _offset) noexcept
            : block(_block)
            , offset(_offset)
            , next(nullptr)
        { }
    };

    struct SlabSet
    {
        AllocPoint* head;
        AllocPoint* tail;
    };

    struct Block final
    {
        DEFAULT_CONSTRUCT_PU(Block);
        DEFAULT_DESTRUCT(Block);
        DEFAULT_CM_PU(Block);
    public:
        void* ptr;
        uSys size;
        uSys slabCount;
        uSys activeAllocations;
    public:
        Block(void* const _ptr, const uSys _size) noexcept
            : ptr(_ptr)
            , size(_size)
            , slabCount(0)
            , activeAllocations(0)
        { }
    };

    struct Allocation final
    {
        DEFAULT_CONSTRUCT_PU(Allocation);
        DEFAULT_DESTRUCT(Allocation);
        DEFAULT_CM_PU(Allocation);
    public:
        void* ptr;
        uSys block;
        uSys slabIndex;
        uSys offset;
    public:
        Allocation(void* const _ptr, const uSys _block, const uSys _slabIndex, const uSys _offset) noexcept
            : ptr(_ptr)
            , block(_block)
            , slabIndex(_slabIndex)
            , offset(_offset)
        { }
    };
#ifdef TAU_PRODUCTION
    using FBAllocator =  FixedBlockAllocator<AllocationTracking::None>;
#else
    using FBAllocator =  FixedBlockAllocator<AllocationTracking::DoubleDeleteCount>;
#endif
protected:
    uSys _basePower;
    DynArray<SlabSet> _slabs;
    ArrayList<Block, ALMoveMethod::MemCopy> _blocks;
    FBAllocator _allocPointAllocator;
public:
    SlabAllocatorHelper(const uSys basePower, const uSys slabCount, const uSys maxBlocks = 256, const uSys maxAllocations = 4096) noexcept
        : _basePower(basePower)
        , _slabs(slabCount)
        , _blocks(maxBlocks)
        , _allocPointAllocator(sizeof(AllocPoint), maxAllocations)
    { ::std::memset(_slabs.arr(), 0, _slabs.size() * sizeof(SlabSet)); }

    [[nodiscard]] Allocation allocate(const uSys size) noexcept
    {
        const uSys slabIndex = computeSlabIndex(size);

        if(!_slabs[slabIndex].head)
        {
            uSys blockIndex;
            Block* block = _blocks.placement(&blockIndex);
            if(!allocateBlock(block))
            { return Allocation(nullptr, 0, 0, 0); }
            if(!carveBlock(blockIndex))
            { return Allocation(nullptr, 0, 0, 0); }
            if(!_slabs[slabIndex].head)
            { return Allocation(nullptr, 0, 0, 0); }
        }

        AllocPoint* const head = _slabs[slabIndex].head;
        Block& block = _blocks[head->block];
        void* ptr = reinterpret_cast<u8*>(block.ptr) + head->offset;
        
        ++block.activeAllocations;

        _slabs[slabIndex].head = _slabs[slabIndex].head->next;
        _allocPointAllocator.deallocateT(head);
        if(!_slabs[slabIndex].head)
        { _slabs[slabIndex].tail = nullptr; }

        return Allocation(ptr, head->block, slabIndex, head->offset);
    }

    void deallocate(Allocation& allocation) noexcept
    {
        AllocPoint* const allocPoint = _allocPointAllocator.allocateT<AllocPoint>(allocation.block, allocation.offset);
        if(!_slabs[allocation.slabIndex].head)
        {
            _slabs[allocation.slabIndex].head = allocPoint;
            _slabs[allocation.slabIndex].tail = allocPoint;
        }
        else
        {
            _slabs[allocation.slabIndex].tail->next = allocPoint;
            _slabs[allocation.slabIndex].tail = allocPoint;
        }
        Block& block = _blocks[allocation.block];
        --block.activeAllocations;
    }
protected:
    /**
     * Computes which slab to use.
     *
     *   Because we don't necessarily use a minimum slab size of
     * one byte we have to do some extra math to get the right
     * slab.
     *
     *   We use a base 2 slab allocator which means that every
     * slab size is a power of 2. Thus to figure out which slab
     * will fit our data we can take the log2 of the required block
     * size. As a performance optimization we can use a
     * specialization for integers, this turns out to just count
     * the number of leading 0's, something common enough to have
     * its own CPU instruction. Unfortunately this will floor the
     * value, resulting in getting a slab that is too small. To fix
     * this we can just shift the block size so that it is smallest
     * value greater than the original that is also a power of 2.
     * This in effect changes log2i from flooring the value to
     * ceiling the value. Afterwords we just have to offset the
     * slab index to fit our minimum slab size.
     */
    [[nodiscard]] uSys computeSlabIndex(const uSys blockSize) const noexcept
    {
        /* Compute the power of slab we need.
         *
         *   log2i floors the value, we need the ceil, thus we call
         * nextPowerOf2 first. */
        const uSys power = log2i(nextPowerOf2(blockSize));

        if(power <= _basePower)
        { return 0; }
        return power - _basePower - 1;
    }

    virtual bool allocateBlock(Block* block) noexcept = 0;

    virtual bool carveBlock(const uSys blockIndex) noexcept
    {
        Block& block = _blocks[blockIndex];

        // Is block already carved?
        if(block.activeAllocations)
        { return false; }

        block.slabCount = 0;

        uSys offset = 0;
        uSys remainingSize = block.size;

        bool didAllocate;

        do
        {
            didAllocate = false;

            for(iSys i = static_cast<iSys>(_basePower) + static_cast<iSys>(_slabs.count()) - 1; i >= static_cast<iSys>(_basePower); --i)
            {
                const uSys slabIndex = static_cast<uSys>(i) - _basePower;
                const uSys slabSize = 1 << i;
                if(slabSize > remainingSize)
                { continue; }

                AllocPoint* const allocPoint = _allocPointAllocator.allocateT<AllocPoint>(blockIndex, offset);

                if(!_slabs[slabIndex].head)
                {
                    _slabs[slabIndex].head = allocPoint;
                    _slabs[slabIndex].tail = allocPoint;
                }
                else
                {
                    _slabs[slabIndex].tail->next = allocPoint;
                    _slabs[slabIndex].tail = allocPoint;
                }

                offset += slabSize;
                remainingSize -= slabSize;
                didAllocate = true;
            }
        } while(didAllocate);

        return true;
    }
};
