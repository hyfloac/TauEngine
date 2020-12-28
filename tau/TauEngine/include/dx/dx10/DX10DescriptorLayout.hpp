#pragma once

#include "graphics/DescriptorLayout.hpp"

#ifdef _WIN32
class TAU_DLL DX10DescriptorLayoutBuilder final : public IDescriptorLayoutBuilder
{
    DEFAULT_CONSTRUCT_PU(DX10DescriptorLayoutBuilder);
    DEFAULT_DESTRUCT(DX10DescriptorLayoutBuilder);
    DEFAULT_CM_PU(DX10DescriptorLayoutBuilder);
public:
    [[nodiscard]] NullableRef<IDescriptorLayout> build(const DescriptorLayoutArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
protected:
    [[nodiscard]] uSys _allocSize() const noexcept override
    { return NullableRef<SimpleDescriptorLayout>::allocSize(); }
};
#endif
