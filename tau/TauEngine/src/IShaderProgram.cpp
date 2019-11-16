#include "shader/IShaderProgram.hpp"
#include "shader/IShader.hpp"
#include "RenderingMode.hpp"
#include "system/RenderingContext.hpp"
#include "gl/GLShaderProgram.hpp"
#include "Timings.hpp"

Ref<IShaderProgram> IShaderProgram::create(IRenderingContext& context) noexcept
{
    switch(context.mode().currentMode())
    {
        case RenderingMode::Mode::DirectX9:
        #ifdef _WIN32
            return null;
        #else
            return null;
        #endif
        case RenderingMode::Mode::DirectX11:
        case RenderingMode::Mode::DirectX12:
        case RenderingMode::Mode::DirectX12_1:
        #ifdef _WIN32
            return null;
        #else
            return null;
        #endif
        case RenderingMode::Mode::Vulkan:
            return null;
        case RenderingMode::Mode::OpenGL2:
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
            return Ref<IShaderProgram>(new(std::nothrow) GLShaderProgram(context));
        default: return null;
    }
}

IShaderProgram::IShaderProgram() noexcept
    : _vertexShader(null),
      _tessellationControlShader(null), _tessellationEvaluationShader(null),
      _geometryShader(null), _pixelShader(null)
{ }

bool IShaderProgram::setVertexShader(IRenderingContext& context, Ref<IShader>& vs) noexcept
{
    if(vs)
    {
        if(vs->shaderType() == IShader::Type::Vertex)
        {
            if(_vertexShader)
            {
                this->detach(context, _vertexShader);
            }
            this->attach(context, vs);
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

bool IShaderProgram::setTessellationControlShader(IRenderingContext& context, Ref<IShader>& tcs) noexcept
{
    if(tcs)
    { 
        if(tcs->shaderType() == IShader::Type::TessellationControl)
        {
            if(_tessellationControlShader)
            {
                this->detach(context, _tessellationControlShader);
            }
            this->attach(context, tcs);
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

bool IShaderProgram::setTessellationEvaluationShader(IRenderingContext& context, Ref<IShader>& tes) noexcept
{
    if(tes)
    {
        if(tes->shaderType() == IShader::Type::TessellationEvaluation)
        {
            if(_tessellationEvaluationShader)
            {
                this->detach(context, _tessellationEvaluationShader);
            }
            this->attach(context, tes);
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

bool IShaderProgram::setGeometryShader(IRenderingContext& context, Ref<IShader>& gs) noexcept
{
    if(gs)
    {
        if(gs->shaderType() == IShader::Type::Geometry)
        {
            if(_geometryShader)
            {
                this->detach(context, _geometryShader);
            }
            this->attach(context, gs);
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

bool IShaderProgram::setPixelShader(IRenderingContext& context, Ref<IShader>& ps) noexcept
{
    if(ps)
    {
        if(ps->shaderType() == IShader::Type::Pixel)
        {
            if(_pixelShader)
            {
                this->detach(context, _pixelShader);
            }
            this->attach(context, ps);
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
