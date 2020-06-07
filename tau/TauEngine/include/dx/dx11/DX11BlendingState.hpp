#pragma once

#include "graphics/BlendingState.hpp"

#ifdef _WIN32
#include <d3d11.h>
#include "dx/DXUtils.hpp"

class DX11GraphicsInterface;
class DX11RenderingContext;

class TAU_DLL DX11BlendingState final : public IBlendingState
{
    BS_IMPL(DX11BlendingState);
private:
    ID3D11BlendState* _d3dBlendState;
public:
    DX11BlendingState(const BlendingArgs& args, ID3D11BlendState* const d3dBlendState) noexcept
        : IBlendingState(args)
        , _d3dBlendState(d3dBlendState)
    { }

    ~DX11BlendingState() noexcept
    { RELEASE_DX(_d3dBlendState); }

    DX11BlendingState(const DX11BlendingState& copy) noexcept
        : IBlendingState(copy)
        , _d3dBlendState(copy._d3dBlendState)
    { _d3dBlendState->AddRef(); }

    DX11BlendingState(DX11BlendingState&& move) noexcept
        : IBlendingState(::std::move(move))
        , _d3dBlendState(move._d3dBlendState)
    { move._d3dBlendState = null; }

    DX11BlendingState& operator=(const DX11BlendingState& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dBlendState);

        IBlendingState::operator=(copy);

        _d3dBlendState = copy._d3dBlendState;
        _d3dBlendState->AddRef();

        return *this;
    }

    DX11BlendingState& operator=(DX11BlendingState&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dBlendState);

        IBlendingState::operator=(::std::move(move));

        _d3dBlendState = move._d3dBlendState;
        move._d3dBlendState = null;

        return *this;
    }

    [[nodiscard]] ID3D11BlendState* d3dBlendState() const noexcept { return _d3dBlendState; }

    void apply(DX11RenderingContext& context, const FLOAT blendFactor[4]) const noexcept;
};

class TAU_DLL DX11BlendingStateBuilder final : public IBlendingStateBuilder
{
    DEFAULT_DESTRUCT(DX11BlendingStateBuilder);
    DEFAULT_CM_PU(DX11BlendingStateBuilder);
public:
    [[nodiscard]] static D3D11_BLEND dxBlendFactor(BlendingArgs::BlendFactor blendFactor) noexcept;
    [[nodiscard]] static D3D11_BLEND_OP dxBlendOp(BlendingArgs::BlendOp blendOp) noexcept;
private:
    DX11GraphicsInterface& _gi;
public:
    DX11BlendingStateBuilder(DX11GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX11BlendingState* build(const BlendingArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11BlendingState* build(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IBlendingState> buildCPPRef(const BlendingArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IBlendingState> buildTauRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IBlendingState> buildTauSRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const BlendingArgs& args, [[tau::out]] ID3D11BlendState** d3dBlendState, [[tau::out]] Error* error) const noexcept;
};

#endif
