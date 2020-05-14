#include "FixedBlockAllocatorTest.hpp"
#include "allocator/FixedBlockAllocator.hpp"
#include "UnitTest.hpp"

class AllocTestObj final
{
    DEFAULT_COPY(AllocTestObj);
private:
    uSys _x;
    float _y;
    bool _print;
public:
    AllocTestObj(const uSys x, const float y, bool print = true) noexcept
        : _x(x), _y(y), _print(print)
    {
        if(print)
        {
            printf("Allocated Object: (%zu, %f)\n", x, y);
        }
    }

    ~AllocTestObj() noexcept
    {
        if(_print)
        {
            printf("Deallocated Object: (%zu, %f)\n", _x, _y);
        }
    }

    [[nodiscard]] uSys x() const noexcept { return _x; }
    [[nodiscard]] uSys& x() noexcept { return _x; }
    [[nodiscard]] float y() const noexcept { return _y; }
    [[nodiscard]] float& y() noexcept { return _y; }
};

template<typename _Allocator>
void arenaAllocationValidity() noexcept
{
    UNIT_TEST();
    _Allocator allocator(sizeof(AllocTestObj));

    AllocTestObj* obj0 = new(allocator.allocate()) AllocTestObj(3, 6.0f);
    AllocTestObj* obj1 = new(allocator.allocate()) AllocTestObj(17, 92.0f);
    AllocTestObj* obj2 = new(allocator.allocate()) AllocTestObj(65536, 873.385782f);

    Assert(obj0 != nullptr);
    Assert(obj0->x() == 3 && obj0->y() == 6.0f);
    Assert(obj1 != nullptr);
    Assert(obj1->x() == 17 && obj1->y() == 92.0f);
    Assert(obj2 != nullptr);
    Assert(obj2->x() == 65536 && obj2->y() == 873.385782f);

    obj0->~AllocTestObj();
    allocator.deallocate(obj0);
    obj1->~AllocTestObj();
    allocator.deallocate(obj1);
    obj2->~AllocTestObj();
    allocator.deallocate(obj2);
}

template<typename _Allocator>
void arenaMacroAllocationValidity() noexcept
{
    UNIT_TEST();
    _Allocator allocator(sizeof(AllocTestObj));

    AllocTestObj* obj0 = allocator.template allocateT<AllocTestObj>(3, 6.0f);
    AllocTestObj* obj1 = allocator.template allocateT<AllocTestObj>(17, 92.0f);
    AllocTestObj* obj2 = allocator.template allocateT<AllocTestObj>(65536, 873.385782f);

    Assert(obj0 != nullptr);
    Assert(obj0->x() == 3 && obj0->y() == 6.0f);
    Assert(obj1 != nullptr);
    Assert(obj1->x() == 17 && obj1->y() == 92.0f);
    Assert(obj2 != nullptr);
    Assert(obj2->x() == 65536 && obj2->y() == 873.385782f);

    allocator.deallocateT(obj0);
    allocator.deallocateT(obj1);
    allocator.deallocateT(obj2);
}

template<typename _Allocator>
void arenaMaxPageExceed() noexcept
{
    UNIT_TEST();
    _Allocator allocator(sizeof(AllocTestObj), 1);
    const uSys allocations = PageAllocator::pageSize() / allocator.blockSize() - 1;
    for(int i = 0; i < allocations; ++i)
    {
        (void) allocator.template allocateT<AllocTestObj>(i, 1.0f / static_cast<float>(i), false);
    }

    AllocTestObj* obj = allocator.template allocateT<AllocTestObj>(3141592, 3.141592f);
    AllocTestObj* nullObj = allocator.template allocateT<AllocTestObj>(6283185, 6.283185f);

    Assert(obj != nullptr);
    Assert(nullObj == nullptr);
}

