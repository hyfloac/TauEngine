#pragma once

#include <TauEngine.hpp>
#include <maths/Vector3f.hpp>

class ITexture;

class Material
{
protected:
    Ref<ITexture> _diffuse;
    Ref<ITexture> _overlay;
    Ref<ITexture> _normalMap;
    Ref<ITexture> _specularMap;
public:
    Material() noexcept = default;
};
