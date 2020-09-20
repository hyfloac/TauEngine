#pragma once

#include "graphics/DepthStencilState.hpp"

#ifdef _WIN32
#include <d3d12.h>

class TAU_DLL DX12DepthStencilState final : public IDepthStencilState
{
    DEFAULT_DESTRUCT(DX12DepthStencilState);
    DEFAULT_CM_PU(DX12DepthStencilState);
    DSS_IMPL(DX12DepthStencilState);
private:
    D3D12_DEPTH_STENCIL_DESC _depthStencilDesc;
public:
    DX12DepthStencilState(const DepthStencilArgs& args, const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc) noexcept
        : IDepthStencilState(args)
        , _depthStencilDesc(depthStencilDesc)
    { }

    [[nodiscard]] const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc() const noexcept { return _depthStencilDesc; }
};

class TAU_DLL DX12DepthStencilStateBuilder final : public IDepthStencilStateBuilder
{
public:
    [[nodiscard]] NullableRef<IDepthStencilState> buildTauRef(const DepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const DepthStencilArgs& args, [[tau::out]] D3D12_DEPTH_STENCIL_DESC* desc, [[tau::out]] Error* error) const noexcept;
};
#endif
