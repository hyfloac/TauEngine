//
// Copyright (c) 2019 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "dx/dx12/D3D12MemAlloc.h"
#include <NumTypes.hpp>

#include <mutex>
#include <atomic>
#include <algorithm>
#include <malloc.h> // for _aligned_malloc, _aligned_free

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Configuration Begin
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef D3D12MA_ASSERT
    #include <cassert>
    #define D3D12MA_ASSERT(cond) assert(cond)
#endif

// Assert that will be called very often, like inside data structures e.g. operator[].
// Making it non-empty can make program slow.
#ifndef D3D12MA_HEAVY_ASSERT
    #ifdef _DEBUG
        #define D3D12MA_HEAVY_ASSERT(expr)   //D3D12MA_ASSERT(expr)
    #else
        #define D3D12MA_HEAVY_ASSERT(expr)
    #endif
#endif

#ifndef D3D12MA_DEBUG_ALWAYS_COMMITTED
    /*
    Every allocation will have its own memory block.
    Define to 1 for debugging purposes only.
    */
    #define D3D12MA_DEBUG_ALWAYS_COMMITTED (0)
#endif

#ifndef D3D12MA_DEBUG_ALIGNMENT
    /*
    Minimum alignment of all allocations, in bytes.
    Set to more than 1 for debugging purposes only. Must be power of two.
    */
    #define D3D12MA_DEBUG_ALIGNMENT (1)
#endif

#ifndef D3D12MA_DEBUG_MARGIN
    // Minimum margin before and after every allocation, in bytes.
    // Set nonzero for debugging purposes only.
    #define D3D12MA_DEBUG_MARGIN (0)
#endif

#ifndef D3D12MA_DEBUG_GLOBAL_MUTEX
    /*
    Set this to 1 for debugging purposes only, to enable single mutex protecting all
    entry calls to the library. Can be useful for debugging multithreading issues.
    */
    #define D3D12MA_DEBUG_GLOBAL_MUTEX (0)
#endif

#ifndef D3D12MA_DEFAULT_BLOCK_SIZE
   /// Default size of a block allocated as single ID3D12Heap.
   #define D3D12MA_DEFAULT_BLOCK_SIZE (256ull * 1024 * 1024)
#endif

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//
// Configuration End
//
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


