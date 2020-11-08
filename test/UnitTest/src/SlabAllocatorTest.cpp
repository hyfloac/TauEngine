#include "UnitTest.hpp"
#include "SlabAllocatorTest.hpp"
#include <allocator/SlabAllocator.hpp>

class TestAllocatorImpl : public SlabAllocatorHelper
{
public:
    TestAllocatorImpl() noexcept
        : SlabAllocatorHelper(0, 8, 256, 16384)
    { }
protected:
    bool allocateBlock(Block* block) noexcept override
    {
        constexpr uSys blockBaseSize = 255;
        constexpr uSys blockMultiple = blockBaseSize * 16;

        new(block) Block(operator new(blockMultiple, ::std::nothrow), blockMultiple);
        return true;
    }
};

TAU_TEST(SlabAllocator, allocateTest)
{
    TestAllocatorImpl allocator;

    {
        SlabAllocatorHelper::Allocation a = allocator.allocate(sizeof(u32));

        TAU_EXPECT(a.ptr);

        u32* b = new(a.ptr) u32(16);

        TAU_EXPECT(b);
        TAU_EXPECT_EQ(*b, 16);

        allocator.deallocate(a);
    }

    {
        for(uSys i = 0; i < 512; ++i)
        {
            SlabAllocatorHelper::Allocation a = allocator.allocate(rand() % 128);
            TAU_EXPECT(a.ptr);
        }
    }
}

namespace SlabAllocatorUnitTest {
void runTests()
{
    RUN_ALL_TESTS();
}
}
