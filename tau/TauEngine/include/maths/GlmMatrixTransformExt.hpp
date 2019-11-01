#pragma once

#include "Maths.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "Vector3f.hpp"

namespace glmExt
{

template<typename _T, glm::qualifier _Q>
GLM_FUNC_QUALIFIER glm::mat<4, 4, _T, _Q> translate(glm::mat<4, 4, _T, _Q> const& m, Vector3f v)
{
    glm::mat<4, 4, _T, _Q> Result(m);
    Result[3] = m[0] * v.x() + m[1] * v.y() + m[2] * v.z() + m[3];
    return Result;
}

template<typename _T, glm::qualifier _Q>
GLM_FUNC_QUALIFIER glm::mat<4, 4, _T, _Q> rotateDegrees(glm::mat<4, 4, _T, _Q> const& m, _T angleDegrees, glm::vec<3, _T, _Q> const& v)
{
    // T const a = angle;
    SinCos sc = fastSinCosD(angleDegrees);
    _T const c = _T(sc.cos);
    _T const s = _T(sc.sin);

    glm::vec<3, _T, _Q> axis(normalize(v));
    glm::vec<3, _T, _Q> temp((_T(1) - c) * axis);

    glm::mat<4, 4, _T, _Q> Rotate;
    Rotate[0][0] = c + temp[0] * axis[0];
    Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
    Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

    Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
    Rotate[1][1] = c + temp[1] * axis[1];
    Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

    Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
    Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
    Rotate[2][2] = c + temp[2] * axis[2];

    glm::mat<4, 4, _T, _Q> Result;
    Result[0] = m[0] * Rotate[0][0] + m[1] * Rotate[0][1] + m[2] * Rotate[0][2];
    Result[1] = m[0] * Rotate[1][0] + m[1] * Rotate[1][1] + m[2] * Rotate[1][2];
    Result[2] = m[0] * Rotate[2][0] + m[1] * Rotate[2][1] + m[2] * Rotate[2][2];
    Result[3] = m[3];
    return Result;
}

}
