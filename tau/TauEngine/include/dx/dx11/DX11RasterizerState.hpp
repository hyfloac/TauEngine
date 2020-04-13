#pragma once

#include "graphics/RasterizerState.hpp"

#ifdef _WIN32
#include <d3d11.h>

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
    {
        _d3dRasterizerState->Release();
        _d3dRasterizerState = null;
    }

    [[nodiscard]] const ID3D11RasterizerState* d3dRasterizerState() const noexcept { return _d3dRasterizerState; }
    [[nodiscard]] ID3D11RasterizerState* d3dRasterizerState() noexcept { return _d3dRasterizerState; }

    void apply(DX11RenderingContext& ctx) const noexcept;
};

class TAU_DLL DX11RasterizerStateBuilder final : public IRasterizerStateBuilder
{
    DEFAULT_DESTRUCT(DX11RasterizerStateBuilder);
    DELETE_COPY(DX11RasterizerStateBuilder);
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
