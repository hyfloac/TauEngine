#pragma once

#include "TauMathLibInternal.h"
#include "Vector2fBase.h"

#if __cplusplus
extern "C" {
#endif

EXTERN void vector2f_add(const _Vector2f* a, const _Vector2f* b, _Vector2f* store) NOEXCEPT;
EXTERN void vector2f_sub(const _Vector2f* a, const _Vector2f* b, _Vector2f* store) NOEXCEPT;
EXTERN void vector2f_mul(const _Vector2f* a, const _Vector2f* b, _Vector2f* store) NOEXCEPT;
EXTERN void vector2f_div(const _Vector2f* a, const _Vector2f* b, _Vector2f* store) NOEXCEPT;

EXTERN void vector2f_addScalar(const _Vector2f* a, float b, _Vector2f* store) NOEXCEPT;
EXTERN void vector2f_subScalar(const _Vector2f* a, float b, _Vector2f* store) NOEXCEPT;
EXTERN void vector2f_mulScalar(const _Vector2f* a, float b, _Vector2f* store) NOEXCEPT;
EXTERN void vector2f_divScalar(const _Vector2f* a, float b, _Vector2f* store) NOEXCEPT;

EXTERN void vector2f_divScalarInv(float a, const _Vector2f* b, _Vector2f* store) NOEXCEPT;

EXTERN void vector2f_neg(const _Vector2f* a, _Vector2f* store) NOEXCEPT;

EXTERN float vector2f_magnitudeSquared(const _Vector2f* a) NOEXCEPT;
EXTERN float vector2f_magnitude(const _Vector2f* a) NOEXCEPT;
EXTERN float vector2f_inverseMagnitude(const _Vector2f* a) NOEXCEPT;

EXTERN void vector2f_normalizeFast(const _Vector2f* a, _Vector2f* store) NOEXCEPT;
EXTERN void vector2f_normalizeExact(const _Vector2f* a, _Vector2f* store) NOEXCEPT;

EXTERN float vector2f_dot(const _Vector2f* a, const _Vector2f* b) NOEXCEPT;

#if __cplusplus
}
#endif
