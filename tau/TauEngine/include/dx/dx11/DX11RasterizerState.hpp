#pragma once

#include "graphics/RasterizerState.hpp"

#ifdef _WIN32
#include <d3d11.h>
#include "dx/DXUtils.hpp"

class DX11GraphicsInterface;
class DX11RenderingContext;

class TAU_DLL DX11RasterizerState final : public IRasterizerState
{
    RS_IMPL(DX11RasterizerState);
private:
    ID3D11RasterizerState* _d3dRasterizerState;
public:
    DX11RasterizerState(const RasterizerArgs& params, ID3D11RasterizerState* const d3dRasterizerState) noexcept
        : IRasterizerState(params)
        , _d3dRasterizerState(d3dRasterizerState)
    { }

    ~DX11RasterizerState() noexcept
    { RELEASE_DX(_d3dRasterizerState); }

    DX11RasterizerState(const DX11RasterizerState& copy) noexcept
        : IRasterizerState(copy)
        , _d3dRasterizerState(copy._d3dRasterizerState)
    { _d3dRasterizerState->AddRef(); }

    DX11RasterizerState(DX11RasterizerState&& move) noexcept
        : IRasterizerState(::std::move(move))
        , _d3dRasterizerState(move._d3dRasterizerState)
    { move._d3dRasterizerState = null; }

    DX11RasterizerState& operator=(const DX11RasterizerState& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dRasterizerState);

        IRasterizerState::operator=(copy);

        _d3dRasterizerState = copy._d3dRasterizerState;
        _d3dRasterizerState->AddRef();

        return *this;
    }

    DX11RasterizerState& operator=(DX11RasterizerState&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dRasterizerState);

        IRasterizerState::operator=(::std::move(move));

        _d3dRasterizerState = move._d3dRasterizerState;
        move._d3dRasterizerState = null;

        return *this;
    }

    [[nodiscard]] const ID3D11RasterizerState* d3dRasterizerState() const noexcept { return _d3dRasterizerState; }
    [[nodiscard]] ID3D11RasterizerState* d3dRasterizerState() noexcept { return _d3dRasterizerState; }

    void apply(DX11RenderingContext& ctx) const noexcept;
};

class TAU_DLL DX11RasterizerStateBuilder final : public IRasterizerStateBuilder
{
    DEFAULT_DESTRUCT(DX11RasterizerStateBuilder);
    DEFAULT_CM_PU(DX11RasterizerStateBuilder);
public:
    static D3D11_CULL_MODE dxCullMode(RasterizerArgs::CullMode cullMode) noexcept;
    static D3D11_FILL_MODE dxFillMode(RasterizerArgs::FillMode fillMode) noexcept;
public:
    struct DXRasterizerArgs final
    {
        D3D11_RASTERIZER_DESC desc;
        ID3D11RasterizerState* state;
    };
private:
    DX11GraphicsInterface& _gi;
public:
    DX11RasterizerStateBuilder(DX11GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX11RasterizerState* build(const RasterizerArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11RasterizerState* build(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IRasterizerState> buildCPPRef(const RasterizerArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IRasterizerState> buildTauRef(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IRasterizerState> buildTauSRef(const RasterizerArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const RasterizerArgs& args, [[tau::out]] DXRasterizerArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
