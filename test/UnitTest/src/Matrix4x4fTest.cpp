#include "UnitTest.hpp"
#include "Matrix4x4fTest.hpp"
#include <Matrix4x4f.hpp>
#include <Vector4f.hpp>

namespace Matrix4x4fTests {

void addTest() noexcept
{
    UNIT_TEST();
    Matrix4x4f a { };
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
    Matrix4x4f a { };
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
    Matrix4x4f a { };
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


void addScalarTest() noexcept
{
    UNIT_TEST();
    Matrix4x4f a{ };
    for(int i = 0; i < 16; ++i)
    {
        a.mRaw[i] = static_cast<float>(i);
    }

    const Matrix4x4f b = Matrix4x4f::add(a, 1.0f);

    for(int i = 0; i < 16; ++i)
    {
        Assert(epsilonEquals(b.mRaw[i], static_cast<float>(i) + 1.0f));
    }
}

void subScalarTest() noexcept
{
    UNIT_TEST();
    Matrix4x4f a{ };
    for(int i = 0; i < 16; ++i)
    {
        a.mRaw[i] = static_cast<float>(i);
    }

    const Matrix4x4f b = Matrix4x4f::sub(a, 1.0f);

    for(int i = 0; i < 16; ++i)
    {
        Assert(epsilonEquals(b.mRaw[i], static_cast<float>(i) - 1.0f));
    }
}

void mulScalarTest() noexcept
{
    UNIT_TEST();
    Matrix4x4f a{ };
    for(int i = 0; i < 16; ++i)
    {
        a.mRaw[i] = static_cast<float>(i);
    }

    const Matrix4x4f b = Matrix4x4f::mul(a, 1.0f);

    for(int i = 0; i < 16; ++i)
    {
        Assert(epsilonEquals(b.mRaw[i], static_cast<float>(i)));
    }

    const Matrix4x4f c = Matrix4x4f::mul(a, 1.5f);

    for(int i = 0; i < 16; ++i)
    {
        Assert(epsilonEquals(c.mRaw[i], static_cast<float>(i) * 1.5f));
    }
}

void divScalarTest() noexcept
{
    UNIT_TEST();
    Matrix4x4f a { };
    for(int i = 0; i < 16; ++i)
    {
        a.mRaw[i] = static_cast<float>(i);
    }

    const Matrix4x4f b = Matrix4x4f::div(a, 1.0f);

    for(int i = 0; i < 16; ++i)
    {
        Assert(epsilonEquals(b.mRaw[i], static_cast<float>(i)));
    }

    const Matrix4x4f c = Matrix4x4f::div(a, 2.0f);

    for(int i = 0; i < 16; ++i)
    {
        Assert(epsilonEquals(c.mRaw[i], static_cast<float>(i) / 2.0f));
    }

    const Matrix4x4f d = Matrix4x4f::div(a, 0.5f);

    for(int i = 0; i < 16; ++i)
    {
        Assert(epsilonEquals(d.mRaw[i], static_cast<float>(i) / 0.5f));
        Assert(epsilonEquals(d.mRaw[i], static_cast<float>(i) * 2.0f));
    }

    const Matrix4x4f cc = Matrix4x4f::divFast(a, 2.0f);

    for(int i = 0; i < 16; ++i)
    {
        Assert(epsilonEquals(cc.mRaw[i], static_cast<float>(i) / 2.0f));
    }

    const Matrix4x4f dd = Matrix4x4f::divFast(a, 0.5f);

    for(int i = 0; i < 16; ++i)
    {
        Assert(epsilonEquals(dd.mRaw[i], static_cast<float>(i) / 0.5f));
        Assert(epsilonEquals(dd.mRaw[i], static_cast<float>(i) * 2.0f));
    }
}


void mulVecTest() noexcept
{
    UNIT_TEST();
    Matrix4x4f a { };
    for(int i = 0; i < 16; ++i)
    {
        a.mRaw[i] = static_cast<float>(i);
    }

    const Vector4f b(3.0f, 4.0f, 5.0f, 6.0f);

    const Vector4f c = Matrix4x4f::mul(a, b);

    Assert(epsilonEquals(c.x, 128.0f));
    Assert(epsilonEquals(c.y, 146.0f));
    Assert(epsilonEquals(c.z, 164.0f));
    Assert(epsilonEquals(c.w, 182.0f));

    const Vector4f d = Matrix4x4f::mul(b, a);

    Assert(epsilonEquals(d.x, 32.0f));
    Assert(epsilonEquals(d.y, 104.0f));
    Assert(epsilonEquals(d.z, 176.0f));
    Assert(epsilonEquals(d.w, 248.0f));
}
}
