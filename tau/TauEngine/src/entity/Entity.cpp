#include "entity/Entity.hpp"

void Entity::update(const float fixedDelta) noexcept
{
    _components.Iterate([fixedDelta](auto* node)
    {
        IEntityComponent* entity = node->Value;
        if(entity->doesUpdate())
        {
            entity->update(fixedDelta);
        }
    });
}

void Entity::render(const DeltaTime& delta) noexcept
{
    _components.Iterate([delta](auto* node)
    {
        IEntityComponent* entity = node->Value;
        if(entity->doesRender() && entity->isVisible())
        {
            entity->render(delta);
        }
    });
}

#if TAU_ECS_EDITOR_MODE
void Entity::debugRender(const DeltaTime& delta) noexcept
{
    _components.Iterate([delta](auto* node)
    {
        IEntityComponent* entity = node->Value;
        if(entity->doesEditorRender() && entity->isEditorVisible())
        {
            entity->debugRender(delta);
        }
    });
}
#endif

IEntityComponent* Entity::getComponent(IEntityComponent::Type type) const noexcept
{
    IEntityComponent* foundNode = nullptr;
    _components.Iterate([type, &foundNode](auto* node)
    {
        IEntityComponent* entity = node->Value;
        if(!foundNode && type == entity->_getType())
        {
            foundNode = entity;
        }
    });

    return foundNode;
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

    builder.Append("#include <ds/AVLTree.hpp>\n");
    builder.Append("#include \"DLL.hpp\"\n");
    builder.Append("#include \"entity/Entity.hpp\"\n");

    builder.Append("class TAU_DLL ").Append(typeName).Append(" : public IEntity\n");
    builder.Append("{\n");
    builder.Append("    DELETE_CM(").Append(typeName).Append(");\n");
    builder.Append("private:\n");
    builder.Append("    void* _placement;\n");
    builder.Append("public:\n");
    builder.Append("    ").Append(typeName).Append("() noexcept\n");
    builder.Append("        : _placement(operator new(::std::nothrow))\n");
    builder.Append("    { }\n");


    return builder.toString();
}
