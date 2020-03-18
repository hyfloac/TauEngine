#include "camera/Camera2D.hpp"
#include "maths/glmExt/GlmMatrixTransformExt.hpp"
#include "maths/glmExt/GlmMatrixProjectionExt.hpp"
#include "system/Window.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera2D::Camera2D(const Window& window, float nearZ, float farZ) noexcept
    : _position(0.0f), _rotation(0.0f),
      _projectionMatrix(glmExt::ortho(static_cast<float>(window.width()), static_cast<float>(window.height()), nearZ, farZ)),
      _viewMatrix(1.0f), _compoundedMatrix(_projectionMatrix * _viewMatrix),
      _viewMatrixTrans(glm::transpose(_viewMatrix)),
      _compoundedMatrixTrans(_projectionMatrix * _viewMatrixTrans)
{ }

Camera2D::Camera2D(float width, float height, float nearZ, float farZ) noexcept
    : _position(0.0f), _rotation(0.0f),
      _projectionMatrix(glmExt::ortho(width, height, nearZ, farZ)),
      _viewMatrix(1.0f), _compoundedMatrix(_projectionMatrix * _viewMatrix),
      _viewMatrixTrans(glm::transpose(_viewMatrix)),
      _compoundedMatrixTrans(_projectionMatrix * _viewMatrixTrans)
{ }

void Camera2D::setProjection(const Window& window, float nearZ, float farZ) noexcept
{
    // _projectionMatrix = glm::ortho(left, static_cast<float>(window.width()), bottom, static_cast<float>(window.height()), -1.0f, 1.0f);
    _projectionMatrix = glmExt::ortho(static_cast<float>(window.width()), static_cast<float>(window.height()), nearZ, farZ);
    _compoundedMatrix = _projectionMatrix * _viewMatrix;

    _compoundedMatrixTrans = _projectionMatrix * _viewMatrixTrans;
}

void Camera2D::setProjection(float width, float height, float nearZ, float farZ) noexcept
{
    // _projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    _projectionMatrix = glmExt::ortho(width, height, nearZ, farZ);
    _compoundedMatrix = _projectionMatrix * _viewMatrix;

    _compoundedMatrixTrans = _projectionMatrix * _viewMatrixTrans;
}

void Camera2D::recomputeMatrices() noexcept
{
    static constexpr glm::vec3 axis(0.0f, 0.0f, 1.0f);
    static constexpr glm::mat4 identity(1.0f);

    const glm::mat4 transform = glmExt::translate(identity, _position) * 
                                glmExt::rotateDegrees(identity, _rotation, axis);
    _viewMatrix = glm::inverse(transform);
    _compoundedMatrix = _projectionMatrix * _viewMatrix;

    glmExt::transpose(_viewMatrix, _viewMatrixTrans);
    _compoundedMatrixTrans = _projectionMatrix * _viewMatrixTrans;
}

void Camera2DController::update(const float fixedDelta) noexcept
{
    const float deltaMove = _moveSpeed * fixedDelta / 1000000.0f;
    const float deltaRotate = _rotateSpeed * fixedDelta / 1000000.0f;

    Vector3f& pos = _camera._position;
    float& rot = _camera._rotation;

    const bool rotC = Keyboard::isKeyPressed(_keyRotateClockwise);
    const bool kUp = Keyboard::isKeyPressed(_keyUp);
    const bool kLeft = Keyboard::isKeyPressed(_keyLeft);

    if(rotC ^ Keyboard::isKeyPressed(_keyRotateCounterClockwise))
    {
        if(rotC)
        { rot -= deltaRotate; }
        else
        { rot += deltaRotate; }

        if(rot >= 360.0f)
        { rot -= 360.0f; }
        else if(rot < 0.0f)
        { rot += 360.0f; }
    }

    const SinCos sc = fastSinCosD(rot);
    const float _sin = sc.sin * deltaMove;
    const float _cos = sc.cos * deltaMove;

    if(kUp ^ Keyboard::isKeyPressed(_keyDown))
    {
        if(kUp)
        {
            pos.x() += -_sin;
            pos.y() += _cos;
        }
        else
        {
            pos.x() -= -_sin;
            pos.y() -= _cos;
        }
    }

    if(kLeft ^ Keyboard::isKeyPressed(_keyRight))
    {
        if(kLeft)
        {
            pos.x() -= _cos;
            pos.y() -= _sin;
        }
        else
        {
            pos.x() += _cos;
            pos.y() += _sin;
        }
    }

    _camera.recomputeMatrices();
}
