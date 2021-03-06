#include "camera/Camera3D.hpp"
#include "maths/glmExt/GlmMatrixTransformExt.hpp"
#include "maths/glmExt/GlmMatrixProjectionExt.hpp"
#include "maths/glmExt/GlmQuaternionTransformExt.hpp"
#include "maths/Maths.hpp"
#include "system/Window.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

Camera3D::Camera3D(const Window& window, const float fov, const float zNear, const float zFar) noexcept
    : _position(0.0f), _pitch(0.0f), _yaw(0.0f), _roll(0.0f), _viewQuaternion(),
      _projectionMatrix(glmExt::perspectiveDegrees(fov, static_cast<float>(window.width()), static_cast<float>(window.height()), zNear, zFar)),
      _viewMatrix(1.0f), _viewRotMatrix(1.0f), 
      _compoundedMatrix(_projectionMatrix * _viewMatrix)
{ }

void Camera3D::setProjection(float width, float height, float fov, float zNear, float zFar) noexcept
{
    _projectionMatrix = glmExt::perspectiveDegrees(fov, width, height, zNear, zFar);
    _compoundedMatrix = _projectionMatrix * _viewMatrix;
}

void Camera3D::setProjection(const Window& window, float fov, float zNear, float zFar) noexcept
{
    setProjection(static_cast<float>(window.width()), static_cast<float>(window.height()), fov, zNear, zFar);
}

