#pragma warning(push, 0)
#include <GL/glew.h>
#include <cstring>
#include <cstdio>
#pragma warning(pop)
#include <gl/GLShader.hpp>
#include <Utils.hpp>
#include <VFS.hpp>
#include "Timings.hpp"
#include "gl/GLRenderingContext.hpp"
#include "shader/bundle/ShaderBundleParser.hpp"
#include "shader/bundle/ShaderInfoExtractorVisitor.hpp"
#include <VariableLengthArray.hpp>

static void clearWhiteSpace(uSys& index, const uSys length, const u8* const arr) noexcept
{
    for(; index < length && isspace(arr[index]); ++index);
}

static void clearLine(uSys& index, const uSys length, const u8* const arr) noexcept
{
    for(; index < length && arr[index] != '\n'; ++index);
}

static uSys readInclude(const uSys index, const uSys length, const u8* const arr) noexcept
{
    uSys i;
    // ReSharper disable once CppPossiblyErroneousEmptyStatements
    for(i = index; i < length && arr[i] != '>'; ++i);
    return i - index;
}

static RefDynArray<u8> handleIncludes(RefDynArray<u8>& shader) noexcept
{
    RefDynArray<u8> ret = shader;
    for(uSys i = 0; i < ret.count(); ++i)
    {
        clearWhiteSpace(i, ret.count(), ret.arr());
        if(ret.arr()[i] == '#')
        {
            const u32 includeBegin = i;
            ++i;
            if(i + 7 < ret.count())
            {
                if(strncmp(reinterpret_cast<const char*>(ret.arr() + i), "include", 7) == 0)
                {
                    i += 7;
                    clearWhiteSpace(i, ret.count(), ret.arr());
                    if(ret.arr()[i] == '<')
                    {
                        ++i;
                        const uSys includeLen = readInclude(i, ret.count(), ret.arr());
                        char* includePath = new char[includeLen + 1];
                        includePath[includeLen] = '\0';
                        ::std::memcpy(includePath, ret.arr() + i, includeLen);

                        i += includeLen + 1;

                        const uSys includeLineLen = i - includeBegin;

                        RefDynArray<u8> includeFileData = VFS::Instance().openFile(includePath, FileProps::Read)->readFile();

                        delete[] includePath;

                        RefDynArray<u8> includedFileData(ret.count() - includeLineLen + includeFileData.count());

                        ::std::memcpy(includedFileData.arr(), ret.arr(), includeBegin);
                        ::std::memcpy(includedFileData.arr() + includeBegin, includeFileData.arr(), includeFileData.count() - 1);
                        ::std::memcpy(includedFileData.arr() + includeBegin + includeFileData.count() - 1, ret.arr() + i, ret.count() - i);

                        ret = includedFileData;
                    }
                }
            }
        }
        clearLine(i, ret.count(), ret.arr());
    }
    return ret;
}

GLShader* GLShaderBuilder::build(const ShaderArgs& args, Error* error) const noexcept
{
    GLShaderArgs glArgs { };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLShader* const shader = new(::std::nothrow) GLShader(glArgs.shaderHandle, args.stage);

    ERROR_CODE_COND_N(!shader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, shader);
}

GLShader* GLShaderBuilder::build(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLShaderArgs glArgs { };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    GLShader* const shader = allocator.allocateT<GLShader>(glArgs.shaderHandle, args.stage);
    ERROR_CODE_COND_N(!shader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, shader);
}

CPPRef<IShader> GLShaderBuilder::buildCPPRef(const ShaderArgs& args, Error* error) const noexcept
{
    GLShaderArgs glArgs { };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const CPPRef<GLShader> shader = CPPRef<GLShader>(new(::std::nothrow) GLShader(glArgs.shaderHandle, args.stage));
    ERROR_CODE_COND_N(!shader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, shader);
}

NullableReferenceCountingPointer<IShader> GLShaderBuilder::buildTauRef(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLShaderArgs glArgs { };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableReferenceCountingPointer<GLShader> shader(allocator, glArgs.shaderHandle, args.stage);
    ERROR_CODE_COND_N(!shader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, shader);
}

