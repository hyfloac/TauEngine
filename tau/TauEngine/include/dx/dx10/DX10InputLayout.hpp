#pragma once

#include "model/InputLayout.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10RenderingContext;

class TAU_DLL DX10InputLayout final : public IInputLayout
{
    DEFAULT_DESTRUCT(DX10InputLayout);
    INPUT_LAYOUT_IMPL(DX10InputLayout);
private:
    DynArray<D3D10_INPUT_ELEMENT_DESC> _inputElements;
    ID3D10InputLayout* _inputLayoutCache;
public:
    DX10InputLayout(const DynArray<InputLayoutDescriptor>& descriptors, const DynArray<D3D10_INPUT_ELEMENT_DESC>& inputElements,
                    ID3D10InputLayout* const inputLayoutCache = null) noexcept
        : IInputLayout(descriptors), _inputElements(inputElements), _inputLayoutCache(inputLayoutCache)
    { }

    [[nodiscard]] const DynArray<D3D10_INPUT_ELEMENT_DESC>& inputElements() const noexcept { return _inputElements; }
    [[nodiscard]] const ID3D10InputLayout* inputLayoutCache() const noexcept { return _inputLayoutCache; }
    [[nodiscard]] ID3D10InputLayout*& inputLayoutCache() noexcept { return _inputLayoutCache; }

    [[nodiscard]] ID3D10InputLayout* inputLayoutCache(DX10RenderingContext& context, ID3D10Blob* dataBlob) noexcept;
};

class TAU_DLL DX10InputLayoutBuilder final : public IInputLayoutBuilder
{
    DEFAULT_DESTRUCT(DX10InputLayoutBuilder);
    DELETE_COPY(DX10InputLayoutBuilder);
public:
    static const char* getDXSemanticName(ShaderSemantic::Semantic semantic) noexcept;
public:
    DX10InputLayoutBuilder(const uSys numDescriptors) noexcept
        : IInputLayoutBuilder(numDescriptors)
    { }

    [[nodiscard]] DX10InputLayout* build([[tau::out]] Error* error) noexcept override;
};
#endif
