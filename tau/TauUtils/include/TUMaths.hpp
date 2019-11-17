#pragma once

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
