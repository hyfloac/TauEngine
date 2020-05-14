#include "Entity.hpp"
#include "ForwardComponent.hpp"
#include "FloatComponent.hpp"
#include "GeneratedEntity_TestEntity.hpp"

#include <iostream>

void genEntity()
{
    ComponentEntity entity;
    entity._components.push_back(new ForwardComponent);
    entity._components.push_back(new FloatComponent);

    std::cout << createConcreteEntity(&entity, "TestEntity") << std::endl;
}

void testEntities()
{
    ComponentEntity entity;
    entity._components.push_back(new ForwardComponent);
    entity._components.push_back(new FloatComponent);

    entity.onUpdate(1.0f);
    entity.onUpdate(1.5f);

    TestEntity testEntity;

    testEntity.onUpdate(1.0f);
    testEntity.onUpdate(1.5f);

    std::cout << "Component Entity: (" << entity.x << ", " << entity.y << ")" << std::endl;
    std::cout << "Generated Entity: (" << testEntity.x << ", " << testEntity.y << ")" << std::endl;
}

int main(int argCount, char* args[])
{
    // genEntity();
    testEntities();
    return 0;
}
