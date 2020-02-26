#pragma once

#include "maths/Maths.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "maths/Vector3f.hpp"
#include "RenderingMode.hpp"

namespace glmExt {

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

template<typename _T, glm::qualifier _Q>
GLM_FUNC_QUALIFIER glm::mat<4, 4, _T, _Q> transpose(glm::mat<4, 4, _T, _Q> const& mat)
{
    glm::mat<4, 4, _T, _Q> ret;

    ret[0][0] = mat[0][0];
    ret[0][1] = mat[1][0];
    ret[0][2] = mat[2][0];
    ret[0][3] = mat[3][0];

    ret[1][0] = mat[0][1];
    ret[1][1] = mat[1][1];
    ret[1][2] = mat[2][1];
    ret[1][3] = mat[3][1];

    ret[2][0] = mat[0][2];
    ret[2][1] = mat[1][2];
    ret[2][2] = mat[2][2];
    ret[2][3] = mat[3][2];

    ret[3][0] = mat[0][3];
    ret[3][1] = mat[1][3];
    ret[3][2] = mat[2][3];
    ret[3][3] = mat[3][3];

    return ret;
}

template<typename _T, glm::qualifier _Q>
GLM_FUNC_QUALIFIER glm::mat<3, 3, _T, _Q> transpose(glm::mat<3, 3, _T, _Q> const& mat)
{
    glm::mat<3, 3, _T, _Q> ret;

    ret[0][0] = mat[0][0];
    ret[0][1] = mat[1][0];
    ret[0][2] = mat[2][0];

    ret[1][0] = mat[0][1];
    ret[1][1] = mat[1][1];
    ret[1][2] = mat[2][1];

    ret[2][0] = mat[0][2];
    ret[2][1] = mat[1][2];
    ret[2][2] = mat[2][2];

    ret[3][0] = mat[0][3];
    ret[3][1] = mat[1][3];
    ret[3][2] = mat[2][3];

    return ret;
}

template<typename _T, glm::qualifier _Q>
GLM_FUNC_QUALIFIER glm::mat<2, 2, _T, _Q> transpose(glm::mat<2, 2, _T, _Q> const& mat)
{
    glm::mat<2, 2, _T, _Q> ret;

    ret[0][0] = mat[0][0];
    ret[0][1] = mat[1][0];

    ret[1][0] = mat[0][1];
    ret[1][1] = mat[1][1];

    ret[2][0] = mat[0][2];
    ret[2][1] = mat[1][2];

    ret[3][0] = mat[0][3];
    ret[3][1] = mat[1][3];

    return ret;
}

static inline bool useTranspose(const RenderingMode::Mode mode) noexcept
{
    switch(mode)
    {
        case RenderingMode::Mode::DirectX9:
        case RenderingMode::Mode::DirectX10:
        case RenderingMode::Mode::DirectX11:
        case RenderingMode::Mode::DirectX12:
        case RenderingMode::Mode::DirectX12_1:
            return true;
        case RenderingMode::Mode::Vulkan:
            return false;
        case RenderingMode::Mode::OpenGL2:
        case RenderingMode::Mode::OpenGL3:
        case RenderingMode::Mode::OpenGL3_1:
        case RenderingMode::Mode::OpenGL3_2:
        case RenderingMode::Mode::OpenGL3_3:
        case RenderingMode::Mode::OpenGL4:
        case RenderingMode::Mode::OpenGL4_2:
        case RenderingMode::Mode::OpenGL4_3:
        case RenderingMode::Mode::OpenGL4_4:
        case RenderingMode::Mode::OpenGL4_5:
        case RenderingMode::Mode::OpenGL4_6:
            return false;
        default: return false;
    }
}

}
