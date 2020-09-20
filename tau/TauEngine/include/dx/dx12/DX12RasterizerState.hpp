#pragma once

#include "graphics/RasterizerState.hpp"

#ifdef _WIN32
#include <d3d12.h>

class TAU_DLL DX12RasterizerState final : public IRasterizerState
{
private:
    D3D12_RASTERIZER_DESC _rasterizerDesc;
public:
    DX12RasterizerState(const RasterizerArgs& args, const D3D12_RASTERIZER_DESC& rasterizerDesc) noexcept
        : IRasterizerState(args)
        , _rasterizerDesc(rasterizerDesc)
    { }

    [[nodiscard]] const D3D12_RASTERIZER_DESC& rasterizerDesc() const noexcept { return _rasterizerDesc; }
};

class TAU_DLL DX12RasterizerStateBuilder final : public IRasterizerStateBuilder
{
public:
    [[nodiscard]] NullableRef<IRasterizerState> buildTauRef(const RasterizerArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const RasterizerArgs& args, [[tau::out]] D3D12_RASTERIZER_DESC* rasterizerDesc, [[tau::out]] Error* error) const noexcept;
};
#endif
