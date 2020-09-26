#include "UnitTest.hpp"
#include "Vector4fTest.hpp"
#include <Vector4f.hpp>

namespace Vector4fTests {

void addTest() noexcept
{
    UNIT_TEST();
    const Vector4f a(1.0f, 2.0f, 3.0f, 4.0f);
    const Vector4f b = Vector4f::add(a, a);
    Assert(b.x == 2.0f && b.y == 4.0f && b.z == 6.0f && b.w == 8.0f);

    const Vector4f c = Vector4f::add(a, 1.0f);
    Assert(c.x == 2.0f && c.y == 3.0f && c.z == 4.0f && c.w == 5.0f);
}

void subTest() noexcept
{
    UNIT_TEST();
    const Vector4f a(1.0f, 2.0f, 3.0f, 4.0f);
    const Vector4f b = Vector4f::sub(a, a);
    Assert(b.x == 0.0f && b.y == 0.0f && b.z == 0.0f && b.w == 0.0f);

    const Vector4f c = Vector4f::sub(a, 1.0f);
    Assert(c.x == 0.0f && c.y == 1.0f && c.z == 2.0f && c.w == 3.0f);
}

void mulTest() noexcept
{
    UNIT_TEST();
    const Vector4f a(1.0f, 2.0f, 3.0f, 4.0f);
    const Vector4f b = Vector4f::mul(a, a);
    Assert(b.x == 1.0f && b.y == 4.0f && b.z == 9.0f && b.w == 16.0f);

    const Vector4f c = Vector4f::mul(a, 1.0f);
    Assert(c.x == 1.0f && c.y == 2.0f && c.z == 3.0f && c.w == 4.0f);

    const Vector4f d = Vector4f::mul(a, 2.0f);
    Assert(d.x == 2.0f && d.y == 4.0f && d.z == 6.0f && d.w == 8.0f);

    const Vector4f e = Vector4f::mul(a, 0.5f);
    Assert(e.x == 0.5f && e.y == 1.0f && e.z == 1.5f && e.w == 2.0f);
}

void divTest() noexcept
{
    UNIT_TEST();
    const Vector4f a(1.0f, 2.0f, 3.0f, 4.0f);
    const Vector4f b = Vector4f::div(a, a);
    Assert(b.x == 1.0f && b.y == 1.0f && b.z == 1.0f && b.w == 1.0f);

    const Vector4f c = Vector4f::div(a, 1.0f);
    Assert(c.x == 1.0f && c.y == 2.0f && c.z == 3.0f && c.w == 4.0f);

    const Vector4f d = Vector4f::div(a, 2.0f);
    Assert(d.x == 0.5f && d.y == 1.0f && d.z == 1.5f && d.w == 2.0f);

    const Vector4f e = Vector4f::div(a, 0.5f);
    Assert(e.x == 2.0f && e.y == 4.0f && e.z == 6.0f && e.w == 8.0f);
}

void negTest() noexcept
{
    UNIT_TEST();
    const Vector4f a(1.0f, 2.0f, 3.0f, 4.0f);
    const Vector4f b = Vector4f::negate(a);
    Assert(b.x == -1.0f && b.y == -2.0f && b.z == -3.0f && b.w == -4.0f);
}

void magnitudeTest() noexcept
{
    UNIT_TEST();
    const Vector4f a(1.0f, 2.0f, 3.0f, 4.0f);

    const float magnitudeSquared = Vector4f::magnitudeSquared(a);
    Assert(rEpsilonEquals(magnitudeSquared, 30.0f));

    const float magnitude = Vector4f::magnitude(a);
    Assert(rEpsilonEquals(magnitude, 5.477225557f));
    Assert(rEpsilonEquals(magnitude, sqrt(magnitudeSquared)));

    const float inverseMagnitude = Vector4f::inverseMagnitude(a);
    Assert(rEpsilonEquals(inverseMagnitude, 0.182574185f, 1E-3f));
    Assert(rEpsilonEquals(inverseMagnitude, 1.0f / magnitude, 1E-3f));
}

void normalizeTest() noexcept
{
    UNIT_TEST();
    const Vector4f a(1.0f, 2.0f, 3.0f, 4.0f);
    {
        const Vector4f b = Vector4f::normalize(a);

        const float magnitudeSquared = Vector4f::magnitudeSquared(b);
        Assert(rEpsilonEquals(magnitudeSquared, 1.0f, 1E-3f));

        const float magnitude = Vector4f::magnitude(b);
        Assert(rEpsilonEquals(magnitude, 1.0f, 1E-3f));

        const float inverseMagnitude = Vector4f::inverseMagnitude(b);
        Assert(rEpsilonEquals(inverseMagnitude, 1.0f, 1E-3f));
    }

    {
        const Vector4f b = Vector4f::normalizeExact(a);

        const float magnitudeSquared = Vector4f::magnitudeSquared(b);
        Assert(rEpsilonEquals(magnitudeSquared, 1.0f, 1E-5f));

        const float magnitude = Vector4f::magnitude(b);
        Assert(rEpsilonEquals(magnitude, 1.0f, 1E-5f));

        const float inverseMagnitude = Vector4f::inverseMagnitude(b);
        Assert(rEpsilonEquals(inverseMagnitude, 1.0f, 1E-3f));
    }
}

void dotTest() noexcept
{
    UNIT_TEST();
    const Vector4f a(1.0f, 2.0f, 3.0f, 4.0f);
    const float b = Vector4f::dot(a, a);
    Assert(rEpsilonEquals(b, 30.0f));

    const Vector4f c(7.3f, 8.4f, 9.5f, 10.6f);
    const float d = Vector4f::dot(a, c);
    Assert(rEpsilonEquals(d, 95.0f));
}
}

