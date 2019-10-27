#include <Camera.hpp>
#include <Windows.h>

void Camera::update(const float fixedDelta) noexcept
{
    checkKeys();

    const Vector3f deltaVelocity = _velocity * (fixedDelta / 1000000.0f);

    _position.y() += deltaVelocity.y();

    // SinCos<float> pitchSC = fastSinCosD(_pitch);
    const SinCos<float> yawSC   = fastSinCosD(_yaw);

    if(deltaVelocity.x() != 0.0f)
    {
        _position.x() -= deltaVelocity.x() * yawSC.sin;
        _position.z() += deltaVelocity.x() * yawSC.cos;
    }

    if(deltaVelocity.z() != 0.0f)
    {
        _position.z() += deltaVelocity.z() * yawSC.sin;
        _position.x() += deltaVelocity.z() * yawSC.cos;
    }

    // pitchSC.negate();
    // yawSC.negate();

    // _viewMatrix.fps(_position, pitchSC, yawSC);
    recomputeViewMatrix();
}

void Camera::updateRotation(const float delta, const i32 dMouseX, const i32 dMouseY) noexcept
{
    if(dMouseX != 0 && dMouseY != 0)
    {
        const float speed = MOUSE_SCALE * delta / 1000.0f;
        _pitch += dMouseY * speed;
        _yaw += dMouseX * speed;

        if(_pitch > 80.0f)  { _pitch = 80.0f; }
        if(_pitch < -87.0f) { _pitch = -87.0f; }

        if(_yaw > 360.0f) { _yaw = fmod(_yaw, 360.0f); }
        else if(_yaw < 0.0f)
        {
            _yaw = fmod(_yaw, 360.0f);
            _yaw = 360.0f - _yaw;
        }

        // _viewMatrix.fpsD(_position, _pitch, _yaw);
        recomputeViewMatrix();
    }
}

void Camera::checkKeys() noexcept
{
    _velocity = 0.0f;

    float speed = WALK_FORWARD_VELOCITY;

    if(GetKeyState(VK_SHIFT) & 0x8000)
    {
        speed = RUN_FORWARD_VELOCITY;
    }

    if(GetKeyState(0x44) & 0x8000) /* d */ { _velocity.z() += speed; }
    if(GetKeyState(0x41) & 0x8000) /* a */ { _velocity.z() -= speed; }
    if(GetKeyState(0x57) & 0x8000) /* w */ { _velocity.x() += speed; }
    if(GetKeyState(0x53) & 0x8000) /* s */ { _velocity.x() -= speed; }
    if(GetKeyState(VK_SPACE)   & 0x8000)   { _velocity.y() += speed; }
    if(GetKeyState(VK_CONTROL) & 0x8000)   { _velocity.y() -= speed; }
}
