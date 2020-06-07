#pragma once

#include "shader/ShaderProgram.hpp"

#ifdef _WIN32
class DX10Shader;
class DX10VertexShader;
class DX10GeometryShader;
class DX10PixelShader;

class TAU_DLL DX10ShaderProgram final : public IShaderProgram
{
    DEFAULT_CONSTRUCT_PU(DX10ShaderProgram);
    DEFAULT_DESTRUCT(DX10ShaderProgram);
    DEFAULT_CM_PU(DX10ShaderProgram);
private:
    NullableRef<DX10VertexShader> _vertexShader;
    NullableRef<DX10GeometryShader> _geometryShader;
    NullableRef<DX10PixelShader> _pixelShader;
public:
    [[nodiscard]] NullableRef<DX10VertexShader>     dxVertexShader() const noexcept { return _vertexShader;   }
    [[nodiscard]] NullableRef<DX10GeometryShader> dxGeometryShader() const noexcept { return _geometryShader; }
    [[nodiscard]] NullableRef<DX10PixelShader>       dxPixelShader() const noexcept { return _pixelShader;    }

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;
};
#endif
