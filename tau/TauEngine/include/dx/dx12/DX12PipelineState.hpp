#pragma once

#include "graphics/PipelineState.hpp"

#ifdef _WIN32
#include <d3d12.h>

class TAU_DLL DX12PipelineStateBuilder final : public PipelineStateBuilder
{
private:
    ID3D12Device* _device;
public:
    DX12PipelineStateBuilder(ID3D12Device* const device) noexcept
        : _device(device)
    { }

    [[nodiscard]] PipelineState build(const PipelineArgs& args, Error* error) const noexcept override;
};
#endif
