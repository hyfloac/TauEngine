#pragma once

#include "shader/ShaderProgram.hpp"
#pragma warning(push, 0)
#include <GL/glew.h>
#include <Objects.hpp>
#pragma warning(pop)

class TAU_DLL GLShaderProgram final : public IShaderProgram
{
    DELETE_COPY(GLShaderProgram);
private:
    GLuint _programID;
public:
    GLShaderProgram() noexcept;

    ~GLShaderProgram() noexcept override;

    [[nodiscard]] GLuint programID() const noexcept { return _programID; }

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    bool link(IRenderingContext& context) noexcept override;
private:
    bool attach(IRenderingContext& context, const CPPRef<IShader>& shader) noexcept override final;
    void detach(IRenderingContext& context, const CPPRef<IShader>& shader) noexcept override final;
};
