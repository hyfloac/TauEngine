#include "shader/ShaderBindMap.hpp"

u32 ShaderBindMap::mapUniformBindPoint(const u32 index, const EShader::Stage stage) noexcept
{
    switch(stage)
    {
        case EShader::Stage::Vertex: return _vertexUniMap[index];
        case EShader::Stage::TessellationControl: return _tessCtrlUniMap[index];
        case EShader::Stage::TessellationEvaluation: return _tessEvalUniMap[index];
        case EShader::Stage::Geometry: return _geometryUniMap[index];
        case EShader::Stage::Pixel: return _pixelUniMap[index];
        default: return -1;
    }
}

u32 ShaderBindMap::mapTextureBindPoint(const u32 index, const EShader::Stage stage) noexcept
{
    switch(stage)
    {
        case EShader::Stage::Vertex: return _vertexTexMap[index];
        case EShader::Stage::TessellationControl: return _tessCtrlTexMap[index];
        case EShader::Stage::TessellationEvaluation: return _tessEvalTexMap[index];
        case EShader::Stage::Geometry: return _geometryTexMap[index];
        case EShader::Stage::Pixel: return _pixelTexMap[index];
        default: return -1;
    }
}
