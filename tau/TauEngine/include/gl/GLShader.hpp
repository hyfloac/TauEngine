#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#include <unordered_map>
#pragma warning(pop)

#include "shader/Shader.hpp"
#include <Safeties.hpp>
#include <allocator/FixedBlockAllocator.hpp>

class GLRenderingContext;
class GLGraphicsInterface;

class TAU_DLL GLShaderData final
{
    DELETE_CM(GLShaderData);
private:
    GLuint _handle;
    EShader::Stage _stage;
    WDynString _path;
    mutable uSys _refCount;
private:
    GLShaderData(const GLuint handle, const EShader::Stage stage, const WDynString& path) noexcept
        : _handle(handle)
        , _stage(stage)
        , _path(path)
        , _refCount(1)
    { }

    GLShaderData(const GLuint handle, const EShader::Stage stage, WDynString&& path) noexcept
        : _handle(handle)
        , _stage(stage)
        , _path(::std::move(path))
        , _refCount(1)
    { }
public:
    ~GLShaderData() noexcept
    {
        if(_handle)
        { glDeleteShader(_handle); }
    }

    [[nodiscard]] GLuint handle() const noexcept { return _handle; }
    [[nodiscard]] EShader::Stage stage() const noexcept { return _stage; }
    [[nodiscard]] const WDynString& path() const noexcept { return _path; }
private:
    friend class GLShaderManager;
};

class TAU_DLL GLShaderManager final
{
    DEFAULT_DESTRUCT(GLShaderManager);
    DELETE_CM(GLShaderManager);
private:
#if defined(TAU_PRODUCTION)
    using FBAllocator = FixedBlockAllocator<AllocationTracking::None>;
#else
    using FBAllocator = FixedBlockAllocator<AllocationTracking::DoubleDeleteCount>;
#endif
private:
    FBAllocator _allocator;
    ::std::unordered_map<WDynString, GLShaderData*> _shaderMap;
public:
    GLShaderManager(const uSys maxShaders) noexcept
        : _allocator(sizeof(GLShaderData), maxShaders)
    { }

    [[nodiscard]] bool contains(const WDynString& path) noexcept
    { return _shaderMap.count(path) > 0; }

    [[nodiscard]] GLShaderData* acquire(const WDynString& path) noexcept;

    void release(GLShaderData* shader) noexcept;

    [[nodiscard]] GLShaderData* create(GLuint handle, EShader::Stage stage, const WDynString& path) noexcept;
    [[nodiscard]] GLShaderData* create(GLuint handle, EShader::Stage stage, WDynString&& path) noexcept;
};

/**
 * Represents an OpenGL shader.
 */
class TAU_DLL GLShader final : public IShader
{
    DELETE_COPY(GLShader);
    SHADER_IMPL(GLShader);
private:
    GLuint _shaderHandle;
    EShader::Stage _shaderType;
public:
    GLShader(const GLuint shaderHandle, const EShader::Stage shaderType) noexcept
        : _shaderHandle(shaderHandle)
        , _shaderType(shaderType)
    { }
	
    ~GLShader() noexcept override
    {
        if(_shaderHandle)
        {
            glDeleteProgram(_shaderHandle);
        }
    }

    GLShader(GLShader&& move) noexcept
        : _shaderHandle(move._shaderHandle)
        , _shaderType(move._shaderType)
    { move._shaderHandle = 0; }

    GLShader& operator=(GLShader&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        _shaderHandle = move._shaderHandle;
        _shaderType = move._shaderType;

        move._shaderHandle = 0;

        return *this;
    }

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return _shaderType; }

    [[nodiscard]] GLuint shaderHandle() const noexcept { return _shaderHandle; }
private:
    friend class GLShaderBuilder;
};

class ShaderInfoExtractorVisitor;

class TAU_DLL GLShaderBuilder final : public IShaderBuilder
{
    DEFAULT_DESTRUCT(GLShaderBuilder);
    DEFAULT_CM_PU(GLShaderBuilder);
private:
	struct GLShaderArgs final
	{
        GLuint shaderHandle;
	};
private:
    ShaderInfoExtractorVisitor* _visitor;
public:
    GLShaderBuilder(ShaderInfoExtractorVisitor* visitor) noexcept
        : _visitor(visitor)
    { }

    [[nodiscard]] GLShader* build(const ShaderArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLShader* build(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IShader> buildCPPRef(const ShaderArgs& args, Error* error) const noexcept override;
    [[nodiscard]] ReferenceCountingPointer<IShader> buildTauRef(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] StrongReferenceCountingPointer<IShader> buildTauSRef(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const ShaderArgs& args, [[tau::out]] GLShaderArgs* glArgs, [[tau::out]] Error* error) const noexcept;

    [[nodiscard]] bool processBundle(const ShaderArgs& args, [[tau::out]] GLShaderArgs* glArgs, GLenum shaderStage, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processShader(const CPPRef<IFile>& file, [[tau::out]] GLShaderArgs* glArgs, GLenum shaderStage, [[tau::out]] Error* error) const noexcept;
private:
    friend class GLShaderProgramBuilder;
};
