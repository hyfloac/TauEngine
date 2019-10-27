#pragma once

#pragma warning(push, 0)
#include <unordered_map>
#pragma warning(pop)

#include <gl/GLProgram.hpp>
#include <shader/IShader.hpp>
#include <Safeties.hpp>

/**
 * Represents an OpenGL shader.
 */
class TAU_DLL GLShader final : public IShader
{
    DELETE_COPY(GLShader);
private:
    // ShaderType _shaderType;
    const char* _shaderPath;
    GLProgram* _glProgram;
    GLuint _shaderId;
    std::unordered_map<String, GLint> _uniforms;
public:
    GLShader(ShaderType shaderType, NotNull<const char> shaderPath, NotNull<GLProgram> glProgram) noexcept;
    ~GLShader() noexcept override final;

    // inline ShaderType shaderType() const noexcept override final { return _shaderType; };

    bool loadShader(const char* src = nullptr) noexcept override final;

    void activateShader() const noexcept override final;

    i32 createUniform(String name) noexcept override final;

    void setUniform(i32 location, const i8 value)          const noexcept override final;
    void setUniform(i32 location, const i16 value)         const noexcept override final;
    void setUniform(i32 location, const i32 value)         const noexcept override final;
    void setUniform(i32 location, const i64 value)         const noexcept override final;
    void setUniform(i32 location, const u8 value)          const noexcept override final;
    void setUniform(i32 location, const u16 value)         const noexcept override final;
    void setUniform(i32 location, const u32 value)         const noexcept override final;
    void setUniform(i32 location, const u64 value)         const noexcept override final;
    void setUniform(i32 location, const f32 value)         const noexcept override final;
    void setUniform(i32 location, const f64 value)         const noexcept override final;
    void setUniform(i32 location, const Vector3f& value)   const noexcept override final;
    void setUniform(i32 location, const Vector3i& value)   const noexcept override final;
    void setUniform(i32 location, const Vector4f& value)   const noexcept override final;
    void setUniform(i32 location, const glm::mat4& value)  const noexcept override final;
    void setUniform(i32 location, const bool value)        const noexcept override final;

    void setUniform(String& name, const i8 value)          const noexcept override final;
    void setUniform(String& name, const i16 value)         const noexcept override final;
    void setUniform(String& name, const i32 value)         const noexcept override final;
    void setUniform(String& name, const i64 value)         const noexcept override final;
    void setUniform(String& name, const u8 value)          const noexcept override final;
    void setUniform(String& name, const u16 value)         const noexcept override final;
    void setUniform(String& name, const u32 value)         const noexcept override final;
    void setUniform(String& name, const u64 value)         const noexcept override final;
    void setUniform(String& name, const f32 value)         const noexcept override final;
    void setUniform(String& name, const f64 value)         const noexcept override final;
    void setUniform(String& name, const Vector3f& value)   const noexcept override final;
    void setUniform(String& name, const Vector3i& value)   const noexcept override final;
    void setUniform(String& name, const Vector4f& value)   const noexcept override final;
    void setUniform(String& name, const glm::mat4& value)  const noexcept override final;
    void setUniform(String& name, const bool value)        const noexcept override final;
};
