#pragma once

#include <maths/Maths.hpp>
#include <maths/Vector3f.hpp>
#include <maths/Matrix4x4f.hpp>
#include <events/WindowEvent.hpp>

class Camera final
{
private:
    static constexpr float WALK_FORWARD_VELOCITY = 1.25f;
    static constexpr float RUN_FORWARD_VELOCITY = 4.25f;
    static constexpr float MOUSE_SCALE = 0.3f;

    Vector3f _position;
    Vector3f _velocity;
    float _pitch;
    float _yaw;
    Matrix4x4f _viewMatrix;
    Matrix4x4f _projectionMatrix;
public:
    inline Camera(Vector3f position = { 0.0f, 0.0f, 0.0f }, float pitch = 0.0f, float yaw = 0.0f) noexcept
        : _position(position), _velocity(0.0f), _pitch(pitch), _yaw(yaw)
    { }

    inline ~Camera() noexcept = default;

    inline Camera(const Camera& copy) noexcept = default;
    inline Camera(Camera&& move) noexcept = default;

    inline Camera& operator =(const Camera& copy) noexcept = default;
    inline Camera& operator =(Camera&& move) noexcept = default;

    void update(const float fixedDelta) noexcept;

    void updateRotation(const float delta, const i32 dMouseX, const i32 dMouseY) noexcept;

    void checkKeys() noexcept;

    [[nodiscard]] inline Vector3f pos() const noexcept { return _position; }
    [[nodiscard]] inline Vector3f position() const noexcept { return _position; }
    [[nodiscard]] inline Vector3f vel() const noexcept { return _velocity; }
    [[nodiscard]] inline Vector3f velocity() const noexcept { return _velocity; }
    [[nodiscard]] inline float pitch() const noexcept { return _pitch; }
    [[nodiscard]] inline float yaw() const noexcept { return _yaw; }
    [[nodiscard]] inline const Matrix4x4f& viewMatrix() const noexcept { return _viewMatrix; }

    void pos(const Vector3f position) noexcept
    {
        _position = position;
        recomputeViewMatrix();
    }

    void position(const Vector3f position) noexcept
    { pos(position); }

    void vel(const Vector3f velocity) noexcept
    {
        _velocity = velocity;
        recomputeViewMatrix();
    }

    void velocity(const Vector3f velocity) noexcept
    { vel(velocity); }

    void pitch(const float pitch) noexcept
    {
        _pitch = pitch;
        recomputeViewMatrix();
    }

    void yaw(const float yaw) noexcept
    {
        _yaw = yaw;
        recomputeViewMatrix();
    }

    void set(const float pitch, const float yaw) noexcept
    {
        _pitch = pitch;
        _yaw = yaw;
        recomputeViewMatrix();
    }

    void set(const Vector3f position, const float pitch, const float yaw) noexcept
    {
        _position = position;
        _pitch = pitch;
        _yaw = yaw;
        recomputeViewMatrix();
    }

    void reset() noexcept
    {
        _position = 0.0f;
        _pitch = 0.0f;
        _yaw = 0.0f;
        recomputeViewMatrix();
    }
private:
    void recomputeViewMatrix() noexcept
    { _viewMatrix.fpsD(_position, _pitch, _yaw); }

    void recomputeProjectionMatrix(WindowResizeEvent& e) noexcept
    { _projectionMatrix = Matrix4x4f::perspective(DEG_2_RAD(90), static_cast<float>(e.newWidth()) / static_cast<float>(e.newHeight()), 0.0001f, 1000.0f); }
};
