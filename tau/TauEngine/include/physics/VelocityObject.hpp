#pragma once

#include <maths/Vector3f.hpp>

class VelocityObject final
{
private:
    Vector3f _position;
    Vector3f _velocity;
    Vector3f _acceleration;
public:
    void update() noexcept
    {
        _velocity += _acceleration;
        _position += _velocity;
    }
};
