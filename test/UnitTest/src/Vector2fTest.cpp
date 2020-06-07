#include "UnitTest.hpp"
#include "Vector2fTest.hpp"
#include <Vector2f.hpp>

namespace Vector2fTests {

void addTest() noexcept
{
    UNIT_TEST();
    const Vector2f a(1.0f, 2.0f);
    const Vector2f b = Vector2f::add(a, a);
    Assert(b.x == 2.0f && b.y == 4.0f);

    const Vector2f c = Vector2f::add(a, 1.0f);
    Assert(c.x == 2.0f && c.y == 3.0f);
}

void subTest() noexcept
{
    UNIT_TEST();
    const Vector2f a(1.0f, 2.0f);
    const Vector2f b = Vector2f::sub(a, a);
    Assert(b.x == 0.0f && b.y == 0.0f);

    const Vector2f c = Vector2f::sub(a, 1.0f);
    Assert(c.x == 0.0f && c.y == 1.0f);
}

void mulTest() noexcept
{
    UNIT_TEST();
    const Vector2f a(1.0f, 2.0f);
    const Vector2f b = Vector2f::mul(a, a);
    Assert(b.x == 1.0f && b.y == 4.0f);

    const Vector2f c = Vector2f::mul(a, 1.0f);
    Assert(c.x == 1.0f && c.y == 2.0f);

    const Vector2f d = Vector2f::mul(a, 2.0f);
    Assert(d.x == 2.0f && d.y == 4.0f);

    const Vector2f e = Vector2f::mul(a, 0.5f);
    Assert(e.x == 0.5f && e.y == 1.0f);
}

void divTest() noexcept
{
    UNIT_TEST();
    const Vector2f a(1.0f, 2.0f);
    const Vector2f b = Vector2f::div(a, a);
    Assert(b.x == 1.0f && b.y == 1.0f);

    const Vector2f c = Vector2f::div(a, 1.0f);
    Assert(c.x == 1.0f && c.y == 2.0f);

    const Vector2f d = Vector2f::div(a, 2.0f);
    Assert(d.x == 0.5f && d.y == 1.0f);

    const Vector2f e = Vector2f::div(a, 0.5f);
    Assert(e.x == 2.0f && e.y == 4.0f);
}

void negTest() noexcept
{
    UNIT_TEST();
    const Vector2f a(1.0f, 2.0f);
    const Vector2f b = Vector2f::negate(a);
    Assert(b.x == -1.0f && b.y == -2.0f);
}

void magnitudeTest() noexcept
{
    UNIT_TEST();
    const Vector2f a(1.0f, 2.0f);

    const float magnitudeSquared = Vector2f::magnitudeSquared(a);
    Assert(epsilonEquals(magnitudeSquared, 5.0f));

    const float magnitude = Vector2f::magnitude(a);
    Assert(epsilonEquals(magnitude, 2.23606797f));
    Assert(epsilonEquals(magnitude, sqrt(magnitudeSquared)));

    const float inverseMagnitude = Vector2f::inverseMagnitude(a);
    Assert(epsilonEquals(inverseMagnitude, 0.4472135954f, 1E-3f));
    Assert(epsilonEquals(inverseMagnitude, 1.0f / magnitude, 1E-3f));
}

void normalizeTest() noexcept
{
    UNIT_TEST();
    const Vector2f a(1.0f, 2.0f);
    {
        const Vector2f b = Vector2f::normalize(a);

        const float magnitudeSquared = Vector2f::magnitudeSquared(b);
        Assert(epsilonEquals(magnitudeSquared, 1.0f, 1E-3f));

        const float magnitude = Vector2f::magnitude(b);
        Assert(epsilonEquals(magnitude, 1.0f, 1E-3f));

        const float inverseMagnitude = Vector2f::inverseMagnitude(b);
        Assert(epsilonEquals(inverseMagnitude, 1.0f, 1E-3f));
    }

    {
        const Vector2f b = Vector2f::normalizeExact(a);

        const float magnitudeSquared = Vector2f::magnitudeSquared(b);
        Assert(epsilonEquals(magnitudeSquared, 1.0f, 1E-5f));

        const float magnitude = Vector2f::magnitude(b);
        Assert(epsilonEquals(magnitude, 1.0f, 1E-5f));

        const float inverseMagnitude = Vector2f::inverseMagnitude(b);
        Assert(epsilonEquals(inverseMagnitude, 1.0f, 1E-3f));
    }
}

void dotTest() noexcept
{
    UNIT_TEST();
    const Vector2f a(1.0f, 2.0f);
    const float b = Vector2f::dot(a, a);
    Assert(epsilonEquals(b, 5.0f));

    const Vector2f c(7.3f, 8.4f);
    const float d = Vector2f::dot(a, c);
    Assert(epsilonEquals(d, 24.1f));
}
}

