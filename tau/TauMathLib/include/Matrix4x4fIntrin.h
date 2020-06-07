#pragma once

#include "TauMathLibInternal.h"

#if __cplusplus
extern "C" {
#endif

EXTERN void matrix4x4f_add(const float* a, const float* b, float* store) NOEXCEPT;
EXTERN void matrix4x4f_sub(const float* a, const float* b, float* store) NOEXCEPT;
EXTERN void matrix4x4f_mul(const float* a, const float* b, float* store) NOEXCEPT;

#if __cplusplus
}
#endif
