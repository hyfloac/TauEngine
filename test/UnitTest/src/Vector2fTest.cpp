#include "UnitTest.hpp"
#include "Vector2fTest.hpp"
#include <Vector2f.hpp>

TAU_TEST(Vector2f, addTest)
{
    const Vector2f a(1.0f, 2.0f);

    {
        const Vector2f b = Vector2f::add(a, a);
        TAU_EXPECT_FP_EQ_ABS(b.x, 2.0f);
        TAU_EXPECT_FP_EQ_ABS(b.y, 4.0f);
    }

    {
        const Vector2f c = Vector2f::add(a, 1.0f);
        TAU_EXPECT_FP_EQ_ABS(c.x, 2.0f);
        TAU_EXPECT_FP_EQ_ABS(c.y, 3.0f);
    }
}

TAU_TEST(Vector2f, subTest)
{
    const Vector2f a(1.0f, 2.0f);
    {
        const Vector2f b = Vector2f::sub(a, a);
        TAU_EXPECT_FP_EQ_ABS(b.x, 0.0f);
        TAU_EXPECT_FP_EQ_ABS(b.y, 0.0f);
    }

    {
        const Vector2f c = Vector2f::sub(a, 1.0f);
        TAU_EXPECT_FP_EQ_ABS(c.x, 0.0f);
        TAU_EXPECT_FP_EQ_ABS(c.y, 1.0f);
    }
}

TAU_TEST(Vector2f, mulTest)
{
    const Vector2f a(1.0f, 2.0f);
    {
        const Vector2f b = Vector2f::mul(a, a);
        TAU_EXPECT_FP_EQ_ABS(b.x, 1.0f);
        TAU_EXPECT_FP_EQ_ABS(b.y, 4.0f);
    }

    {
        const Vector2f c = Vector2f::mul(a, 1.0f);
        TAU_EXPECT_FP_EQ_ABS(c.x, 1.0f);
        TAU_EXPECT_FP_EQ_ABS(c.y, 2.0f);
    }

    {
        const Vector2f d = Vector2f::mul(a, 2.0f);
        TAU_EXPECT_FP_EQ_ABS(d.x, 2.0f);
        TAU_EXPECT_FP_EQ_ABS(d.y, 4.0f);
    }

    {
        const Vector2f e = Vector2f::mul(a, 0.5f);
        TAU_EXPECT_FP_EQ_ABS(e.x, 0.5f);
        TAU_EXPECT_FP_EQ_ABS(e.y, 1.0f);
    }
}

TAU_TEST(Vector2f, divTest)
{
    const Vector2f a(1.0f, 2.0f);
    {
        const Vector2f b = Vector2f::div(a, a);
        TAU_EXPECT_FP_EQ_ABS(b.x, 1.0f);
        TAU_EXPECT_FP_EQ_ABS(b.y, 1.0f);
    }

    {
        const Vector2f c = Vector2f::div(a, 1.0f);
        TAU_EXPECT_FP_EQ_ABS(c.x, 1.0f);
        TAU_EXPECT_FP_EQ_ABS(c.y, 2.0f);
    }

    {
        const Vector2f d = Vector2f::div(a, 2.0f);
        TAU_EXPECT_FP_EQ_ABS(d.x, 0.5f);
        TAU_EXPECT_FP_EQ_ABS(d.y, 1.0f);
    }

    {
        const Vector2f e = Vector2f::div(a, 0.5f);
        TAU_EXPECT_FP_EQ_ABS(e.x, 2.0f);
        TAU_EXPECT_FP_EQ_ABS(e.y, 4.0f);
    }
}

TAU_TEST(Vector2f, negTest)
{
    const Vector2f a(1.0f, 2.0f);
    const Vector2f b = Vector2f::negate(a);
    TAU_EXPECT_FP_EQ_ABS(b.x, -1.0f);
    TAU_EXPECT_FP_EQ_ABS(b.y, -2.0f);
}

TAU_TEST(Vector2f, magnitudeTest)
{
    const Vector2f a(1.0f, 2.0f);

    const float magnitudeSquared = Vector2f::magnitudeSquared(a);
    TAU_EXPECT_FP_EQ_ABS(magnitudeSquared, 5.0f);

    const float magnitude = Vector2f::magnitude(a);
    TAU_EXPECT_FP_EQ_ABS(magnitude, 2.23606797f);
    TAU_EXPECT_FP_EQ_ABS(magnitude, sqrt(magnitudeSquared));

    const float inverseMagnitude = Vector2f::inverseMagnitude(a);
    TAU_EXPECT_FPE_EQ_ABS(inverseMagnitude, 0.4472135954f, 1E-3f);
    TAU_EXPECT_FPE_EQ_ABS(inverseMagnitude, 1.0f / magnitude, 1E-3f);
}

TAU_TEST(Vector2f, normalizeTest)
{
    const Vector2f a(1.0f, 2.0f);
    {
        const Vector2f b = Vector2f::normalize(a);

        const float magnitudeSquared = Vector2f::magnitudeSquared(b);
        TAU_EXPECT_FPE_EQ_ABS(magnitudeSquared, 1.0f, 1E-3f);

        const float magnitude = Vector2f::magnitude(b);
        TAU_EXPECT_FPE_EQ_ABS(magnitude, 1.0f, 1E-3f);

        const float inverseMagnitude = Vector2f::inverseMagnitude(b);
        TAU_EXPECT_FPE_EQ_ABS(inverseMagnitude, 1.0f, 1E-3f);
    }

    {
        const Vector2f b = Vector2f::normalizeExact(a);

        const float magnitudeSquared = Vector2f::magnitudeSquared(b);
        TAU_EXPECT_FPE_EQ_ABS(magnitudeSquared, 1.0f, 1E-5f);

        const float magnitude = Vector2f::magnitude(b);
        TAU_EXPECT_FPE_EQ_ABS(magnitude, 1.0f, 1E-5f);

        const float inverseMagnitude = Vector2f::inverseMagnitude(b);
        TAU_EXPECT_FPE_EQ_ABS(inverseMagnitude, 1.0f, 1E-5f);
    }
}

TAU_TEST(Vector2f, dotTest)
{
    const Vector2f a(1.0f, 2.0f);
    {
        const float b = Vector2f::dot(a, a);
        TAU_EXPECT_FP_EQ_ABS(b, 5.0f);
    }

    {
        const Vector2f c(7.3f, 8.4f);
        const float d = Vector2f::dot(a, c);
        TAU_EXPECT_FP_EQ_ABS(d, 24.1f);
    }
}

namespace Vector2fTests {
void runTests() noexcept
{
    RUN_ALL_TESTS();
}
}

