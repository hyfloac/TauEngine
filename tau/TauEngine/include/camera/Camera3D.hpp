#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "maths/Vector3f.hpp"
#include "system/Keyboard.hpp"

#include <Objects.hpp>
#include <GameRecorder.hpp>

class Window;

class Camera3D final
{
    DEFAULT_DESTRUCT(Camera3D);
    DEFAULT_COPY(Camera3D);
private:
    Vector3f _position;
    float _pitch; // Degrees
    float _yaw; // Degrees
    glm::quat _viewQuaternion;
    glm::mat4 _projectionMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _compoundedMatrix;
public:
    Camera3D(const Window& window, float fov, float zNear, float zFar) noexcept;

    [[nodiscard]] Vector3f position() const noexcept { return _position; }
    [[nodiscard]] float pitch() const noexcept { return _pitch; }
    [[nodiscard]] float yaw() const noexcept { return _yaw; }
    [[nodiscard]] glm::quat viewQuaterion() const noexcept { return _viewQuaternion; }
    [[nodiscard]] glm::mat4 projectionMatrix() const noexcept { return _projectionMatrix; }
    [[nodiscard]] const glm::mat4& viewMatrix() const noexcept { return _viewMatrix; }
    /** _projectionMatrix * _viewMatrix */
    [[nodiscard]] const glm::mat4& compoundedMatrix() const noexcept { return _compoundedMatrix; }

    void setProjection(const Window& window, float fov, float zNear, float zFar) noexcept;

    void position(const Vector3f pos) noexcept
    {
        _position = pos;
        recomputeMatrices();
    }

    void pitch(float pitch) noexcept
    {
        if(pitch > 89.0f) { pitch = 89.0f; }
        else if(pitch < -89.0f) { pitch = -89.0f; }
        _pitch = pitch;
        _viewQuaternion = glm::quat(glm::vec3(-_pitch, -_yaw, 0.0f));
        recomputeMatrices();
    }

    void yaw(float yaw) noexcept
    {
        if(yaw > 360.0f) { yaw = fmod(yaw, 360.0f); }
        else if(yaw < 0.0f)
        {
            yaw = fmod(yaw, 360.0f);
            yaw = 360.0f - yaw;
        }
        _yaw = yaw;
        _viewQuaternion = glm::quat(glm::vec3(-_pitch, -_yaw, 0.0f));
        recomputeMatrices();
    }

    void set(float pitch, float yaw) noexcept
    {
        if(pitch > 89.0f) { pitch = 89.0f; }
        else if(pitch < -89.0f) { pitch = -89.0f; }

        if(yaw > 360.0f) { yaw = fmod(yaw, 360.0f); }
        else if(yaw < 0.0f)
        {
            yaw = fmod(yaw, 360.0f);
            yaw = 360.0f - yaw;
        }
        _pitch = pitch;
        _yaw = yaw;
        _viewQuaternion = glm::quat(glm::vec3(-_pitch, -_yaw, 0.0f));
        recomputeMatrices();
    }

    void set(Vector3f pos, float pitch, float yaw) noexcept
    {
        _position = pos;

        if(pitch > 89.0f) { pitch = 89.0f; }
        else if(pitch < -89.0f) { pitch = -89.0f; }

        if(yaw > 360.0f) { yaw = fmod(yaw, 360.0f); }
        else if(yaw < 0.0f)
        {
            yaw = fmod(yaw, 360.0f);
            yaw = 360.0f - yaw;
        }
        _pitch = pitch;
        _yaw = yaw;
        _viewQuaternion = glm::quat(glm::vec3(DEG_2_RAD_F(_pitch), DEG_2_RAD_F(_yaw), 0.0f));
        recomputeMatrices();
    }
private:
    void recomputeMatrices() noexcept;
private:
    friend class FPSCamera3DController;
    friend class FreeCamCamera3DController;
};

class FPSCamera3DController final
{
    DEFAULT_DESTRUCT(FPSCamera3DController);
    DEFAULT_COPY(FPSCamera3DController);
private:
    /* Acquired some of these from this paper:
     * https://rmets.onlinelibrary.wiley.com/doi/pdf/10.1256/wea.29.02 */
    static constexpr float PLAYER_MASS = 70.0f; // Kilograms
    static constexpr float PLAYER_MASS_G = PLAYER_MASS * 1000.0f; // Grams
    static constexpr float GRAVITY = -9.807f; // Meters/Second**2
    static constexpr float AIR_DENSITY = 1.2f; // Kilograms/Meter**3
    static constexpr float AIR_DENSITY_G = AIR_DENSITY * 1000.0f; // Grams/Meter**3
    static constexpr float PLAYER_DRAG_COEFFICIENT = 0.6f; 
    static constexpr float PLAYER_CROSS_SECTIONAL_AREA = 0.68f; // Meter**2
    static constexpr float FORCE_GRAVITY = PLAYER_MASS_G * GRAVITY;
    /**
     * F_air = 1/2 * p * A * C * v**2
     */
    static constexpr float FORCE_AIR = 0.5 * AIR_DENSITY_G * PLAYER_CROSS_SECTIONAL_AREA * PLAYER_DRAG_COEFFICIENT;

    Camera3D _camera;
    Vector3f _acceleration;
    Vector3f _velocity;
    float _runForwardAcceleration;
    float _walkForwardAcceleration;
    float _walkBackwardsAcceleration;
    float _walkSidewaysAcceleration;
    float _jumpAcceleration;
    float _rotateSpeed;
    Keyboard::Key _keySprint;
    Keyboard::Key _keyForwards;
    Keyboard::Key _keyBackwards;
    Keyboard::Key _keyLeft;
    Keyboard::Key _keyRight;
    Keyboard::Key _keyJump;
public:

