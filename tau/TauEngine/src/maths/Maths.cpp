/**
 * @file
 */
#include <maths/Maths.hpp>
#include <MathDefines.hpp>
#include <Safeties.hpp>

/**
 * A unit circle with 2**16 elements.
 */
static float  SIN_TABLE_F[65536];
/**
 * A unit circle with 2**16 elements.
 */
static double SIN_TABLE_D[65536];

void initSinTable() noexcept
{
    static bool initializationComplete = false;

    if(!initializationComplete)
    {
        for(u32 i = 0; i < 65536; ++i)
        {
            SIN_TABLE_F[i] = std::sin((static_cast<float>(i) * static_cast<float>(T_PI)) / 32768.0F);
            SIN_TABLE_D[i] = std::sin((static_cast<double>(i) * static_cast<double>(T_PI)) / 32768.0);
        }

        // Sets some hard values to ensure better accuracy.
        SIN_TABLE_F[0]     =  0.0F;
        SIN_TABLE_F[16384] =  1.0F;
        SIN_TABLE_F[32768] =  0.0F;
        SIN_TABLE_F[49152] = -1.0F;

        SIN_TABLE_D[0]     =  0.0;
        SIN_TABLE_D[16384] =  1.0;
        SIN_TABLE_D[32768] =  0.0;
        SIN_TABLE_D[49152] = -1.0;

        initializationComplete = true;
    }
}

static constexpr float  SC_RAD_SCALAR_F = 10430.3783504705F; // 65536 / tau
static constexpr double SC_RAD_SCALAR_D = 10430.3783504704527249495663163811;

static constexpr float  SC_DEG_SCALAR_F = 182.044444444444444444444444444444F;
static constexpr double SC_DEG_SCALAR_D = 182.044444444444444444444444444444444444444444444444444444444444444444444444444444F;

float fastSinR(float radians) noexcept
{
    Ensure(radians == radians);
    Ensure(!isinf(radians));

    return SIN_TABLE_F[static_cast<u32>(radians * SC_RAD_SCALAR_F) & 65535];
}

double fastSinR(double radians) noexcept
{
    Ensure(radians == radians);
    Ensure(!isinf(radians));

    return SIN_TABLE_D[static_cast<u32>(radians * SC_RAD_SCALAR_D) & 65535];
}

float fastSinD(float degrees) noexcept
{
    Ensure(degrees == degrees);
    Ensure(!isinf(degrees));

    return SIN_TABLE_F[static_cast<u32>(degrees * SC_DEG_SCALAR_F) & 65535];
}

double fastSinD(double degrees) noexcept
{
    Ensure(degrees == degrees);
    Ensure(!isinf(degrees));

    return SIN_TABLE_D[static_cast<u32>(degrees * SC_DEG_SCALAR_D) & 65535];
}

float fastCosR(float radians) noexcept
{
    Ensure(radians == radians);
    Ensure(!isinf(radians));

    return SIN_TABLE_F[(static_cast<u32>(radians * SC_RAD_SCALAR_F) + 16384) & 65535];
}

double fastCosR(double radians) noexcept
{
    Ensure(radians == radians);
    Ensure(!isinf(radians));

    return SIN_TABLE_D[(static_cast<u32>(radians * SC_RAD_SCALAR_D) + 16384) & 65535];
}

float fastCosD(float degrees) noexcept
{
    Ensure(degrees == degrees);
    Ensure(!isinf(degrees));

    return SIN_TABLE_F[(static_cast<u32>(degrees * SC_DEG_SCALAR_F) + 16384) & 65535];
}

double fastCosD(double degrees) noexcept
{
    Ensure(degrees == degrees);
    Ensure(!isinf(degrees));

    return SIN_TABLE_D[(static_cast<u32>(degrees * SC_DEG_SCALAR_D) + 16384) & 65535];
}

SinCos<float> fastSinCosR(float radians) noexcept
{
    Ensure(radians == radians);
    Ensure(!isinf(radians));

    const u32 index = static_cast<u32>(radians * SC_RAD_SCALAR_F);

    const float _sin = SIN_TABLE_F[index & 65535];
    const float _cos = SIN_TABLE_F[(index + 16384) & 65535];

    return { _sin, _cos };
}

SinCos<double> fastSinCosR(double radians) noexcept
{
    Ensure(radians == radians);
    Ensure(!isinf(radians));

    const u32 index = static_cast<u32>(radians * SC_RAD_SCALAR_D);

    const double _sin = SIN_TABLE_D[index & 65535];
    const double _cos = SIN_TABLE_D[(index + 16384) & 65535];

    return { _sin, _cos };
}

