#include "UnitTest.hpp"
#include "Vector3fTest.hpp"
#include <Vector3f.hpp>

namespace Vector3fTests {

void addTest() noexcept
{
    UNIT_TEST();
    const Vector3f a(1.0f, 2.0f, 3.0f);
    const Vector3f b = Vector3f::add(a, a);
    Assert(b.x == 2.0f && b.y == 4.0f && b.z == 6.0f);

    const Vector3f c = Vector3f::add(a, 1.0f);
    Assert(c.x == 2.0f && c.y == 3.0f && c.z == 4.0f);
}

void subTest() noexcept
{
    UNIT_TEST();
    const Vector3f a(1.0f, 2.0f, 3.0f);
    const Vector3f b = Vector3f::sub(a, a);
    Assert(b.x == 0.0f && b.y == 0.0f && b.z == 0.0f);

    const Vector3f c = Vector3f::sub(a, 1.0f);
    Assert(c.x == 0.0f && c.y == 1.0f && c.z == 2.0f);
}

void mulTest() noexcept
{
    UNIT_TEST();
    const Vector3f a(1.0f, 2.0f, 3.0f);
    const Vector3f b = Vector3f::mul(a, a);
    Assert(b.x == 1.0f && b.y == 4.0f && b.z == 9.0f);

    const Vector3f c = Vector3f::mul(a, 1.0f);
    Assert(c.x == 1.0f && c.y == 2.0f && c.z == 3.0f);

    const Vector3f d = Vector3f::mul(a, 2.0f);
    Assert(d.x == 2.0f && d.y == 4.0f && d.z == 6.0f);

    const Vector3f e = Vector3f::mul(a, 0.5f);
    Assert(e.x == 0.5f && e.y == 1.0f && e.z == 1.5f);
}

void divTest() noexcept
{
    UNIT_TEST();
    const Vector3f a(1.0f, 2.0f, 3.0f);
    const Vector3f b = Vector3f::div(a, a);
    Assert(b.x == 1.0f && b.y == 1.0f && b.z == 1.0f);

    const Vector3f c = Vector3f::div(a, 1.0f);
    Assert(c.x == 1.0f && c.y == 2.0f && c.z == 3.0f);

    const Vector3f d = Vector3f::div(a, 2.0f);
    Assert(d.x == 0.5f && d.y == 1.0f && d.z == 1.5f);

    const Vector3f e = Vector3f::div(a, 0.5f);
    Assert(e.x == 2.0f && e.y == 4.0f && e.z == 6.0f);
}

void negTest() noexcept
{
    UNIT_TEST();
    const Vector3f a(1.0f, 2.0f, 3.0f);
    const Vector3f b = Vector3f::negate(a);
    Assert(b.x == -1.0f && b.y == -2.0f && b.z == -3.0f);
}

void magnitudeTest() noexcept
{
    UNIT_TEST();
    const Vector3f a(1.0f, 2.0f, 3.0f);

    const float magnitudeSquared = Vector3f::magnitudeSquared(a);
    Assert(epsilonEquals(magnitudeSquared, 14.0f));

    const float magnitude = Vector3f::magnitude(a);
    Assert(epsilonEquals(magnitude, 3.7416573f));
    Assert(epsilonEquals(magnitude, sqrt(magnitudeSquared)));

    const float inverseMagnitude = Vector3f::inverseMagnitude(a);
    Assert(epsilonEquals(inverseMagnitude, 0.2672612f, 1E-3f));
    Assert(epsilonEquals(inverseMagnitude, 1.0f / magnitude, 1E-3f));
}

void normalizeTest() noexcept
{
    UNIT_TEST();
    const Vector3f a(1.0f, 2.0f, 3.0f);
    {
        const Vector3f b = Vector3f::normalize(a);

        const float magnitudeSquared = Vector3f::magnitudeSquared(b);
        Assert(epsilonEquals(magnitudeSquared, 1.0f, 1E-3f));

        const float magnitude = Vector3f::magnitude(b);
        Assert(epsilonEquals(magnitude, 1.0f, 1E-3f));

        const float inverseMagnitude = Vector3f::inverseMagnitude(b);
        Assert(epsilonEquals(inverseMagnitude, 1.0f, 1E-3f));
    }

    {
        const Vector3f b = Vector3f::normalizeExact(a);

        const float magnitudeSquared = Vector3f::magnitudeSquared(b);
        Assert(epsilonEquals(magnitudeSquared, 1.0f, 1E-5f));

        const float magnitude = Vector3f::magnitude(b);
        Assert(epsilonEquals(magnitude, 1.0f, 1E-5f));

        const float inverseMagnitude = Vector3f::inverseMagnitude(b);
        Assert(epsilonEquals(inverseMagnitude, 1.0f, 1E-3f));
    }
}

void dotTest() noexcept
{
    UNIT_TEST();
    const Vector3f a(1.0f, 2.0f, 3.0f);
    const float b = Vector3f::dot(a, a);
    Assert(epsilonEquals(b, 14.0f));

    const Vector3f c(7.3f, 8.4f, 9.5f);
    const float d = Vector3f::dot(a, c);
    Assert(epsilonEquals(d, 52.6f));
}

void crossTest() noexcept
{
    UNIT_TEST();
    const Vector3f a(1.0f, 2.0f, 3.0f);
    const Vector3f b(7.3f, 8.4f, 9.5f);
    const Vector3f c = Vector3f::cross(a, b);
    Assert(epsilonEquals(c.x, -6.2f) && epsilonEquals(c.y, 12.4f) && epsilonEquals(c.z, -6.2f));
}
}

