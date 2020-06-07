#pragma once

#include "TauMathLibInternal.h"
#include <xmmintrin.h>

#if __cplusplus
extern "C" {
#endif

EXTERN __m128 vector4f_add(__m128 a, __m128 b) NOEXCEPT;
EXTERN __m128 vector4f_sub(__m128 a, __m128 b) NOEXCEPT;
EXTERN __m128 vector4f_mul(__m128 a, __m128 b) NOEXCEPT;
EXTERN __m128 vector4f_div(__m128 a, __m128 b) NOEXCEPT;

EXTERN __m128 vector4f_addF(__m128 a, float b) NOEXCEPT;
EXTERN __m128 vector4f_subF(__m128 a, float b) NOEXCEPT;
EXTERN __m128 vector4f_mulF(__m128 a, float b) NOEXCEPT;
EXTERN __m128 vector4f_divF(__m128 a, float b) NOEXCEPT;

EXTERN __m128 vector4f_divFInv(float a, __m128 b) NOEXCEPT;

EXTERN __m128 vector4f_neg(__m128 a) NOEXCEPT;

EXTERN float vector4f_magnitudeSquared(__m128 a) NOEXCEPT;
EXTERN float vector4f_magnitude(__m128 a) NOEXCEPT;
EXTERN float vector4f_inverseMagnitude(__m128 a) NOEXCEPT;

EXTERN __m128 vector4f_normalizeFast(__m128 a) NOEXCEPT;
EXTERN __m128 vector4f_normalizeExact(__m128 a) NOEXCEPT;

EXTERN float vector4f_dot(__m128 a, __m128 b) NOEXCEPT;

#if __cplusplus
}
#endif