NullableStrongReferenceCountingPointer<IShader> GLShaderBuilder::buildTauSRef(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLShaderArgs glArgs { };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const NullableStrongReferenceCountingPointer<GLShader> shader(allocator, glArgs.shaderHandle, args.stage);
    ERROR_CODE_COND_N(!shader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, shader);
}

bool GLShaderBuilder::processArgs(const ShaderArgs& args, GLShaderArgs* const glArgs, Error* error) const noexcept
{
    ERROR_CODE_COND_F(!args.file, Error::InvalidFile);
	
    GLenum glShaderStage;
	
    switch(args.stage)
    {
	    case EShader::Stage::Vertex:
	        glShaderStage = GL_VERTEX_SHADER;
	        break;
	    case EShader::Stage::TessellationControl:
	        glShaderStage = GL_TESS_CONTROL_SHADER;
	        break;
	    case EShader::Stage::TessellationEvaluation:
	        glShaderStage = GL_TESS_EVALUATION_SHADER;
	        break;
	    case EShader::Stage::Geometry:
	        glShaderStage = GL_GEOMETRY_SHADER;
	        break;
	    case EShader::Stage::Fragment:
	        glShaderStage = GL_FRAGMENT_SHADER;
	        break;
	    default: ERROR_CODE_F(Error::InvalidShaderStage);
    }

    const DynString path = args.file->name();

    if(VFS::getFileExt(path, false) == "tausi")
    {
        return processBundle(args, glArgs, glShaderStage, error);
    }
    else
    {
        return processShader(args.file, glArgs, glShaderStage, error);
    }
}

bool GLShaderBuilder::processBundle(const ShaderArgs& args, GLShaderArgs* const glArgs, const GLenum shaderStage, Error* const error) const noexcept
{
    ShaderBundleParser parser(args.file);

    ShaderBundleParser::Error parseError;
    auto ast = parser.parse(&parseError);

    _visitor->reset();
    _visitor->visit(ast.get());
    const sbp::ShaderInfo& info = _visitor->get(args.stage);

    const CPPRef<IFile> file = VFS::Instance().openFile(info.fileName, FileProps::Read);
    if(!processShader(file, glArgs, shaderStage, error))
    { return false; }

    for(auto& uniPoint : info.uniformPoints)
    {
        GLuint index;

        if(uniPoint.type == sbp::BindPointUnion::Str)
        {
            index = glGetUniformBlockIndex(glArgs->shaderHandle, uniPoint.bindName);
            
        }
        else if(uniPoint.type == sbp::BindPointUnion::Number)
        {
            index = uniPoint.mapPoint;
        }
        else
        {
            continue;
        }

        switch(uniPoint.crmTarget)
        {
            case CommonRenderingModelToken::UniformBindingCameraDynamic:
                glUniformBlockBinding(glArgs->shaderHandle, index, 0);
                break;
            case CommonRenderingModelToken::UniformBindingCameraStatic:
                glUniformBlockBinding(glArgs->shaderHandle, index, 1);
                break;
            default: break;
        }
    }

    for(auto& texPoint : info.texturePoints)
    {
        GLuint location;

        if(texPoint.type == sbp::BindPointUnion::Str)
        {
            location = glGetUniformLocation(glArgs->shaderHandle, texPoint.bindName);
        }
        else if(texPoint.type == sbp::BindPointUnion::Number)
        {
            location = texPoint.mapPoint;
        }
        else
        {
            continue;
        }

        switch(texPoint.crmTarget)
        {
            case CommonRenderingModelToken::TextureNormal:
                glProgramUniform1i(glArgs->shaderHandle, location, 0);
                break;
            case CommonRenderingModelToken::TextureDiffuse:
                glProgramUniform1i(glArgs->shaderHandle, location, 1);
                break;
            case CommonRenderingModelToken::TexturePBRCompound:
                glProgramUniform1i(glArgs->shaderHandle, location, 2);
                break;
            case CommonRenderingModelToken::TextureEmissivity:
                glProgramUniform1i(glArgs->shaderHandle, location, 3);
                break;
            case CommonRenderingModelToken::TexturePosition:
                glProgramUniform1i(glArgs->shaderHandle, location, 4);
                break;
            case CommonRenderingModelToken::TextureDepth:
                glProgramUniform1i(glArgs->shaderHandle, location, 5);
                break;
            case CommonRenderingModelToken::TextureStencil:
                glProgramUniform1i(glArgs->shaderHandle, location, 6);
                break;
            default: break;
        }
    }

    return true;
}

