#pragma once

#include "shader/ShaderProgram.hpp"

#ifdef _WIN32
class DX10GraphicsInterface;

class DX10Shader;
class DX10VertexShader;
class DX10GeometryShader;
class DX10PixelShader;

struct DXShaderProgramArgs final
{
    NullableRef<DX10VertexShader> vertexShader;
    NullableRef<DX10GeometryShader> geometryShader;
    NullableRef<DX10PixelShader> pixelShader;
};

class TAU_DLL DX10ShaderProgram final : public IShaderProgram
{
    DEFAULT_CONSTRUCT_PU(DX10ShaderProgram);
    DEFAULT_DESTRUCT(DX10ShaderProgram);
    DEFAULT_CM_PU(DX10ShaderProgram);
    SHADER_PROGRAM_IMPL(DX10ShaderProgram);
private:
    NullableRef<DX10VertexShader> _vertexShader;
    NullableRef<DX10GeometryShader> _geometryShader;
    NullableRef<DX10PixelShader> _pixelShader;
public:
    [[nodiscard]] const NullableRef<DX10VertexShader>&     dxVertexShader() const noexcept { return _vertexShader;   }
    [[nodiscard]] const NullableRef<DX10GeometryShader>& dxGeometryShader() const noexcept { return _geometryShader; }
    [[nodiscard]] const NullableRef<DX10PixelShader>&       dxPixelShader() const noexcept { return _pixelShader;    }
};

class TAU_DLL DX10ShaderProgramBuilder final : public IShaderProgramBuilder
{
    DEFAULT_DESTRUCT(DX10ShaderProgramBuilder);
    DEFAULT_CM_PU(DX10ShaderProgramBuilder);
private:
    DX10GraphicsInterface& _gi;
public:
    DX10ShaderProgramBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] NullableRef<IShaderProgram> build(const ShaderProgramAutoArgs&   args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableRef<IShaderProgram> build(const ShaderProgramManualArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const ShaderProgramAutoArgs&   args, [[tau::out]] DXShaderProgramArgs* dxArgs, [[tau::out]] Error* error);
    [[nodiscard]] bool processArgs(const ShaderProgramManualArgs& args, [[tau::out]] DXShaderProgramArgs* dxArgs, [[tau::out]] Error* error);
};
#endif
