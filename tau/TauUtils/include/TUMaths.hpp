#pragma once

#include "NumTypes.hpp"

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

template<typename _T>
constexpr _T minT(const _T a, const _T b) noexcept
{ return a < b ? a : b; }

template<typename _T>
constexpr _T minT(const _T a, const _T b, const _T c) noexcept
{ return minT(minT(a, b), c); }

template<typename _T>
constexpr _T minT(const _T a, const _T b, const _T c, const _T d) noexcept
{ return minT(minT(a, b), minT(c, d)); }

template<typename _T>
constexpr _T maxT(const _T a, const _T b) noexcept
{ return a > b ? a : b; }

template<typename _T>
constexpr _T maxT(const _T a, const _T b, const _T c) noexcept
{ return maxT(maxT(a, b), c); }

template<typename _T>
constexpr _T maxT(const _T a, const _T b, const _T c, const _T d) noexcept
{ return maxT(maxT(a, b), maxT(c, d)); }

const u32 tab64[64] = {
    63,  0, 58,  1, 59, 47, 53,  2,
    60, 39, 48, 27, 54, 33, 42,  3,
    61, 51, 37, 40, 49, 18, 28, 20,
    55, 30, 34, 11, 43, 14, 22,  4,
    62, 57, 46, 52, 38, 26, 32, 41,
    50, 36, 17, 19, 29, 10, 13, 21,
    56, 45, 25, 31, 35, 16,  9, 12,
    44, 24, 15,  8, 23,  7,  6,  5 };


const u32 tab32[32] = {
     0,  9,  1, 10, 13, 21,  2, 29,
    11, 14, 16, 18, 22, 25,  3, 30,
     8, 12, 20, 28, 15, 17, 24,  7,
    19, 27, 23,  6, 26,  5,  4, 31 };

static inline u32 log2i_64(u64 value) noexcept
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    value |= value >> 32;
    return tab64[((value - (value >> 1)) * 0x07EDD5E59A4E28C2ull) >> 58];
}

static inline u32 log2i_32(u32 value) noexcept
{
    value |= value >> 1;
    value |= value >> 2;
    value |= value >> 4;
    value |= value >> 8;
    value |= value >> 16;
    return tab32[((value * 0x07C4ACDDu)) >> 27];
}

static inline u32 log2i(const u64 value) noexcept
{ return log2i_64(value); }

static inline u32 log2i(const u32 value) noexcept
{ return log2i_32(value); }
