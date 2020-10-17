#pragma once

#include "entity/EntityComponent.hpp"
#include "maths/Transform.hpp"

class TAU_DLL TransformEntityComponent final : public AbstractEntityComponent<TransformEntityComponent>
{
private:
    Transform _transform;
public:
    explicit TransformEntityComponent(const WeakRef<Entity>& entity)
        : AbstractEntityComponent(entity)
    {
        setVisibleInit(false);
        _doesUpdate = false;
        _doesRender = false;
        _isEditorVisible = true;
        _isEditorOnly = false;
        _doesEditorRender = true;
    }

    [[nodiscard]]       Transform& transform()       noexcept { return _transform; }
    [[nodiscard]] const Transform& transform() const noexcept { return _transform; }
};
