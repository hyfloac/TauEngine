#include "entity/Entity.hpp"
#include "TUID.hpp"

extern TUID& getTUID(const Mesh& mesh) noexcept;
extern TUID& getTUID(const Material& material) noexcept;

bool Entity::compare(const Entity& a, const Entity& b) noexcept
{
    if(getTUID(*a._mesh) == getTUID(*b._mesh))
    {
        if(getTUID(*a._material) == getTUID(*b._material))
        {
            return a._cameraDistance < b._cameraDistance;
        }
        else
        {
            return getTUID(*a._material) < getTUID(*b._material);
        }
    }
    else
    {
        return getTUID(*a._mesh) < getTUID(*b._mesh);
    }
}
