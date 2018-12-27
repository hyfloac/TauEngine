/**
 * @file
 */
#include <Maths.hpp>
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
            // SIN_TABLE_F[i] = std::sin((i * ((float) T_PI) * 2.0F) / 65536.0F);
            SIN_TABLE_F[i] = std::sin((i * static_cast<float>(T_PI)) / 32768.0F);
            SIN_TABLE_D[i] = std::sin((i * static_cast<double>(T_PI)) / 32768.0);
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

float fastSin(float value) noexcept
{
    Ensure(value == value);
    Ensure(!isinf(value));

    return SIN_TABLE_F[static_cast<u32>(value * 10430.3783504704F) & 65535];
}

double fastSin(double value) noexcept
{
    Ensure(value == value);
    Ensure(!isinf(value));

    return SIN_TABLE_D[static_cast<u32>(value * 10430.3783504704527249495663163811) & 65535];
}

float fastCos(float value) noexcept
{
    Ensure(value == value);
    Ensure(!isinf(value));

    // return SIN_TABLE_F[(int) (value * (65536.0f / (2.0f * (float) T_PI)) + 4.0f / 65536.0f) & 65535];
    return SIN_TABLE_F[(static_cast<u32>(value * 10430.3783504704F) + 16384) & 65535];
}

double fastCos(double value) noexcept
{
    Ensure(value == value);
    Ensure(!isinf(value));

    return SIN_TABLE_D[(static_cast<u32>(value * 10430.3783504704527249495663163811) + 16384) & 65535];
}
