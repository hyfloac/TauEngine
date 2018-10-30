#pragma once

#include "DLL.hpp"

#define RADIANS_TO_DEGREES_CONVERTER_VAL 57.29577951308232087679815481410517
#define DEGREES_TO_RADIANS_CONVERTER_VAL 0.017453292519943295769236907684886

#define RAD_2_DEG_F(__F) (float) (__F * RADIANS_TO_DEGREES_CONVERTER_VAL##f)
#define DEG_2_RAD_F(__F) (float) (__F * DEGREES_TO_RADIANS_CONVERTER_VAL##f)
#define RAD_2_DEG_D(__D) (double) (__D * RADIANS_TO_DEGREES_CONVERTER_VAL)
#define DEG_2_RAD_D(__D) (double) (__D * DEGREES_TO_RADIANS_CONVERTER_VAL)

#define RAD_2_DEG(__F) RAD_2_DEG_F(__F)
#define DEG_2_RAD(__F) DEG_2_RAD_F(__F)

#include <cmath>
#include "NumTypes.hpp"

template<u32 _Base>
float logN(float x) noexcept
{
    return log(x) * log(_Base);
}

template<u32 _Base>
double logN(double x) noexcept
{
    return log(x) * log(_Base);
}

template<u32 _Base>
long double logN(long double x) noexcept
{
    return log(x) * log(_Base);
}

/**
 * Initializes a very fast Sin/Cos table.
 */
TAU_DLL void initSinTable() noexcept;

/**
 * Computes the sin value from a lookup table.
 */
TAU_DLL float fastSin(float value) noexcept;

/**
 * Computes the sin value from a lookup table.
 */
TAU_DLL float fastCos(float value) noexcept;

/**
 * Computes the sin value from a lookup table.
 */
TAU_DLL float fastSin(double value) noexcept;

/**
 * Computes the sin value from a lookup table.
 */
TAU_DLL float fastCos(double value) noexcept;
