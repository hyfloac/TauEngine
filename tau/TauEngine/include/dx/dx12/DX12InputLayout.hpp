#pragma once

#include "graphics/InputLayout.hpp"

#ifdef _WIN32
#include <d3d12.h>

class TAU_DLL DX12InputLayout final : public IInputLayout
{
    DEFAULT_DESTRUCT(DX12InputLayout);
    DELETE_CM(DX12InputLayout);
    INPUT_LAYOUT_IMPL(DX12InputLayout);
private:
    ConstRefDynArray<D3D12_INPUT_ELEMENT_DESC> _inputLayout;
public:
    DX12InputLayout(const RefDynArray<D3D12_INPUT_ELEMENT_DESC>& inputLayout) noexcept
        : _inputLayout(inputLayout)
    { }

    DX12InputLayout(RefDynArray<D3D12_INPUT_ELEMENT_DESC>&& inputLayout) noexcept
        : _inputLayout(::std::move(inputLayout))
    { }

    DX12InputLayout(const ConstRefDynArray<D3D12_INPUT_ELEMENT_DESC>& inputLayout) noexcept
        : _inputLayout(inputLayout)
    { }

    DX12InputLayout(ConstRefDynArray<D3D12_INPUT_ELEMENT_DESC>&& inputLayout) noexcept
        : _inputLayout(::std::move(inputLayout))
    { }

    [[nodiscard]] const ConstRefDynArray<D3D12_INPUT_ELEMENT_DESC>& inputLayout() const noexcept { return _inputLayout; }
};

class TAU_DLL DX12InputLayoutBuilder final : public IInputLayoutBuilder
{
private:
    struct DXInputLayoutArgs final
    {
        RefDynArray<D3D12_INPUT_ELEMENT_DESC> inputLayout;

        DXInputLayoutArgs(const uSys descriptorCount) noexcept
            : inputLayout(descriptorCount)
        { }
    };
public:
    [[nodiscard]] NullableRef<IInputLayout> buildTauRef(const InputLayoutArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const InputLayoutArgs& args, [[tau::out]] DXInputLayoutArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
