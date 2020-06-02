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
    SHADER_IMPL(GLShader);
private:
    GLuint _shaderID;
    EShader::Stage _shaderType;
public:
    GLShader(const GLuint shaderID, const EShader::Stage shaderType) noexcept
        : _shaderID(shaderID)
        , _shaderType(shaderType)
    { }
	
    ~GLShader() noexcept override;

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return _shaderType; }

    [[nodiscard]] GLuint shaderId() const noexcept { return _shaderID; }
private:
    friend class GLShaderBuilder;
};

class ShaderInfoExtractorVisitor;

class TAU_DLL GLShaderBuilder final : public IShaderBuilder
{
    DEFAULT_DESTRUCT(GLShaderBuilder);
    DELETE_COPY(GLShaderBuilder);
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
