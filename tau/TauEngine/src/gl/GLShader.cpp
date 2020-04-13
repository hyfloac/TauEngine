#pragma warning(push, 0)
#include <GL/glew.h>
#include <cstring>
#pragma warning(pop)
#include <gl/GLShader.hpp>
#include <Utils.hpp>
#include <cstdio>
#include "VFS.hpp"
#include "Timings.hpp"
#include "gl/GLRenderingContext.hpp"
#include "gl/GLGraphicsInterface.hpp"

GLShader::GLShader(const GLuint shaderID, const EShader::Stage shaderType) noexcept
    : IShader(), _shaderID(shaderID), _shaderType(shaderType)
{ }

GLShader::~GLShader() noexcept
{
    if(this->_shaderID)
    {
        glDeleteShader(this->_shaderID);
        this->_shaderID = 0;
    }
}

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
                        memcpy(includePath, ret.arr() + i, includeLen);

                        i += includeLen + 1;

                        const uSys includeLineLen = i - includeBegin;

                        RefDynArray<u8> includeFileData = VFS::Instance().openFile(includePath, FileProps::Read)->readFile();

                        delete[] includePath;

                        RefDynArray<u8> includedFileData(ret.count() - includeLineLen + includeFileData.count());

                        memcpy(includedFileData.arr(), ret.arr(), includeBegin);
                        memcpy(includedFileData.arr() + includeBegin, includeFileData.arr(), includeFileData.count() - 1);
                        memcpy(includedFileData.arr() + includeBegin + includeFileData.count() - 1, ret.arr() + i, ret.count() - i);

                        ret = includedFileData;
                    }
                }
            }
        }
        clearLine(i, ret.count(), ret.arr());
    }
    return ret;
}

GLShaderBuilder::GLShaderBuilder(GLGraphicsInterface& gi) noexcept
	: IShaderBuilder(), _resIndex(IShaderBuilder::rsTransformer->transform(gi.renderingMode()))
{ }

GLShader* GLShaderBuilder::build(const ShaderArgs& args, Error* error) const noexcept
{
    GLShaderArgs glArgs { };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    if(!compileShader(args, glArgs, error))
    { return null; }

    GLShader* const shader = new(::std::nothrow) GLShader(glArgs.shaderID, args.stage);

    ERROR_CODE_COND_N(!shader, Error::MemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, shader);
}

GLShader* GLShaderBuilder::build(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLShaderArgs glArgs { };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    if(!compileShader(args, glArgs, error))
    { return null; }

    GLShader* const shader = allocator.allocateT<GLShader>(glArgs.shaderID, args.stage);

    ERROR_CODE_COND_N(!shader, Error::MemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, shader);
}

CPPRef<IShader> GLShaderBuilder::buildCPPRef(const ShaderArgs& args, Error* error) const noexcept
{
    GLShaderArgs glArgs { };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    if(!compileShader(args, glArgs, error))
    { return null; }

    const CPPRef<GLShader> shader = CPPRef<GLShader>(new(::std::nothrow) GLShader(glArgs.shaderID, args.stage));

    ERROR_CODE_COND_N(!shader, Error::MemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, shader);
}

NullableReferenceCountingPointer<IShader> GLShaderBuilder::buildTauRef(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLShaderArgs glArgs { };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    if(!compileShader(args, glArgs, error))
    { return null; }

    const NullableReferenceCountingPointer<GLShader> shader(allocator, glArgs.shaderID, args.stage);

    ERROR_CODE_COND_N(!shader, Error::MemoryAllocationFailure);

    NullableReferenceCountingPointer<IShader> iShader = RCPCast<IShader>(shader);
	
    ERROR_CODE_V(Error::NoError, iShader);
}

NullableStrongReferenceCountingPointer<IShader> GLShaderBuilder::buildTauSRef(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLShaderArgs glArgs { };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    if(!compileShader(args, glArgs, error))
    { return null; }

    const NullableStrongReferenceCountingPointer<GLShader> shader(allocator, glArgs.shaderID, args.stage);

    ERROR_CODE_COND_N(!shader, Error::MemoryAllocationFailure);

    NullableStrongReferenceCountingPointer<IShader> iShader = RCPCast<IShader>(shader);
	
    ERROR_CODE_V(Error::NoError, iShader);
}

bool GLShaderBuilder::processArgs(const ShaderArgs& args, GLShaderArgs* glArgs, Error* error) noexcept
{
    ERROR_CODE_COND_F(!args.vfsMount, Error::InvalidFile);
    ERROR_CODE_COND_F(!args.path, Error::InvalidFile);
    ERROR_CODE_COND_F(!args.fileName, Error::InvalidFile);
	
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

    glArgs->shaderID = glCreateShader(glShaderStage);

    if(glArgs->shaderID == GL_FALSE)
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
            glGetShaderInfoLog(glArgs->shaderID, result, &result, errorMsg);
            fprintf(stderr, "OpenGL failed to create a shader.\n  Error Message: %s\n", errorMsg);
            delete[] errorMsg;
        }
#endif
        ERROR_CODE_F(Error::ShaderObjectCreationFailure);
    }
	
    return true;
}

bool GLShaderBuilder::compileShader(const ShaderArgs& args, GLShaderArgs& glArgs, Error* error) const noexcept
{
    const ResourceSelector shaderSelector = ResourceSelectorLoader::load(args.vfsMount, args.path, args.fileName, IShaderBuilder::rsTransformer);

    ERROR_CODE_COND_F(shaderSelector.count() == 0, Error::InvalidFile);
    auto& selected = shaderSelector.select(_resIndex);
    ERROR_CODE_COND_F(!selected.loader() || selected.path().length() == 0 || selected.name().length() == 0, Error::InvalidFile);
    const CPPRef<IFile> shaderFile = selected.loadFile(FileProps::Read);

    ERROR_CODE_COND_F(!shaderFile, Error::InvalidFile);

#if defined(TAU_PRODUCTION)
    {
#endif
    RefDynArray<u8> data = shaderFile->readFile();
    data = handleIncludes(data);

    const GLchar* const shaderSrc = reinterpret_cast<GLchar*>(data.arr());
    const GLint length = static_cast<GLint>(data.length());

    glShaderSource(glArgs.shaderID, 1, &shaderSrc, &length);
    glCompileShader(glArgs.shaderID);
#if defined(TAU_PRODUCTION)
    }
#endif

    GLint result;
    glGetShaderiv(glArgs.shaderID, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
#if !defined(TAU_PRODUCTION)
        glGetShaderiv(glArgs.shaderID, GL_INFO_LOG_LENGTH, &result);
        if(result <= 0)
        {
            fprintf(stderr, "OpenGL failed to compile a shader, but no error message was generated.\n");
        }
        else
        {
            GLchar* errorMsg = new GLchar[result];
            glGetShaderInfoLog(glArgs.shaderID, result, &result, errorMsg);
            fprintf(stderr, "OpenGL failed to compile a shader.\n  Error Message: %s\n", errorMsg);
            delete[] errorMsg;
            fprintf(stderr, "File Path: %s%s%s\n", args.vfsMount, args.path, args.fileName);
            fprintf(stderr, "File Data: \n%s\n", shaderSrc);
        }
#endif

        glDeleteShader(glArgs.shaderID);
        ERROR_CODE_F(Error::CompileError);
    }
	
    return true;
}
