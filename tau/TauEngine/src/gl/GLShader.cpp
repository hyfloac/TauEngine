#pragma warning(push, 0)
#include <GL/glew.h>
#include <cstring>
#include <cstdio>
#pragma warning(pop)

#include <Utils.hpp>
#include <VFS.hpp>
#include <VariableLengthArray.hpp>
#include <ConPrinter.hpp>

#include "Timings.hpp"
#include "gl/GLShader.hpp"
#include "shader/bundle/ShaderBundleParser.hpp"
#include "shader/bundle/ShaderInfoExtractorVisitor.hpp"

static bool validateFail(GLuint shaderHandle, const char* type) noexcept;

GLShaderData* GLShaderManager::acquire(const DynString& path) noexcept
{
    if(_shaderMap.contains(path))
    {
        GLShaderData* const shader = _shaderMap.at(path);

        ++shader->_refCount;

        return shader;
    }

    return null;
}

void GLShaderManager::release(GLShaderData* const shader) noexcept
{
    if(!shader)
    { return; }

    if(--shader->_refCount > 0)
    { return; }

    if(_shaderMap.contains(shader->path()))
    { _shaderMap.erase(shader->path()); }

    _allocator.deallocateT(shader);
}

GLShaderData* GLShaderManager::create(GLuint handle, EShader::Stage stage, const DynString& path) noexcept
{
    if(_shaderMap.contains(path))
    { return null; }

    GLShaderData* const shader = _allocator.allocateT<GLShaderData>(handle, stage, path);

    _shaderMap.emplace(path, shader);

    return shader;
}

GLShaderData* GLShaderManager::create(GLuint handle, EShader::Stage stage, DynString&& path) noexcept
{
    if(_shaderMap.contains(path))
    { return null; }

    GLShaderData* const shader = _allocator.allocateT<GLShaderData>(handle, stage, ::std::move(path));

    _shaderMap.emplace(path, shader);

    return shader;
}

static bool isWhitespace(const char c) noexcept
{
    switch(c)
    {
        case ' ':
        case '\t':
        case '\r':
        case '\n': return true;
        default: return false;
    }
}

static void clearWhiteSpace(uSys& index, const uSys length, const u8* const arr) noexcept
{
    for(; index < length && isWhitespace(arr[index]); ++index);
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

ReferenceCountingPointer<IShader> GLShaderBuilder::buildTauRef(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLShaderArgs glArgs { };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const ReferenceCountingPointer<GLShader> shader(allocator, glArgs.shaderHandle, args.stage);
    ERROR_CODE_COND_N(!shader, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, shader);
}

StrongReferenceCountingPointer<IShader> GLShaderBuilder::buildTauSRef(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept
{
    GLShaderArgs glArgs { };
    if(!processArgs(args, &glArgs, error))
    { return null; }

    const StrongReferenceCountingPointer<GLShader> shader(allocator, glArgs.shaderHandle, args.stage);
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

    return processShader(file, glArgs, shaderStage, error);
}

bool GLShaderBuilder::processShader(const CPPRef<IFile>& file, GLShaderArgs* const glArgs, const GLenum shaderStage, Error* const error) const noexcept
{
    RefDynArray<u8> data = file->readFile();
    data = handleIncludes(data);

    const GLchar* const shaderSrc = reinterpret_cast<GLchar*>(data.arr());

    glArgs->shaderHandle = glCreateShader(shaderStage);

    if(glArgs->shaderHandle == GL_FALSE)
    {
#if !defined(TAU_PRODUCTION)
        (void) validateFail(glArgs->shaderHandle, "create");
#else
        glDeleteShader(glArgs->shaderHandle);
#endif
        ERROR_CODE_F(Error::DriverMemoryAllocationFailure);
    }

    const GLint shaderLength = data.length();
    glShaderSource(glArgs->shaderHandle, 1, &shaderSrc, &shaderLength);

    glCompileShader(glArgs->shaderHandle);

    GLint result;
    glGetShaderiv(glArgs->shaderHandle, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
#if !defined(TAU_PRODUCTION)
        (void) validateFail(glArgs->shaderHandle, "compile");
        ConPrinter::print(stderr, "File Path: %\n", file->name());
        ConPrinter::print(stderr, "File Data: \n%\n", shaderSrc);
#else
        glDeleteProgram(glArgs->shaderHandle);
#endif
        ERROR_CODE_F(Error::CompileError);
    }

    return true;
}

static bool validateFail(const GLuint shaderHandle, const char* const type) noexcept
{
    PERF();
    GLint length;

    glGetProgramiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);

    if(!length)
    {
        ConPrinter::print(stderr, "OpenGL failed to % shader, but no error message was generated.\n", type);
    }
    else if(length >= VLA_MAX_LEN)
    {
        GLchar* errorMsg = new GLchar[length];
        glGetShaderInfoLog(shaderHandle, length, &length, errorMsg);
        ConPrinter::print(stderr, "OpenGL failed to % shader.\n  Error Message: %\n", type, errorMsg);
        delete[] errorMsg;
    }
    else
    {
        VLA(GLchar, errorMsg, length);
        glGetShaderInfoLog(shaderHandle, length, &length, errorMsg);
        ConPrinter::print(stderr, "OpenGL failed to % shader.\n  Error Message: %\n", type, errorMsg);
    }

    glDeleteShader(shaderHandle);

    return false;
}