void Camera3D::recomputeMatrices() noexcept
{
    static constexpr glm::mat4 identity(1.0f);

    const glm::mat4 rotMat = glm::toMat4(_viewQuaternion);

    const glm::mat4 transform = glmExt::translate(identity, _position) * rotMat;
    _viewMatrix = glm::inverse(transform);
    _compoundedMatrix = _projectionMatrix * _viewMatrix;

    _viewRotMatrix = glmExt::rotateDegrees(glm::inverse(rotMat), 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera3D::computeQuat() noexcept
{
    static constexpr glm::mat4 identity(1.0f);
    static constexpr glm::vec3 xAxis(-1.0f, 0.0f, 0.0f);
    static constexpr glm::vec3 yAxis(0.0f, -1.0f, 0.0f);
    static constexpr glm::vec3 zAxis(0.0f, -1.0f, -1.0f);

    const float pitch = 180.0f - _pitch;
    const glm::quat q0 = glmExt::rotateDegrees(identity, -_yaw, yAxis);
    const glm::quat q1 = glmExt::rotateDegrees(q0, -pitch, xAxis);
    const glm::quat q2 = glmExt::rotateDegrees(q1, _roll, zAxis);
    _viewQuaternion = q2;

    const SinCos<float> pitchSC = fastSinCosD(_pitch);
    const SinCos<float> yawSC = fastSinCosD(_yaw);

    _front.x() = yawSC.sin * pitchSC.cos;
    _front.y() = pitchSC.sin;
    _front.z() = yawSC.cos * pitchSC.cos;
    _front.normalize();
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

        _camera.computeQuat();
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

        const SinCos<float> yawSC = fastSinCosD(-_nextYaw);

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
                _nextPos.x() += deltaVelocity.x() * yawSC.sin;
                _nextPos.z() -= deltaVelocity.x() * yawSC.cos;
            }

            if(dvz)
            {
                _nextPos.z() -= deltaVelocity.z() * -yawSC.sin;
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

void FreeCamCamera3DController::lerp(const DeltaTime& delta) noexcept
{
    _lerp = minT(_lerp + delta.microseconds(), _maxLerp);

    {
        _camera._pitch = largeLerp(_lastPitch, _nextPitch, _lerp, _maxLerp);
        _camera._yaw = largeLerp(_lastYaw, _nextYaw, _lerp, _maxLerp);
        _camera.computeQuat();
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
    if(Keyboard::isKeyPressed(_keyLeft))      { _velocity.z() += speed; }
    if(Keyboard::isKeyPressed(_keyRight))     { _velocity.z() -= speed; }
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

void VRFreeCamCamera3DController::update(const DeltaTime& delta, const vr::TrackedDevicePose_t& pose) noexcept
{
    static constexpr float moveFactor = 1.0f;

    checkKeys();

    Vector3f rot(RAD_2_DEG(pose.vAngularVelocity.v[0]), RAD_2_DEG(pose.vAngularVelocity.v[1]), RAD_2_DEG(pose.vAngularVelocity.v[2]));
    rot /= 90.0f;
    // rot *= delta.microseconds();

    _camera._pitch += -rot.x();
    _camera._yaw += rot.y();
    _camera._roll += rot.z();

    // Vector3f vel(RAD_2_DEG(pose.vVelocity.v[0]), RAD_2_DEG(pose.vVelocity.v[1]), RAD_2_DEG(pose.vVelocity.v[2]));
    // vel *= moveFactor;

    // const Vector3f deltaVelocity = ((_velocity * delta.microseconds()) / 1000000.0f) + vel;
    //
    // const bool dvx = deltaVelocity.x() != 0.0f;
    // const bool dvy = deltaVelocity.y() != 0.0f;
    // const bool dvz = deltaVelocity.z() != 0.0f;
    //
    // if(dvx || dvy || dvz)
    // {
    //     _camera._position.y() -= deltaVelocity.y();
    //
    //     const SinCos<float> yawSC = fastSinCosD(_yaw);
    //
    //     if(dvx)
    //     {
    //         _camera._position.x() += deltaVelocity.x() * yawSC.sin;
    //         _camera._position.z() -= deltaVelocity.x() * yawSC.cos;
    //     }
    //
    //     if(dvz)
    //     {
    //         _camera._position.z() -= deltaVelocity.z() * -yawSC.sin;
    //         _camera._position.x() += deltaVelocity.z() * yawSC.cos;
    //     }
    // }
    _camera.computeQuat();
    _camera.recomputeMatrices();

    computeMatrix(vr::Eye_Left, pose);
    computeMatrix(vr::Eye_Right, pose);
}

void VRFreeCamCamera3DController::checkKeys() noexcept
{
    _velocity = 0.0f;

    float speed = _normalSpeed;

    if constexpr(false)
    {
        speed = _fastSpeed;
    }

    vr::InputAnalogActionData_t analogData;
    if(vr::VRInput()->GetAnalogActionData(_movePlane, &analogData, sizeof(analogData), vr::k_ulInvalidInputValueHandle) == vr::VRInputError_None && analogData.bActive)
    {
        // Pushing forward on the joystick increases its y value,
        // but forward movement is on the x axis.
        _velocity.x() = analogData.y;
        _velocity.z() = analogData.x;
    }

    if(buttonClicked(_moveUp)) { _velocity.y() += speed; }
    if(buttonClicked(_moveDown)) { _velocity.y() -= speed; }
}

bool VRFreeCamCamera3DController::buttonClicked(vr::VRActionHandle_t action, vr::VRInputValueHandle_t* devicePath) noexcept
{
    vr::InputDigitalActionData_t actionData;
    vr::VRInput()->GetDigitalActionData(action, &actionData, sizeof(actionData), vr::k_ulInvalidInputValueHandle);
    if(devicePath)
    {
        *devicePath = vr::k_ulInvalidInputValueHandle;
        if(actionData.bActive)
        {
            vr::InputOriginInfo_t originInfo;
            if(vr::VRInput()->GetOriginTrackedDeviceInfo(actionData.activeOrigin, &originInfo, sizeof(originInfo)) == vr::VRInputError_None)
            {
                *devicePath = originInfo.devicePath;
            }
        }
    }
    return actionData.bActive && actionData.bState;
}

glm::mat4 VRFreeCamCamera3DController::convertMatrix(const vr::HmdMatrix34_t& matPose) noexcept
{
    glm::mat4 mat(1.0f);

    for(uSys y = 0; y < 4; ++y)
    {
        for(uSys x = 0; x < 3; ++x)
        {
            mat[x][y] = matPose.m[x][y];
        }
        mat[3][y] = 0.0f;
    }
    mat[3][3] = 1.0f;

    return glm::inverse(mat);
}

void VRFreeCamCamera3DController::computeMatrix(const vr::Hmd_Eye eye, const vr::TrackedDevicePose_t& pose) noexcept
{
    if(eye == vr::Eye_Left)
    {
        _leftVP = _leftProjection * _leftPos * convertMatrix(pose.mDeviceToAbsoluteTracking);
    }
    else if(eye == vr::Eye_Right)
    {
        _rightVP = _rightProjection * _rightPos * convertMatrix(pose.mDeviceToAbsoluteTracking);
    }
}

void VRFreeCamCamera3DController::setupCamera(const float zNear, const float zFar) noexcept
{
    vr::HmdMatrix44_t leftProj = _vr->GetProjectionMatrix(vr::Eye_Left, zNear, zFar);
    vr::HmdMatrix44_t rightProj = _vr->GetProjectionMatrix(vr::Eye_Right, zNear, zFar);
    _leftProjection = reinterpret_cast<glm::mat4&>(leftProj);
    _rightProjection = reinterpret_cast<glm::mat4&>(rightProj);

    _leftPos = convertMatrix(_vr->GetEyeToHeadTransform(vr::Eye_Left));
    _rightPos = convertMatrix(_vr->GetEyeToHeadTransform(vr::Eye_Right));
}