static bool validateFail(GLuint& programId, const char* type) noexcept;

bool GLShaderBuilder::processShader(const CPPRef<IFile>& file, GLShaderArgs* const glArgs, const GLenum shaderStage, Error* const error) const noexcept
{
    RefDynArray<u8> data = file->readFile();
    data = handleIncludes(data);

    const GLchar* const shaderSrc = reinterpret_cast<GLchar*>(data.arr());

    glArgs->shaderHandle = glCreateShaderProgramv(shaderStage, 1, &shaderSrc);

    if(glArgs->shaderHandle == GL_FALSE)
    {
#if !defined(TAU_PRODUCTION)
        GLint result;
        glGetShaderiv(GL_FALSE, GL_INFO_LOG_LENGTH, &result);
        if(result <= 0)
        {
            fprintf(stderr, "OpenGL failed to create a shader, but no error message was generated.\n");
        }
        else
        {
            GLchar* errorMsg = new GLchar[result];
            glGetShaderInfoLog(glArgs->shaderHandle, result, &result, errorMsg);
            fprintf(stderr, "OpenGL failed to create a shader.\n  Error Message: %s\n", errorMsg);
            delete[] errorMsg;
        }
#endif
        ERROR_CODE_F(Error::DriverMemoryAllocationFailure);
    }

    GLint result;
    glGetShaderiv(glArgs->shaderHandle, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
#if !defined(TAU_PRODUCTION)
        glGetShaderiv(glArgs->shaderHandle, GL_INFO_LOG_LENGTH, &result);
        if(result <= 0)
        {
            fprintf(stderr, "OpenGL failed to compile a shader, but no error message was generated.\n");
        }
        else
        {
            GLchar* errorMsg = new GLchar[result];
            glGetShaderInfoLog(glArgs->shaderHandle, result, &result, errorMsg);
            fprintf(stderr, "OpenGL failed to compile a shader.\n  Error Message: %s\n", errorMsg);
            delete[] errorMsg;
            fprintf(stderr, "File Path: %s\n", file->name());
            fprintf(stderr, "File Data: \n%s\n", shaderSrc);
        }
#endif

        glDeleteProgram(glArgs->shaderHandle);
        ERROR_CODE_F(Error::CompileError);
    }

    glGetProgramiv(glArgs->shaderHandle, GL_LINK_STATUS, &result);
    if(result == GL_FALSE)
    {
        return validateFail(glArgs->shaderHandle, "link");
    }

    return true;
}

static bool validateFail(GLuint& programId, const char* const type) noexcept
{
    PERF();
    GLint length;

    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);

    if(!length)
    {
        fprintf(stderr, "OpenGL failed to %s program, but no error message was generated.\n", type);
    }
    else if(length >= VLA_MAX_LEN)
    {
        GLchar* errorMsg = new GLchar[length];
        glGetShaderInfoLog(programId, length, &length, errorMsg);
        fprintf(stderr, "OpenGL failed to %s program.\n  Error Message: %s\n", type, errorMsg);
        delete[] errorMsg;
    }
    else
    {
        VLA(GLchar, errorMsg, length);
        glGetShaderInfoLog(programId, length, &length, errorMsg);
        fprintf(stderr, "OpenGL failed to %s program.\n  Error Message: %s\n", type, errorMsg);
    }

    glDeleteProgram(programId);
    programId = 0;

    return false;
}
