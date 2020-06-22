#pragma once

#include "graphics/BufferView.hpp"

class TAU_DLL GLBufferViewBuilder final : public IBufferViewBuilder
{
    DEFAULT_CONSTRUCT_PU(GLBufferViewBuilder);
    DEFAULT_DESTRUCT(GLBufferViewBuilder);
    DEFAULT_CM_PU(GLBufferViewBuilder);
public:
    [[nodiscard]] UniformBufferView build(const UniformBufferViewArgs& args, Error* error, DescriptorTable table, uSys tableIndex) const noexcept override;
};
