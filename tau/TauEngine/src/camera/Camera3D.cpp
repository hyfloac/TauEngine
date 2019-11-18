#include "camera/Camera3D.hpp"
#include "maths/GlmMatrixTransformExt.hpp"
#include "maths/Maths.hpp"
#include "system/Window.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

Camera3D::Camera3D(const Window& window, const float fov, const float zNear, const float zFar) noexcept
    : _position(0.0f), _pitch(0.0f), _yaw(0.0f), _viewQuaternion(),
      _projectionMatrix(glm::perspective(fov, static_cast<float>(window.width()) / static_cast<float>(window.height()), zNear, zFar)),
      _viewMatrix(1.0f), _compoundedMatrix(_projectionMatrix * _viewMatrix)
{ }

void Camera3D::setProjection(const Window& window, float fov, float zNear, float zFar) noexcept
{
    _projectionMatrix = glm::perspective(fov, static_cast<float>(window.width()) / static_cast<float>(window.height()), zNear, zFar);
    _compoundedMatrix = _projectionMatrix * _viewMatrix;
}

void Camera3D::recomputeMatrices() noexcept
{
    static const glm::mat4 identity(1.0f);
    // static const glm::vec3 xAxis(-1.0f, 0.0f, 0.0f);
    // static const glm::vec3 yAxis(0.0f, 1.0f, 0.0f);

    const glm::mat4 transform = glmExt::translate(identity, _position) * 
                                glm::toMat4(_viewQuaternion);
    _viewMatrix = glm::inverse(transform);
    _compoundedMatrix = _projectionMatrix * _viewMatrix;
}

static float largeLerp(const float v0, const float v1, const float t, const float max_t) noexcept
{
    return ((max_t - t) * v0 + t * v1) / max_t;
}

void FPSCamera3DController::update(float fixedDelta) noexcept
{
    const Vector3f velocitySquared = _velocity.mulC(_velocity);
    const Vector3f forceAir = velocitySquared * FORCE_AIR;

    if(Keyboard::isKeyPressed(_keyForwards))
    {
        const float forceForward = _walkForwardAcceleration * PLAYER_MASS_G - forceAir.x();
        _acceleration.y() = forceForward / PLAYER_MASS_G;
    }

    const float forceY = FORCE_GRAVITY - forceAir.y();

    _acceleration.y() = forceY / PLAYER_MASS;

    _velocity += _acceleration * fixedDelta / 1000000.0f;

    _camera._position += _velocity * fixedDelta / 1000000.0f;
}

void FreeCamCamera3DController::update(const float fixedDelta, const Vector3f velocity, const i32 dMouseX, const i32 dMouseY) noexcept
{
    _lerp = 0.0f;
    _maxLerp = fixedDelta;

    if(_nextYaw > 180.0f) { _nextYaw -= 360.0f; }
    else if(_nextYaw < -180.0f) { _nextYaw += 360.0f; }

    _camera._pitch = _nextPitch;
    _camera._yaw = _nextYaw;

    _lastPitch = _nextPitch;
    _lastYaw = _nextYaw;

    if(dMouseX != 0 || dMouseY != 0)
    {
        const float speed = _rotateSpeed * fixedDelta / 1000.0f;

        _nextPitch += dMouseY * speed;
        _nextYaw += dMouseX * speed;

        if(_nextPitch > 80.0f) { _nextPitch = 80.0f; }
        else if(_nextPitch < -87.0f) { _nextPitch = -87.0f; }

        _camera._viewQuaternion = glm::quat(glm::vec3(DEG_2_RAD_F(_camera._pitch), DEG_2_RAD_F(_camera._yaw), 0.0f));
    }

    const Vector3f deltaVelocity = (velocity * fixedDelta) / 1000000.0f;

    const bool dvx = deltaVelocity.x() != 0.0f;
    const bool dvy = deltaVelocity.y() != 0.0f;
    const bool dvz = deltaVelocity.z() != 0.0f;

    _camera._position = _nextPos;
    _lastPos = _nextPos;

    if(dvx || dvy || dvz)
    {
        _nextPos.y() += deltaVelocity.y();

        const SinCos<float> yawSC = fastSinCosD(_nextYaw);

        if(_lookY)
        {
            // const SinCos<float> pitchSC = fastSinCosD(_nextPitch);

            if(dvx)
            {
                _nextPos.x() -= deltaVelocity.x() * -yawSC.sin;
                _nextPos.z() += deltaVelocity.x() * yawSC.cos;
            }

            if(dvz)
            {
                _nextPos.z() -= deltaVelocity.z() * yawSC.sin;
                _nextPos.x() += deltaVelocity.z() * yawSC.cos;
            }
        }
        else
        {
            if(dvx)
            {
                _nextPos.x() -= deltaVelocity.x() * -yawSC.sin;
                _nextPos.z() += deltaVelocity.x() * yawSC.cos;
            }

            if(dvz)
            {
                _nextPos.z() -= deltaVelocity.z() * yawSC.sin;
                _nextPos.x() += deltaVelocity.z() * yawSC.cos;
            }
        }
    }
    _camera.recomputeMatrices();
}

