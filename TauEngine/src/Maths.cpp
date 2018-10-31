#include "Maths.hpp"
#include "MathDefines.hpp"

/**
 * A unit circle with 2**16 elements.
 */
static float  SIN_TABLE_F[65536];
static double SIN_TABLE_D[65536];

/**
 * Initializes the sin table.
 * 
 * Each element is computed by taking the index, multiplying it by Tau (2pi), divide by the element count.
 * 
 * i * 2 * pi        i * 2 * pi
 * ----------   ->   ----------
 * blip count          65536
 */
void initSinTable() noexcept
{
    static bool initializationComplete = false;

    if(!initializationComplete)
    {
        for(u32 i = 0; i < 65536; ++i)
        {
            // SIN_TABLE_F[i] = std::sin((i * ((float) T_PI) * 2.0F) / 65536.0F);
            SIN_TABLE_F[i] = std::sin((i * (float) T_PI) / 32768.0F);
            SIN_TABLE_D[i] = std::sin((i * (double) T_PI) / 32768.0);
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

/**
 * Sin looked up in a table
 * 
 * Access index by multiplying the input by blip count divided by Tau (2pi), binary AND it against the blip count minus 1
 * 
 *          65536
 * value * ------- & 0xFFFF
 *           Tau
 */
float fastSin(float value) noexcept
{
    return SIN_TABLE_F[(u32) (value * 10430.378F) & 65535];
}

/**
 * Sin looked up in a table
 *
 * Access index by multiplying the input by blip count divided by Tau (2pi), binary AND it against the blip count minus 1
 *
 *          65536
 * value * ------- & 0xFFFF
 *           Tau
 */
double fastSin(double value) noexcept
{
    return SIN_TABLE_D[(u32) (value * 10430.378) & 65535];
}

/**
 * Cos looked up in the sin table with the appropriate offset
 * 
 * Access index by multiplying the input by blip count divided by Tau (2pi), add pi/2 (the shift to convert sin to cosine), binary AND it against the blip count minus 1
 * 
 *            65536     65536
 * ( value * ------- + ------- ) & 0xFFFF
 *             Tau        4
 */
float fastCos(float value) noexcept
{
    // return SIN_TABLE_F[(int) (value * (65536.0f / (2.0f * (float) T_PI)) + 4.0f / 65536.0f) & 65535];
    return SIN_TABLE_F[(u32) (value * 10430.3783504704F + 16384.0F) & 65535];
}

/**
 * Cos looked up in the sin table with the appropriate offset
 *
 * Access index by multiplying the input by blip count divided by Tau (2pi), add pi/2 (the shift to convert sin to cosine), binary AND it against the blip count minus 1
 *
 *            65536     65536
 * ( value * ------- + ------- ) & 0xFFFF
 *             Tau        4
 */
double fastCos(double value) noexcept
{
    return SIN_TABLE_D[(u32) (value * 10430.3783504704527249495663163811 + 16384.0) & 65535];
}
