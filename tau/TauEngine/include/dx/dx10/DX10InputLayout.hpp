#pragma once

#include "graphics/InputLayout.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"
#include "graphics/BufferDescriptor.hpp"

class DX10GraphicsInterface;
class DX10RenderingContext;

struct DXInputLayoutArgs final
{
    DELETE_CM(DXInputLayoutArgs);
public:
    UINT* iaStrides;
    UINT* iaOffsets;
    ID3D10InputLayout* inputLayout;

    DXInputLayoutArgs() noexcept
        : iaStrides(nullptr)
        , iaOffsets(nullptr)
        , inputLayout(nullptr)
    { }

    ~DXInputLayoutArgs() noexcept
    {
        delete[] iaStrides;
        delete[] iaOffsets;
        RELEASE_DX(inputLayout);
    }
};

class TAU_DLL DX10InputLayout final : public IInputLayout
{
    DELETE_CM(DX10InputLayout);
private:
    ID3D10InputLayout* _inputLayout;
    UINT* _iaStrides;
    UINT* _iaOffsets;
public:
    DX10InputLayout(const DXInputLayoutArgs& dxArgs) noexcept
        : _inputLayout(dxArgs.inputLayout)
        , _iaStrides(dxArgs.iaStrides)
        , _iaOffsets(dxArgs.iaOffsets)
    { }

    ~DX10InputLayout() noexcept
    {
        RELEASE_DX(_inputLayout);
        delete[] _iaStrides;
        delete[] _iaOffsets;
    }

    [[nodiscard]] ID3D10InputLayout* inputLayout() const noexcept { return _inputLayout; }

    [[nodiscard]] const UINT* strides() const noexcept { return _iaStrides; }
    [[nodiscard]] const UINT* offsets() const noexcept { return _iaOffsets; }

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;
};

class TAU_DLL DX10InputLayoutBuilder final : public IInputLayoutBuilder
{
    DEFAULT_DESTRUCT(DX10InputLayoutBuilder);
    DEFAULT_CM_PU(DX10InputLayoutBuilder);
public:
    /**
     *   DirectX doesn't natively allow you to pass doubles or
     * matrices to a shader. As such these types take up more
     * than one element in the input layout.
     */
    static uSys computeNumElements(ShaderDataType::Type type) noexcept;

    static DXGI_FORMAT getDXType(ShaderDataType::Type type) noexcept;

    static const char* getDXSemanticName(ShaderSemantic::Semantic semantic) noexcept;
private:
    DX10GraphicsInterface& _gi;
public:
    DX10InputLayoutBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10InputLayout* build(const InputLayoutArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] DX10InputLayout* build(const InputLayoutArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IInputLayout> buildCPPRef(const InputLayoutArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] NullableRef<IInputLayout> buildTauRef(const InputLayoutArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] NullableStrongRef<IInputLayout> buildTauSRef(const InputLayoutArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
private:
    [[nodiscard]] bool processArgs(const InputLayoutArgs& args, DXInputLayoutArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