namespace FixedBlockAllocatorUnitTest {

void arenaAllocationValidityTest() noexcept
{
    arenaAllocationValidity<FixedBlockArenaAllocator<AllocationTracking::None>>();
    arenaAllocationValidity<FixedBlockArenaAllocator<AllocationTracking::Count>>();
    arenaAllocationValidity<FixedBlockArenaAllocator<AllocationTracking::DoubleDeleteCount>>();
}

void arenaMacroAllocateTest() noexcept
{
    arenaMacroAllocationValidity<FixedBlockArenaAllocator<AllocationTracking::None>>();
    arenaMacroAllocationValidity<FixedBlockArenaAllocator<AllocationTracking::Count>>();
    arenaMacroAllocationValidity<FixedBlockArenaAllocator<AllocationTracking::DoubleDeleteCount>>();
}

void arenaMaxPageExceedTest() noexcept
{
    arenaMaxPageExceed<FixedBlockArenaAllocator<AllocationTracking::None>>();
    arenaMaxPageExceed<FixedBlockArenaAllocator<AllocationTracking::Count>>();
    arenaMaxPageExceed<FixedBlockArenaAllocator<AllocationTracking::DoubleDeleteCount>>();
}

void arenaCountTest() noexcept
{
    UNIT_TEST();
#define TEST_COUNT 512
    {
        FixedBlockArenaAllocator<AllocationTracking::Count> allocator(sizeof(AllocTestObj));

        AllocTestObj** objects = new(::std::nothrow) AllocTestObj* [TEST_COUNT];
        for(int i = 0; i < TEST_COUNT; ++i)
        {
            objects[i] = allocator.allocateT<AllocTestObj>(i, 1.0f / static_cast<float>(i), false);
        }

        for(int i = 0; i < TEST_COUNT; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        delete[] objects;

        Assert(allocator.allocationDifference() == 0);
    }

    {
        FixedBlockArenaAllocator<AllocationTracking::Count> allocator(sizeof(AllocTestObj));

        AllocTestObj** objects = new(::std::nothrow) AllocTestObj * [TEST_COUNT];
        for(int i = 0; i < TEST_COUNT; ++i)
        {
            objects[i] = allocator.allocateT<AllocTestObj>(i, 1.0f / static_cast<float>(i), false);
        }

        for(int i = 0; i < TEST_COUNT - 10; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        delete[] objects;

        Assert(allocator.allocationDifference() == 10);
    }

    {
        FixedBlockArenaAllocator<AllocationTracking::Count> allocator(sizeof(AllocTestObj));

        AllocTestObj** objects = new(::std::nothrow) AllocTestObj * [TEST_COUNT];
        for(int i = 0; i < TEST_COUNT; ++i)
        {
            objects[i] = allocator.allocateT<AllocTestObj>(i, 1.0f / static_cast<float>(i), false);
        }

        for(int i = 0; i < TEST_COUNT; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        for(int i = 0; i < 10; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        delete[] objects;

        Assert(allocator.allocationDifference() == -10);
    }
#undef TEST_COUNT
}

void arenaMultipleDeleteTest() noexcept
{
    UNIT_TEST();
#define TEST_COUNT 512
    {
        FixedBlockArenaAllocator<AllocationTracking::DoubleDeleteCount> allocator(sizeof(AllocTestObj));

        AllocTestObj** objects = new(::std::nothrow) AllocTestObj * [TEST_COUNT];
        for(int i = 0; i < TEST_COUNT; ++i)
        {
            objects[i] = allocator.allocateT<AllocTestObj>(i, 1.0f / static_cast<float>(i), false);
        }

        for(int i = 0; i < TEST_COUNT; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        delete[] objects;

        Assert(allocator.allocationDifference() == 0);
        Assert(allocator.doubleDeleteCount() == 0);
        Assert(allocator.multipleDeleteCount() == 0);
    }

    {
        FixedBlockArenaAllocator<AllocationTracking::DoubleDeleteCount> allocator(sizeof(AllocTestObj));

        AllocTestObj** objects = new(::std::nothrow) AllocTestObj * [TEST_COUNT];
        for(int i = 0; i < TEST_COUNT; ++i)
        {
            objects[i] = allocator.allocateT<AllocTestObj>(i, 1.0f / static_cast<float>(i), false);
        }

        for(int i = 0; i < TEST_COUNT - 10; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        delete[] objects;

        Assert(allocator.allocationDifference() == 10);
        Assert(allocator.doubleDeleteCount() == 0);
        Assert(allocator.multipleDeleteCount() == 0);
    }

    {
        FixedBlockArenaAllocator<AllocationTracking::DoubleDeleteCount> allocator(sizeof(AllocTestObj));

        AllocTestObj** objects = new(::std::nothrow) AllocTestObj * [TEST_COUNT];
        for(int i = 0; i < TEST_COUNT; ++i)
        {
            objects[i] = allocator.allocateT<AllocTestObj>(i, 1.0f / static_cast<float>(i), false);
        }

        for(int i = 0; i < TEST_COUNT; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        for(int i = 0; i < 10; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        delete[] objects;

        Assert(allocator.allocationDifference() == -10);
        Assert(allocator.doubleDeleteCount() == 10);
        Assert(allocator.multipleDeleteCount() == 10);
    }

    {
        FixedBlockArenaAllocator<AllocationTracking::DoubleDeleteCount> allocator(sizeof(AllocTestObj));

        AllocTestObj** objects = new(::std::nothrow) AllocTestObj * [TEST_COUNT];
        for(int i = 0; i < TEST_COUNT; ++i)
        {
            objects[i] = allocator.allocateT<AllocTestObj>(i, 1.0f / static_cast<float>(i), false);
        }

        for(int i = 0; i < TEST_COUNT; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        for(int i = 0; i < 10; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        for(int i = 0; i < 10; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        delete[] objects;

        Assert(allocator.allocationDifference() == -20);
        Assert(allocator.doubleDeleteCount() == 10);
        Assert(allocator.multipleDeleteCount() == 20);
    }
#undef TEST_COUNT
}

void allocationValidityTest() noexcept
{
    arenaAllocationValidity<FixedBlockAllocator<AllocationTracking::None>>();
    arenaAllocationValidity<FixedBlockAllocator<AllocationTracking::Count>>();
    arenaAllocationValidity<FixedBlockAllocator<AllocationTracking::DoubleDeleteCount>>();
}

void macroAllocateTest() noexcept
{
    arenaMacroAllocationValidity<FixedBlockAllocator<AllocationTracking::None>>();
    arenaMacroAllocationValidity<FixedBlockAllocator<AllocationTracking::Count>>();
    arenaMacroAllocationValidity<FixedBlockAllocator<AllocationTracking::DoubleDeleteCount>>();
}

void maxPageExceedTest() noexcept
{
    arenaMaxPageExceed<FixedBlockAllocator<AllocationTracking::None>>();
    arenaMaxPageExceed<FixedBlockAllocator<AllocationTracking::Count>>();
    arenaMaxPageExceed<FixedBlockAllocator<AllocationTracking::DoubleDeleteCount>>();
}

void countTest() noexcept
{
    UNIT_TEST();
#define TEST_COUNT 512
    {
        FixedBlockAllocator<AllocationTracking::Count> allocator(sizeof(AllocTestObj));

        AllocTestObj** objects = new(::std::nothrow) AllocTestObj * [TEST_COUNT];
        for(int i = 0; i < TEST_COUNT; ++i)
        {
            objects[i] = allocator.allocateT<AllocTestObj>(i, 1.0f / static_cast<float>(i), false);
        }

        for(int i = 0; i < TEST_COUNT; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        delete[] objects;

        Assert(allocator.allocationDifference() == 0);
    }

    {
        FixedBlockAllocator<AllocationTracking::Count> allocator(sizeof(AllocTestObj));

        AllocTestObj** objects = new(::std::nothrow) AllocTestObj * [TEST_COUNT];
        for(int i = 0; i < TEST_COUNT; ++i)
        {
            objects[i] = allocator.allocateT<AllocTestObj>(i, 1.0f / static_cast<float>(i), false);
        }

        for(int i = 0; i < TEST_COUNT - 10; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        delete[] objects;

        Assert(allocator.allocationDifference() == 10);
    }
#undef TEST_COUNT
}

void multipleDeleteTest() noexcept
{
    UNIT_TEST();
#define TEST_COUNT 512
    {
        FixedBlockAllocator<AllocationTracking::DoubleDeleteCount> allocator(sizeof(AllocTestObj));

        AllocTestObj** objects = new(::std::nothrow) AllocTestObj * [TEST_COUNT];
        for(int i = 0; i < TEST_COUNT; ++i)
        {
            objects[i] = allocator.allocateT<AllocTestObj>(i, 1.0f / static_cast<float>(i), false);
        }

        for(int i = 0; i < TEST_COUNT; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        delete[] objects;

        Assert(allocator.allocationDifference() == 0);
        Assert(allocator.doubleDeleteCount() == 0);
        Assert(allocator.multipleDeleteCount() == 0);
    }

    {
        FixedBlockAllocator<AllocationTracking::DoubleDeleteCount> allocator(sizeof(AllocTestObj));

        AllocTestObj** objects = new(::std::nothrow) AllocTestObj * [TEST_COUNT];
        for(int i = 0; i < TEST_COUNT; ++i)
        {
            objects[i] = allocator.allocateT<AllocTestObj>(i, 1.0f / static_cast<float>(i), false);
        }

        for(int i = 0; i < TEST_COUNT - 10; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        delete[] objects;

        Assert(allocator.allocationDifference() == 10);
        Assert(allocator.doubleDeleteCount() == 0);
        Assert(allocator.multipleDeleteCount() == 0);
    }

    {
        FixedBlockAllocator<AllocationTracking::DoubleDeleteCount> allocator(sizeof(AllocTestObj));

        AllocTestObj** objects = new(::std::nothrow) AllocTestObj * [TEST_COUNT];
        for(int i = 0; i < TEST_COUNT; ++i)
        {
            objects[i] = allocator.allocateT<AllocTestObj>(i, 1.0f / static_cast<float>(i), false);
        }

        for(int i = 0; i < TEST_COUNT; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        for(int i = 0; i < 10; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        delete[] objects;

        Assert(allocator.allocationDifference() == -10);
        Assert(allocator.doubleDeleteCount() == 10);
        Assert(allocator.multipleDeleteCount() == 10);
    }

    {
        FixedBlockAllocator<AllocationTracking::DoubleDeleteCount> allocator(sizeof(AllocTestObj));

        AllocTestObj** objects = new(::std::nothrow) AllocTestObj * [TEST_COUNT];
        for(int i = 0; i < TEST_COUNT; ++i)
        {
            objects[i] = allocator.allocateT<AllocTestObj>(i, 1.0f / static_cast<float>(i), false);
        }

        for(int i = 0; i < TEST_COUNT; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        for(int i = 0; i < 10; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        for(int i = 0; i < 10; ++i)
        {
            allocator.deallocateT(objects[i]);
        }

        delete[] objects;

        Assert(allocator.allocationDifference() == -20);
        Assert(allocator.doubleDeleteCount() == 10);
        Assert(allocator.multipleDeleteCount() == 20);
    }
#undef TEST_COUNT
}

}
