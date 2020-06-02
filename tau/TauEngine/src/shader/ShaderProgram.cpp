#include "shader/ShaderProgram.hpp"
#include "shader/Shader.hpp"
#include "system/RenderingContext.hpp"

bool IShaderProgram::setVertexShader(IRenderingContext&, const NullableRef<IShader>& vs) noexcept
{
    if(vs)
    {
        if(vs->shaderStage() == EShader::Stage::Vertex)
        {
            _vertexShader = vs;
            return true;
        }
    }
    return false;
}

bool IShaderProgram::setTessCtrlShader(IRenderingContext&, const NullableRef<IShader>& tcs) noexcept
{
    if(tcs)
    { 
        if(tcs->shaderStage() == EShader::Stage::TessellationControl)
        {
            _tessCtrlShader = tcs;
            return true;
        }
    }
    return false;
}

bool IShaderProgram::setTessEvalShader(IRenderingContext&, const NullableRef<IShader>& tes) noexcept
{
    if(tes)
    {
        if(tes->shaderStage() == EShader::Stage::TessellationEvaluation)
        {
            _tessEvalShader = tes;
            return true;
        }
    }
    return false;
}

bool IShaderProgram::setGeometryShader(IRenderingContext&, const NullableRef<IShader>& gs) noexcept
{
    if(gs)
    {
        if(gs->shaderStage() == EShader::Stage::Geometry)
        {
            _geometryShader = gs;
            return true;
        }
    }
    return false;
}

bool IShaderProgram::setPixelShader(IRenderingContext&, const NullableRef<IShader>& ps) noexcept
{
    if(ps)
    {
        if(ps->shaderStage() == EShader::Stage::Pixel)
        {
            _pixelShader = ps;
            return true;
        }
    }
    return false;
}
