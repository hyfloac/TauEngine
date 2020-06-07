#include "UnitTest.hpp"
#include "Matrix4x4fTest.hpp"
#include <Matrix4x4f.hpp>

namespace Matrix4x4fTests {

void addTest() noexcept
{
    UNIT_TEST();
    Matrix4x4f a{ };
    for(int i = 0; i < 16; ++i)
    {
        a.mRaw[i] = static_cast<float>(i);
    }

    const Matrix4x4f b = Matrix4x4f::add(a, a);

    for(int i = 0; i < 16; ++i)
    {
        Assert(epsilonEquals(b.mRaw[i], static_cast<float>(i) * 2.0f));
    }
}

void subTest() noexcept
{
    UNIT_TEST();
    Matrix4x4f a{ };
    for(int i = 0; i < 16; ++i)
    {
        a.mRaw[i] = static_cast<float>(i);
    }

    const Matrix4x4f b = Matrix4x4f::sub(a, a);

    for(int i = 0; i < 16; ++i)
    {
        Assert(epsilonEquals(b.mRaw[i], 0.0f));
    }
}

void mulTest() noexcept
{
    UNIT_TEST();
    Matrix4x4f a{ };
    for(int i = 0; i < 16; ++i)
    {
        a.mRaw[i] = static_cast<float>(i);
    }

    const Matrix4x4f b = Matrix4x4f::mul(a, a);

    Assert(epsilonEquals(b.m[0][0], 56.0f));
    Assert(epsilonEquals(b.m[0][1], 62.0f));
    Assert(epsilonEquals(b.m[0][2], 68.0f));
    Assert(epsilonEquals(b.m[0][3], 74.0f));

    Assert(epsilonEquals(b.m[1][0], 152.0f));
    Assert(epsilonEquals(b.m[1][1], 174.0f));
    Assert(epsilonEquals(b.m[1][2], 196.0f));
    Assert(epsilonEquals(b.m[1][3], 218.0f));

    Assert(epsilonEquals(b.m[2][0], 248.0f));
    Assert(epsilonEquals(b.m[2][1], 286.0f));
    Assert(epsilonEquals(b.m[2][2], 324.0f));
    Assert(epsilonEquals(b.m[2][3], 362.0f));

    Assert(epsilonEquals(b.m[3][0], 344.0f));
    Assert(epsilonEquals(b.m[3][1], 398.0f));
    Assert(epsilonEquals(b.m[3][2], 452.0f));
    Assert(epsilonEquals(b.m[3][3], 506.0f));
}
}