namespace D3D12MA
{

////////////////////////////////////////////////////////////////////////////////
// Private globals - CPU memory allocation

static void* DefaultAllocate(const uSys Size, const uSys Alignment, void* /*pUserData*/) noexcept
{
    return _aligned_malloc(Size, Alignment);
}

static void DefaultFree(void* const pMemory, void* /*pUserData*/) noexcept
{
    return _aligned_free(pMemory);
}

static void* Malloc(const ALLOCATION_CALLBACKS& allocs, const uSys size, const uSys alignment) noexcept
{
    return (*allocs.pAllocate)(size, alignment, allocs.pUserData);
}

static void Free(const ALLOCATION_CALLBACKS& allocs, void* const memory) noexcept
{
    (*allocs.pFree)(memory, allocs.pUserData);
}

template<typename _T>
static _T* Allocate(const ALLOCATION_CALLBACKS& allocs) noexcept
{
    return reinterpret_cast<_T*>(Malloc(allocs, sizeof(_T), __alignof(_T)));
}

template<typename _T>
static _T* AllocateArray(const ALLOCATION_CALLBACKS& allocs, const uSys count) noexcept
{
    return reinterpret_cast<_T*>(Malloc(allocs, sizeof(_T) * count, __alignof(_T)));
}

#define D3D12MA_NEW(allocs, type) new(D3D12MA::Allocate<type>(allocs))(type)
#define D3D12MA_NEW_ARRAY(allocs, type, count) new(D3D12MA::AllocateArray<type>((allocs), (count)))(type)

template<typename _T>
static void D3D12MA_DELETE(const ALLOCATION_CALLBACKS& allocs, _T* const memory) noexcept
{
    if(memory)
    {
        memory->~T();
        Free(allocs, memory);
    }
}

template<typename _T>
static void D3D12MA_DELETE_ARRAY(const ALLOCATION_CALLBACKS& allocs, _T* const memory, const uSys count) noexcept
{
    if(memory)
    {
        for(uSys i = count - 1; i > 0; --i)
        {
            memory[i].~T();
        }
        Free(allocs, memory);
    }
}

static void SetupAllocationCallbacks(ALLOCATION_CALLBACKS& outAllocs, const ALLOCATOR_DESC& allocatorDesc) noexcept
{
    if(allocatorDesc.pAllocationCallbacks)
    {
        outAllocs = *allocatorDesc.pAllocationCallbacks;
        D3D12MA_ASSERT(outAllocs.pAllocate != nullptr && outAllocs.pFree != nullptr);
    }
    else
    {
        outAllocs.pAllocate = &DefaultAllocate;
        outAllocs.pFree = &DefaultFree;
        outAllocs.pUserData = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Private globals - basic facilities

#define D3D12MA_VALIDATE(cond) do { if(!(cond)) { \
        D3D12MA_ASSERT(0 && "Validation failed: " #cond); \
        return false; \
    } } while(false)

template<typename _T>
static inline _T D3D12MA_MIN(const _T& a, const _T& b) noexcept
{ return a <= b ? a : b; }

template<typename _T>
static inline _T D3D12MA_MAX(const _T& a, const _T& b) noexcept
{ return a <= b ? b : a; }

template<typename _T>
static inline void D3D12MA_SWAP(_T& a, _T& b) noexcept
{
    _T tmp = a; a = b; b = tmp;
}

#ifndef D3D12MA_MUTEX
    class Mutex final
    {
    public:
        void Lock() noexcept { m_Mutex.lock(); }
        void Unlock() noexcept { m_Mutex.unlock(); }
    private:
        ::std::mutex m_Mutex;
    };
    #define D3D12MA_MUTEX Mutex
#endif

#if !defined(_WIN32) || !defined(WINVER) || WINVER < 0x0600
    #error Required at least WinAPI version supporting: client = Windows Vista, server = Windows Server 2008.
#endif

#ifndef D3D12MA_RW_MUTEX
    class RWMutex final
    {
    public:
        RWMutex() noexcept { InitializeSRWLock(&m_Lock); }
        void LockRead() noexcept { AcquireSRWLockShared(&m_Lock); }
        void UnlockRead() noexcept { ReleaseSRWLockShared(&m_Lock); }
        void LockWrite() noexcept { AcquireSRWLockExclusive(&m_Lock); }
        void UnlockWrite() noexcept { ReleaseSRWLockExclusive(&m_Lock); }
    private:
        SRWLOCK m_Lock;
    };
    #define D3D12MA_RW_MUTEX RWMutex
#endif

/*
If providing your own implementation, you need to implement a subset of std::atomic:

- Constructor(UINT desired)
- UINT load() const
- void store(UINT desired)
- bool compare_exchange_weak(UINT& expected, UINT desired)
*/
#ifndef D3D12MA_ATOMIC_UINT32
    #define D3D12MA_ATOMIC_UINT32 std::atomic<UINT>  // NOLINT(clang-diagnostic-unused-macros)
#endif

// Aligns given value up to nearest multiply of align value. For example: AlignUp(11, 8) = 16.
// Use types like UINT, uint64_t as T.
template<typename _T>
static inline constexpr _T AlignUp(const _T val, const _T align) noexcept
{ return (val + align - 1) / align * align; }

// Aligns given value down to nearest multiply of align value. For example: AlignDown(11, 8) = 8.
// Use types like UINT, uint64_t as T.
template<typename _T>
static inline constexpr _T AlignDown(const _T val, const _T align) noexcept
{ return val / align * align; }

// Division with mathematical rounding to nearest number.
template<typename _T>
static inline constexpr _T RoundDiv(const _T x, const _T y) noexcept
{ return (x + (y / static_cast<_T>(2))) / y; }

/*
Returns true if given number is a power of two.
T must be unsigned integer number or signed integer but always nonnegative.
For 0 returns true.
*/
template<typename _T>
static inline constexpr bool IsPow2(_T x) noexcept
{ return (x & (x-1)) == 0; }

// Helper RAII class to lock a mutex in constructor and unlock it in destructor (at the end of scope).
struct MutexLock final
{
    DELETE_CM(MutexLock);
public:
    MutexLock(D3D12MA_MUTEX& mutex, const bool useMutex = true) noexcept
        : m_pMutex(useMutex ? &mutex : nullptr)
    {
        if(m_pMutex)
        { m_pMutex->Lock(); }
    }

    ~MutexLock() noexcept 
    {
        if(m_pMutex)
        { m_pMutex->Unlock(); }
    }
private:
    D3D12MA_MUTEX* m_pMutex;
};

// Helper RAII class to lock a RW mutex in constructor and unlock it in destructor (at the end of scope), for reading.
struct MutexLockRead final
{
    DELETE_CM(MutexLockRead);
public:
    MutexLockRead(D3D12MA_RW_MUTEX& mutex, const bool useMutex) noexcept
        : m_pMutex(useMutex ? &mutex : nullptr)
    {
        if(m_pMutex)
        { m_pMutex->LockRead(); }
    }

    ~MutexLockRead() noexcept
    {
        if(m_pMutex)
        { m_pMutex->UnlockRead(); }
    }
private:
    D3D12MA_RW_MUTEX* m_pMutex;
};

// Helper RAII class to lock a RW mutex in constructor and unlock it in destructor (at the end of scope), for writing.
struct MutexLockWrite final
{
    DELETE_CM(MutexLockWrite);
public:
    MutexLockWrite(D3D12MA_RW_MUTEX& mutex, const bool useMutex) noexcept
        : m_pMutex(useMutex ? &mutex : nullptr)
    {
        if(m_pMutex)
        { m_pMutex->LockWrite(); }
    }

    ~MutexLockWrite() noexcept
    {
        if(m_pMutex)
        { m_pMutex->UnlockWrite(); }
    }
private:
    D3D12MA_RW_MUTEX* m_pMutex;
};

#if D3D12MA_DEBUG_GLOBAL_MUTEX
    static D3D12MA_MUTEX g_DebugGlobalMutex;
    #define D3D12MA_DEBUG_GLOBAL_MUTEX_LOCK MutexLock debugGlobalMutexLock(g_DebugGlobalMutex, true);
#else
    #define D3D12MA_DEBUG_GLOBAL_MUTEX_LOCK
#endif

// Minimum size of a free suballocation to register it in the free suballocation collection.
static constexpr UINT64 MIN_FREE_SUBALLOCATION_SIZE_TO_REGISTER = 16;

/*
Performs binary search and returns iterator to first element that is greater or
equal to `key`, according to comparison `cmp`.

Cmp should return true if first argument is less than second argument.

Returned value is the found element, if present in the collection or place where
new element with value (key) should be inserted.
*/
template<typename _CmpLess, typename _IterT, typename _KeyT>
static _IterT BinaryFindFirstNotLess(_IterT beg, _IterT end, const _KeyT &key, const _CmpLess& cmp) noexcept
{
    uSys down = 0, up = (end - beg);
    while(down < up)
    {
        const uSys mid = (down + up) / 2;
        if(cmp(*(beg + mid), key))
        {
            down = mid + 1;
        }
        else
        {
            up = mid;
        }
    }
    return beg + down;
}

/*
Performs binary search and returns iterator to an element that is equal to `key`,
according to comparison `cmp`.

Cmp should return true if first argument is less than second argument.

Returned value is the found element, if present in the collection or end if not
found.
*/
template<typename _CmpLess, typename _IterT, typename _KeyT>
_IterT BinaryFindSorted(const _IterT& beg, const _IterT& end, const _KeyT& value, const _CmpLess& cmp) noexcept
{
    _IterT it = BinaryFindFirstNotLess<_CmpLess, _IterT, _KeyT>(beg, end, value, cmp);
    if(it == end ||
        (!cmp(*it, value) && !cmp(value, *it)))
    {
        return it;
    }
    return end;
}

struct PointerLess final
{
    [[nodiscard]] bool operator()(const void* const lhs, const void* const rhs) const noexcept
    { return lhs < rhs; }
};

static constexpr UINT HEAP_TYPE_COUNT = 3;

static constexpr UINT HeapTypeToIndex(const D3D12_HEAP_TYPE type) noexcept
{
    switch(type)
    {
        case D3D12_HEAP_TYPE_DEFAULT:  return 0;
        case D3D12_HEAP_TYPE_UPLOAD:   return 1;
        case D3D12_HEAP_TYPE_READBACK: return 2;
        default: D3D12MA_ASSERT(0); return UINT_MAX;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Private class Vector

/*
Dynamically resizing continuous array. Class with interface similar to std::vector.
T must be POD because constructors and destructors are not called and memcpy is
used for these objects.
*/
template<typename _T>
class Vector final
{
    DELETE_MOVE(Vector);
public:
    typedef _T value_type;
    typedef _T* iterator;
private:
    const ALLOCATION_CALLBACKS& m_AllocationCallbacks;
    _T* m_pArray;
    uSys m_Count;
    uSys m_Capacity;
public:
    // allocationCallbacks externally owned, must outlive this object.
    Vector(const ALLOCATION_CALLBACKS& allocationCallbacks) noexcept
        : m_AllocationCallbacks(allocationCallbacks)
        , m_pArray(nullptr)
        , m_Count(0)
        , m_Capacity(0)
    { }

    Vector(const uSys count, const ALLOCATION_CALLBACKS& allocationCallbacks) noexcept
        : m_AllocationCallbacks(allocationCallbacks)
        , m_pArray(count ? AllocateArray<_T>(allocationCallbacks, count) : nullptr)
        , m_Count(count)
        , m_Capacity(count)
    { }

    ~Vector() noexcept
    { Free(m_AllocationCallbacks, m_pArray); }

    Vector(const Vector<_T>& src) noexcept
        : m_AllocationCallbacks(src.m_AllocationCallbacks)
        , m_pArray(src.m_Count ? AllocateArray<_T>(src.m_AllocationCallbacks, src.m_Count) : nullptr)
        , m_Count(src.m_Count)
        , m_Capacity(src.m_Count)
    {
        if(m_Count > 0)
        { (void) ::std::memcpy(m_pArray, src.m_pArray, m_Count * sizeof(_T)); }
    }

    Vector& operator=(const Vector<_T>& rhs) noexcept
    {
        if(&rhs != this)
        {
            resize(rhs.m_Count);
            if(m_Count != 0)
            { ::std::memcpy(m_pArray, rhs.m_pArray, m_Count * sizeof(_T)); }
        }
        return *this;
    }

    [[nodiscard]] bool empty() const noexcept { return m_Count == 0; }
    [[nodiscard]] uSys size() const noexcept { return m_Count; }

    [[nodiscard]]       _T* data()       noexcept { return m_pArray; }
    [[nodiscard]] const _T* data() const noexcept { return m_pArray; }

    _T& operator[](const uSys index) noexcept
    {
        D3D12MA_HEAVY_ASSERT(index < m_Count);
        return m_pArray[index];
    }

    const _T& operator[](const uSys index) const noexcept
    {
        D3D12MA_HEAVY_ASSERT(index < m_Count);
        return m_pArray[index];
    }

    _T& front() noexcept
    {
        D3D12MA_HEAVY_ASSERT(m_Count > 0);
        return m_pArray[0];
    }

    const _T& front() const noexcept
    {
        D3D12MA_HEAVY_ASSERT(m_Count > 0);
        return m_pArray[0];
    }

    _T& back() noexcept
    {
        D3D12MA_HEAVY_ASSERT(m_Count > 0);
        return m_pArray[m_Count - 1];
    }

    const _T& back() const noexcept
    {
        D3D12MA_HEAVY_ASSERT(m_Count > 0);
        return m_pArray[m_Count - 1];
    }

    void reserve(uSys newCapacity, const bool freeMemory = false) noexcept
    {
        newCapacity = D3D12MA_MAX(newCapacity, m_Count);

        if((newCapacity < m_Capacity) && !freeMemory)
        { newCapacity = m_Capacity; }

        if(newCapacity != m_Capacity)
        {
            _T* const newArray = newCapacity ? AllocateArray<_T>(m_AllocationCallbacks, newCapacity) : nullptr;
            if(m_Count != 0)
            { ::std::memcpy(newArray, m_pArray, m_Count * sizeof(_T)); }
            Free(m_AllocationCallbacks, m_pArray);
            m_Capacity = newCapacity;
            m_pArray = newArray;
        }
    }

    void resize(const uSys newCount, const bool freeMemory = false) noexcept
    {
        uSys newCapacity = m_Capacity;
        if(newCount > m_Capacity)
        { newCapacity = D3D12MA_MAX(newCount, D3D12MA_MAX(m_Capacity * 3 / 2, static_cast<uSys>(8))); }
        else if(freeMemory)
        { newCapacity = newCount; }

        if(newCapacity != m_Capacity)
        {
            _T* const newArray = newCapacity ? AllocateArray<_T>(m_AllocationCallbacks, newCapacity) : nullptr;
            const uSys elementsToCopy = D3D12MA_MIN(m_Count, newCount);
            if(elementsToCopy != 0)
            { ::std::memcpy(newArray, m_pArray, elementsToCopy * sizeof(_T)); }
            Free(m_AllocationCallbacks, m_pArray);
            m_Capacity = newCapacity;
            m_pArray = newArray;
        }

        m_Count = newCount;
    }

    void clear(const bool freeMemory = false) noexcept
    { resize(0, freeMemory); }

    void insert(const uSys index, const _T& src) noexcept
    {
        D3D12MA_HEAVY_ASSERT(index <= m_Count);
        const uSys oldCount = size();
        resize(oldCount + 1);
        if(index < oldCount)
        {
            ::std::memmove(m_pArray + (index + 1), m_pArray + index, (oldCount - index) * sizeof(_T));
        }
        m_pArray[index] = src;
    }

    void remove(const uSys index) noexcept
    {
        D3D12MA_HEAVY_ASSERT(index < m_Count);
        const uSys oldCount = size();
        if(index < oldCount - 1)
        {
            ::std::memmove(m_pArray + index, m_pArray + (index + 1), (oldCount - index - 1) * sizeof(_T));
        }
        resize(oldCount - 1);
    }

    void push_back(const _T& src) noexcept
    {
        const uSys newIndex = size();
        resize(newIndex + 1);
        m_pArray[newIndex] = src;
    }

    void pop_back() noexcept
    {
        D3D12MA_HEAVY_ASSERT(m_Count > 0);
        resize(size() - 1);
    }

    void push_front(const _T& src) noexcept
    { insert(0, src); }

    void pop_front() noexcept
    {
        D3D12MA_HEAVY_ASSERT(m_Count > 0);
        remove(0);
    }

    [[nodiscard]] iterator begin() noexcept { return m_pArray; }
    [[nodiscard]] iterator end() noexcept { return m_pArray + m_Count; }

    template<typename _CmpLess>
    uSys InsertSorted(const _T& value, const _CmpLess& cmp) noexcept
    {
        const uSys indexToInsert = BinaryFindFirstNotLess<_CmpLess, iterator, _T>(
            m_pArray,
            m_pArray + m_Count,
            value,
            cmp) - m_pArray;
        insert(indexToInsert, value);
        return indexToInsert;
    }

    template<typename _CmpLess>
    bool RemoveSorted(const _T& value, const _CmpLess& cmp) noexcept
    {
        const iterator it = BinaryFindFirstNotLess(
            m_pArray,
            m_pArray + m_Count,
            value,
            cmp);
        if((it != end()) && !cmp(*it, value) && !cmp(value, *it))
        {
            const uSys indexToRemove = it - begin();
            remove(indexToRemove);
            return true;
        }
        return false;
    }
};

////////////////////////////////////////////////////////////////////////////////
// Private class PoolAllocator

/*
Allocator for objects of type T using a list of arrays (pools) to speed up
allocation. Number of elements that can be allocated is not bounded because
allocator can create multiple blocks.
T should be POD because constructor and destructor is not called in Alloc or
Free.
*/
template<typename _T>
class PoolAllocator final
{
    DELETE_CM(PoolAllocator);
public:
    union Item final
    {
        UINT NextFreeIndex; // UINT32_MAX means end of list.
        _T Value;
    };

    struct ItemBlock final
    {
        Item* pItems;
        UINT Capacity;
        UINT FirstFreeIndex;
    };
private:
    const ALLOCATION_CALLBACKS& m_AllocationCallbacks;
    const UINT m_FirstBlockCapacity;
    Vector<ItemBlock> m_ItemBlocks;
public:
    // allocationCallbacks externally owned, must outlive this object.
    PoolAllocator(const ALLOCATION_CALLBACKS& allocationCallbacks, const UINT firstBlockCapacity) noexcept
        : m_AllocationCallbacks(allocationCallbacks)
        , m_FirstBlockCapacity(firstBlockCapacity)
        , m_ItemBlocks(allocationCallbacks)
    { D3D12MA_ASSERT(m_FirstBlockCapacity > 1); }

    ~PoolAllocator() noexcept
    { Clear(); }

    void Clear() noexcept
    {
        for(uSys i = m_ItemBlocks.size() - 1; i > 0; --i)
        {
            D3D12MA_DELETE_ARRAY(m_AllocationCallbacks, m_ItemBlocks[i].pItems, m_ItemBlocks[i].Capacity);
        }
        m_ItemBlocks.clear(true);
    }

    [[nodiscard]] _T* Alloc() noexcept
    {
        for(uSys i = m_ItemBlocks.size() - 1; i > 0; --i)
        {
            ItemBlock& block = m_ItemBlocks[i];
            // This block has some free items: Use first one.
            if(block.FirstFreeIndex != UINT32_MAX)
            {
                Item* const pItem = &block.pItems[block.FirstFreeIndex];
                block.FirstFreeIndex = pItem->NextFreeIndex;
                return &pItem->Value;
            }
        }

        // No block has free item: Create new one and use it.
        ItemBlock& newBlock = CreateNewBlock();
        Item* const pItem = &newBlock.pItems[0];
        newBlock.FirstFreeIndex = pItem->NextFreeIndex;
        return &pItem->Value;
    }

    void Free(_T* const ptr) noexcept
    {
        // Search all memory blocks to find ptr.
        for(uSys i = m_ItemBlocks.size() - 1; i > 0; --i)
        {
            ItemBlock& block = m_ItemBlocks[i];

            Item* pItemPtr;
            ::std::memcpy(&pItemPtr, &ptr, sizeof(pItemPtr));

            // Check if pItemPtr is in address range of this block.
            if((pItemPtr >= block.pItems) && (pItemPtr < block.pItems + block.Capacity))
            {
                const UINT index = static_cast<UINT>(pItemPtr - block.pItems);
                pItemPtr->NextFreeIndex = block.FirstFreeIndex;
                block.FirstFreeIndex = index;
                return;
            }
        }
        D3D12MA_ASSERT(0 && "Pointer doesn't belong to this memory pool.");
    }
private:
    [[nodiscard]] ItemBlock& CreateNewBlock() noexcept
    {
        const UINT newBlockCapacity = m_ItemBlocks.empty() ?
            m_FirstBlockCapacity : (m_ItemBlocks.back().Capacity * 3) / 2;

        const ItemBlock newBlock = 
        {
            D3D12MA_NEW_ARRAY(m_AllocationCallbacks, Item, newBlockCapacity),
            newBlockCapacity,
            0
        };

        m_ItemBlocks.push_back(newBlock);

        // Setup singly-linked list of all free items in this block.
        for(UINT i = 0; i < newBlockCapacity - 1; ++i)
        {
            newBlock.pItems[i].NextFreeIndex = i + 1;
        }
        newBlock.pItems[newBlockCapacity - 1].NextFreeIndex = UINT32_MAX;
        return m_ItemBlocks.back();
    }
};

////////////////////////////////////////////////////////////////////////////////
// Private class List

/**
 *   Doubly linked list, with elements allocated out of
 * PoolAllocator.
 *
 *   Has custom interface, as well as STL-style interface,
 * including iterator and const_iterator.
 */
template<typename _T>
class List final
{
    // Intentionally not calling Clear, because that would be unnecessary
    // computations to return all items to m_ItemAllocator as free.
    DEFAULT_DESTRUCT(List);
    DELETE_CM(List);
public:
    struct Item final
    {
        Item* pPrev;
        Item* pNext;
        _T Value;
    };

    class iterator final
    {
        DEFAULT_DESTRUCT(iterator);
        DEFAULT_CM_PU(iterator);
    private:
        List<_T>* m_pList;
        Item* m_pItem;
    private:
        iterator(List<_T>* const pList, Item* const pItem) noexcept
            : m_pList(pList)
            , m_pItem(pItem)
        { }
    public:
        iterator() noexcept
            : m_pList(nullptr)
            , m_pItem(nullptr)
        { }

        [[nodiscard]] _T& operator*() const noexcept
        {
            D3D12MA_HEAVY_ASSERT(m_pItem != nullptr);
            return m_pItem->Value;
        }

        [[nodiscard]] _T* operator->() const noexcept
        {
            D3D12MA_HEAVY_ASSERT(m_pItem != nullptr);
            return &m_pItem->Value;
        }

        iterator& operator++() noexcept
        {
            D3D12MA_HEAVY_ASSERT(m_pItem != nullptr);
            m_pItem = m_pItem->pNext;
            return *this;
        }

        iterator& operator--() noexcept
        {
            if(m_pItem)
            {
                m_pItem = m_pItem->pPrev;
            }
            else
            {
                D3D12MA_HEAVY_ASSERT(!m_pList->IsEmpty());
                m_pItem = m_pList->Back();
            }
            return *this;
        }

        [[nodiscard]] iterator operator++(int) noexcept
        {
            iterator result = *this;
            ++*this;
            return result;
        }

        [[nodiscard]] iterator operator--(int) noexcept
        {
            iterator result = *this;
            --*this;
            return result;
        }

        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept
        {
            D3D12MA_HEAVY_ASSERT(m_pList == rhs.m_pList);
            return m_pItem == rhs.m_pItem;
        }

        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept
        {
            D3D12MA_HEAVY_ASSERT(m_pList == rhs.m_pList);
            return m_pItem != rhs.m_pItem;
        }
    private:
        friend class List<_T>;
    };

    class const_iterator final
    {
        DEFAULT_DESTRUCT(const_iterator);
        DEFAULT_CM_PU(const_iterator);
    private:
        const List<_T>* m_pList;
        const Item* m_pItem;
    private:
        const_iterator(const List<_T>* pList, const Item* pItem) noexcept
            : m_pList(pList)
            , m_pItem(pItem)
        { }
    public:
        const_iterator() noexcept
            : m_pList(nullptr)
            , m_pItem(nullptr)
        { }

        const_iterator(const iterator& src) noexcept
            : m_pList(src.m_pList)
            , m_pItem(src.m_pItem)
        { }

        [[nodiscard]] const _T& operator*() const noexcept
        {
            D3D12MA_HEAVY_ASSERT(m_pItem != nullptr);
            return m_pItem->Value;
        }

        [[nodiscard]] const _T* operator->() const noexcept
        {
            D3D12MA_HEAVY_ASSERT(m_pItem != nullptr);
            return &m_pItem->Value;
        }

        const_iterator& operator++() noexcept
        {
            D3D12MA_HEAVY_ASSERT(m_pItem != nullptr);
            m_pItem = m_pItem->pNext;
            return *this;
        }

        const_iterator& operator--() noexcept
        {
            if(m_pItem)
            {
                m_pItem = m_pItem->pPrev;
            }
            else
            {
                D3D12MA_HEAVY_ASSERT(!m_pList->IsEmpty());
                m_pItem = m_pList->Back();
            }
            return *this;
        }

        [[nodiscard]] const_iterator operator++(int) noexcept
        {
            const_iterator result = *this;
            ++*this;
            return result;
        }

        [[nodiscard]] const_iterator operator--(int) noexcept
        {
            const_iterator result = *this;
            --*this;
            return result;
        }

        [[nodiscard]] bool operator==(const const_iterator& rhs) const noexcept
        {
            D3D12MA_HEAVY_ASSERT(m_pList == rhs.m_pList);
            return m_pItem == rhs.m_pItem;
        }

        [[nodiscard]] bool operator!=(const const_iterator& rhs) const noexcept
        {
            D3D12MA_HEAVY_ASSERT(m_pList == rhs.m_pList);
            return m_pItem != rhs.m_pItem;
        }
    private:
        friend class List<_T>;
    };
private:
    const ALLOCATION_CALLBACKS& m_AllocationCallbacks;
    PoolAllocator<Item> m_ItemAllocator;
    Item* m_pFront;
    Item* m_pBack;
    uSys m_Count;
public:
    // allocationCallbacks externally owned, must outlive this object.
    List(const ALLOCATION_CALLBACKS& allocationCallbacks) noexcept;
    void Clear() noexcept;

    [[nodiscard]] uSys GetCount() const noexcept { return m_Count; }
    [[nodiscard]] bool IsEmpty() const noexcept { return m_Count == 0; }

    [[nodiscard]]       Item* Front()       noexcept { return m_pFront; }
    [[nodiscard]] const Item* Front() const noexcept { return m_pFront; }
    [[nodiscard]]       Item*  Back()       noexcept { return m_pBack;  }
    [[nodiscard]] const Item*  Back() const noexcept { return m_pBack;  }

    Item* PushBack() noexcept;
    Item* PushFront() noexcept;
    Item* PushBack(const _T& value) noexcept;
    Item* PushFront(const _T& value) noexcept;
    void PopBack() noexcept;
    void PopFront() noexcept;

    // Item can be null - it means PushBack.
    Item* InsertBefore(Item* pItem) noexcept;
    // Item can be null - it means PushFront.
    Item* InsertAfter(Item* pItem) noexcept;

    Item* InsertBefore(Item* pItem, const _T& value) noexcept;
    Item* InsertAfter(Item* pItem, const _T& value) noexcept;

    void Remove(Item* pItem) noexcept;

    [[nodiscard]] bool empty() const noexcept { return IsEmpty(); }
    [[nodiscard]] uSys size() const noexcept { return GetCount(); }

    [[nodiscard]] iterator begin() noexcept { return iterator(this, Front()); }
    [[nodiscard]] iterator   end() noexcept { return iterator(this, nullptr); }

    [[nodiscard]] const_iterator cbegin() const noexcept { return const_iterator(this, Front()); }
    [[nodiscard]] const_iterator   cend() const noexcept { return const_iterator(this, nullptr); }

    void clear() noexcept { Clear(); }
    void push_back(const _T& value) noexcept { PushBack(value); }
    void erase(const iterator it) noexcept { Remove(it.m_pItem); }
    iterator insert(const iterator it, const _T& value) noexcept { return iterator(this, InsertBefore(it.m_pItem, value)); }
};

template<typename _T>
List<_T>::List(const ALLOCATION_CALLBACKS& allocationCallbacks) noexcept
    : m_AllocationCallbacks(allocationCallbacks)
    , m_ItemAllocator(allocationCallbacks, 128)
    , m_pFront(nullptr)
    , m_pBack(nullptr)
    , m_Count(0)
{ }

template<typename _T>
void List<_T>::Clear() noexcept
{
    if(!IsEmpty())
    {
        Item* pItem = m_pBack;
        while(pItem)
        {
            Item* const pPrevItem = pItem->pPrev;
            m_ItemAllocator.Free(pItem);
            pItem = pPrevItem;
        }
        m_pFront = nullptr;
        m_pBack = nullptr;
        m_Count = 0;
    }
}

template<typename _T>
typename List<_T>::Item* List<_T>::PushBack() noexcept
{
    Item* const pNewItem = m_ItemAllocator.Alloc();
    pNewItem->pNext = nullptr;
    if(IsEmpty())
    {
        pNewItem->pPrev = nullptr;
        m_pFront = pNewItem;
        m_pBack = pNewItem;
        m_Count = 1;
    }
    else
    {
        pNewItem->pPrev = m_pBack;
        m_pBack->pNext = pNewItem;
        m_pBack = pNewItem;
        ++m_Count;
    }
    return pNewItem;
}

template<typename _T>
typename List<_T>::Item* List<_T>::PushFront() noexcept
{
    Item* const pNewItem = m_ItemAllocator.Alloc();
    pNewItem->pPrev = nullptr;
    if(IsEmpty())
    {
        pNewItem->pNext = nullptr;
        m_pFront = pNewItem;
        m_pBack = pNewItem;
        m_Count = 1;
    }
    else
    {
        pNewItem->pNext = m_pFront;
        m_pFront->pPrev = pNewItem;
        m_pFront = pNewItem;
        ++m_Count;
    }
    return pNewItem;
}

template<typename _T>
typename List<_T>::Item* List<_T>::PushBack(const _T& value) noexcept
{
    Item* const pNewItem = PushBack();
    pNewItem->Value = value;
    return pNewItem;
}

template<typename _T>
typename List<_T>::Item* List<_T>::PushFront(const _T& value) noexcept
{
    Item* const pNewItem = PushFront();
    pNewItem->Value = value;
    return pNewItem;
}

template<typename _T>
void List<_T>::PopBack() noexcept
{
    D3D12MA_HEAVY_ASSERT(m_Count > 0);
    Item* const pBackItem = m_pBack;
    Item* const pPrevItem = pBackItem->pPrev;
    if(pPrevItem)
    {
        pPrevItem->pNext = nullptr;
    }
    m_pBack = pPrevItem;
    m_ItemAllocator.Free(pBackItem);
    --m_Count;
}

template<typename _T>
void List<_T>::PopFront() noexcept
{
    D3D12MA_HEAVY_ASSERT(m_Count > 0);
    Item* const pFrontItem = m_pFront;
    Item* const pNextItem = pFrontItem->pNext;
    if(pNextItem)
    {
        pNextItem->pPrev = nullptr;
    }
    m_pFront = pNextItem;
    m_ItemAllocator.Free(pFrontItem);
    --m_Count;
}

template<typename _T>
void List<_T>::Remove(Item* const pItem) noexcept
{
    D3D12MA_HEAVY_ASSERT(pItem != nullptr);
    D3D12MA_HEAVY_ASSERT(m_Count > 0);

    if(pItem->pPrev)
    {
        pItem->pPrev->pNext = pItem->pNext;
    }
    else
    {
        D3D12MA_HEAVY_ASSERT(m_pFront == pItem);
        m_pFront = pItem->pNext;
    }

    if(pItem->pNext)
    {
        pItem->pNext->pPrev = pItem->pPrev;
    }
    else
    {
        D3D12MA_HEAVY_ASSERT(m_pBack == pItem);
        m_pBack = pItem->pPrev;
    }

    m_ItemAllocator.Free(pItem);
    --m_Count;
}

template<typename _T>
typename List<_T>::Item* List<_T>::InsertBefore(Item* const pItem) noexcept
{
    if(pItem)
    {
        Item* const prevItem = pItem->pPrev;
        Item* const newItem = m_ItemAllocator.Alloc();
        newItem->pPrev = prevItem;
        newItem->pNext = pItem;
        pItem->pPrev = newItem;
        if(prevItem)
        {
            prevItem->pNext = newItem;
        }
        else
        {
            D3D12MA_HEAVY_ASSERT(m_pFront == pItem);
            m_pFront = newItem;
        }
        ++m_Count;
        return newItem;
    }
    else
    { return PushBack(); }
}

template<typename _T>
typename List<_T>::Item* List<_T>::InsertAfter(Item* const pItem) noexcept
{
    if(pItem)
    {
        Item* const nextItem = pItem->pNext;
        Item* const newItem = m_ItemAllocator.Alloc();
        newItem->pNext = nextItem;
        newItem->pPrev = pItem;
        pItem->pNext = newItem;
        if(nextItem)
        {
            nextItem->pPrev = newItem;
        }
        else
        {
            D3D12MA_HEAVY_ASSERT(m_pBack == pItem);
            m_pBack = newItem;
        }
        ++m_Count;
        return newItem;
    }
    else
    { return PushFront(); }
}

template<typename _T>
typename List<_T>::Item* List<_T>::InsertBefore(Item* const pItem, const _T& value) noexcept
{
    Item* const newItem = InsertBefore(pItem);
    newItem->Value = value;
    return newItem;
}

template<typename _T>
typename List<_T>::Item* List<_T>::InsertAfter(Item* const pItem, const _T& value) noexcept
{
    Item* const newItem = InsertAfter(pItem);
    newItem->Value = value;
    return newItem;
}

////////////////////////////////////////////////////////////////////////////////
// Private class BlockMetadata and derived classes - declarations

enum SuballocationType
{
    SUBALLOCATION_TYPE_FREE = 0,
    SUBALLOCATION_TYPE_ALLOCATION = 1,
};

/*
Represents a region of DeviceMemoryBlock that is either assigned and returned as
allocated memory block or free.
*/
struct Suballocation final
{
    UINT64 offset;
    UINT64 size;
    Allocation* allocation;
    SuballocationType type;
};

// Comparator for offsets.
struct SuballocationOffsetLess final
{
    [[nodiscard]] bool operator()(const Suballocation& lhs, const Suballocation& rhs) const noexcept
    { return lhs.offset < rhs.offset; }
};

struct SuballocationOffsetGreater final
{
    [[nodiscard]] bool operator()(const Suballocation& lhs, const Suballocation& rhs) const noexcept
    { return lhs.offset > rhs.offset; }
};

typedef List<Suballocation> SuballocationList;

struct SuballocationItemSizeLess final
{
    [[nodiscard]] bool operator()(const SuballocationList::iterator lhs, const SuballocationList::iterator rhs) const noexcept
    { return lhs->size < rhs->size; }

    [[nodiscard]] bool operator()(const SuballocationList::iterator lhs, const UINT64 rhsSize) const noexcept
    { return lhs->size < rhsSize; }
};

/*
Parameters of planned allocation inside a DeviceMemoryBlock.
*/
struct AllocationRequest final
{
    UINT64 offset;
    UINT64 sumFreeSize; // Sum size of free items that overlap with proposed allocation.
    UINT64 sumItemSize; // Sum size of items to make lost that overlap with proposed allocation.
    SuballocationList::iterator item;
};

/*
Data structure used for bookkeeping of allocations and unused ranges of memory
in a single ID3D12Heap memory block.
*/
class BlockMetadata
{
    DEFAULT_DESTRUCT_VI(BlockMetadata);
    DELETE_CM(BlockMetadata);
protected:
    UINT64 m_Size;
    const ALLOCATION_CALLBACKS* m_pAllocationCallbacks;
public:
    BlockMetadata(const ALLOCATION_CALLBACKS* allocationCallbacks) noexcept;
    
    [[nodiscard]] UINT64 GetSize() const noexcept { return m_Size; }

    [[nodiscard]] virtual uSys GetAllocationCount() const noexcept = 0;
    [[nodiscard]] virtual UINT64 GetSumFreeSize() const noexcept = 0;
    [[nodiscard]] virtual UINT64 GetUnusedRangeSizeMax() const noexcept = 0;

    virtual void Init(const UINT64 size) noexcept { m_Size = size; }

    /**
     *   Validates all data structures inside this object. If not
     * valid, returns false.
     *
     * @return
     *      Whether or not the data object is valid.
     */
    virtual bool Validate() const noexcept = 0;

    /**
     *   Returns true if this block is empty - contains only single
     * free suballocation.
     *
     * @return
     *      Whether or not the block is empty.
     */
    virtual bool IsEmpty() const noexcept = 0;

    // Tries to find a place for suballocation with given parameters inside this block.
    // If succeeded, fills pAllocationRequest and returns true.
    // If failed, returns false.
    virtual bool CreateAllocationRequest(
        UINT64 allocSize,
        UINT64 allocAlignment,
        AllocationRequest* pAllocationRequest) noexcept = 0;

    // Makes actual allocation based on request. Request must already be checked and valid.
    virtual void Alloc(
        const AllocationRequest& request,
        UINT64 allocSize,
        Allocation* Allocation) noexcept = 0;

    // Frees suballocation assigned to given memory region.
    virtual void Free(const Allocation* allocation) noexcept = 0;
    virtual void FreeAtOffset(UINT64 offset) noexcept = 0;
protected:
    [[nodiscard]] const ALLOCATION_CALLBACKS* GetAllocs() const noexcept { return m_pAllocationCallbacks; }
};

class BlockMetadata_Generic : public BlockMetadata
{
    DEFAULT_DESTRUCT_VI(BlockMetadata_Generic);
    DELETE_CM(BlockMetadata_Generic);
private:
    UINT m_FreeCount;
    UINT64 m_SumFreeSize;
    SuballocationList m_Suballocations;
    // Suballocations that are free and have size greater than certain threshold.
    // Sorted by size, ascending.
    Vector<SuballocationList::iterator> m_FreeSuballocationsBySize;
public:
    BlockMetadata_Generic(const ALLOCATION_CALLBACKS* allocationCallbacks) noexcept;

    void Init(UINT64 size) noexcept override;

    bool Validate() const noexcept override;

    uSys GetAllocationCount() const noexcept override { return m_Suballocations.size() - m_FreeCount; }
    UINT64 GetSumFreeSize() const noexcept override { return m_SumFreeSize; }
    UINT64 GetUnusedRangeSizeMax() const noexcept override;
    bool IsEmpty() const noexcept override;

    bool CreateAllocationRequest(
        UINT64 allocSize,
        UINT64 allocAlignment,
        AllocationRequest* pAllocationRequest) noexcept override;

    void Alloc(
        const AllocationRequest& request,
        UINT64 allocSize,
        Allocation* hAllocation) noexcept override;

    void Free(const Allocation* allocation) noexcept override;
    void FreeAtOffset(UINT64 offset) noexcept override;
private:
    bool ValidateFreeSuballocationList() const noexcept;

    /**
     *   Checks if requested suballocation with given parameters
     * can be placed in given pFreeSuballocItem. If yes, fills
     * pOffset and returns true. If no, returns false.
     */
    bool CheckAllocation(
        UINT64 allocSize,
        UINT64 allocAlignment,
        SuballocationList::const_iterator suballocItem,
        UINT64* pOffset,
        UINT64* pSumFreeSize,
        UINT64* pSumItemSize) const noexcept;

    /**
     *   Given free suballocation, it merges it with following
     * one, which must also be free.
     */
    void MergeFreeWithNext(SuballocationList::iterator item) noexcept;

    /**
     * Releases given suballocation, making it free.
     * Merges it with adjacent free suballocations if applicable.
     * Returns iterator to new free suballocation at this place.
     */
    SuballocationList::iterator FreeSuballocation(SuballocationList::iterator suballocItem) noexcept;

    /**
     *   Given free suballocation, it inserts it into sorted list
     * of m_FreeSuballocationsBySize if it's suitable.
     */
    void RegisterFreeSuballocation(SuballocationList::iterator item) noexcept;

    /**
     *   Given free suballocation, it removes it from sorted list
     * of m_FreeSuballocationsBySize if it's suitable.
     */
    void UnregisterFreeSuballocation(SuballocationList::iterator item) noexcept;
};

////////////////////////////////////////////////////////////////////////////////
// Private class DeviceMemoryBlock definition

/**
 *   Represents a single block of device memory (heap) with all
 * the data about its regions (aka suballocations, #Allocation),
 * assigned and free.
 * 
 * Thread-safety: This class must be externally synchronized.
 */
class DeviceMemoryBlock final
{
    DELETE_CM(DeviceMemoryBlock);
private:
    BlockVector* m_BlockVector;
    D3D12_HEAP_TYPE m_HeapType;
    UINT m_Id;
    ID3D12Heap* m_Heap;
public:
    BlockMetadata* m_pMetadata;

    DeviceMemoryBlock() noexcept;

    ~DeviceMemoryBlock() noexcept
    { D3D12MA_ASSERT(m_Heap == nullptr); }

    // Always call after construction.
    void Init(
        AllocatorPimpl* allocator,
        BlockVector* blockVector,
        D3D12_HEAP_TYPE newHeapType,
        ID3D12Heap* newHeap,
        UINT64 newSize,
        UINT id) noexcept;

    // Always call before destruction.
    void Destroy(AllocatorPimpl* allocator) noexcept;

    BlockVector* GetBlockVector() const noexcept { return m_BlockVector; }
    ID3D12Heap* GetHeap() const noexcept { return m_Heap; }
    D3D12_HEAP_TYPE GetHeapType() const noexcept { return m_HeapType; }
    UINT GetId() const noexcept { return m_Id; }

    // Validates all data structures inside this object. If not valid, returns false.
    bool Validate() const;
};

////////////////////////////////////////////////////////////////////////////////
// Private class BlockVector definition

/*
Sequence of DeviceMemoryBlock. Represents memory blocks allocated for a specific
heap type and possibly resource type (if only Tier 1 is supported).

Synchronized internally with a mutex.
*/
class BlockVector
{
    DELETE_CM(BlockVector);
private:
    static UINT64 HeapFlagsToAlignment(D3D12_HEAP_FLAGS flags) noexcept;
private:
    AllocatorPimpl* const m_hAllocator;
    const D3D12_HEAP_TYPE m_HeapType;
    const D3D12_HEAP_FLAGS m_HeapFlags;
    const UINT64 m_PreferredBlockSize;
    const uSys m_MinBlockCount;
    const uSys m_MaxBlockCount;
    const bool m_ExplicitBlockSize;
    /* There can be at most one allocation that is completely empty - a
    hysteresis to avoid pessimistic case of alternating creation and destruction
    of a VkDeviceMemory. */
    bool m_HasEmptyBlock;
    D3D12MA_RW_MUTEX m_Mutex;
    // Incrementally sorted by sumFreeSize, ascending.
    Vector<DeviceMemoryBlock*> m_Blocks;
    UINT m_NextBlockId;
public:
    BlockVector(
        AllocatorPimpl* hAllocator,
        D3D12_HEAP_TYPE heapType,
        D3D12_HEAP_FLAGS heapFlags,
        UINT64 preferredBlockSize,
        uSys minBlockCount,
        uSys maxBlockCount,
        bool explicitBlockSize);

    ~BlockVector();

    HRESULT CreateMinBlocks();

    UINT GetHeapType() const noexcept { return m_HeapType; }
    UINT64 GetPreferredBlockSize() const noexcept { return m_PreferredBlockSize; }

    bool IsEmpty() const noexcept { return m_Blocks.empty(); }

    HRESULT Allocate(
        UINT64 size,
        UINT64 alignment,
        const ALLOCATION_DESC& createInfo,
        uSys allocationCount,
        Allocation** pAllocations) noexcept;

    void Free(Allocation* hAllocation) noexcept;
private:
    UINT64 CalcMaxBlockSize() const noexcept;

    // Finds and removes given block from vector.
    void Remove(DeviceMemoryBlock* pBlock) noexcept;

    // Performs single step in sorting m_Blocks. They may not be fully sorted
    // after this call.
    void IncrementallySortBlocks() noexcept;

    HRESULT AllocatePage(
        UINT64 size,
        UINT64 alignment,
        const ALLOCATION_DESC& createInfo,
        Allocation** pAllocation) noexcept;

    HRESULT AllocateFromBlock(
        DeviceMemoryBlock* pBlock,
        UINT64 size,
        UINT64 alignment,
        ALLOCATION_FLAGS allocFlags,
        Allocation** pAllocation) noexcept;

    HRESULT CreateBlock(UINT64 blockSize, uSys* pNewBlockIndex) noexcept;
    HRESULT CreateD3d12Heap(ID3D12Heap*& outHeap, UINT64 size) const noexcept;
};

////////////////////////////////////////////////////////////////////////////////
// Private class AllocatorPimpl definition

static const UINT DEFAULT_POOL_MAX_COUNT = 9;

class AllocatorPimpl final
{
    DELETE_CM(AllocatorPimpl);
public:
    typedef Vector<Allocation*> AllocationVectorType;
private:
    /**
     * Heuristics that decides whether a resource should better be placed in its own,
     * dedicated allocation (committed resource rather than placed resource).
     */
    static bool PrefersCommittedAllocation(const D3D12_RESOURCE_DESC& resourceDesc) noexcept;
private:
    bool m_UseMutex;
    ID3D12Device* m_Device;
    UINT64 m_PreferredBlockSize;
    ALLOCATION_CALLBACKS m_AllocationCallbacks;

    D3D12_FEATURE_DATA_D3D12_OPTIONS m_D3D12Options;

    AllocationVectorType* m_pCommittedAllocations[HEAP_TYPE_COUNT];
    D3D12MA_RW_MUTEX m_CommittedAllocationsMutex[HEAP_TYPE_COUNT];

    // Default pools.
    BlockVector* m_BlockVectors[DEFAULT_POOL_MAX_COUNT];
public:
    AllocatorPimpl(const ALLOCATION_CALLBACKS& allocationCallbacks, const ALLOCATOR_DESC& desc) noexcept;

    ~AllocatorPimpl() noexcept;

    HRESULT Init() noexcept;

    [[nodiscard]] ID3D12Device* GetDevice() const noexcept { return m_Device; }

    /**
     * Shortcut for "Allocation Callbacks", because this function is called so often.
     */
    [[nodiscard]] const ALLOCATION_CALLBACKS& GetAllocs() const noexcept { return m_AllocationCallbacks; }

    [[nodiscard]] const D3D12_FEATURE_DATA_D3D12_OPTIONS& GetD3D12Options() const noexcept { return m_D3D12Options; }

    [[nodiscard]] bool SupportsResourceHeapTier2() const noexcept { return m_D3D12Options.ResourceHeapTier >= D3D12_RESOURCE_HEAP_TIER_2; }

    [[nodiscard]] bool UseMutex() const noexcept { return m_UseMutex; }

    HRESULT CreateResource(
        const ALLOCATION_DESC* pAllocDesc,
        const D3D12_RESOURCE_DESC* pResourceDesc,
        D3D12_RESOURCE_STATES InitialResourceState,
        const D3D12_CLEAR_VALUE *pOptimizedClearValue,
        Allocation** ppAllocation,
        REFIID riidResource,
        void** ppvResource) noexcept;

    // Unregisters allocation from the collection of dedicated allocations.
    // Allocation object must be deleted externally afterwards.
    void FreeCommittedMemory(Allocation* allocation) noexcept;
    // Unregisters allocation from the collection of placed allocations.
    // Allocation object must be deleted externally afterwards.
    void FreePlacedMemory(Allocation* allocation) noexcept;
private:
    /**
     *   Allocates and registers new committed resource with
     * implicit heap, as dedicated allocation.
     *
     * Creates and returns Allocation objects.
     *
     * @param[in] pAllocDesc
     *      The arguments for the allocator.
     * @param[in] pResourceDesc
     *      The arguments for the resource.
     * @param[in] resAllocInfo
     *      Size and alignment info.
     * @param[in] InitialResourceState
     *      The initial state the resource should be put in.
     * @param[in] pOptimizedClearValue
     *      The value to be used for initially clearing a texture.
     * @param[out] ppAllocation
     *      A pointer to store the allocation info at.
     * @param[in] riidResource
     *      The COM type of the resource.
     * @param[out] ppvResource
     *      A pointer to store the resource at.
     * @return
     *      A success or failure code.
     */
    HRESULT AllocateCommittedMemory(
        const ALLOCATION_DESC* pAllocDesc,
        const D3D12_RESOURCE_DESC* pResourceDesc,
        const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo,
        D3D12_RESOURCE_STATES InitialResourceState,
        const D3D12_CLEAR_VALUE *pOptimizedClearValue,
        Allocation** ppAllocation,
        REFIID riidResource,
        void** ppvResource) noexcept;

    /**
     * If SupportsResourceHeapTier2():
     *     0: D3D12_HEAP_TYPE_DEFAULT
     *     1: D3D12_HEAP_TYPE_UPLOAD
     *     2: D3D12_HEAP_TYPE_READBACK
     * else:
     *     0: D3D12_HEAP_TYPE_DEFAULT + buffer
     *     1: D3D12_HEAP_TYPE_DEFAULT + texture
     *     2: D3D12_HEAP_TYPE_DEFAULT + texture RT or DS
     *     3: D3D12_HEAP_TYPE_UPLOAD + buffer
     *     4: D3D12_HEAP_TYPE_UPLOAD + texture
     *     5: D3D12_HEAP_TYPE_UPLOAD + texture RT or DS
     *     6: D3D12_HEAP_TYPE_READBACK + buffer
     *     7: D3D12_HEAP_TYPE_READBACK + texture
     *     8: D3D12_HEAP_TYPE_READBACK + texture RT or DS
     */
    UINT CalcDefaultPoolCount() const noexcept;
    UINT CalcDefaultPoolIndex(const ALLOCATION_DESC& allocDesc, const D3D12_RESOURCE_DESC& resourceDesc) const noexcept;
    void CalcDefaultPoolParams(D3D12_HEAP_TYPE& outHeapType, D3D12_HEAP_FLAGS& outHeapFlags, UINT index) const noexcept;
private:
    friend class Allocator;
};

////////////////////////////////////////////////////////////////////////////////
// Private class BlockMetadata implementation

BlockMetadata::BlockMetadata(const ALLOCATION_CALLBACKS* allocationCallbacks) noexcept
    : m_Size(0)
    , m_pAllocationCallbacks(allocationCallbacks)
{ D3D12MA_ASSERT(allocationCallbacks); }

////////////////////////////////////////////////////////////////////////////////
// Private class BlockMetadata_Generic implementation

BlockMetadata_Generic::BlockMetadata_Generic(const ALLOCATION_CALLBACKS* const allocationCallbacks) noexcept
    : BlockMetadata(allocationCallbacks)
    , m_FreeCount(0)
    , m_SumFreeSize(0)
    , m_Suballocations(*allocationCallbacks)
    , m_FreeSuballocationsBySize(*allocationCallbacks)
{ D3D12MA_ASSERT(allocationCallbacks); }

void BlockMetadata_Generic::Init(const UINT64 size) noexcept
{
    BlockMetadata::Init(size);

    m_FreeCount = 1;
    m_SumFreeSize = size;

    Suballocation suballoc;
    suballoc.offset = 0;
    suballoc.size = size;
    suballoc.type = SUBALLOCATION_TYPE_FREE;
    suballoc.allocation = nullptr;

    D3D12MA_ASSERT(size > MIN_FREE_SUBALLOCATION_SIZE_TO_REGISTER);
    m_Suballocations.push_back(suballoc);
    SuballocationList::iterator suballocItem = m_Suballocations.end();
    --suballocItem;
    m_FreeSuballocationsBySize.push_back(suballocItem);
}

bool BlockMetadata_Generic::Validate() const noexcept
{
    D3D12MA_VALIDATE(!m_Suballocations.empty());

    // Expected offset of new suballocation as calculated from previous ones.
    UINT64 calculatedOffset = 0;
    // Expected number of free suballocations as calculated from traversing their list.
    UINT calculatedFreeCount = 0;
    // Expected sum size of free suballocations as calculated from traversing their list.
    UINT64 calculatedSumFreeSize = 0;
    // Expected number of free suballocations that should be registered in
    // m_FreeSuballocationsBySize calculated from traversing their list.
    uSys freeSuballocationsToRegister = 0;
    // True if previous visited suballocation was free.
    bool prevFree = false;

    for(SuballocationList::const_iterator suballocItem = m_Suballocations.cbegin();
        suballocItem != m_Suballocations.cend();
        ++suballocItem)
    {
        const Suballocation& subAlloc = *suballocItem;

        // Actual offset of this suballocation doesn't match expected one.
        D3D12MA_VALIDATE(subAlloc.offset == calculatedOffset);

        const bool currFree = (subAlloc.type == SUBALLOCATION_TYPE_FREE);
        // Two adjacent free suballocations are invalid. They should be merged.
        D3D12MA_VALIDATE(!prevFree || !currFree);

        D3D12MA_VALIDATE(currFree == (subAlloc.allocation == nullptr));

        if(currFree)
        {
            calculatedSumFreeSize += subAlloc.size;
            ++calculatedFreeCount;
            if(subAlloc.size >= MIN_FREE_SUBALLOCATION_SIZE_TO_REGISTER)
            {
                ++freeSuballocationsToRegister;
            }

            // Margin required between allocations - every free space must be at least that large.
            D3D12MA_VALIDATE(subAlloc.size >= D3D12MA_DEBUG_MARGIN);
        }
        else
        {
            D3D12MA_VALIDATE(subAlloc.allocation->GetOffset() == subAlloc.offset);
            D3D12MA_VALIDATE(subAlloc.allocation->GetSize() == subAlloc.size);

            // Margin required between allocations - previous allocation must be free.
            // D3D12MA_VALIDATE(D3D12MA_DEBUG_MARGIN == 0 || prevFree);
        }

        calculatedOffset += subAlloc.size;
        prevFree = currFree;
    }

    // Number of free suballocations registered in m_FreeSuballocationsBySize doesn't
    // match expected one.
    D3D12MA_VALIDATE(m_FreeSuballocationsBySize.size() == freeSuballocationsToRegister);

    UINT64 lastSize = 0;
    for(uSys i = 0; i < m_FreeSuballocationsBySize.size(); ++i)
    {
        const SuballocationList::iterator suballocItem = m_FreeSuballocationsBySize[i];

        // Only free suballocations can be registered in m_FreeSuballocationsBySize.
        D3D12MA_VALIDATE(suballocItem->type == SUBALLOCATION_TYPE_FREE);
        // They must be sorted by size ascending.
        D3D12MA_VALIDATE(suballocItem->size >= lastSize);

        lastSize = suballocItem->size;
    }

    // Check if totals match calculated values.
    D3D12MA_VALIDATE(ValidateFreeSuballocationList());
    D3D12MA_VALIDATE(calculatedOffset == GetSize());
    D3D12MA_VALIDATE(calculatedSumFreeSize == m_SumFreeSize);
    D3D12MA_VALIDATE(calculatedFreeCount == m_FreeCount);

    return true;
}

UINT64 BlockMetadata_Generic::GetUnusedRangeSizeMax() const noexcept
{
    if(!m_FreeSuballocationsBySize.empty())
    { return m_FreeSuballocationsBySize.back()->size; }
    else
    { return 0; }
}

bool BlockMetadata_Generic::IsEmpty() const noexcept
{
    return (m_Suballocations.size() == 1) && (m_FreeCount == 1);
}

bool BlockMetadata_Generic::CreateAllocationRequest(
    const UINT64 allocSize,
    const UINT64 allocAlignment,
    AllocationRequest* const pAllocationRequest) noexcept
{
    D3D12MA_ASSERT(allocSize > 0);
    D3D12MA_ASSERT(pAllocationRequest != nullptr);
    D3D12MA_HEAVY_ASSERT(Validate());

    // There is not enough total free space in this block to fulfill the request: Early return.
    if(m_SumFreeSize < allocSize + 2 * D3D12MA_DEBUG_MARGIN)
    { return false; }

    // New algorithm, efficiently searching freeSuballocationsBySize.
    const uSys freeSuballocCount = m_FreeSuballocationsBySize.size();
    if(freeSuballocCount > 0)
    {
        // Find first free suballocation with size not less than allocSize + 2 * D3D12MA_DEBUG_MARGIN.
        SuballocationList::iterator* const it = BinaryFindFirstNotLess(
            m_FreeSuballocationsBySize.data(),
            m_FreeSuballocationsBySize.data() + freeSuballocCount,
            allocSize + 2 * D3D12MA_DEBUG_MARGIN,
            SuballocationItemSizeLess());

        for(uSys index = it - m_FreeSuballocationsBySize.data(); index < freeSuballocCount; ++index)
        {
            if(CheckAllocation(
                allocSize,
                allocAlignment,
                m_FreeSuballocationsBySize[index],
                &pAllocationRequest->offset,
                &pAllocationRequest->sumFreeSize,
                &pAllocationRequest->sumItemSize))
            {
                pAllocationRequest->item = m_FreeSuballocationsBySize[index];
                return true;
            }
        }
    }

    return false;
}

void BlockMetadata_Generic::Alloc(
    const AllocationRequest& request,
    const UINT64 allocSize,
    Allocation* const allocation) noexcept
{
    D3D12MA_ASSERT(request.item != m_Suballocations.end());
    Suballocation& suballoc = *request.item;
    // Given suballocation is a free block.
    D3D12MA_ASSERT(suballoc.type == SUBALLOCATION_TYPE_FREE);
    // Given offset is inside this suballocation.
    D3D12MA_ASSERT(request.offset >= suballoc.offset);
    const UINT64 paddingBegin = request.offset - suballoc.offset;
    D3D12MA_ASSERT(suballoc.size >= paddingBegin + allocSize);
    const UINT64 paddingEnd = suballoc.size - paddingBegin - allocSize;

    // Unregister this free suballocation from m_FreeSuballocationsBySize and update
    // it to become used.
    UnregisterFreeSuballocation(request.item);

    suballoc.offset = request.offset;
    suballoc.size = allocSize;
    suballoc.type = SUBALLOCATION_TYPE_ALLOCATION;
    suballoc.allocation = allocation;

    // If there are any free bytes remaining at the end, insert new free suballocation after current one.
    if(paddingEnd)
    {
        Suballocation paddingSuballoc = { };
        paddingSuballoc.offset = request.offset + allocSize;
        paddingSuballoc.size = paddingEnd;
        paddingSuballoc.type = SUBALLOCATION_TYPE_FREE;
        SuballocationList::iterator next = request.item;
        ++next;
        const SuballocationList::iterator paddingEndItem =
            m_Suballocations.insert(next, paddingSuballoc);
        RegisterFreeSuballocation(paddingEndItem);
    }

    // If there are any free bytes remaining at the beginning, insert new free suballocation before current one.
    if(paddingBegin)
    {
        Suballocation paddingSuballoc = { };
        paddingSuballoc.offset = request.offset - paddingBegin;
        paddingSuballoc.size = paddingBegin;
        paddingSuballoc.type = SUBALLOCATION_TYPE_FREE;
        const SuballocationList::iterator paddingBeginItem =
            m_Suballocations.insert(request.item, paddingSuballoc);
        RegisterFreeSuballocation(paddingBeginItem);
    }

    // Update totals.
    m_FreeCount = m_FreeCount - 1;
    if(paddingBegin > 0)
    { ++m_FreeCount; }
    if(paddingEnd > 0)
    {  ++m_FreeCount; }
    m_SumFreeSize -= allocSize;
}

void BlockMetadata_Generic::Free(const Allocation* const allocation) noexcept
{
    for(SuballocationList::iterator suballocItem = m_Suballocations.begin();
        suballocItem != m_Suballocations.end();
        ++suballocItem)
    {
        Suballocation& suballoc = *suballocItem;
        if(suballoc.allocation == allocation)
        {
            FreeSuballocation(suballocItem);
            D3D12MA_HEAVY_ASSERT(Validate());
            return;
        }
    }

    D3D12MA_ASSERT(0 && "Not found!");
}

void BlockMetadata_Generic::FreeAtOffset(const UINT64 offset) noexcept
{
    for(SuballocationList::iterator suballocItem = m_Suballocations.begin();
        suballocItem != m_Suballocations.end();
        ++suballocItem)
    {
        Suballocation& suballoc = *suballocItem;
        if(suballoc.offset == offset)
        {
            FreeSuballocation(suballocItem);
            return;
        }
    }

    D3D12MA_ASSERT(0 && "Not found!");
}

bool BlockMetadata_Generic::ValidateFreeSuballocationList() const noexcept
{
    UINT64 lastSize = 0;
    for(uSys i = 0, count = m_FreeSuballocationsBySize.size(); i < count; ++i)
    {
        const SuballocationList::iterator it = m_FreeSuballocationsBySize[i];

        D3D12MA_VALIDATE(it->type == SUBALLOCATION_TYPE_FREE);
        D3D12MA_VALIDATE(it->size >= MIN_FREE_SUBALLOCATION_SIZE_TO_REGISTER);
        D3D12MA_VALIDATE(it->size >= lastSize);
        lastSize = it->size;
    }
    return true;
}

bool BlockMetadata_Generic::CheckAllocation(
    const UINT64 allocSize,
    const UINT64 allocAlignment,
    const SuballocationList::const_iterator suballocItem,
    UINT64* const pOffset,
    UINT64* const pSumFreeSize,
    UINT64* const pSumItemSize) const noexcept
{
    D3D12MA_ASSERT(allocSize > 0);
    D3D12MA_ASSERT(suballocItem != m_Suballocations.cend());
    D3D12MA_ASSERT(pOffset != nullptr);

    *pSumFreeSize = 0;
    *pSumItemSize = 0;

    const Suballocation& suballoc = *suballocItem;
    D3D12MA_ASSERT(suballoc.type == SUBALLOCATION_TYPE_FREE);

    *pSumFreeSize = suballoc.size;

    // Size of this suballocation is too small for this request: Early return.
    if(suballoc.size < allocSize)
    { return false; }

    // Start from offset equal to beginning of this suballocation.
    *pOffset = suballoc.offset;

    // Apply D3D12MA_DEBUG_MARGIN at the beginning.
    if constexpr(D3D12MA_DEBUG_MARGIN > 0)
    { *pOffset += D3D12MA_DEBUG_MARGIN; }

    // Apply alignment.
    *pOffset = AlignUp(*pOffset, allocAlignment);

    // Calculate padding at the beginning based on current offset.
    const UINT64 paddingBegin = *pOffset - suballoc.offset;

    // Calculate required margin at the end.
    const UINT64 requiredEndMargin = D3D12MA_DEBUG_MARGIN;

    // Fail if requested size plus margin before and after is bigger than size of this suballocation.
    if(paddingBegin + allocSize + requiredEndMargin > suballoc.size)
    { return false; }

    // All tests passed: Success. pOffset is already filled.
    return true;
}

void BlockMetadata_Generic::MergeFreeWithNext(const SuballocationList::iterator item) noexcept
{
    D3D12MA_ASSERT(item != m_Suballocations.end());
    D3D12MA_ASSERT(item->type == SUBALLOCATION_TYPE_FREE);

    SuballocationList::iterator nextItem = item;
    ++nextItem;
    D3D12MA_ASSERT(nextItem != m_Suballocations.end());
    D3D12MA_ASSERT(nextItem->type == SUBALLOCATION_TYPE_FREE);

    item->size += nextItem->size;
    --m_FreeCount;
    m_Suballocations.erase(nextItem);
}

SuballocationList::iterator BlockMetadata_Generic::FreeSuballocation(const SuballocationList::iterator suballocItem) noexcept
{
    // Change this suballocation to be marked as free.
    Suballocation& suballoc = *suballocItem;
    suballoc.type = SUBALLOCATION_TYPE_FREE;
    suballoc.allocation = nullptr;

    // Update totals.
    ++m_FreeCount;
    m_SumFreeSize += suballoc.size;

    // Merge with previous and/or next suballocation if it's also free.
    bool mergeWithNext = false;
    bool mergeWithPrev = false;

    SuballocationList::iterator nextItem = suballocItem;
    ++nextItem;
    if((nextItem != m_Suballocations.end()) && (nextItem->type == SUBALLOCATION_TYPE_FREE))
    {
        mergeWithNext = true;
    }

    SuballocationList::iterator prevItem = suballocItem;
    if(suballocItem != m_Suballocations.begin())
    {
        --prevItem;
        if(prevItem->type == SUBALLOCATION_TYPE_FREE)
        { mergeWithPrev = true; }
    }

    if(mergeWithNext)
    {
        UnregisterFreeSuballocation(nextItem);
        MergeFreeWithNext(suballocItem);
    }

    if(mergeWithPrev)
    {
        UnregisterFreeSuballocation(prevItem);
        MergeFreeWithNext(prevItem);
        RegisterFreeSuballocation(prevItem);
        return prevItem;
    }
    else
    {
        RegisterFreeSuballocation(suballocItem);
        return suballocItem;
    }
}

void BlockMetadata_Generic::RegisterFreeSuballocation(const SuballocationList::iterator item) noexcept
{
    D3D12MA_ASSERT(item->type == SUBALLOCATION_TYPE_FREE);
    D3D12MA_ASSERT(item->size > 0);

    // You may want to enable this validation at the beginning or at the end of
    // this function, depending on what do you want to check.
    D3D12MA_HEAVY_ASSERT(ValidateFreeSuballocationList());

    if(item->size >= MIN_FREE_SUBALLOCATION_SIZE_TO_REGISTER)
    {
        if(m_FreeSuballocationsBySize.empty())
        {
            m_FreeSuballocationsBySize.push_back(item);
        }
        else
        {
            m_FreeSuballocationsBySize.InsertSorted(item, SuballocationItemSizeLess());
        }
    }
}


void BlockMetadata_Generic::UnregisterFreeSuballocation(const SuballocationList::iterator item) noexcept
{
    D3D12MA_ASSERT(item->type == SUBALLOCATION_TYPE_FREE);
    D3D12MA_ASSERT(item->size > 0);

    // You may want to enable this validation at the beginning or at the end of
    // this function, depending on what do you want to check.
    D3D12MA_HEAVY_ASSERT(ValidateFreeSuballocationList());

    if(item->size >= MIN_FREE_SUBALLOCATION_SIZE_TO_REGISTER)
    {
        SuballocationList::iterator* const it = BinaryFindFirstNotLess(
            m_FreeSuballocationsBySize.data(),
            m_FreeSuballocationsBySize.data() + m_FreeSuballocationsBySize.size(),
            item,
            SuballocationItemSizeLess());
        for(uSys index = it - m_FreeSuballocationsBySize.data();
            index < m_FreeSuballocationsBySize.size();
            ++index)
        {
            if(m_FreeSuballocationsBySize[index] == item)
            {
                m_FreeSuballocationsBySize.remove(index);
                return;
            }
            D3D12MA_ASSERT((m_FreeSuballocationsBySize[index]->size == item->size) && "Not found.");
        }
        D3D12MA_ASSERT(0 && "Not found.");
    }

    //D3D12MA_HEAVY_ASSERT(ValidateFreeSuballocationList());
}

////////////////////////////////////////////////////////////////////////////////
// Private class DeviceMemoryBlock implementation

DeviceMemoryBlock::DeviceMemoryBlock() noexcept
    : m_BlockVector(nullptr)
    , m_HeapType(D3D12_HEAP_TYPE_CUSTOM)
    , m_Id(0)
    , m_Heap(nullptr)
    , m_pMetadata(nullptr)
{ }

void DeviceMemoryBlock::Init(
    AllocatorPimpl* const allocator,
    BlockVector* const blockVector,
    const D3D12_HEAP_TYPE newHeapType,
    ID3D12Heap* const newHeap,
    const UINT64 newSize,
    const UINT id) noexcept
{
    D3D12MA_ASSERT(m_Heap == nullptr);

    m_BlockVector = blockVector;
    m_HeapType = newHeapType;
    m_Id = id;
    m_Heap = newHeap;

    const ALLOCATION_CALLBACKS& allocs = allocator->GetAllocs();

    m_pMetadata = D3D12MA_NEW(allocs, BlockMetadata_Generic)(&allocs);
    m_pMetadata->Init(newSize);
}

void DeviceMemoryBlock::Destroy(AllocatorPimpl* const allocator) noexcept
{
    // THIS IS THE MOST IMPORTANT ASSERT IN THE ENTIRE LIBRARY!
    // Hitting it means you have some memory leak - unreleased Allocation objects.
    D3D12MA_ASSERT(m_pMetadata->IsEmpty() && "Some allocations were not freed before destruction of this memory block!");

    D3D12MA_ASSERT(m_Heap != nullptr);
    m_Heap->Release();
    m_Heap = nullptr;

    D3D12MA_DELETE(allocator->GetAllocs(), m_pMetadata);
    m_pMetadata = nullptr;
}

bool DeviceMemoryBlock::Validate() const
{
    D3D12MA_VALIDATE(m_Heap && m_pMetadata && m_pMetadata->GetSize() != 0);
    return m_pMetadata->Validate();
}

////////////////////////////////////////////////////////////////////////////////
// Private class BlockVector implementation

BlockVector::BlockVector(
    AllocatorPimpl* const hAllocator,
    const D3D12_HEAP_TYPE heapType,
    const D3D12_HEAP_FLAGS heapFlags,
    const UINT64 preferredBlockSize,
    const uSys minBlockCount,
    const uSys maxBlockCount,
    const bool explicitBlockSize)
    : m_hAllocator(hAllocator)
    , m_HeapType(heapType)
    , m_HeapFlags(heapFlags)
    , m_PreferredBlockSize(preferredBlockSize)
    , m_MinBlockCount(minBlockCount)
    , m_MaxBlockCount(maxBlockCount)
    , m_ExplicitBlockSize(explicitBlockSize)
    , m_HasEmptyBlock(false)
    , m_Blocks(hAllocator->GetAllocs())
    , m_NextBlockId(0)
{ }

BlockVector::~BlockVector()
{
    for(uSys i = m_Blocks.size() - 1; i > 0; --i)
    {
        m_Blocks[i]->Destroy(m_hAllocator);
        D3D12MA_DELETE(m_hAllocator->GetAllocs(), m_Blocks[i]);
    }
}

HRESULT BlockVector::CreateMinBlocks()
{
    for(uSys i = 0; i < m_MinBlockCount; ++i)
    {
        const HRESULT hr = CreateBlock(m_PreferredBlockSize, nullptr);
        if(FAILED(hr))
        { return hr; }
    }
    return S_OK;
}

HRESULT BlockVector::Allocate(
    const UINT64 size,
    const UINT64 alignment,
    const ALLOCATION_DESC& createInfo,
    const uSys allocationCount,
    Allocation** const pAllocations) noexcept
{
    uSys allocIndex;
    HRESULT hr = S_OK;

    {
        MutexLockWrite lock(m_Mutex, m_hAllocator->UseMutex());
        for(allocIndex = 0; allocIndex < allocationCount; ++allocIndex)
        {
            hr = AllocatePage(
                size,
                alignment,
                createInfo,
                pAllocations + allocIndex);
            if(FAILED(hr))
            { break; }
        }
    }

    if(FAILED(hr))
    {
        // Free all already created allocations.
        while(allocIndex--)
        {
            Free(pAllocations[allocIndex]);
        }
        ::std::memset(pAllocations, 0, sizeof(Allocation*) * allocationCount);
    }

    return hr;
}

HRESULT BlockVector::AllocatePage(
    const UINT64 size,
    const UINT64 alignment,
    const ALLOCATION_DESC& createInfo,
    Allocation** const pAllocation) noexcept
{
    // Early reject: requested allocation size is larger that maximum block size for this block vector.
    if(size + 2 * D3D12MA_DEBUG_MARGIN > m_PreferredBlockSize)
    { return E_OUTOFMEMORY; }

    const bool canCreateNewBlock =
        ((createInfo.Flags & ALLOCATION_FLAG_NEVER_ALLOCATE) == 0) &&
        (m_Blocks.size() < m_MaxBlockCount);

    if(canCreateNewBlock)
    {
        // 1. Search existing allocations. Try to allocate without making other allocations lost.
        const ALLOCATION_FLAGS allocFlagsCopy = createInfo.Flags;

        // Forward order in m_Blocks - prefer blocks with smallest amount of free space.
        for(uSys blockIndex = 0; blockIndex < m_Blocks.size(); ++blockIndex )
        {
            DeviceMemoryBlock* const pCurrBlock = m_Blocks[blockIndex];
            D3D12MA_ASSERT(pCurrBlock);
            const HRESULT hr = AllocateFromBlock(
                pCurrBlock,
                size,
                alignment,
                allocFlagsCopy,
                pAllocation);
            if(SUCCEEDED(hr))
            { return hr; }
        }

        // 2. Try to create new block.
            
        // Calculate optimal size for new block.
        UINT64 newBlockSize = m_PreferredBlockSize;
        UINT newBlockSizeShift = 0;
        const UINT NEW_BLOCK_SIZE_SHIFT_MAX = 3;

        if(!m_ExplicitBlockSize)
        {
            // Allocate 1/8, 1/4, 1/2 as first blocks.
            const UINT64 maxExistingBlockSize = CalcMaxBlockSize();
            for(UINT i = 0; i < NEW_BLOCK_SIZE_SHIFT_MAX; ++i)
            {
                const UINT64 smallerNewBlockSize = newBlockSize / 2;
                if(smallerNewBlockSize > maxExistingBlockSize && smallerNewBlockSize >= size * 2)
                {
                    newBlockSize = smallerNewBlockSize;
                    ++newBlockSizeShift;
                }
                else
                { break; }
            }
        }

        uSys newBlockIndex = 0;
        HRESULT hr = CreateBlock(newBlockSize, &newBlockIndex);
        // Allocation of this size failed? Try 1/2, 1/4, 1/8 of m_PreferredBlockSize.
        if(!m_ExplicitBlockSize)
        {
            while(FAILED(hr) && newBlockSizeShift < NEW_BLOCK_SIZE_SHIFT_MAX)
            {
                const UINT64 smallerNewBlockSize = newBlockSize / 2;
                if(smallerNewBlockSize >= size)
                {
                    newBlockSize = smallerNewBlockSize;
                    ++newBlockSizeShift;
                    hr = CreateBlock(newBlockSize, &newBlockIndex);
                }
                else
                { break; }
            }
        }

        if(SUCCEEDED(hr))
        {
            DeviceMemoryBlock* const pBlock = m_Blocks[newBlockIndex];
            D3D12MA_ASSERT(pBlock->m_pMetadata->GetSize() >= size);

            hr = AllocateFromBlock(
                pBlock,
                size,
                alignment,
                allocFlagsCopy,
                pAllocation);
            if(SUCCEEDED(hr))
            { return hr; }
            else
            {
                // Allocation from new block failed, possibly due to D3D12MA_DEBUG_MARGIN or alignment.
                return E_OUTOFMEMORY;
            }
        }
    }

    return E_OUTOFMEMORY;
}

void BlockVector::Free(Allocation* const hAllocation) noexcept
{
    DeviceMemoryBlock* pBlockToDelete = nullptr;

    // Scope for lock.
    {
        MutexLockWrite lock(m_Mutex, m_hAllocator->UseMutex());

        DeviceMemoryBlock* const pBlock = hAllocation->GetBlock();

        pBlock->m_pMetadata->Free(hAllocation);
        D3D12MA_HEAVY_ASSERT(pBlock->Validate());

        // pBlock became empty after this deallocation.
        if(pBlock->m_pMetadata->IsEmpty())
        {
            // Already has empty Allocation. We don't want to have two, so delete this one.
            if(m_HasEmptyBlock && m_Blocks.size() > m_MinBlockCount)
            {
                pBlockToDelete = pBlock;
                Remove(pBlock);
            }
            // We now have first empty block.
            else
            {
                m_HasEmptyBlock = true;
            }
        }
        // pBlock didn't become empty, but we have another empty block - find and free that one.
        // (This is optional, heuristics.)
        else if(m_HasEmptyBlock)
        {
            DeviceMemoryBlock* pLastBlock = m_Blocks.back();
            if(pLastBlock->m_pMetadata->IsEmpty() && m_Blocks.size() > m_MinBlockCount)
            {
                pBlockToDelete = pLastBlock;
                m_Blocks.pop_back();
                m_HasEmptyBlock = false;
            }
        }

        IncrementallySortBlocks();
    }

    // Destruction of a free Allocation. Deferred until this point, outside of mutex
    // lock, for performance reason.
    if(pBlockToDelete)
    {
        pBlockToDelete->Destroy(m_hAllocator);
        D3D12MA_DELETE(m_hAllocator->GetAllocs(), pBlockToDelete);
    }
}

UINT64 BlockVector::HeapFlagsToAlignment(const D3D12_HEAP_FLAGS flags) noexcept
{
    /*
    Documentation of D3D12_HEAP_DESC structure says:

    - D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT   defined as 64KB.
    - D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT   defined as 4MB. An
      application must decide whether the heap will contain multi-sample
      anti-aliasing (MSAA), in which case, the application must choose [this flag].

    https://docs.microsoft.com/en-us/windows/desktop/api/d3d12/ns-d3d12-d3d12_heap_desc
    */

    const D3D12_HEAP_FLAGS denyAllTexturesFlags =
        D3D12_HEAP_FLAG_DENY_NON_RT_DS_TEXTURES | D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES;
    const bool canContainAnyTextures =
        (flags & denyAllTexturesFlags) != denyAllTexturesFlags;
    return canContainAnyTextures ?
        D3D12_DEFAULT_MSAA_RESOURCE_PLACEMENT_ALIGNMENT : D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
}

UINT64 BlockVector::CalcMaxBlockSize() const noexcept
{
    UINT64 result = 0;
    for(uSys i = m_Blocks.size(); i--; )
    {
        result = D3D12MA_MAX(result, m_Blocks[i]->m_pMetadata->GetSize());
        if(result >= m_PreferredBlockSize)
        {
            break;
        }
    }
    return result;
}

void BlockVector::Remove(DeviceMemoryBlock* const pBlock) noexcept
{
    for(UINT blockIndex = 0; blockIndex < m_Blocks.size(); ++blockIndex)
    {
        if(m_Blocks[blockIndex] == pBlock)
        {
            m_Blocks.remove(blockIndex);
            return;
        }
    }
    D3D12MA_ASSERT(0);
}

void BlockVector::IncrementallySortBlocks() noexcept
{
    // Bubble sort only until first swap.
    for(uSys i = 1; i < m_Blocks.size(); ++i)
    {
        if(m_Blocks[i - 1]->m_pMetadata->GetSumFreeSize() > m_Blocks[i]->m_pMetadata->GetSumFreeSize())
        {
            D3D12MA_SWAP(m_Blocks[i - 1], m_Blocks[i]);
            return;
        }
    }
}

HRESULT BlockVector::AllocateFromBlock(
    DeviceMemoryBlock* const pBlock,
    const UINT64 size,
    const UINT64 alignment,
    const ALLOCATION_FLAGS allocFlags,
    Allocation** const pAllocation) noexcept
{
    AllocationRequest currRequest = {};
    if(pBlock->m_pMetadata->CreateAllocationRequest(
        size,
        alignment,
        &currRequest))
    {
        // We no longer have an empty Allocation.
        if(pBlock->m_pMetadata->IsEmpty())
        {
            m_HasEmptyBlock = false;
        }

        *pAllocation = D3D12MA_NEW(m_hAllocator->GetAllocs(), Allocation)();
        pBlock->m_pMetadata->Alloc(currRequest, size, *pAllocation);
        (*pAllocation)->InitPlaced(
            m_hAllocator,
            size,
            currRequest.offset,
            alignment,
            pBlock);
        D3D12MA_HEAVY_ASSERT(pBlock->Validate());
        return S_OK;
    }
    return E_OUTOFMEMORY;
}

HRESULT BlockVector::CreateBlock(const UINT64 blockSize, uSys* const pNewBlockIndex) noexcept
{
    ID3D12Heap* heap = nullptr;
    const HRESULT hr = CreateD3d12Heap(heap, blockSize);
    if(FAILED(hr))
    { return hr; }

    DeviceMemoryBlock* const pBlock = D3D12MA_NEW(m_hAllocator->GetAllocs(), DeviceMemoryBlock)();
    pBlock->Init(
        m_hAllocator,
        this,
        m_HeapType,
        heap,
        blockSize,
        m_NextBlockId++);

    m_Blocks.push_back(pBlock);
    if(pNewBlockIndex)
    { *pNewBlockIndex = m_Blocks.size() - 1; }

    return hr;
}

HRESULT BlockVector::CreateD3d12Heap(ID3D12Heap*& outHeap, const UINT64 size) const noexcept
{
    D3D12_HEAP_DESC heapDesc = { };
    heapDesc.SizeInBytes = size;
    heapDesc.Properties.Type = m_HeapType;
    heapDesc.Alignment = HeapFlagsToAlignment(m_HeapFlags);
    heapDesc.Flags = m_HeapFlags;

    return m_hAllocator->GetDevice()->CreateHeap(&heapDesc, IID_PPV_ARGS(&outHeap));
}

////////////////////////////////////////////////////////////////////////////////
// Private class AllocatorPimpl implementation

AllocatorPimpl::AllocatorPimpl(const ALLOCATION_CALLBACKS& allocationCallbacks, const ALLOCATOR_DESC& desc) noexcept
    : m_UseMutex((desc.Flags & ALLOCATOR_FLAG_SINGLETHREADED) == 0)
    , m_Device(desc.pDevice)
    , m_PreferredBlockSize(desc.PreferredBlockSize != 0 ? desc.PreferredBlockSize : D3D12MA_DEFAULT_BLOCK_SIZE)
    , m_AllocationCallbacks(allocationCallbacks)
{
    // desc.pAllocationCallbacks intentionally ignored here, preprocessed by CreateAllocator.
    ::std::memset(&m_D3D12Options, 0, sizeof(m_D3D12Options));

    ::std::memset(m_pCommittedAllocations, 0, sizeof(m_pCommittedAllocations));
    ::std::memset(m_BlockVectors, 0, sizeof(m_BlockVectors));

    for(UINT heapTypeIndex = 0; heapTypeIndex < HEAP_TYPE_COUNT; ++heapTypeIndex)
    {
        m_pCommittedAllocations[heapTypeIndex] = D3D12MA_NEW(GetAllocs(), AllocationVectorType)(GetAllocs());
    }
}

AllocatorPimpl::~AllocatorPimpl() noexcept
{
    for(UINT i = DEFAULT_POOL_MAX_COUNT - 1; i > 0; --i)
    {
        D3D12MA_DELETE(GetAllocs(), m_BlockVectors[i]);
    }

    for(UINT i = HEAP_TYPE_COUNT - 1; i > 0; --i)
    {
        if(m_pCommittedAllocations[i] && !m_pCommittedAllocations[i]->empty())
        {
            D3D12MA_ASSERT(0 && "Unfreed committed allocations found.");
        }

        D3D12MA_DELETE(GetAllocs(), m_pCommittedAllocations[i]);
    }
}

HRESULT AllocatorPimpl::Init() noexcept
{
    const HRESULT hr = m_Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &m_D3D12Options, sizeof(m_D3D12Options));
    if(FAILED(hr))
    { return hr; }

    const UINT defaultPoolCount = CalcDefaultPoolCount();
    for(UINT i = 0; i < defaultPoolCount; ++i)
    {
        D3D12_HEAP_TYPE heapType;
        D3D12_HEAP_FLAGS heapFlags;
        CalcDefaultPoolParams(heapType, heapFlags, i);

        m_BlockVectors[i] = D3D12MA_NEW(GetAllocs(), BlockVector)(
            this, // hAllocator
            heapType, // heapType
            heapFlags, // heapFlags
            m_PreferredBlockSize,
            0, // minBlockCount
            SIZE_MAX, // maxBlockCount
            false); // explicitBlockSize
        // No need to call m_pBlockVectors[i]->CreateMinBlocks here, because minBlockCount is 0.
    }

    return S_OK;
}

HRESULT AllocatorPimpl::CreateResource(
    const ALLOCATION_DESC* pAllocDesc,
    const D3D12_RESOURCE_DESC* pResourceDesc,
    D3D12_RESOURCE_STATES InitialResourceState,
    const D3D12_CLEAR_VALUE *pOptimizedClearValue,
    Allocation** ppAllocation,
    REFIID riidResource,
    void** ppvResource) noexcept
{
    if(pAllocDesc->HeapType != D3D12_HEAP_TYPE_DEFAULT &&
       pAllocDesc->HeapType != D3D12_HEAP_TYPE_UPLOAD &&
       pAllocDesc->HeapType != D3D12_HEAP_TYPE_READBACK)
    { return E_INVALIDARG; }

    ALLOCATION_DESC finalAllocDesc = *pAllocDesc;

    *ppvResource = nullptr;

    D3D12_RESOURCE_ALLOCATION_INFO resAllocInfo = m_Device->GetResourceAllocationInfo(0, 1, pResourceDesc);
    resAllocInfo.Alignment = D3D12MA_MAX<UINT64>(resAllocInfo.Alignment, D3D12MA_DEBUG_ALIGNMENT);
    D3D12MA_ASSERT(IsPow2(resAllocInfo.Alignment));
    D3D12MA_ASSERT(resAllocInfo.SizeInBytes > 0);

    const UINT defaultPoolIndex = CalcDefaultPoolIndex(*pAllocDesc, *pResourceDesc);
    BlockVector* blockVector = m_BlockVectors[defaultPoolIndex];
    D3D12MA_ASSERT(blockVector);

    const UINT64 preferredBlockSize = blockVector->GetPreferredBlockSize();
    const bool preferCommittedMemory =
        D3D12MA_DEBUG_ALWAYS_COMMITTED ||
        PrefersCommittedAllocation(*pResourceDesc) ||
        // Heuristics: Allocate committed memory if requested size if greater than half of preferred block size.
        resAllocInfo.SizeInBytes > preferredBlockSize / 2;
    if(preferCommittedMemory &&
        (finalAllocDesc.Flags & ALLOCATION_FLAG_NEVER_ALLOCATE) == 0)
    {
        finalAllocDesc.Flags |= ALLOCATION_FLAG_COMMITTED;
    }

    if((finalAllocDesc.Flags & ALLOCATION_FLAG_COMMITTED) != 0)
    {
        return AllocateCommittedMemory(
            &finalAllocDesc,
            pResourceDesc,
            resAllocInfo,
            InitialResourceState,
            pOptimizedClearValue,
            ppAllocation,
            riidResource,
            ppvResource);
    }
    else
    {
        HRESULT hr = blockVector->Allocate(
            resAllocInfo.SizeInBytes,
            resAllocInfo.Alignment,
            finalAllocDesc,
            1,
            ppAllocation);
        if(SUCCEEDED(hr))
        {
            hr = m_Device->CreatePlacedResource(
                (*ppAllocation)->GetBlock()->GetHeap(),
                (*ppAllocation)->GetOffset(),
                pResourceDesc,
                InitialResourceState,
                pOptimizedClearValue,
                riidResource,
                ppvResource);
            if(SUCCEEDED(hr))
            {
                return hr;
            }
            else
            {
                (*ppAllocation)->Release();
                return hr;
            }
        }

        return AllocateCommittedMemory(
            &finalAllocDesc,
            pResourceDesc,
            resAllocInfo,
            InitialResourceState,
            pOptimizedClearValue,
            ppAllocation,
            riidResource,
            ppvResource);
    }
}

bool AllocatorPimpl::PrefersCommittedAllocation(const D3D12_RESOURCE_DESC&) noexcept
{
    // Intentional. It may change in the future.
    return false;
}

HRESULT AllocatorPimpl::AllocateCommittedMemory(
    const ALLOCATION_DESC* const pAllocDesc,
    const D3D12_RESOURCE_DESC* const pResourceDesc,
    const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo,
    const D3D12_RESOURCE_STATES InitialResourceState,
    const D3D12_CLEAR_VALUE* const pOptimizedClearValue,
    Allocation** const ppAllocation,
    REFIID riidResource,
    void** const ppvResource) noexcept
{
    if((pAllocDesc->Flags & ALLOCATION_FLAG_NEVER_ALLOCATE) != 0)
    { return E_OUTOFMEMORY; }

    D3D12_HEAP_PROPERTIES heapProps = { };
    heapProps.Type = pAllocDesc->HeapType;
    const HRESULT hr = m_Device->CreateCommittedResource(
        &heapProps, D3D12_HEAP_FLAG_NONE, pResourceDesc, InitialResourceState,
        pOptimizedClearValue, riidResource, ppvResource);

    if(SUCCEEDED(hr))
    {
        Allocation* const alloc = D3D12MA_NEW(m_AllocationCallbacks, Allocation)();
        alloc->InitCommitted(this, resAllocInfo.SizeInBytes, pAllocDesc->HeapType);
        *ppAllocation = alloc;

        const UINT heapTypeIndex = HeapTypeToIndex(pAllocDesc->HeapType);

        {
            MutexLockWrite lock(m_CommittedAllocationsMutex[heapTypeIndex], m_UseMutex);
            AllocationVectorType* const committedAllocations = m_pCommittedAllocations[heapTypeIndex];
            D3D12MA_ASSERT(committedAllocations);
            committedAllocations->InsertSorted(alloc, PointerLess());
        }
    }

    return hr;
}

UINT AllocatorPimpl::CalcDefaultPoolCount() const noexcept
{
    if(SupportsResourceHeapTier2())
    { return 3; }
    else
    { return 9; }
}

UINT AllocatorPimpl::CalcDefaultPoolIndex(const ALLOCATION_DESC& allocDesc, const D3D12_RESOURCE_DESC& resourceDesc) const noexcept
{
    UINT poolIndex = UINT_MAX;
    switch(allocDesc.HeapType)
    {
        case D3D12_HEAP_TYPE_DEFAULT:  poolIndex = 0; break;
        case D3D12_HEAP_TYPE_UPLOAD:   poolIndex = 1; break;
        case D3D12_HEAP_TYPE_READBACK: poolIndex = 2; break;
        default: D3D12MA_ASSERT(0);
    }

    if(!SupportsResourceHeapTier2())
    {
        poolIndex *= 3;
        if(resourceDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER)
        {
            ++poolIndex;
            const bool isRenderTargetOrDepthStencil =
                (resourceDesc.Flags & (D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)) != 0;
            if(isRenderTargetOrDepthStencil)
            {
                ++poolIndex;
            }
        }
    }

    return poolIndex;
}

void AllocatorPimpl::CalcDefaultPoolParams(D3D12_HEAP_TYPE& outHeapType, D3D12_HEAP_FLAGS& outHeapFlags, UINT index) const noexcept
{
    outHeapType = D3D12_HEAP_TYPE_DEFAULT;
    outHeapFlags = D3D12_HEAP_FLAG_NONE;

    if(!SupportsResourceHeapTier2())
    {
        switch(index % 3)
        {
            case 0:
                outHeapFlags = D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES | D3D12_HEAP_FLAG_DENY_NON_RT_DS_TEXTURES;
                break;
            case 1:
                outHeapFlags = D3D12_HEAP_FLAG_DENY_BUFFERS | D3D12_HEAP_FLAG_DENY_RT_DS_TEXTURES;
                break;
            case 2:
                outHeapFlags = D3D12_HEAP_FLAG_DENY_BUFFERS | D3D12_HEAP_FLAG_DENY_NON_RT_DS_TEXTURES;
                break;
            default: break;
        }

        index /= 3;
    }

    switch(index)
    {
        case 0:
            outHeapType = D3D12_HEAP_TYPE_DEFAULT;
            break;
        case 1:
            outHeapType = D3D12_HEAP_TYPE_UPLOAD;
            break;
        case 2:
            outHeapType = D3D12_HEAP_TYPE_READBACK;
            break;
        default:
            D3D12MA_ASSERT(0);
    }
}

void AllocatorPimpl::FreeCommittedMemory(Allocation* const allocation) noexcept
{
    D3D12MA_ASSERT(allocation && allocation->m_Type == Allocation::TYPE_COMMITTED);
    const UINT heapTypeIndex = HeapTypeToIndex(allocation->m_Committed.heapType);

    {
        MutexLockWrite lock(m_CommittedAllocationsMutex[heapTypeIndex], m_UseMutex);
        AllocationVectorType* const committedAllocations = m_pCommittedAllocations[heapTypeIndex];
        D3D12MA_ASSERT(committedAllocations);
        const bool success = committedAllocations->RemoveSorted(allocation, PointerLess());
        D3D12MA_ASSERT(success);
    }
}

void AllocatorPimpl::FreePlacedMemory(Allocation* const allocation) noexcept
{
    D3D12MA_ASSERT(allocation && allocation->m_Type == Allocation::TYPE_PLACED);
    
    DeviceMemoryBlock* const block = allocation->GetBlock();
    D3D12MA_ASSERT(block);
    BlockVector* const blockVector = block->GetBlockVector();
    D3D12MA_ASSERT(blockVector);
    blockVector->Free(allocation);
}


////////////////////////////////////////////////////////////////////////////////
// Public class Allocation implementation

void Allocation::Release() noexcept
{
    if(!this)  // NOLINT(clang-diagnostic-undefined-bool-conversion)
    { return; }

    D3D12MA_DEBUG_GLOBAL_MUTEX_LOCK

    switch(m_Type)
    {
        case TYPE_COMMITTED:
            m_Allocator->FreeCommittedMemory(this);
            break;
        case TYPE_PLACED:
            m_Allocator->FreePlacedMemory(this);
            break;
        case TYPE_COUNT:
        default: break;
    }

    FreeName();

    D3D12MA_DELETE(m_Allocator->GetAllocs(), this);
}

UINT64 Allocation::GetOffset() const noexcept
{
    switch(m_Type)
    {
        case TYPE_COMMITTED:
            return 0;
        case TYPE_PLACED:
            return m_Placed.offset;
        default:
            D3D12MA_ASSERT(0);
            return 0;
    }
}

ID3D12Heap* Allocation::GetHeap() const noexcept
{
    switch(m_Type)
    {
        case TYPE_COMMITTED:
            return nullptr;
        case TYPE_PLACED:
            return m_Placed.block->GetHeap();
        default:
            D3D12MA_ASSERT(0);
            return nullptr;
    }
}

void Allocation::SetName(LPCWSTR const Name) noexcept
{
    FreeName();

    if(Name)
    {
        const uSys nameCharCount = ::std::wcslen(Name) + 1;
        m_Name = D3D12MA_NEW_ARRAY(m_Allocator->GetAllocs(), wchar_t, nameCharCount);
        ::std::memcpy(m_Name, Name, nameCharCount * sizeof(wchar_t));
    }
}

void Allocation::InitCommitted(AllocatorPimpl* const allocator, const UINT64 size, const D3D12_HEAP_TYPE heapType) noexcept
{
    m_Allocator = allocator;
    m_Type = TYPE_COMMITTED;
    m_Size = size;
    m_Name = nullptr;
    m_Committed.heapType = heapType;
}

void Allocation::InitPlaced(AllocatorPimpl* const allocator, const UINT64 size, const UINT64 offset, const UINT64 alignment, DeviceMemoryBlock* const block) noexcept
{
    m_Allocator = allocator;
    m_Type = TYPE_PLACED;
    m_Size = size;
    m_Name = nullptr;
    m_Placed.offset = offset;
    m_Placed.block = block;
}

DeviceMemoryBlock* Allocation::GetBlock() noexcept
{
    D3D12MA_ASSERT(m_Type == TYPE_PLACED);
    return m_Placed.block;
}

void Allocation::FreeName() noexcept
{
    if(m_Name)
    {
        const uSys nameCharCount = ::std::wcslen(m_Name) + 1;
        D3D12MA_DELETE_ARRAY(m_Allocator->GetAllocs(), m_Name, nameCharCount);
        m_Name = nullptr;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Public class Allocator implementation

Allocator::Allocator(const ALLOCATION_CALLBACKS& allocationCallbacks, const ALLOCATOR_DESC& desc) noexcept
    : m_Pimpl(D3D12MA_NEW(allocationCallbacks, AllocatorPimpl)(allocationCallbacks, desc))
{ }

Allocator::~Allocator() noexcept
{ D3D12MA_DELETE(m_Pimpl->GetAllocs(), m_Pimpl); }

void Allocator::Release()
{
    D3D12MA_DEBUG_GLOBAL_MUTEX_LOCK

    // Copy is needed because otherwise we would call destructor and invalidate the structure with callbacks before using it to free memory.
    const ALLOCATION_CALLBACKS allocationCallbacksCopy = m_Pimpl->GetAllocs();
    D3D12MA_DELETE(allocationCallbacksCopy, this);
}


const D3D12_FEATURE_DATA_D3D12_OPTIONS& Allocator::GetD3D12Options() const noexcept
{
    return m_Pimpl->GetD3D12Options();
}

HRESULT Allocator::CreateResource(
    const ALLOCATION_DESC* pAllocDesc,
    const D3D12_RESOURCE_DESC* pResourceDesc,
    D3D12_RESOURCE_STATES InitialResourceState,
    const D3D12_CLEAR_VALUE *pOptimizedClearValue,
    Allocation** ppAllocation,
    REFIID riidResource,
    void** ppvResource) noexcept
{
    D3D12MA_ASSERT(pAllocDesc && pResourceDesc && ppAllocation && riidResource != IID_NULL && ppvResource);
    D3D12MA_DEBUG_GLOBAL_MUTEX_LOCK
    return m_Pimpl->CreateResource(pAllocDesc, pResourceDesc, InitialResourceState, pOptimizedClearValue, ppAllocation, riidResource, ppvResource);
}

////////////////////////////////////////////////////////////////////////////////
// Public global functions

HRESULT CreateAllocator(const ALLOCATOR_DESC* const pDesc, Allocator** const ppAllocator) noexcept
{
    D3D12MA_ASSERT(pDesc && ppAllocator);
    D3D12MA_ASSERT(pDesc->pDevice);
    D3D12MA_ASSERT(pDesc->PreferredBlockSize == 0 || (pDesc->PreferredBlockSize >= 16 && pDesc->PreferredBlockSize < 0x10000000000ull));

    D3D12MA_DEBUG_GLOBAL_MUTEX_LOCK

    ALLOCATION_CALLBACKS allocationCallbacks;
    SetupAllocationCallbacks(allocationCallbacks, *pDesc);

    *ppAllocator = D3D12MA_NEW(allocationCallbacks, Allocator)(allocationCallbacks, *pDesc);
    const HRESULT hr = (*ppAllocator)->m_Pimpl->Init();
    if(FAILED(hr))
    {
        D3D12MA_DELETE(allocationCallbacks, *ppAllocator);
        *ppAllocator = nullptr;
    }
    return hr;
}

} // namespace D3D12MA
