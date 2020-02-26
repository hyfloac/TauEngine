#pragma once

#include <glm/gtx/quaternion.hpp>
#include "maths/Maths.hpp"

namespace glmExt {

template<typename _T, glm::qualifier _Q>
GLM_FUNC_QUALIFIER glm::qua<_T, _Q> rotateDegrees(glm::qua<_T, _Q> const& q, const _T angle, glm::vec<3, _T, _Q> v) noexcept
{
    static constexpr _T ONE = static_cast<_T>(1);
    static constexpr _T HALF = static_cast<_T>(0.5);

    const _T len = glm::length(v);

    if(glm::abs(len - ONE) > static_cast<_T>(0.001))
    {
        const _T oneOverLen = ONE / len;
        v *= oneOverLen;
    }

    const SinCos<_T> sinCos = fastSinCosD(angle * HALF);

    v *= sinCos.sin;

    const glm::qua<_T, _Q> tmp(sinCos.cos, v.x, v.y, v.z);
    return q * tmp;
}

}
