#include <Safeties.hpp>
#include <VariableLengthArray.hpp>
#include <VFS.hpp>

#include "gl/GLShader.hpp"
#include "gl/GLShaderProgram.hpp"
#include "shader/bundle/ShaderBundleParser.hpp"
#include "shader/bundle/ShaderInfoExtractorVisitor.hpp"

GLShaderProgram::~GLShaderProgram() noexcept
{
    glDeleteProgram(_programHandle);
    _shaderManager->release(_vertex);
    _shaderManager->release(_tessCtrl);
    _shaderManager->release(_tessEval);
    _shaderManager->release(_geometry);
    _shaderManager->release(_pixel);
}

bool GLShaderProgramBuilder::processArgs(const ShaderProgramArgs& args, GLShaderProgramArgs* glArgs, Error* error) noexcept
{
    ERROR_CODE_COND_F(!args.bundleFile, Error::InvalidFile);

    ERROR_CODE_T(Error::NoError);
}

static GLint transformCRM(CommonRenderingModelToken crmTarget) noexcept;
static GLenum glShaderStage(EShader::Stage stage) noexcept;

bool GLShaderProgramBuilder::processBundle(const ShaderProgramArgs& args, GLShaderProgramArgs* glArgs, Error* error) noexcept
{
    ShaderBundleParser parser(args.bundleFile);

    ShaderBundleParser::Error parseError;
    auto ast = parser.parse(&parseError);

    _visitor->reset();
    _visitor->visit(ast.get());

    const GLuint programHandle = glCreateProgram();

    for(auto it = _visitor->begin(); it != _visitor->end(); ++it)
    {
        const auto& info = *it;

        GLShaderData* shader = _shaderManager->acquire(info.fileName);

        if(!shader)
        {
            if(!processShader(info.fileName, it.stage(), &shader, error))
            {
                glDeleteProgram(programHandle);

                _shaderManager->release(glArgs->vertex);
                _shaderManager->release(glArgs->tessCtrl);
                _shaderManager->release(glArgs->tessEval);
                _shaderManager->release(glArgs->geometry);
                _shaderManager->release(glArgs->pixel);

                return false;
            }

        }

        glAttachShader(programHandle, shader->handle());
        glArgs->shaders[shaderIndex(it.stage())] = shader;
    }

    glLinkProgram(programHandle);

    GLint result;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &result);
    if(result == GL_FALSE)
    {
        glDeleteProgram(programHandle);

        _shaderManager->release(glArgs->vertex);
        _shaderManager->release(glArgs->tessCtrl);
        _shaderManager->release(glArgs->tessEval);
        _shaderManager->release(glArgs->geometry);
        _shaderManager->release(glArgs->pixel);

        return false;
    }

    for(const auto& info : *_visitor)
    {
        for(auto& uniPoint : info.uniformPoints)
        {
            GLuint index;

            if(uniPoint.binding.type == sbp::BindingUnion::Str)
            {
                index = glGetUniformBlockIndex(programHandle, uniPoint.binding.str);
            }
            else if(uniPoint.binding.type == sbp::BindingUnion::Number)
            {
                index = uniPoint.binding.number;
            }
            else
            {
                continue;
            }

            const GLint bindingPoint = transformCRM(uniPoint.crmTarget);
            glUniformBlockBinding(programHandle, index, bindingPoint);
        }

        for(auto& texPoint : info.texturePoints)
        {
            GLuint location;

            if(texPoint.binding.type == sbp::BindingUnion::Str)
            {
                location = glGetUniformLocation(programHandle, texPoint.binding.str);
            }
            else if(texPoint.binding.type == sbp::BindingUnion::Number)
            {
                location = texPoint.binding.number;
            }
            else
            {
                continue;
            }

            const GLint bindingPoint = transformCRM(texPoint.crmTarget);
            glProgramUniform1i(programHandle, location, bindingPoint);
        }
    }

    ERROR_CODE_T(Error::NoError);
}

bool GLShaderProgramBuilder::processShader(const DynString& path, const EShader::Stage stage, GLShaderData** const shaderData, Error* const error) noexcept
{
    const GLenum glStage = glShaderStage(stage);
    ERROR_CODE_COND_F(glStage == 0, Error::InvalidShaderStage);

    const CPPRef<IFile> file = VFS::Instance().openFile(path, FileProps::Read);

    IShaderBuilder::Error shaderError;

    GLShaderBuilder::GLShaderArgs glShaderArgs;
    if(!_shaderBuilder->processShader(file, &glShaderArgs, glStage, &shaderError))
    {
        switch(shaderError)
        {
            case IShaderBuilder::Error::InvalidShaderStage:            ERROR_CODE_F(Error::InvalidShaderStage);
            case IShaderBuilder::Error::CompileError:                  ERROR_CODE_F(Error::CompileError);
            case IShaderBuilder::Error::InvalidFile:                   ERROR_CODE_F(Error::InvalidFile);
            case IShaderBuilder::Error::InvalidInclude:                ERROR_CODE_F(Error::InvalidInclude);
            case IShaderBuilder::Error::SystemMemoryAllocationFailure: ERROR_CODE_F(Error::SystemMemoryAllocationFailure);
            case IShaderBuilder::Error::DriverMemoryAllocationFailure: ERROR_CODE_F(Error::DriverMemoryAllocationFailure);
            default:                                                   ERROR_CODE_F(Error::InternalError);
        }
    }

    *shaderData = _shaderManager->create(glShaderArgs.shaderHandle, stage, path);

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

static GLenum glShaderStage(const EShader::Stage stage) noexcept
{
    switch(stage)
    {
        case EShader::Stage::Vertex:                 return GL_VERTEX_SHADER;
        case EShader::Stage::TessellationControl:    return GL_TESS_CONTROL_SHADER;
        case EShader::Stage::TessellationEvaluation: return GL_TESS_EVALUATION_SHADER;
        case EShader::Stage::Geometry:               return GL_GEOMETRY_SHADER;
        case EShader::Stage::Fragment:               return GL_FRAGMENT_SHADER;
        default:                                     return 0;
    }
}
