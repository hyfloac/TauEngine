#pragma once

#include "TauMathLibInternal.h"
#include <xmmintrin.h>
#include <emmintrin.h>
#if __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif

#if __cplusplus
extern "C" {
#endif

    EXTERN __m128i vector4f_add(__m128i a, __m128i b) NOEXCEPT;
    EXTERN __m128i vector4f_sub(__m128i a, __m128i b) NOEXCEPT;
    EXTERN __m128i vector4f_mul(__m128i a, __m128i b) NOEXCEPT;
    EXTERN __m128i vector4f_div(__m128i a, __m128i b) NOEXCEPT;
    EXTERN __m128  vector4f_divF(__m128i a, __m128i b) NOEXCEPT;

    EXTERN __m128i vector4f_addScalar(__m128i a, int32_t b) NOEXCEPT;
    EXTERN __m128i vector4f_subScalar(__m128i a, int32_t b) NOEXCEPT;
    EXTERN __m128i vector4f_mulScalar(__m128i a, int32_t b) NOEXCEPT;
    EXTERN __m128i vector4f_divScalar(__m128i a, int32_t b) NOEXCEPT;

    EXTERN __m128i vector4f_divScalarInv(int32_t a, __m128 b) NOEXCEPT;

    EXTERN __m128 vector4f_divScalarF(__m128i a, float b) NOEXCEPT;
    EXTERN __m128 vector4f_divScalarInvF(float a, __m128i b) NOEXCEPT;

    EXTERN __m128i vector4f_neg(__m128i a) NOEXCEPT;

    EXTERN int32_t vector4f_magnitudeSquared(__m128i a) NOEXCEPT;
    EXTERN float   vector4f_magnitude(__m128i a) NOEXCEPT;
    EXTERN float   vector4f_inverseMagnitude(__m128i a) NOEXCEPT;

    EXTERN __m128 vector4f_normalizeFast(__m128i a) NOEXCEPT;
    EXTERN __m128 vector4f_normalizeExact(__m128i a) NOEXCEPT;

    EXTERN int32_t vector4f_dot(__m128i a, __m128i b) NOEXCEPT;

#if __cplusplus
}
#endif
