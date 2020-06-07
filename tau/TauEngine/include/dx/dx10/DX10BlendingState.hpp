#pragma once

#include "graphics/BlendingState.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"

class DX10GraphicsInterface;
class DX10RenderingContext;

class TAU_DLL DX10BlendingState final : public IBlendingState
{
    BS_IMPL(DX10BlendingState);
private:
    ID3D10BlendState* _d3dBlendState;
public:
    DX10BlendingState(const BlendingArgs& args, ID3D10BlendState* const d3dBlendState) noexcept
        : IBlendingState(args)
        , _d3dBlendState(d3dBlendState)
    { }

    ~DX10BlendingState() noexcept
    { RELEASE_DX(_d3dBlendState); }

    DX10BlendingState(const DX10BlendingState& copy) noexcept
        : IBlendingState(copy)
        , _d3dBlendState(copy._d3dBlendState)
    { _d3dBlendState->AddRef(); }

    DX10BlendingState(DX10BlendingState&& move) noexcept
        : IBlendingState(::std::move(move))
        , _d3dBlendState(move._d3dBlendState)
    { move._d3dBlendState = null; }

    DX10BlendingState& operator=(const DX10BlendingState& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dBlendState);

        IBlendingState::operator=(copy);

        _d3dBlendState = copy._d3dBlendState;
        _d3dBlendState->AddRef();

        return *this;
    }

    DX10BlendingState& operator=(DX10BlendingState&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dBlendState);

        IBlendingState::operator=(::std::move(move));

        _d3dBlendState = move._d3dBlendState;
        move._d3dBlendState = null;

        return *this;
    }

    [[nodiscard]] ID3D10BlendState* d3dBlendState() const noexcept { return _d3dBlendState; }

    void apply(DX10RenderingContext& context, const FLOAT blendFactor[4]) const noexcept;
};

class TAU_DLL DX10BlendingStateBuilder final : public IBlendingStateBuilder
{
    DEFAULT_DESTRUCT(DX10BlendingStateBuilder);
    DEFAULT_CM_PU(DX10BlendingStateBuilder);
public:
    [[nodiscard]] static D3D10_BLEND dxBlendFactor(BlendingArgs::BlendFactor blendFactor) noexcept;
    [[nodiscard]] static D3D10_BLEND_OP dxBlendOp(BlendingArgs::BlendOp blendOp) noexcept;
private:
    DX10GraphicsInterface& _gi;
public:
    DX10BlendingStateBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10BlendingState* build(const BlendingArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10BlendingState* build(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IBlendingState> buildCPPRef(const BlendingArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IBlendingState> buildTauRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IBlendingState> buildTauSRef(const BlendingArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const BlendingArgs& args, [[tau::out]] ID3D10BlendState** d3dBlendState, [[tau::out]] Error* error) const noexcept;
};

#endif
