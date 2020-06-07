#pragma once

#include "shader/ShaderProgram.hpp"

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
    DEFAULT_CM_PU(DX11ShaderProgram);
public:
    [[nodiscard]] NullableRef<DX11VertexShader>     dxVertexShader() const noexcept;
    [[nodiscard]] NullableRef<DX11HullShader>         dxHullShader() const noexcept;
    [[nodiscard]] NullableRef<DX11DomainShader>     dxDomainShader() const noexcept;
    [[nodiscard]] NullableRef<DX11GeometryShader> dxGeometryShader() const noexcept;
    [[nodiscard]] NullableRef<DX11PixelShader>       dxPixelShader() const noexcept;

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;
};
#endif
