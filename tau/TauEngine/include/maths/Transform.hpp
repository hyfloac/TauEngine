#pragma once

#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform final
{
    glm::mat4 matrix;
    glm::quat rotation;
    glm::vec3 position;
    glm::vec3 scale;

    Transform(const glm::vec3 _position = glm::vec3(0.0f), const glm::quat _rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), const glm::vec3 _scale = glm::vec3(1.0f))
        : matrix(1.0f)
        , rotation(_rotation)
        , position(_position)
        , scale(_scale)
    { }

    void recomputeMatrix() noexcept
    {
        glm::mat4 mat(1.0f);
        mat = glm::translate(mat, position);
        mat = glm::scale(mat, scale);
        const glm::mat4 rotMatrix = glm::toMat4(rotation);
        matrix = mat * rotMatrix;
    }
};
