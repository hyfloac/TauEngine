#pragma once

#include "graphics/BufferView.hpp"

#ifdef _WIN32
class TAU_DLL DX10BufferViewBuilder final : public IBufferViewBuilder
{
    DEFAULT_CONSTRUCT_PU(DX10BufferViewBuilder);
    DEFAULT_DESTRUCT(DX10BufferViewBuilder);
    DEFAULT_CM_PU(DX10BufferViewBuilder);
public:
    [[nodiscard]] UniformBufferView build(const UniformBufferViewArgs& args, CPUDescriptorHandle handle, Error* error) const noexcept override;
};
#endif
