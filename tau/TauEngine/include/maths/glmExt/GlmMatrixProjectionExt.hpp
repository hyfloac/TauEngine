#pragma once

#include "maths/Maths.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace glmExt {

template<typename _T>
GLM_FUNC_QUALIFIER glm::mat<4, 4, _T, glm::defaultp> ortho(const _T width, const _T height, const _T nearZ, const _T farZ) noexcept
{
    static constexpr _T ZERO = static_cast<_T>(0);
    static constexpr _T ONE = static_cast<_T>(1);
    static constexpr _T TWO = static_cast<_T>(2);

    glm::mat<4, 4, _T, glm::defaultp> ret(ZERO);

    const _T fRange = 1.0f / (farZ - nearZ);

    ret[0][0] = TWO / width;
    ret[0][1] = ZERO;
    ret[0][2] = ZERO;
    ret[0][3] = ZERO;

    ret[1][0] = ZERO;
    ret[1][1] = TWO / height;
    ret[1][2] = ZERO;
    ret[1][3] = ZERO;

    ret[2][0] = ZERO;
    ret[2][1] = ZERO;
    ret[2][2] = fRange;
    ret[2][3] = ZERO;

    ret[3][0] = ZERO;
    ret[3][1] = ZERO;
    ret[3][2] = -fRange * nearZ;
    ret[3][3] = ONE;

    return ret;
}

template<typename _T>
GLM_FUNC_QUALIFIER glm::mat<4, 4, _T, glm::defaultp> perspectiveDegrees(const _T fov, const _T width, const _T height, const _T nearZ, const _T farZ) noexcept
{
    static constexpr _T ZERO = static_cast<_T>(0);
    static constexpr _T ONE = static_cast<_T>(1);
    static constexpr _T TWO = static_cast<_T>(2);
    static constexpr _T HALF = static_cast<_T>(0.5);

    const _T cotangent = -fastCotD(fov * HALF);
    const _T aspect = height / width;
    const _T diff = farZ - nearZ;

    glm::mat<4, 4, _T, glm::defaultp> ret(ZERO);

    ret[0][0] = aspect * cotangent;
    ret[0][1] = ZERO;
    ret[0][2] = ZERO;
    ret[0][3] = ZERO;

    ret[1][0] = ZERO;
    ret[1][1] = cotangent;
    ret[1][2] = ZERO;
    ret[1][3] = ZERO;

    ret[2][0] = ZERO;
    ret[2][1] = ZERO;
    ret[2][2] = -(farZ + nearZ) / diff;
    ret[2][3] = -ONE;

    ret[3][0] = ZERO;
    ret[3][1] = ZERO;
    ret[3][2] = -(TWO * farZ * nearZ) / diff;
    ret[3][3] = ZERO;

    return ret;
}

}
