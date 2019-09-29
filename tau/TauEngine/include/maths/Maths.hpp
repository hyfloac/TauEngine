/**
 * @file
 * 
 * Some useful math functions.
 */
#pragma once

#pragma warning(push, 0)
#include <cmath>
#include <immintrin.h>
#pragma warning(pop)

#include <NumTypes.hpp>
#include <DLL.hpp>

/**
 * Equal to pi/180.
 */
#define RADIANS_TO_DEGREES_CONVERTER_VAL 57.29577951308232087679815481410517
/**
 * Equal to 180/pi
 */
#define DEGREES_TO_RADIANS_CONVERTER_VAL 0.017453292519943295769236907684886

/**
 * Converts radians to degrees (single precision).
 */
#define RAD_2_DEG_F(__F) (float) ((__F) * RADIANS_TO_DEGREES_CONVERTER_VAL)
 /**
  * Converts degrees to radians (single precision).
  */
#define DEG_2_RAD_F(__F) (float) ((__F) * DEGREES_TO_RADIANS_CONVERTER_VAL)
 /**
  * Converts radians to degrees (double precision).
  */
#define RAD_2_DEG_D(__D) (double) ((__D) * RADIANS_TO_DEGREES_CONVERTER_VAL)
  /**
   * Converts degrees to radians (double precision).
   */
#define DEG_2_RAD_D(__D) (double) ((__D) * DEGREES_TO_RADIANS_CONVERTER_VAL)

#define RAD_2_DEG(__F) RAD_2_DEG_F(__F)
#define DEG_2_RAD(__F) DEG_2_RAD_F(__F)

/**
 * Computes the log base n of a value (single precision).
 * 
 * @param[in] x
 *    The value to find the logarithm of.
 */
template<u32 _Base>
float logN(float x) noexcept
{
    return log(x) / log(_Base);
}

/**
 * Computes the log base n of a value (double precision).
 *
 * @param[in] x
 *    The value to find the logarithm of.
 */
template<u32 _Base>
double logN(double x) noexcept
{
    return log(x) / log(_Base);
}

/**
 *   Computes the log base n of a value (double precision on
 * windows, 2.5x precision (80 bit) in GCC).
 *
 * @param[in] x
 *    The value to find the logarithm of.
 */
template<u32 _Base>
long double logN(long double x) noexcept
{
    return log(x) / log(_Base);
}

/**
 * Initializes the sin table.
 *
 * Each element is computed by taking the index, multiplying it by Tau (2pi), divide by the element count.
 *
 * \f$\LARGE \frac{i * 2 * \pi}{65536}\f$ -> \f$\LARGE \frac{i*\pi}{32768}\f$
 */
void initSinTable() noexcept;

/**
 * Sin looked up in a table
 *
 * Access index by multiplying the input by blip count divided by Tau (2pi), binary AND it against the blip count minus 1
 *
 * \f$\LARGE x * \frac{65536}{\tau} \normalsize \& \large 65535\f$
 *
 * \f$\LARGE x * \frac{32768}{\pi} \normalsize \& \large 65535\f$
 */
TAU_DLL float fastSinR(float radians) noexcept;

/**
 * Sin looked up in a table
 *
 * Access index by multiplying the input by blip count divided by Tau (2pi), binary AND it against the blip count minus 1
 *
 * \f$\LARGE x * \frac{65536}{\tau} \normalsize \& \large 65535\f$
 *
 * \f$\LARGE x * \frac{32768}{\pi} \normalsize \& \large 65535\f$
 */
TAU_DLL double fastSinR(double radians) noexcept;

/**
 * Sin looked up in a table
 *
 * Access index by multiplying the input by blip count divided by Tau (2pi), binary AND it against the blip count minus 1
 *
 * \f$\LARGE x * \frac{65536}{360} \normalsize \& \large 65535\f$
 *
 * \f$\LARGE x * \frac{32768}{180} \normalsize \& \large 65535\f$
 */
TAU_DLL float fastSinD(float degrees) noexcept;

/**
 * Sin looked up in a table
 *
 * Access index by multiplying the input by blip count divided by Tau (2pi), binary AND it against the blip count minus 1
 *
 * \f$\LARGE x * \frac{65536}{360} \normalsize \& \large 65535\f$
 *
 * \f$\LARGE x * \frac{32768}{180} \normalsize \& \large 65535\f$
 */
TAU_DLL double fastSinD(double degrees) noexcept;

