#pragma once

#include "graphics/RasterizerState.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"

class DX10GraphicsInterface;
class DX10RenderingContext;

class TAU_DLL DX10RasterizerState final : public IRasterizerState
{
    RS_IMPL(DX10RasterizerState);
private:
    ID3D10RasterizerState* _d3dRasterizerState;
public:
    DX10RasterizerState(const RasterizerArgs& params, ID3D10RasterizerState* const d3dRasterizerState) noexcept
        : IRasterizerState(params)
        , _d3dRasterizerState(d3dRasterizerState)
    { }

    ~DX10RasterizerState() noexcept override
    { RELEASE_DX(_d3dRasterizerState); }

    DX10RasterizerState(const DX10RasterizerState& copy) noexcept
        : IRasterizerState(copy)
        , _d3dRasterizerState(copy._d3dRasterizerState)
    { _d3dRasterizerState->AddRef(); }

    DX10RasterizerState(DX10RasterizerState&& move) noexcept
        : IRasterizerState(::std::move(move))
        , _d3dRasterizerState(move._d3dRasterizerState)
    { move._d3dRasterizerState = null; }

    DX10RasterizerState& operator=(const DX10RasterizerState& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dRasterizerState);

        IRasterizerState::operator=(copy);

        _d3dRasterizerState = copy._d3dRasterizerState;
        _d3dRasterizerState->AddRef();

        return *this;
    }

    DX10RasterizerState& operator=(DX10RasterizerState&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dRasterizerState);

        IRasterizerState::operator=(::std::move(move));

        _d3dRasterizerState = move._d3dRasterizerState;
        move._d3dRasterizerState = null;

        return *this;
    }

    [[nodiscard]] ID3D10RasterizerState* d3dRasterizerState() const noexcept { return _d3dRasterizerState; }

    void apply(DX10RenderingContext& ctx) const noexcept;
};

class TAU_DLL DX10RasterizerStateBuilder final : public IRasterizerStateBuilder
{
    DEFAULT_DESTRUCT(DX10RasterizerStateBuilder);
    DEFAULT_CM_PU(DX10RasterizerStateBuilder);
public:
    static D3D10_CULL_MODE dxCullMode(RasterizerArgs::CullMode cullMode) noexcept;
    static D3D10_FILL_MODE dxFillMode(RasterizerArgs::FillMode fillMode) noexcept;
public:
    struct DXRasterizerArgs final
    {
        D3D10_RASTERIZER_DESC desc;
        ID3D10RasterizerState* state;
    };
private:
    DX10GraphicsInterface& _gi;
public:
    DX10RasterizerStateBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10RasterizerState* build(const RasterizerArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10RasterizerState* build(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IRasterizerState> buildCPPRef(const RasterizerArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IRasterizerState> buildTauRef(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IRasterizerState> buildTauSRef(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const RasterizerArgs& args, [[tau::out]] DXRasterizerArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