    void update(float fixedDelta) noexcept;

    [[nodiscard]] const Camera3D& camera() const noexcept { return _camera; }
    [[nodiscard]] Camera3D& camera() noexcept { return _camera; }

    [[nodiscard]] Camera3D* operator->() noexcept { return &_camera; }
    [[nodiscard]] const Camera3D* operator->() const noexcept { return &_camera; }
};

class FreeCamCamera3DController final
{
    DEFAULT_DESTRUCT(FreeCamCamera3DController);
    DEFAULT_COPY(FreeCamCamera3DController);
public:
    struct BlipDataUpdate final
    {
        float delta;
        Vector3f velocity;
    };
    struct BlipDataRender final
    {
        float delta;
        i32 dMouseX;
        i32 dMouseY;
    };
    struct BlipDataInitial final
    {
        Vector3f position;
        float pitch; // Degrees
        float yaw; // Degrees
        float rotateSpeed;
    };

    static bool __cdecl blipHandler(GameRecorder::Blip& blip, void* userParam) noexcept;
    static GameRecorder::Blip __cdecl initialBlip(void* userParam) noexcept;

    static RunTimeType<GameRecorder::Blip> cameraBlip() noexcept;
private:
    Camera3D _camera;
    float _normalSpeed;
    float _fastSpeed;
    float _rotateSpeed;
    /**
     *   Controls whether when looking up and moving if
     * you move simply forward, or your y position also
     * changes.
     */
    bool _lookY;
    Vector3f _velocity;
    Keyboard::Key _keyAccelerate;
    Keyboard::Key _keyForwards;
    Keyboard::Key _keyBackwards;
    Keyboard::Key _keyLeft;
    Keyboard::Key _keyRight;
    Keyboard::Key _keyUp;
    Keyboard::Key _keyDown;

    GameRecorder* _recorder;
public:
    FreeCamCamera3DController(const Window& window, float fov, float zNear, float zFar,
                              const float normalSpeed, const float fastSpeed, const float rotateSpeed, const bool lookY,
                              const Keyboard::Key keyAccelerate, const Keyboard::Key keyForwards,
                              const Keyboard::Key keyBackwards, const Keyboard::Key keyLeft, const Keyboard::Key keyRight,
                              const Keyboard::Key keyUp, const Keyboard::Key keyDown,
                              GameRecorder* recorder)
        : _camera(window, fov, zNear, zFar),
          _normalSpeed(normalSpeed), _fastSpeed(fastSpeed),
          _rotateSpeed(rotateSpeed), _lookY(lookY),
          _velocity(0.0f),
          _keyAccelerate(keyAccelerate),
          _keyForwards(keyForwards), _keyBackwards(keyBackwards),
          _keyLeft(keyLeft), _keyRight(keyRight),
          _keyUp(keyUp), _keyDown(keyDown),
          _recorder(recorder)
    {
        if(recorder)
        {
            recorder->addBlipHandler(cameraBlip(), blipHandler, initialBlip, this);
        }
    }
private:
    void update(float fixedDelta, Vector3f velocity) noexcept;
public:
    void update(float fixedDelta) noexcept;
    void updateRotation(float delta, i32 dMouseX, i32 dMouseY) noexcept;

    [[nodiscard]] const Camera3D& camera() const noexcept { return _camera; }
    [[nodiscard]] Camera3D& camera() noexcept { return _camera; }

    [[nodiscard]] const Camera3D* operator->() const noexcept { return &_camera; }
    [[nodiscard]] Camera3D* operator->() noexcept { return &_camera; }

    [[nodiscard]] float normalSpeed() const noexcept { return _normalSpeed; }
    [[nodiscard]] float& normalSpeed() noexcept { return _normalSpeed; }

    [[nodiscard]] float fastSpeed() const noexcept { return _fastSpeed; }
    [[nodiscard]] float& fastSpeed() noexcept { return _fastSpeed; }

    [[nodiscard]] float rotateSpeed() const noexcept { return _rotateSpeed; }
    [[nodiscard]] float& rotateSpeed() noexcept { return _rotateSpeed; }

    [[nodiscard]] bool lookY() const noexcept { return _lookY; }
    [[nodiscard]] bool& lookY() noexcept { return _lookY; }

    [[nodiscard]] Vector3f velocity() const noexcept { return _velocity; }

    [[nodiscard]] Keyboard::Key keyAccelerate() const noexcept { return _keyAccelerate; }
    [[nodiscard]] Keyboard::Key& keyAccelerate() noexcept { return _keyAccelerate; }

    [[nodiscard]] Keyboard::Key keyForwards() const noexcept { return _keyForwards; }
    [[nodiscard]] Keyboard::Key& keyForwards() noexcept { return _keyForwards; }

    [[nodiscard]] Keyboard::Key keyBackwards() const noexcept { return _keyBackwards; }
    [[nodiscard]] Keyboard::Key& keyBackwards() noexcept { return _keyBackwards; }

    [[nodiscard]] Keyboard::Key keyLeft() const noexcept { return _keyLeft; }
    [[nodiscard]] Keyboard::Key& keyLeft() noexcept { return _keyLeft; }

    [[nodiscard]] Keyboard::Key keyRight() const noexcept { return _keyRight; }
    [[nodiscard]] Keyboard::Key& keyRight() noexcept { return _keyRight; }

    [[nodiscard]] Keyboard::Key keyUp() const noexcept { return _keyUp; }
    [[nodiscard]] Keyboard::Key& keyUp() noexcept { return _keyUp; }

    [[nodiscard]] Keyboard::Key keyDown() const noexcept { return _keyDown; }
    [[nodiscard]] Keyboard::Key& keyDown() noexcept { return _keyDown; }
private:
    void checkKeys() noexcept;
};