void FreeCamCamera3DController::update(const float fixedDelta, const i32 dMouseX, const i32 dMouseY) noexcept
{
    checkKeys();
    if(_recorder && _recorder->recording())
    {
        _recorder->addBlip(GameRecorder::Blip { GameRecorder::BlipType::Update, cameraBlip(), new BlipDataUpdate{ fixedDelta, _velocity, dMouseX, dMouseY } });
    }
    update(fixedDelta, _velocity, dMouseX, dMouseY);
}

void FreeCamCamera3DController::lerp(float delta) noexcept
{
    _lerp = minT(_lerp + delta, _maxLerp);

    {
        _camera._pitch = largeLerp(_lastPitch, _nextPitch, _lerp, _maxLerp);
        _camera._yaw = largeLerp(_lastYaw, _nextYaw, _lerp, _maxLerp);
        _camera._viewQuaternion = glm::quat(glm::vec3(DEG_2_RAD_F(_camera._pitch), DEG_2_RAD_F(_camera._yaw), 0.0f));
    }

    {
        _camera._position = Vector3f::largeLerp(_lastPos, _nextPos, _lerp, _maxLerp);
    }

    _camera.recomputeMatrices();
}

void FreeCamCamera3DController::checkKeys() noexcept
{
    _velocity = 0.0f;

    float speed = _normalSpeed;

    if(Keyboard::isKeyPressed(_keyAccelerate))
    { speed = _fastSpeed; }

    if(Keyboard::isKeyPressed(_keyBackwards)) { _velocity.x() += speed; }
    if(Keyboard::isKeyPressed(_keyForwards))  { _velocity.x() -= speed; }
    if(Keyboard::isKeyPressed(_keyUp))        { _velocity.y() += speed; }
    if(Keyboard::isKeyPressed(_keyDown))      { _velocity.y() -= speed; }
    if(Keyboard::isKeyPressed(_keyRight))     { _velocity.z() += speed; }
    if(Keyboard::isKeyPressed(_keyLeft))      { _velocity.z() -= speed; }
}

bool FreeCamCamera3DController::blipHandler(GameRecorder::Blip& blip, void* userParam) noexcept
{
    FreeCamCamera3DController& controller = *reinterpret_cast<FreeCamCamera3DController*>(userParam);
    if(blip.type == GameRecorder::BlipType::Update)
    {
        BlipDataUpdate* data = reinterpret_cast<BlipDataUpdate*>(blip.data);
        controller.update(data->delta, data->velocity, data->dMouseX, data->dMouseY);
    }
    // else if(blip.type == GameRecorder::BlipType::Render)
    // {
    //     BlipDataRender* data = reinterpret_cast<BlipDataRender*>(blip.data);
    //     controller.updateRotation(data->delta, data->dMouseX, data->dMouseY);
    // }
    else if(blip.type == GameRecorder::BlipType::Initial)
    {
        BlipDataInitial* data = reinterpret_cast<BlipDataInitial*>(blip.data);
        controller._camera.set(data->position, data->pitch, data->yaw);
        controller._rotateSpeed = data->rotateSpeed;
    }
    return true;
}

GameRecorder::Blip FreeCamCamera3DController::initialBlip(void* userParam) noexcept
{
    FreeCamCamera3DController& controller = *reinterpret_cast<FreeCamCamera3DController*>(userParam);
    return { GameRecorder::BlipType::Initial, cameraBlip(), new BlipDataInitial{
        controller._camera._position,
        controller._camera._pitch,
        controller._camera._yaw,
        controller._rotateSpeed,
    } };
}

RunTimeType<GameRecorder::Blip> FreeCamCamera3DController::cameraBlip() noexcept
{
    static RunTimeType<GameRecorder::Blip> type = RunTimeType<GameRecorder::Blip>::define();
    return type;
}
