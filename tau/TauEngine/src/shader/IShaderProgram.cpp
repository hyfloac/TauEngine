#include "shader/IShaderProgram.hpp"
#include "shader/IShader.hpp"
#include "RenderingMode.hpp"
#include "system/RenderingContext.hpp"
#include "system/GraphicsInterface.hpp"
#include "gl/GLShaderProgram.hpp"
#include "dx/dx10/DX10ShaderProgram.hpp"
#include "dx/dx11/DX11ShaderProgram.hpp"

CPPRef<IShaderProgram> IShaderProgram::create(IGraphicsInterface& gi) noexcept
{
    switch(gi.renderingMode().currentMode())
    {
        case RenderingMode::Mode::DirectX9:
        #ifdef _WIN32
            return null;
        #else
            return null;
        #endif
        case RenderingMode::Mode::DirectX10:
        #ifdef _WIN32
            return CPPRef<IShaderProgram>(new(::std::nothrow) DX10ShaderProgram);
        #else
            return null;
        #endif
        case RenderingMode::Mode::DirectX11:
        #ifdef _WIN32
            return CPPRef<IShaderProgram>(new(::std::nothrow) DX11ShaderProgram);
        #else
            return null;
        #endif
        case RenderingMode::Mode::DirectX12:
        case RenderingMode::Mode::DirectX12_1:
        #ifdef _WIN32
            return null;
        #else
            return null;
        #endif
        case RenderingMode::Mode::Vulkan:
            return null;
        case RenderingMode::Mode::OpenGL3:
        case RenderingMode::Mode::OpenGL3_1:
        case RenderingMode::Mode::OpenGL3_2:
        case RenderingMode::Mode::OpenGL3_3:
        case RenderingMode::Mode::OpenGL4:
        case RenderingMode::Mode::OpenGL4_2:
        case RenderingMode::Mode::OpenGL4_3:
        case RenderingMode::Mode::OpenGL4_4:
        case RenderingMode::Mode::OpenGL4_5:
        case RenderingMode::Mode::OpenGL4_6:
            return CPPRef<IShaderProgram>(new(std::nothrow) GLShaderProgram);
        default: return null;
    }
}

IShaderProgram::IShaderProgram() noexcept
    : _vertexShader(null),
      _tessellationControlShader(null), _tessellationEvaluationShader(null),
      _geometryShader(null), _pixelShader(null)
{ }

bool IShaderProgram::setVertexShader(IRenderingContext& context, const CPPRef<IShader>& vs) noexcept
{
    if(vs)
    {
        if(vs->shaderStage() == EShader::Stage::Vertex)
        {
            if(_vertexShader)
            {
                this->detach(context, _vertexShader);
            }
            if(!this->attach(context, vs))
            {
                return false;
            }
            _vertexShader = vs;
            return true;
        }
    }
    else
    {
        if(_vertexShader)
        {
            this->detach(context, _vertexShader);
        }
    }
    return false;
}

bool IShaderProgram::setTessellationControlShader(IRenderingContext& context, const CPPRef<IShader>& tcs) noexcept
{
    if(tcs)
    { 
        if(tcs->shaderStage() == EShader::Stage::TessellationControl)
        {
            if(_tessellationControlShader)
            {
                this->detach(context, _tessellationControlShader);
            }
            if(!this->attach(context, tcs))
            {
                return false;
            }
            _tessellationControlShader = tcs;
            return true;
        }
    }
    else
    {
        if(_tessellationControlShader)
        {
            this->detach(context, _tessellationControlShader);
        }
    }
    return false;
}

bool IShaderProgram::setTessellationEvaluationShader(IRenderingContext& context, const CPPRef<IShader>& tes) noexcept
{
    if(tes)
    {
        if(tes->shaderStage() == EShader::Stage::TessellationEvaluation)
        {
            if(_tessellationEvaluationShader)
            {
                this->detach(context, _tessellationEvaluationShader);
            }
            if(!this->attach(context, tes))
            {
                return false;
            }
            _tessellationEvaluationShader = tes;
            return true;
        }
    }
    else
    {
        if(_tessellationEvaluationShader)
        {
            this->detach(context, _tessellationEvaluationShader);
        }
    }
    return false;
}

bool IShaderProgram::setGeometryShader(IRenderingContext& context, const CPPRef<IShader>& gs) noexcept
{
    if(gs)
    {
        if(gs->shaderStage() == EShader::Stage::Geometry)
        {
            if(_geometryShader)
            {
                this->detach(context, _geometryShader);
            }
            if(!this->attach(context, gs))
            {
                return false;
            }
            _geometryShader = gs;
            return true;
        }
    }
    else
    {
        if(_geometryShader)
        {
            this->detach(context, _geometryShader);
        }
    }
    return false;
}

bool IShaderProgram::setPixelShader(IRenderingContext& context, const CPPRef<IShader>& ps) noexcept
{
    if(ps)
    {
        if(ps->shaderStage() == EShader::Stage::Pixel)
        {
            if(_pixelShader)
            {
                this->detach(context, _pixelShader);
            }
            if(!this->attach(context, ps))
            {
                return false;
            }
            _pixelShader = ps;
            return true;
        }
    }
    else
    {
        if(_pixelShader)
        {
            this->detach(context, _pixelShader);
        }
    }
    return false;
}
