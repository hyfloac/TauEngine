#pragma once

#include "Component.hpp"
#include "Entity.hpp"

class ForwardComponent final : public Component
{
    COMPONENT_IMPL(ForwardComponent);
public:
    ForwardComponent() = default;

    void onUpdate(float fixedDelta, Entity* entity) override
    {
        entity->x += 10 * fixedDelta;
    }
};
