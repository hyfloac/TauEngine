#include <Safeties.hpp>
#include <VariableLengthArray.hpp>
#include <VFS.hpp>

#include "gl/GLShader.hpp"
#include "gl/GLShaderProgram.hpp"
#include "shader/bundle/ShaderBundleParser.hpp"
#include "shader/bundle/ShaderInfoExtractorVisitor.hpp"

GLShaderProgram::GLShaderProgram() noexcept
    : IShaderProgram()
    , _pipelineHandle(glCreateProgram())
{ Ensure(_pipelineHandle != 0); }

GLShaderProgram::~GLShaderProgram() noexcept
{
    glDeleteProgramPipelines(1, &_pipelineHandle);
    _pipelineHandle = 0;
}

void GLShaderProgram::bind(IRenderingContext&) noexcept
{
    glUseProgramStages(_pipelineHandle);
}

void GLShaderProgram::unbind(IRenderingContext&) noexcept
{ glUseProgram(0); }

bool GLShaderProgramBuilder::processArgs(const ShaderProgramArgs& args, GLShaderProgramArgs* glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.bundleFile, Error::InvalidFile);

    ERROR_CODE_T(Error::NoError);
}

static GLint transformCRM(const CommonRenderingModelToken crmTarget) noexcept;

bool GLShaderProgramBuilder::processBundle(const ShaderProgramArgs& args, GLShaderProgramArgs* glArgs, Error* error) const noexcept
{
    ShaderBundleParser parser(args.bundleFile);

    ShaderBundleParser::Error parseError;
    auto ast = parser.parse(&parseError);

    _visitor->reset();
    _visitor->visit(ast.get());

    for(const auto& info : *_visitor)
    {
        const CPPRef<IFile> file = VFS::Instance().openFile(info.fileName, FileProps::Read);

        GLuint shaderHandle;
        if(!processShader(file, glArgs, &shaderHandle, info, error))
        { return false; }

        for(auto& uniPoint : info.uniformPoints)
        {
            GLuint index;

            if(uniPoint.type == sbp::BindPointUnion::Str)
            {
                index = glGetUniformBlockIndex(shaderHandle, uniPoint.bindName);
            }
            else if(uniPoint.type == sbp::BindPointUnion::Number)
            {
                index = uniPoint.mapPoint;
            }
            else
            {
                continue;
            }

            const GLint bindingPoint = transformCRM(uniPoint.crmTarget);
            glUniformBlockBinding(shaderHandle, index, bindingPoint);
        }

        for(auto& texPoint : info.texturePoints)
        {
            GLuint location;

            if(texPoint.type == sbp::BindPointUnion::Str)
            {
                location = glGetUniformLocation(shaderHandle, texPoint.bindName);
            }
            else if(texPoint.type == sbp::BindPointUnion::Number)
            {
                location = texPoint.mapPoint;
            }
            else
            {
                continue;
            }

            const GLint bindingPoint = transformCRM(texPoint.crmTarget);
            glProgramUniform1i(shaderHandle, location, bindingPoint);
        }
    }

    ERROR_CODE_T(Error::NoError);
}

bool GLShaderProgramBuilder::processShader(const CPPRef<IFile>& args, GLShaderProgramArgs* const glArgs, GLuint* const shaderHandle, const sbp::ShaderInfo& shaderInfo, Error* const error) const noexcept
{
    return true;
}

static GLint transformCRM(const CommonRenderingModelToken crmTarget) noexcept
{
    switch(crmTarget)
    {
        case CommonRenderingModelToken::UniformBindingCameraDynamic: return 0;
        case CommonRenderingModelToken::UniformBindingCameraStatic:  return 1;
        case CommonRenderingModelToken::TextureNormal:      return 0;
        case CommonRenderingModelToken::TextureDiffuse:     return 1;
        case CommonRenderingModelToken::TexturePBRCompound: return 2;
        case CommonRenderingModelToken::TextureEmissivity:  return 3;
        case CommonRenderingModelToken::TexturePosition:    return 4;
        case CommonRenderingModelToken::TextureDepth:       return 5;
        case CommonRenderingModelToken::TextureStencil:     return 6;
        default: return -1;;
    }
}
