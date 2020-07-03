#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include <DynArray.hpp>

#include "graphics/DescriptorLayout.hpp"

class TAU_DLL GLDescriptorLayout final
{
    DEFAULT_CONSTRUCT_PU(GLDescriptorLayout);
    DEFAULT_DESTRUCT(GLDescriptorLayout);
    DELETE_CM(GLDescriptorLayout);
private:
    RefDynArray<DescriptorLayoutEntry> _entries;
public:

    [[nodiscard]] const RefDynArray<DescriptorLayoutEntry>& entries() const noexcept { return _entries; }
};
