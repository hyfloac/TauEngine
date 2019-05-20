#pragma once

#include <gl/GLProgram.hpp>
#include <shader/IShader.hpp>
#include <Safeties.hpp>

/**
 * Represents an OpenGL shader.
 */
class TAU_DLL GLShader final : public IShader
{
private:
    ShaderType _shaderType;
    const char* _shaderPath;
    GLProgram* _glProgram;
    GLuint _shaderId;
    std::unordered_map<String, GLint> _uniforms;
public:
    GLShader(ShaderType shaderType, NotNull<const char> shaderPath, NotNull<GLProgram> glProgram) noexcept;
    ~GLShader() noexcept override final;

    inline ShaderType shaderType() const noexcept override final { return _shaderType; };

    bool loadShader(const char* src = nullptr) noexcept override final;

    void activateShader() const noexcept override final;

    i32 createUniform(String name) noexcept override final;

    void setUniform(i32 location, i8 value)          const noexcept override final;
    void setUniform(i32 location, i16 value)         const noexcept override final;
    void setUniform(i32 location, i32 value)         const noexcept override final;
    void setUniform(i32 location, i64 value)         const noexcept override final;
    void setUniform(i32 location, u8 value)          const noexcept override final;
    void setUniform(i32 location, u16 value)         const noexcept override final;
    void setUniform(i32 location, u32 value)         const noexcept override final;
    void setUniform(i32 location, u64 value)         const noexcept override final;
    void setUniform(i32 location, f32 value)         const noexcept override final;
    void setUniform(i32 location, f64 value)         const noexcept override final;
    void setUniform(i32 location, Vector3f& value)   const noexcept override final;
    void setUniform(i32 location, Vector3i& value)   const noexcept override final;
    void setUniform(i32 location, Vector4f& value)   const noexcept override final;
    void setUniform(i32 location, const Matrix4x4f& value) const noexcept override final;
    void setUniform(i32 location, bool value)        const noexcept override final;

    void setUniform(String& name, i8 value)          const noexcept override final;
    void setUniform(String& name, i16 value)         const noexcept override final;
    void setUniform(String& name, i32 value)         const noexcept override final;
    void setUniform(String& name, i64 value)         const noexcept override final;
    void setUniform(String& name, u8 value)          const noexcept override final;
    void setUniform(String& name, u16 value)         const noexcept override final;
    void setUniform(String& name, u32 value)         const noexcept override final;
    void setUniform(String& name, u64 value)         const noexcept override final;
    void setUniform(String& name, f32 value)         const noexcept override final;
    void setUniform(String& name, f64 value)         const noexcept override final;
    void setUniform(String& name, Vector3f& value)   const noexcept override final;
    void setUniform(String& name, Vector3i& value)   const noexcept override final;
    void setUniform(String& name, Vector4f& value)   const noexcept override final;
    void setUniform(String& name, Matrix4x4f& value) const noexcept override final;
    void setUniform(String& name, bool value)        const noexcept override final;
};
