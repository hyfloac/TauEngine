#pragma once

#include "graphics/BlendingState.hpp"

#ifdef _WIN32
#include <d3d12.h>

class TAU_DLL DX12BlendingState final : public IBlendingState
{
    DEFAULT_DESTRUCT(DX12BlendingState);
    DEFAULT_CM_PU(DX12BlendingState);
    BS_IMPL(DX12BlendingState);
private:
    D3D12_BLEND_DESC _blendDesc;
public:
    DX12BlendingState(const BlendingArgs& args, const D3D12_BLEND_DESC& blendDesc)
        : IBlendingState(args)
        , _blendDesc(blendDesc)
    { }

    [[nodiscard]] const D3D12_BLEND_DESC& blendDesc() const noexcept { return _blendDesc; }
};

class TAU_DLL DX12BlendingStateBuilder final : public IBlendingStateBuilder
{
    DEFAULT_CM_PU(DX12BlendingStateBuilder);
public:
    [[nodiscard]] NullableRef<IBlendingState> buildTauRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const BlendingArgs& args, [[tau::out]] D3D12_BLEND_DESC* desc, [[tau::out]] Error* error) const noexcept;
};
#endif
