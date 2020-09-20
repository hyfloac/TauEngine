#pragma once

#ifdef _WIN32
#include <d3d12.h>
#include <Objects.hpp>
#include <NumTypes.hpp>
#include <allocator/FixedBlockAllocator.hpp>
#include <Safeties.hpp>
#include "DLL.hpp"

struct DX12Allocation final
{
    ID3D12Resource* resource;
    u64 offset;
    u64 size;
};

class DX12AutoAllocation;
class DX12AutoAllocation2;

class TAU_DLL DX12BufferResourceUploadHandler final
{
    DELETE_CM(DX12BufferResourceUploadHandler);
public:
    struct FreeBlock final
    {
        static constexpr uSys TOMBSTONE_BIT = 0x01ull << (sizeof(uSys) * CHAR_BIT - 1);

        uSys offset;
        uSys count;
        FreeBlock* prev;
        FreeBlock* next;

        FreeBlock(const uSys _offset, const uSys _count, FreeBlock* const _prev, FreeBlock* const _next) noexcept
            : offset(_offset)
            , count(_count)
            , prev(_prev)
            , next(_next)
        { }

        uSys rawOffset() const noexcept { return offset & ~TOMBSTONE_BIT; }

        bool isTombstone() const noexcept { return offset & TOMBSTONE_BIT; }
    };

#ifdef TAU_PRODUCTION
    using FBAllocator = FixedBlockAllocator<AllocationTracking::None>;
#else
    using FBAllocator = FixedBlockAllocator<AllocationTracking::DoubleDeleteCount>;
#endif
private:
    ID3D12Device* _device;
    ID3D12Heap* _heap;
    FBAllocator _freeBlockAllocator;
    uSys _heapSize;
    FreeBlock* _beginBlock;
public:
    DX12BufferResourceUploadHandler(ID3D12Device* const device, ID3D12Heap* const heap, const uSys heapSize) noexcept
        : _device(device)
        , _heap(heap)
        , _freeBlockAllocator(sizeof(FreeBlock), heapSize / 65536 + 1)
        , _heapSize(heapSize)
        , _beginBlock(_freeBlockAllocator.allocateT<FreeBlock>(0, heapSize, null, null))
    { }

    ~DX12BufferResourceUploadHandler() noexcept
    {
        _heap->Release();
        _freeBlockAllocator.deallocateT(_beginBlock);
    }

    [[nodisard]] uSys bufferHeapSize() const noexcept { return _heapSize; }

    [[nodiscard]] DX12Allocation alloc(uSys minSize) noexcept;
    [[nodiscard]] NullableRef<DX12AutoAllocation> allocAuto(uSys minSize) noexcept;

    void free(DX12Allocation& resource) noexcept;
private:
    [[nodiscard]] FreeBlock* findFreeBlock(uSys minSize) noexcept;

    void resizeFreeBlock(FreeBlock* hostBlock, u64 size) noexcept;

    void mergeFreeBlock(FreeBlock* freeBlock) noexcept;
};

class TAU_DLL DX12BufferResourceUploadHandler2 final
{
    DELETE_CM(DX12BufferResourceUploadHandler2);
public:
    struct FreeBlock final
    {
        uSys offset;
        uSys count;
        FreeBlock* prev;
        FreeBlock* next;

        FreeBlock(const uSys _offset, const uSys _count, FreeBlock* const _prev, FreeBlock* const _next) noexcept
            : offset(_offset)
            , count(_count)
            , prev(_prev)
            , next(_next)
        { }
    };

#ifdef TAU_PRODUCTION
    using FBAllocator = FixedBlockAllocator<AllocationTracking::None>;
#else
    using FBAllocator = FixedBlockAllocator<AllocationTracking::DoubleDeleteCount>;
#endif
private:
    ID3D12Resource* _heap;
    FBAllocator _freeBlockAllocator;
    uSys _heapSize;
    FreeBlock* _beginBlock;
public:
    DX12BufferResourceUploadHandler2(ID3D12Resource* const heap, const uSys heapSize) noexcept
        : _heap(heap)
        , _freeBlockAllocator(sizeof(FreeBlock), heapSize / 65536 + 1)
        , _heapSize(heapSize)
        , _beginBlock(_freeBlockAllocator.allocateT<FreeBlock>(0, heapSize, null, null))
    { }

    ~DX12BufferResourceUploadHandler2() noexcept
    {
        _heap->Release();
        _freeBlockAllocator.deallocateT(_beginBlock);
    }

    [[nodisard]] uSys bufferHeapSize() const noexcept { return _heapSize; }

    [[nodiscard]] DX12Allocation alloc(uSys minSize) noexcept;
    [[nodiscard]] NullableRef<DX12AutoAllocation2> allocAuto(uSys minSize) noexcept;

    void free(DX12Allocation& resource) noexcept;
private:
    [[nodiscard]] FreeBlock* findFreeBlock(uSys minSize) noexcept;

    void resizeFreeBlock(FreeBlock* hostBlock, u64 size) noexcept;

    void mergeFreeBlock(FreeBlock* freeBlock) noexcept;
};

class DX12AutoAllocation final
{
    DELETE_CM(DX12AutoAllocation);
private:
    DX12Allocation _allocation;
    DX12BufferResourceUploadHandler& _handler;
public:
    DX12AutoAllocation(const DX12Allocation& allocation, DX12BufferResourceUploadHandler& handler) noexcept
        : _allocation(allocation)
        , _handler(handler)
    { }

    ~DX12AutoAllocation() noexcept
    { _handler.free(_allocation); }
};

class DX12AutoAllocation2 final
{
    DELETE_CM(DX12AutoAllocation2);
private:
    DX12Allocation _allocation;
    DX12BufferResourceUploadHandler2& _handler;
public:
    DX12AutoAllocation2(const DX12Allocation& allocation, DX12BufferResourceUploadHandler2& handler) noexcept
        : _allocation(allocation)
        , _handler(handler)
    { }

    ~DX12AutoAllocation2() noexcept
    { _handler.free(_allocation); }

    [[nodiscard]] DX12Allocation allocation() const noexcept { return _allocation; }
    [[nodiscard]] DX12Allocation& allocation() noexcept { return _allocation; }
};
#endif
