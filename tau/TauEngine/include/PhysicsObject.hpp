#pragma once

#include <WorldObject.hpp>

class PhysicsObject final
{
private:
    CPPRef<WorldObject> _object;
    Vector3f _acceleration;
    Vector3f _velocity;
public:
    PhysicsObject(CPPRef<WorldObject>& object) noexcept
        : _object(object), _acceleration(), _velocity()
    { }

    PhysicsObject(CPPRef<WorldObject>& object, Vector3f acceleration) noexcept
        : _object(object), _acceleration(acceleration), _velocity()
    { }

    PhysicsObject(CPPRef<WorldObject>& object, Vector3f acceleration, Vector3f velocity) noexcept
        : _object(object), _acceleration(acceleration), _velocity(velocity)
    { }

    ~PhysicsObject() noexcept = default;

    PhysicsObject(const PhysicsObject& copy) = default;
    PhysicsObject(PhysicsObject&& move) noexcept = default;

    PhysicsObject& operator=(const PhysicsObject& copy) = default;
    PhysicsObject& operator=(PhysicsObject&& move) noexcept = default;

    [[nodiscard]] const CPPRef<WorldObject>& Object() const noexcept { return _object; }
    [[nodiscard]] Vector3f acceleration() const noexcept { return _acceleration; }
    [[nodiscard]] Vector3f velocity() const noexcept { return _velocity; }

    void update() noexcept
    {
        _velocity += _acceleration;
        _object->addPosition(_velocity);
    }

    void setAcceleration(const Vector3f acceleration) noexcept
    {
        _acceleration = acceleration;
    }

    void addAcceleration(const Vector3f acceleration) noexcept
    {
        _acceleration += acceleration;
    }

    void subAcceleration(const Vector3f acceleration) noexcept
    {
        _acceleration -= acceleration;
    }

    void crossAcceleration(const Vector3f acceleration) noexcept
    {
        _acceleration.cross(acceleration);
    }
};

