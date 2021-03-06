#pragma once

#include "graphics/BufferView.hpp"

class TAU_DLL GLBufferViewBuilder final : public IBufferViewBuilder
{
    DEFAULT_CONSTRUCT_PU(GLBufferViewBuilder);
    DEFAULT_DESTRUCT(GLBufferViewBuilder);
    DEFAULT_CM_PU(GLBufferViewBuilder);
public:
    [[nodiscard]] UniformBufferView build(const UniformBufferViewArgs& args, CPUDescriptorHandle handle, Error* error) const noexcept override;
};
