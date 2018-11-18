#pragma once

#include <gl/GLProgram.hpp>
#include <shader/IShader.hpp>
#include <Safeties.hpp>

class TAU_DLL GLShader : public IShader
{
private:
    ShaderType _shaderType;
    const char* _shaderPath;
    GLProgram* _glProgram;
    GLuint _shaderId;
public:
    GLShader(ShaderType shaderType, NotNull<const char> shaderPath, NotNull<GLProgram> glProgram) noexcept;
    ~GLShader() noexcept override;

    inline ShaderType shaderType() const noexcept override { return _shaderType; };

    bool loadShader() noexcept override;

    void activateShader() const noexcept override;
};