/**
 * Cos looked up in the sin table with the appropriate offset
 *
 * Access index by multiplying the input by blip count divided by Tau (2pi), add pi/2 (the shift to convert sin to cosine), binary AND it against the blip count minus 1
 *
 * \f$\huge[ \LARGE x * \frac{65536}{\tau} \Large + \LARGE \frac{65536}{4} \huge] \large \& \Large 65535\f$
 *
 * \f$\huge[ \LARGE x * \frac{32768}{\pi} \Large + \large 16384 \huge] \large \& \Large 65535\f$
 */
TAU_DLL float fastCosR(float radians) noexcept;

/**
 * Cos looked up in the sin table with the appropriate offset
 *
 * Access index by multiplying the input by blip count divided by Tau (2pi), add pi/2 (the shift to convert sin to cosine), binary AND it against the blip count minus 1
 *
 * \f$\huge[ \LARGE x * \frac{65536}{\tau} \Large + \LARGE \frac{65536}{4} \huge] \large \& \Large 65535\f$
 *
 * \f$\huge[ \LARGE x * \frac{32768}{\pi} \Large + \large 16384 \huge] \large \& \Large 65535\f$
 */
TAU_DLL double fastCosR(double radians) noexcept;

/**
 * Cos looked up in the sin table with the appropriate offset
 *
 * Access index by multiplying the input by blip count divided by Tau (2pi), add pi/2 (the shift to convert sin to cosine), binary AND it against the blip count minus 1
 *
 * \f$\huge[ \LARGE x * \frac{65536}{360} \Large + \LARGE \frac{65536}{4} \huge] \large \& \Large 65535\f$
 *
 * \f$\huge[ \LARGE x * \frac{32768}{180} \Large + \large 16384 \huge] \large \& \Large 65535\f$
 */
TAU_DLL float fastCosD(float degrees) noexcept;

/**
 * Cos looked up in the sin table with the appropriate offset
 *
 * Access index by multiplying the input by blip count divided by Tau (2pi), add pi/2 (the shift to convert sin to cosine), binary AND it against the blip count minus 1
 *
 * \f$\huge[ \LARGE x * \frac{65536}{360} \Large + \LARGE \frac{65536}{4} \huge] \large \& \Large 65535\f$
 *
 * \f$\huge[ \LARGE x * \frac{32768}{180} \Large + \large 16384 \huge] \large \& \Large 65535\f$
 */
TAU_DLL double fastCosD(double degrees) noexcept;

template<typename _F>
struct SinCos final
{
    _F sin;
    _F cos;

    SinCos& negate() noexcept
    {
        this->sin = -this->sin;
        return *this;
    }
};

TAU_DLL SinCos<float> fastSinCosR(float radians) noexcept;

TAU_DLL SinCos<double> fastSinCosR(double radians) noexcept;

TAU_DLL SinCos<float> fastSinCosD(float degrees) noexcept;

TAU_DLL SinCos<double> fastSinCosD(double degrees) noexcept;

TAU_DLL float fastTanR(float radians) noexcept;

TAU_DLL double fastTanR(double radians) noexcept;

TAU_DLL float fastTanD(float degrees) noexcept;

TAU_DLL double fastTanD(double degrees) noexcept;

TAU_DLL float fastCotR(float radians) noexcept;

TAU_DLL double fastCotR(double radians) noexcept;

TAU_DLL float fastCotD(float degrees) noexcept;

TAU_DLL double fastCotD(double degrees) noexcept;

TAU_DLL float fastInverseSqrt(float x) noexcept;

static inline float rSqrt(float x) noexcept
{
    return _mm_cvtss_f32(_mm_rsqrt_ss(_mm_set_ss(x)));
}

TAU_DLL double fastInverseSqrt(double x) noexcept;

template<typename _T>
constexpr _T minT(const _T a, const _T b)
{
    return a < b ? a : b;
}

template<typename _T>
constexpr _T minT(const _T a, const _T b, const _T c)
{
    return minT(minT(a, b), c);
}

template<typename _T>
constexpr _T minT(const _T a, const _T b, const _T c, const _T d)
{
    return minT(minT(a, b), minT(c, d));
}

template<typename _T>
constexpr _T maxT(const _T a, const _T b)
{
    return a > b ? a : b;
}

template<typename _T>
constexpr _T maxT(const _T a, const _T b, const _T c)
{
    return maxT(maxT(a, b), c);
}

template<typename _T>
constexpr _T maxT(const _T a, const _T b, const _T c, const _T d)
{
    return maxT(maxT(a, b), maxT(c, d));
}
