#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "shader/Shader.hpp"
#include <Safeties.hpp>

class GLRenderingContext;
class GLGraphicsInterface;

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
    [[nodiscard]] NullableReferenceCountingPointer<IShader> buildTauRef(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongReferenceCountingPointer<IShader> buildTauSRef(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const ShaderArgs& args, [[tau::out]] GLShaderArgs* glArgs, [[tau::out]] Error* error) const noexcept;

    [[nodiscard]] bool processBundle(const ShaderArgs& args, [[tau::out]] GLShaderArgs* glArgs, GLenum shaderStage, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processShader(const CPPRef<IFile>& file, [[tau::out]] GLShaderArgs* glArgs, GLenum shaderStage, [[tau::out]] Error* error) const noexcept;
};
