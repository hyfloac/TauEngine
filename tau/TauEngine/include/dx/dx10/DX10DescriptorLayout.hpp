#pragma once

#include "graphics/DescriptorLayout.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include <DynArray.hpp>

class TAU_DLL DX10DescriptorLayout final
{
    DEFAULT_DESTRUCT(DX10DescriptorLayout);
    DELETE_CM(DX10DescriptorLayout);
private:
    RefDynArray<DescriptorLayoutEntry> _entries;
public:
    DX10DescriptorLayout(const RefDynArray<DescriptorLayoutEntry>& entries) noexcept
        : _entries(entries)
    { }

    [[nodiscard]] const RefDynArray<DescriptorLayoutEntry>& entries() const noexcept { return _entries; }
};
#endif
