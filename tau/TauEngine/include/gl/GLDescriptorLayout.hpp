#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include <DynArray.hpp>

#include "graphics/DescriptorLayout.hpp"

class TAU_DLL GLDescriptorLayout final
{
    DEFAULT_DESTRUCT(GLDescriptorLayout);
    DELETE_CM(GLDescriptorLayout);
private:
    RefDynArray<DescriptorLayoutEntry> _entries;
public:
    GLDescriptorLayout(const RefDynArray<DescriptorLayoutEntry>& entries) noexcept
        : _entries(entries)
    { }

    [[nodiscard]] const RefDynArray<DescriptorLayoutEntry>& entries() const noexcept { return _entries; }
};
