#include "dx/dx10/DX10ShaderProgram.hpp"

#ifdef _WIN32
#include "dx/dx10/DX10Shader.hpp"

Ref<DX10VertexShader> DX10ShaderProgram::dxVertexShader() const noexcept
{
    return RefCast<DX10VertexShader>(_vertexShader);
}
#endif
