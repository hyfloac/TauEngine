#pragma once

#include "graphics/PipelineState.hpp"

#ifdef _WIN32
class TAU_DLL DX10PipelineStateBuilder final : public PipelineStateBuilder
{
public:
    [[nodiscard]] NullableRef<IPipelineState> build(const PipelineArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
};
#endif
