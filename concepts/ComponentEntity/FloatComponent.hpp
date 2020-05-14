#pragma once

#include "Component.hpp"
#include "Entity.hpp"

class FloatComponent final : public Component
{
    COMPONENT_IMPL(FloatComponent);
public:
    FloatComponent() = default;

    void onUpdate(float fixedDelta, Entity* entity) override
    {
        entity->y += 10 * fixedDelta;
    }
};
