#pragma once

#include "graphics/PipelineState.hpp"

#ifdef _WIN32
#include <d3d10.h>

class TAU_DLL DX10PipelineState final
{
};

class TAU_DLL DX10PipelineStateBuilder final : public PipelineStateBuilder
{
public:
    [[nodiscard]] PipelineState build(const PipelineArgs& args, Error* error) const noexcept override;
};
#endif
