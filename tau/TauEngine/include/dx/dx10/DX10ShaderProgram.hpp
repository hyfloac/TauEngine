#pragma once

#include "shader/IShaderProgram.hpp"

#ifdef _WIN32

class DX10VertexShader;
class DX10GeometryShader;
class DX10PixelShader;

class TAU_DLL DX10ShaderProgram final : public IShaderProgram
{
    DEFAULT_CONSTRUCT_PU(DX10ShaderProgram);
    DEFAULT_DESTRUCT(DX10ShaderProgram);
    DELETE_COPY(DX10ShaderProgram);
public:
    [[nodiscard]] Ref<DX10VertexShader> dxVertexShader() const noexcept;
};
#endif
