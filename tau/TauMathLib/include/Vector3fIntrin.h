#pragma once

#include "TauMathLibInternal.h"
#include <xmmintrin.h>

#if __cplusplus
extern "C" {
#endif

EXTERN __m128 vector3f_add(__m128 a, __m128 b) NOEXCEPT;
EXTERN __m128 vector3f_sub(__m128 a, __m128 b) NOEXCEPT;
EXTERN __m128 vector3f_mul(__m128 a, __m128 b) NOEXCEPT;
EXTERN __m128 vector3f_div(__m128 a, __m128 b) NOEXCEPT;

EXTERN __m128 vector3f_addScalar(__m128 a, float b) NOEXCEPT;
EXTERN __m128 vector3f_subScalar(__m128 a, float b) NOEXCEPT;
EXTERN __m128 vector3f_mulScalar(__m128 a, float b) NOEXCEPT;
EXTERN __m128 vector3f_divScalar(__m128 a, float b) NOEXCEPT;

EXTERN __m128 vector3f_divScalarInv(float a, __m128 b) NOEXCEPT;

EXTERN __m128 vector3f_neg(__m128 a) NOEXCEPT;

EXTERN float vector3f_magnitudeSquared(__m128 a) NOEXCEPT;
EXTERN float vector3f_magnitude(__m128 a) NOEXCEPT;
EXTERN float vector3f_inverseMagnitude(__m128 a) NOEXCEPT;

EXTERN __m128 vector3f_normalizeFast(__m128 a) NOEXCEPT;
EXTERN __m128 vector3f_normalizeExact(__m128 a) NOEXCEPT;

EXTERN float vector3f_dot(__m128 a, __m128 b) NOEXCEPT;

EXTERN __m128 vector3f_cross(__m128 a, __m128 b) NOEXCEPT;

#if __cplusplus
}
#endif
