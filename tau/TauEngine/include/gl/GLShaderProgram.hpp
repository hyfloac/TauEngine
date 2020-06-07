#pragma once

#include "shader/ShaderProgram.hpp"
#pragma warning(push, 0)
#include <GL/glew.h>
#include <Objects.hpp>
#pragma warning(pop)

class TAU_DLL GLShaderProgram final : public IShaderProgram
{
    DELETE_CM(GLShaderProgram);
private:
    GLuint _pipelineHandle;
public:
    GLShaderProgram() noexcept;

    ~GLShaderProgram() noexcept override;

    [[nodiscard]] GLuint programID() const noexcept { return _pipelineHandle; }

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;
};

class ShaderInfoExtractorVisitor;
namespace sbp { struct ShaderInfo; }

class TAU_DLL GLShaderProgramBuilder final : public IShaderProgramBuilder
{
    DEFAULT_DESTRUCT(GLShaderProgramBuilder);
    DELETE_CM(GLShaderProgramBuilder);
private:
    struct GLShaderProgramArgs final
    {
        GLuint pipeline;
        NullableRef<IShader> vertexShader;
        NullableRef<IShader> tessCtrlShader;
        NullableRef<IShader> tessEvalShader;
        NullableRef<IShader> geometryShader;
        NullableRef<IShader> pixelShader;
    };
private:
    ShaderInfoExtractorVisitor* _visitor;
public:
    GLShaderProgramBuilder(ShaderInfoExtractorVisitor* const visitor) noexcept
        : _visitor(visitor)
    { }

    [[nodiscard]] IShaderProgram* build(const ShaderProgramArgs& args, Error* error) const noexcept override;
    [[nodiscard]] IShaderProgram* build(const ShaderProgramArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IShaderProgram> buildCPPRef(const ShaderProgramArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IShaderProgram> buildTauRef(const ShaderProgramArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IShaderProgram> buildTauSRef(const ShaderProgramArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const ShaderProgramArgs& args, [[tau::out]] GLShaderProgramArgs* glArgs, [[tau::out]] Error* error) const noexcept;

    [[nodiscard]] bool processBundle(const ShaderProgramArgs& args, [[tau::out]] GLShaderProgramArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processShader(const CPPRef<IFile>& args, [[tau::out]] GLShaderProgramArgs* glArgs, [[tau::out]] GLuint* shaderHandle, const sbp::ShaderInfo& shaderInfo, [[tau::out]] Error* error) const noexcept;
};
