#include "FreeListAllocatorTest.hpp"
#include "allocator/FreeListAllocator.hpp"
#include "ReferenceCountingPointer.hpp"
#include "Safeties.hpp"
#include "UnitTest.hpp"

class AllocTestObj final
{
    DEFAULT_CM_PU(AllocTestObj);
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

class AllocTestObj1 final
{
    DEFAULT_CM_PU(AllocTestObj1);
private:
    bool _x;
    bool _print;
public:
    AllocTestObj1(const bool x, bool print = true) noexcept
        : _x(x), _print(print)
    {
        if(print)
        {
            printf("Allocated Object 1: (%d)\n", x);
        }
    }

    ~AllocTestObj1() noexcept
    {
        if(_print)
        {
            printf("Deallocated Object 1: (%d)\n", _x);
        }
    }

    [[nodiscard]] bool x() const noexcept { return _x; }
    [[nodiscard]] bool& x() noexcept { return _x; }
};

namespace FreeListAllocatorTest {

void resetTest() noexcept
{
    UNIT_TEST();
    FreeListAllocator allocator;

    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 1, 5.7f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 1, 19.3f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 2, 3.141592f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 3, 0.00001f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 5, 2165465.95f);

    allocator.reset();

    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 8, 6.28f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 13, 2.7182818f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 21, 1.618033988f);
}

void destructTest() noexcept
{
    UNIT_TEST();
    FreeListAllocator allocator;

    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 1, 5.7f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 1, 19.3f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 2, 3.141592f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 3, 0.00001f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 5, 2165465.95f);
}

void difTypeResetTest() noexcept
{
    UNIT_TEST();
    FreeListAllocator allocator;

    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 1, 5.7f);
    allocator.allocateT<NullableRef<AllocTestObj1>>(DefaultTauAllocator::Instance(), true);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 2, 3.141592f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 3, 0.00001f);
    allocator.allocateT<NullableRef<AllocTestObj1>>(DefaultTauAllocator::Instance(), false);

    allocator.reset();

    allocator.allocateT<NullableRef<AllocTestObj1>>(DefaultTauAllocator::Instance(), true);
    allocator.allocateT<NullableRef<AllocTestObj1>>(DefaultTauAllocator::Instance(), true);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 21, 1.618033988f);
}

void difTypeDestructTest() noexcept
{
    UNIT_TEST();
    FreeListAllocator allocator;

    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 1, 5.7f);
    allocator.allocateT<NullableRef<AllocTestObj1>>(DefaultTauAllocator::Instance(), true);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 2, 3.141592f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 3, 0.00001f);
    allocator.allocateT<NullableRef<AllocTestObj1>>(DefaultTauAllocator::Instance(), false);
}

void difPtrResetTest() noexcept
{
    UNIT_TEST();
    FreeListAllocator allocator;

    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 1, 5.7f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 1, 19.3f);
    auto sPtr = allocator.allocateT<NullableStrongRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 2, 3.141592f);
    allocator.allocateT<NullableStrongRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 3, 0.00001f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 5, 2165465.95f);
    allocator.allocateT<NullableWeakRef<AllocTestObj>>(*sPtr);

    allocator.reset();

    sPtr = allocator.allocateT<NullableStrongRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 8, 6.28f);
    allocator.allocateT<NullableWeakRef<AllocTestObj>>(*sPtr);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 21, 1.618033988f);
}

void difPtrDestructTest() noexcept
{
    UNIT_TEST();
    FreeListAllocator allocator;

    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 1, 5.7f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 1, 19.3f);
    allocator.allocateT<NullableStrongRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 2, 3.141592f);
    allocator.allocateT<NullableStrongRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 3, 0.00001f);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 5, 2165465.95f);
}

void difPtrTypeResetTest() noexcept
{
    UNIT_TEST();
    FreeListAllocator allocator;

    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 1, 5.7f);
    allocator.allocateT<NullableRef<AllocTestObj1>>(DefaultTauAllocator::Instance(), true);
    auto sPtr = allocator.allocateT<NullableStrongRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 2, 3.141592f);
    allocator.allocateT<NullableStrongRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 3, 0.00001f);
    allocator.allocateT<NullableRef<AllocTestObj1>>(DefaultTauAllocator::Instance(), false);
    allocator.allocateT<NullableWeakRef<AllocTestObj>>(*sPtr);

    allocator.reset();

    auto sPtr0 = allocator.allocateT<NullableStrongRef<AllocTestObj1>>(DefaultTauAllocator::Instance(), true);
    allocator.allocateT<NullableWeakRef<AllocTestObj1>>(*sPtr0);
    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 21, 1.618033988f);
}

void difPtrTypeDestructTest() noexcept
{
    UNIT_TEST();
    FreeListAllocator allocator;

    allocator.allocateT<NullableRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 1, 5.7f);
    allocator.allocateT<NullableRef<AllocTestObj1>>(DefaultTauAllocator::Instance(), true);
    auto sPtr = allocator.allocateT<NullableStrongRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 2, 3.141592f);
    allocator.allocateT<NullableStrongRef<AllocTestObj>>(DefaultTauAllocator::Instance(), 3, 0.00001f);
    allocator.allocateT<NullableRef<AllocTestObj1>>(DefaultTauAllocator::Instance(), false);
    allocator.allocateT<NullableWeakRef<AllocTestObj>>(*sPtr);
}
}
