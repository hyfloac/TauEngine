#pragma once

#include <Safeties.hpp>

class Mesh;
class Material;

class Entity
{
public:
    static bool compare(const Entity& a, const Entity& b) noexcept;
private:
    NullableRef<Mesh> _mesh;
    NullableRef<Material> _material;
    float _cameraDistance;
};
