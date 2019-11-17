/**
 * @file
 * 
 * Some useful math functions.
 */
#pragma once

#pragma warning(push, 0)
#include <cmath>
#include <immintrin.h>
#include <type_traits>
#ifdef _WIN32
#include <intrin.h>
#endif
#pragma warning(pop)

#include <NumTypes.hpp>
#include <DLL.hpp>
#include <TUMaths.hpp>

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

template<typename _Int>
static inline constexpr _Int rotL(const _Int n, unsigned int c) noexcept
{
    static_assert(std::is_unsigned<_Int>::value, "Rotate Left only makes sense for unsigned types");

    constexpr _Int mask = 8 * sizeof(n) - 1;
    c &= mask;
    signed int cTmp = reinterpret_cast<signed int&>(c);
    cTmp = -cTmp;
    const unsigned int negC = reinterpret_cast<unsigned&>(cTmp)& mask;
    return (n << c) | (n >> negC);
}

template<typename _Int>
static inline constexpr _Int rotR(const _Int n, unsigned int c) noexcept
{
    static_assert(std::is_unsigned<_Int>::value, "Rotate Left only makes sense for unsigned types");

    constexpr _Int mask = 8 * sizeof(n) - 1;
    c &= mask;
    signed int cTmp = reinterpret_cast<signed int&>(c);
    cTmp = -cTmp;
    const unsigned int negC = reinterpret_cast<unsigned&>(cTmp)& mask;
    return (n >> c) | (n << negC);
}

#if defined(_WIN32)
template<>
inline u8 rotL<u8>(const u8 n, unsigned int c) noexcept
{ return _rotl8(n, static_cast<unsigned char>(c)); }

template<>
inline u8 rotR<u8>(const u8 n, unsigned int c) noexcept
{ return _rotr8(n, static_cast<unsigned char>(c)); }

template<>
inline u16 rotL<u16>(const u16 n, unsigned int c) noexcept
{ return _rotl16(n, static_cast<unsigned char>(c)); }

template<>
inline u16 rotR<u16>(const u16 n, unsigned int c) noexcept
{ return _rotr16(n, static_cast<unsigned char>(c)); }
#endif

template<>
inline u32 rotL<u32>(const u32 n, unsigned int c) noexcept
{ return _rotl(n, static_cast<int>(c)); }

template<>
inline u32 rotR<u32>(const u32 n, unsigned int c) noexcept
{ return _rotr(n, static_cast<int>(c)); }

template<>
inline u64 rotL<u64>(const u64 n, unsigned int c) noexcept
{ return _rotl64(n, static_cast<int>(c)); }

template<>
inline u64 rotR<u64>(const u64 n, unsigned int c) noexcept
{ return _rotr64(n, static_cast<int>(c)); }
