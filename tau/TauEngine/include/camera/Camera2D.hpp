/**
 * @file
 */
#pragma once

#include <glm/mat4x4.hpp>
#include "maths/Vector3f.hpp"
#include <Objects.hpp>
#include "system/Keyboard.hpp"

class Window;

class Camera2D final
{
    DEFAULT_DESTRUCT(Camera2D);
    DEFAULT_COPY(Camera2D);
private:
    Vector3f _position;
    /**
     *  The rotation of the camera in degrees.
     */
    float _rotation;
    glm::mat4 _projectionMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _compoundedMatrix;
    // glm::mat4 _projectionMatrixTrans;
    glm::mat4 _viewMatrixTrans;
    glm::mat4 _compoundedMatrixTrans;
public:
    Camera2D(const Window& window, float nearZ = -1.0f, float farZ = 1.0f) noexcept;
    Camera2D(float width, float height, float nearZ = -1.0f, float farZ = 1.0f) noexcept;

    [[nodiscard]] Vector3f position() const noexcept { return _position; }
    [[nodiscard]] float rotation() const noexcept { return _rotation; }
    [[nodiscard]] const glm::mat4& projectionMatrix() const noexcept { return _projectionMatrix; }
    [[nodiscard]] const glm::mat4& viewMatrix() const noexcept { return _viewMatrix; }
    /** _projectionMatrix * _viewMatrix */
    [[nodiscard]] const glm::mat4& compoundedMatrix() const noexcept { return _compoundedMatrix; }
    // [[nodiscard]] const glm::mat4& projectionMatrixTrans() const noexcept { return _projectionMatrixTrans; }
    [[nodiscard]] const glm::mat4& viewMatrixTrans() const noexcept { return _viewMatrixTrans; }
    /** _projectionMatrixTrans * _viewMatrixTrans */
    [[nodiscard]] const glm::mat4& compoundedMatrixTrans() const noexcept { return _compoundedMatrixTrans; }

    void setProjection(const Window& window, float nearZ = -1.0f, float farZ = 1.0f) noexcept;
    void setProjection(float width, float height, float nearZ = -1.0f, float farZ = 1.0f) noexcept;

    void position(Vector3f pos) noexcept
    {
        _position = pos;
        recomputeMatrices();
    }

    void rotation(float rot) noexcept
    {
        _rotation = rot;
        recomputeMatrices();
    }

    void set(Vector3f pos, float rot)
    {
        _position = pos;
        _rotation = rot;
        recomputeMatrices();
    }
private:
    void recomputeMatrices() noexcept;
private:
    friend class Camera2DController;
};

class Camera2DController final
{
    DEFAULT_DESTRUCT(Camera2DController);
    DEFAULT_COPY(Camera2DController);
private:
    Camera2D _camera;
    float _moveSpeed;
    float _rotateSpeed;
    Keyboard::Key _keyUp;
    Keyboard::Key _keyDown;
    Keyboard::Key _keyLeft;
    Keyboard::Key _keyRight;
    Keyboard::Key _keyRotateClockwise;
    Keyboard::Key _keyRotateCounterClockwise;
public:
    Camera2DController(const Window& window, const float moveSpeed, const float rotateSpeed, 
                       const Keyboard::Key keyUp, const Keyboard::Key keyDown, 
                       const Keyboard::Key keyLeft, const Keyboard::Key keyRight,
                       const Keyboard::Key keyRotateClockwise, const Keyboard::Key keyRotateCounterClockwise) noexcept
        : _camera(window),
          _moveSpeed(moveSpeed), _rotateSpeed(rotateSpeed),
          _keyUp(keyUp), _keyDown(keyDown),
          _keyLeft(keyLeft), _keyRight(keyRight),
          _keyRotateClockwise(keyRotateClockwise),
          _keyRotateCounterClockwise(keyRotateCounterClockwise)
    { }

    void update(float fixedDelta) noexcept;

    [[nodiscard]] const Camera2D& camera() const noexcept { return _camera; }
    [[nodiscard]] Camera2D& camera() noexcept { return _camera; }

    [[nodiscard]] Camera2D* operator->() noexcept { return &_camera; }
    [[nodiscard]] const Camera2D* operator->() const noexcept { return &_camera; }
};
