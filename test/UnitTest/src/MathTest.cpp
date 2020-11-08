#include "UnitTest.hpp"
#include "MathTest.hpp"
#include <TUMaths.hpp>

TAU_TEST(MathTest, nextPowerOf2_32Test)
{
    TAU_EXPECT_EQ(nextPowerOf2(1u), 1);
    TAU_EXPECT_EQ(nextPowerOf2(2u), 2);
    TAU_EXPECT_EQ(nextPowerOf2(3u), 4);

    for(u32 i = 2; i < 31; ++i)
    {
        const u32 val = 1 << i;
        const u32 nextVal = 1 << (i + 1);
        TAU_EXPECT_EQ(nextPowerOf2(val - 1), val);
        TAU_EXPECT_EQ(nextPowerOf2(val), val);
        TAU_EXPECT_EQ(nextPowerOf2(val + 1), nextVal);

        if(i >= 4 && i < 30)
        {
            const u32 halfVal = val + (1 << (i - 1));
            TAU_EXPECT_EQ(nextPowerOf2(halfVal - 1), nextVal);
            TAU_EXPECT_EQ(nextPowerOf2(halfVal), nextVal);
            TAU_EXPECT_EQ(nextPowerOf2(halfVal + 1), nextVal);
        }
    }
}

TAU_TEST(MathTest, nextPowerOf2_64Test)
{
    TAU_EXPECT_EQ(nextPowerOf2(1ull), 1);
    TAU_EXPECT_EQ(nextPowerOf2(2ull), 2);
    TAU_EXPECT_EQ(nextPowerOf2(3ull), 4);

    for(u64 i = 2; i < 63; ++i)
    {
        const u64 val = 1ull << i;
        const u64 nextVal = 1ull << (i + 1ull);
        TAU_EXPECT_EQ(nextPowerOf2(val - 1), val);
        TAU_EXPECT_EQ(nextPowerOf2(val), val);
        TAU_EXPECT_EQ(nextPowerOf2(val + 1ull), nextVal);

        if(i >= 4 && i < 62)
        {
            const u64 halfVal = val + (1ull << (i - 1ull));
            TAU_EXPECT_EQ(nextPowerOf2(halfVal - 1ull), nextVal);
            TAU_EXPECT_EQ(nextPowerOf2(halfVal), nextVal);
            TAU_EXPECT_EQ(nextPowerOf2(halfVal + 1ull), nextVal);
        }
    }
}

TAU_TEST(MathTest, log2i_32Test)
{
    TAU_EXPECT_EQ(log2i(1u), 0);
    TAU_EXPECT_EQ(log2i(2u), 1);
    TAU_EXPECT_EQ(log2i(3u), 1);

    for(u32 i = 2; i < 32; ++i)
    {
        const u32 val = 1 << i;
        TAU_EXPECT_EQ(log2i(val - 1), i - 1);
        TAU_EXPECT_EQ(log2i(val), i);
        TAU_EXPECT_EQ(log2i(val + 1), i);

        if(i >= 4)
        {
            const u32 halfVal = val + (1 << (i - 1));
            TAU_EXPECT_EQ(log2i(halfVal - 1), i);
            TAU_EXPECT_EQ(log2i(halfVal), i);
            TAU_EXPECT_EQ(log2i(halfVal + 1), i);
        }
    }
}

TAU_TEST(MathTest, log2i_64Test)
{
    TAU_EXPECT_EQ(log2i(1ull), 0);
    TAU_EXPECT_EQ(log2i(2ull), 1);
    TAU_EXPECT_EQ(log2i(3ull), 1);

    for(u64 i = 2; i < 64; ++i)
    {
        const u64 val = 1ull << i;
        TAU_EXPECT_EQ(log2i(val - 1), i - 1);
        TAU_EXPECT_EQ(log2i(val), i);
        TAU_EXPECT_EQ(log2i(val + 1), i);

        if(i >= 4)
        {
            const u64 halfVal = val + (1ull << (i - 1ull));
            TAU_EXPECT_EQ(log2i(halfVal - 1), i);
            TAU_EXPECT_EQ(log2i(halfVal), i);
            TAU_EXPECT_EQ(log2i(halfVal + 1), i);
        }
    }
}

void MathTest::runTests()
{
    RUN_ALL_TESTS();
}
