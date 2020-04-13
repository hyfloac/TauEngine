#pragma once

#include "shader/IShaderProgram.hpp"

#ifdef _WIN32
class DX10Shader;
class DX10VertexShader;
class DX10GeometryShader;
class DX10PixelShader;

class TAU_DLL DX10ShaderProgram final : public IShaderProgram
{
    DEFAULT_CONSTRUCT_PU(DX10ShaderProgram);
    DEFAULT_DESTRUCT(DX10ShaderProgram);
    DELETE_COPY(DX10ShaderProgram);
public:
    [[nodiscard]] CPPRef<DX10VertexShader> dxVertexShader() const noexcept;
    [[nodiscard]] CPPRef<DX10GeometryShader> dxGeometryShader() const noexcept;
    [[nodiscard]] CPPRef<DX10PixelShader> dxPixelShader() const noexcept;

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;
    bool link(IRenderingContext& context) noexcept override;
protected:
    bool attach(IRenderingContext& context, const CPPRef<IShader>& shader) noexcept override;
    void detach(IRenderingContext& context, const CPPRef<IShader>& shader) noexcept override { }
};
#endif
