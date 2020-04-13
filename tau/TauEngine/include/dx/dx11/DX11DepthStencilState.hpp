#pragma once

#include "graphics/DepthStencilState.hpp"

#ifdef _WIN32
#include <d3d11.h>

class DX11GraphicsInterface;
class DX11RenderingContext;

class TAU_DLL DX11DepthStencilState final : public IDepthStencilState
{
    DSS_IMPL(DX11DepthStencilState);
private:
    ID3D11DepthStencilState* _d3dDepthStencilState;
public:
    DX11DepthStencilState(const DepthStencilArgs& params, ID3D11DepthStencilState* const d3dDepthStencilState) noexcept
        : IDepthStencilState(params)
        , _d3dDepthStencilState(d3dDepthStencilState)
    { }

    ~DX11DepthStencilState() noexcept
    {
        _d3dDepthStencilState->Release();
        _d3dDepthStencilState = null;
    }

    [[nodiscard]] const ID3D11DepthStencilState* d3dDepthStencilState() const noexcept { return _d3dDepthStencilState; }
    [[nodiscard]] ID3D11DepthStencilState* d3dDepthStencilState() noexcept { return _d3dDepthStencilState; }

    void apply(DX11RenderingContext& ctx) const noexcept;
};

class TAU_DLL DX11DepthStencilStateBuilder final : public IDepthStencilStateBuilder
{
    DEFAULT_DESTRUCT(DX11DepthStencilStateBuilder);
    DELETE_COPY(DX11DepthStencilStateBuilder);
public:
    static D3D11_DEPTH_WRITE_MASK dxDepthWriteMask(DepthStencilArgs::DepthWriteMask depthWriteMask) noexcept;
    static D3D11_COMPARISON_FUNC dxComparisonFunc(DepthStencilArgs::CompareFunc compareFunc) noexcept;
    static D3D11_STENCIL_OP dxStencilOp(DepthStencilArgs::StencilOp stencilOp) noexcept;
public:
    struct DXDepthStencilArgs final
    {
        D3D11_DEPTH_STENCIL_DESC desc;
        ID3D11DepthStencilState* state;
    };
private:
    DX11GraphicsInterface& _gi;
public:
    DX11DepthStencilStateBuilder(DX11GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX11DepthStencilState* build(const DepthStencilArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX11DepthStencilState* build(const DepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IDepthStencilState> buildCPPRef(const DepthStencilArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IDepthStencilState> buildTauRef(const DepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IDepthStencilState> buildTauSRef(const DepthStencilArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    bool processArgs(const DepthStencilArgs& args, [[tau::out]] ID3D11DepthStencilState** d3dDepthStencilState, [[tau::out]] Error* error) const noexcept;
};
#endif
