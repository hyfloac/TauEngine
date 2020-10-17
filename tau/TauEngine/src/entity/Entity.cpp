#include "entity/Entity.hpp"

void Entity::update(const float fixedDelta) noexcept
{
    _components.iterate([fixedDelta](auto* node)
    {
        IEntityComponent* entity = node->value;
        if(entity->doesUpdate())
        {
            entity->update(fixedDelta);
        }
    });
}

void Entity::render(const DeltaTime& delta) noexcept
{
    _components.iterate([delta](auto* node)
    {
        IEntityComponent* entity = node->value;
        if(entity->doesRender() && entity->isVisible())
        {
            entity->render(delta);
        }
    });
}

#if TAU_ECS_EDITOR_MODE
void Entity::debugRender(const DeltaTime& delta) noexcept
{
    _components.iterate([delta](auto* node)
    {
        IEntityComponent* entity = node->value;
        if(entity->doesEditorRender() && entity->isEditorVisible())
        {
            entity->debugRender(delta);
        }
    });
}
#endif

IEntityComponent* Entity::getComponent(IEntityComponent::Type type) const noexcept
{
    const auto* node = _components.get(type);
    if(!node)
    { return nullptr; }

    return node->value;
}

::std::unordered_map<IEntityComponent::Type, EntityManager::ComponentData> EntityManager::components;

IEntityComponent* EntityManager::buildComponent(const IEntityComponent::Type type, const WeakRef<Entity>& entity) noexcept
{
    const auto& componentData = components.at(type);
    return componentData.ctor(entity);
}

IEntityComponent* EntityManager::buildComponent(const IEntityComponent::Type type, void* const placement, const WeakRef<Entity>& entity) noexcept
{
    const auto& componentData = components.at(type);
    return componentData.placementCtor(placement, entity);
}

DynString EntityManager::compileEntity(Entity* entity, const DynString& typeName) noexcept
{
    if(!entity)
    { return DynString(); }

    StringBuilder builder(1024);

    builder.append("#include <ds/AVLTree.hpp>\n");
    builder.append("#include \"DLL.hpp\"\n");
    builder.append("#include \"entity/Entity.hpp\"\n");

    builder.append("class TAU_DLL ").append(typeName).append(" : public IEntity\n");
    builder.append("{\n");
    builder.append("    DELETE_CM(").append(typeName).append(");\n");
    builder.append("private:\n");
    builder.append("    void* _placement;\n");
    builder.append("public:\n");
    builder.append("    ").append(typeName).append("() noexcept\n");
    builder.append("        : _placement(operator new(::std::nothrow))\n");
    builder.append("    { }\n");


    return builder.toString();
}
