#pragma once

#include "shader/IShaderProgram.hpp"

#ifdef _WIN32

class DX11Shader;
class DX11VertexShader;
class DX11HullShader;
class DX11DomainShader;
class DX11GeometryShader;
class DX11PixelShader;

class TAU_DLL DX11ShaderProgram final : public IShaderProgram
{
    DEFAULT_CONSTRUCT_PU(DX11ShaderProgram);
    DEFAULT_DESTRUCT(DX11ShaderProgram);
    DELETE_COPY(DX11ShaderProgram);
public:
    [[nodiscard]] CPPRef<DX11VertexShader> dxVertexShader() const noexcept;
    [[nodiscard]] CPPRef<DX11HullShader> dxHullShader() const noexcept;
    [[nodiscard]] CPPRef<DX11DomainShader> dxDomainShader() const noexcept;
    [[nodiscard]] CPPRef<DX11GeometryShader> dxGeometryShader() const noexcept;
    [[nodiscard]] CPPRef<DX11PixelShader> dxPixelShader() const noexcept;

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;
    bool link(IRenderingContext& context) noexcept override;
protected:
    bool attach(IRenderingContext& context, CPPRef<IShader> shader) noexcept override;
    void detach(IRenderingContext& context, CPPRef<IShader> shader) noexcept override;
};
#endif
