#pragma once

#include "TauMathLibInternal.h"
#include <xmmintrin.h>

#if __cplusplus
extern "C" {
#endif

EXTERN void matrix4x4f_transpose(const float* a, float* store) NOEXCEPT;

EXTERN void matrix4x4f_add(const float* a, const float* b, float* store) NOEXCEPT;
EXTERN void matrix4x4f_sub(const float* a, const float* b, float* store) NOEXCEPT;
EXTERN void matrix4x4f_mul(const float* a, const float* b, float* store) NOEXCEPT;

EXTERN void matrix4x4f_addScalar(const float* a, float b, float* store) NOEXCEPT;
EXTERN void matrix4x4f_subScalar(const float* a, float b, float* store) NOEXCEPT;
EXTERN void matrix4x4f_mulScalar(const float* a, float b, float* store) NOEXCEPT;
EXTERN void matrix4x4f_divScalar(const float* a, float b, float* store) NOEXCEPT;
EXTERN void matrix4x4f_divFastScalar(const float* a, float b, float* store) NOEXCEPT;

EXTERN __m128 matrix4x4f_mulVector(const float* a, __m128 b) noexcept;

#if __cplusplus
}
#endif
