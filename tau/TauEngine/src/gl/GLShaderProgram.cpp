#include "TauEngine.hpp"
#include "gl/GLShaderProgram.hpp"
#include "shader/Shader.hpp"
#include "gl/GLShader.hpp"
#include <Safeties.hpp>
#include <VariableLengthArray.hpp>

#include "shader/bundle/ShaderBundleParser.hpp"
#include "shader/bundle/ShaderInfoExtractorVisitor.hpp"
#include "Timings.hpp"
#include "VFS.hpp"

GLShaderProgram::GLShaderProgram() noexcept
    : IShaderProgram()
    , _programID(glCreateProgram())
{ Ensure(_programID != 0); }

GLShaderProgram::~GLShaderProgram() noexcept
{
    glDeleteProgram(_programID);
    _programID = 0;
}

void GLShaderProgram::bind(IRenderingContext&) noexcept
{ glUseProgram(_programID); }

void GLShaderProgram::unbind(IRenderingContext&) noexcept
{ glUseProgram(0); }

static bool validateFail(GLuint& programId, const char* const type) noexcept
{
    PERF();
    GLint length;

    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);

    if(!length)
    {
        printf("OpenGL failed to %s program, but no error message was generated.\n", type);
    }
    else if(length >= VLA_MAX_LEN)
    {
        GLchar* errorMsg = new GLchar[length];
        glGetShaderInfoLog(programId, length, &length, errorMsg);
        printf("OpenGL failed to %s program.\n  Error Message: %s\n", type, errorMsg);
        delete[] errorMsg;
    }
    else
    {
        VLA(GLchar, errorMsg, length);
        glGetShaderInfoLog(programId, length, &length, errorMsg);
        printf("OpenGL failed to %s program.\n  Error Message: %s\n", type, errorMsg);
    }

    glDeleteProgram(programId);
    programId = 0;

    return false;
}

bool GLShaderProgram::link(IRenderingContext&) noexcept
{
    PERF();
    glLinkProgram(_programID);

    GLint result;
    glGetProgramiv(_programID, GL_LINK_STATUS, &result);
    if(result == GL_FALSE)
    {
        return validateFail(_programID, "link");
    }

    glValidateProgram(_programID);

    glGetProgramiv(_programID, GL_VALIDATE_STATUS, &result);
    if(result == GL_FALSE)
    {
        return validateFail(_programID, "validate");
    }

    return true;
}

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