SinCos<float> fastSinCosD(float degrees) noexcept
{
    Ensure(degrees == degrees);
    Ensure(!isinf(degrees));

    const u32 index = static_cast<u32>(degrees * SC_DEG_SCALAR_F);

    const float _sin = SIN_TABLE_F[index & 65535];
    const float _cos = SIN_TABLE_F[(index + 16384) & 65535];

    return { _sin, _cos };
}

SinCos<double> fastSinCosD(double degrees) noexcept
{
    Ensure(degrees == degrees);
    Ensure(!isinf(degrees));

    const u32 index = static_cast<u32>(degrees * SC_DEG_SCALAR_D);

    const double _sin = SIN_TABLE_D[index & 65535];
    const double _cos = SIN_TABLE_D[(index + 16384) & 65535];

    return { _sin, _cos };
}

float fastTanR(float radians) noexcept
{
    Ensure(radians == radians);
    Ensure(!isinf(radians));

    const u32 index = static_cast<u32>(radians * SC_RAD_SCALAR_F);

    const float _sin = SIN_TABLE_F[index & 65535];
    const float _cos = SIN_TABLE_F[(index + 16384) & 65535];

    return _sin / _cos;
}

double fastTanR(double radians) noexcept
{
    Ensure(radians == radians);
    Ensure(!isinf(radians));

    const u32 index = static_cast<u32>(radians * SC_RAD_SCALAR_D);

    const double _sin = SIN_TABLE_D[index & 65535];
    const double _cos = SIN_TABLE_D[(index + 16384) & 65535];

    return _sin / _cos;
}

float fastTanD(float degrees) noexcept
{
    Ensure(degrees == degrees);
    Ensure(!isinf(degrees));

    const u32 index = static_cast<u32>(degrees * SC_DEG_SCALAR_F);

    const float _sin = SIN_TABLE_F[index & 65535];
    const float _cos = SIN_TABLE_F[(index + 16384) & 65535];

    return _sin / _cos;
}

double fastTanD(double degrees) noexcept
{
    Ensure(degrees == degrees);
    Ensure(!isinf(degrees));

    const u32 index = static_cast<u32>(degrees * SC_DEG_SCALAR_D);

    const double _sin = SIN_TABLE_D[index & 65535];
    const double _cos = SIN_TABLE_D[(index + 16384) & 65535];

    return _sin / _cos;
}

float fastCotR(float radians) noexcept
{
    Ensure(radians == radians);
    Ensure(!isinf(radians));

    const u32 index = static_cast<u32>(radians * SC_RAD_SCALAR_F);

    const float _sin = SIN_TABLE_F[index & 65535];
    const float _cos = SIN_TABLE_F[(index + 16384) & 65535];

    return _cos / _sin;
}

double fastCotR(double radians) noexcept
{
    Ensure(radians == radians);
    Ensure(!isinf(radians));

    const u32 index = static_cast<u32>(radians * SC_RAD_SCALAR_D);

    const double _sin = SIN_TABLE_D[index & 65535];
    const double _cos = SIN_TABLE_D[(index + 16384) & 65535];

    return _cos / _sin;
}

float fastCotD(float degrees) noexcept
{
    Ensure(degrees == degrees);
    Ensure(!isinf(degrees));

    const u32 index = static_cast<u32>(degrees * SC_DEG_SCALAR_F);

    const float _sin = SIN_TABLE_F[index & 65535];
    const float _cos = SIN_TABLE_F[(index + 16384) & 65535];

    return _cos / _sin;
}

double fastCotD(double degrees) noexcept
{
    Ensure(degrees == degrees);
    Ensure(!isinf(degrees));

    const u32 index = static_cast<u32>(degrees * SC_DEG_SCALAR_D);

    const double _sin = SIN_TABLE_D[index & 65535];
    const double _cos = SIN_TABLE_D[(index + 16384) & 65535];

    return _cos / _sin;
}

float fastInverseSqrt(float x) noexcept
{
    const float halfX = x * 0.5f;
    i32 i = *reinterpret_cast<i32*>(&x);

    i = 0x5F3759DF - (i >> 1);
    const float y = *reinterpret_cast<float*>(&i);

    return y * (1.5f - (halfX * y * y));
}

double fastInverseSqrt(double x) noexcept
{
    const double halfX = x * 0.5f;
    i64 i = *reinterpret_cast<i64*>(&x);

    i = 0x5FE6EB50C7B537A9 - (i >> 1);
    const double y = *reinterpret_cast<double*>(&i);

    return y * (1.5f - (halfX * y * y));
}
