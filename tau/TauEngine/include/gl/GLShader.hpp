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
    SHADER_IMPL(GLShader);
private:
    // ShaderType _shaderType;
    const char* _shaderPath;
    GLuint _shaderID;
    std::unordered_map<String, GLint> _uniforms;
private:
    GLShader(IShader::Type shaderType, const NotNull<const char>& shaderPath, GLuint shaderID) noexcept;
public:
    static Ref<GLShader> create(IShader::Type shaderType, const NotNull<const char>& shaderPath) noexcept;
public:
    ~GLShader() noexcept override final;

    [[nodiscard]] GLuint shaderId() const noexcept { return _shaderID; }

    bool loadShader(const char* src = nullptr) noexcept override final;

    // i32 createUniform(String name) noexcept override final;
    //
    // void setUniform(i32 location, i8   value) const noexcept override final;
    // void setUniform(i32 location, i16  value) const noexcept override final;
    // void setUniform(i32 location, i32  value) const noexcept override final;
    // void setUniform(i32 location, i64  value) const noexcept override final;
    // void setUniform(i32 location, u8   value) const noexcept override final;
    // void setUniform(i32 location, u16  value) const noexcept override final;
    // void setUniform(i32 location, u32  value) const noexcept override final;
    // void setUniform(i32 location, u64  value) const noexcept override final;
    // void setUniform(i32 location, f32  value) const noexcept override final;
    // void setUniform(i32 location, f64  value) const noexcept override final;
    // void setUniform(i32 location, bool value) const noexcept override final;
    // void setUniform(i32 location, const Vector2f&  value) const noexcept override final;
    // void setUniform(i32 location, const Vector3f&  value) const noexcept override final;
    // void setUniform(i32 location, const Vector3i&  value) const noexcept override final;
    // void setUniform(i32 location, const Vector4f&  value) const noexcept override final;
    // void setUniform(i32 location, const glm::mat4& value) const noexcept override final;
    //                                                       
    // void setUniform(String& name, i8   value) const noexcept override final;
    // void setUniform(String& name, i16  value) const noexcept override final;
    // void setUniform(String& name, i32  value) const noexcept override final;
    // void setUniform(String& name, i64  value) const noexcept override final;
    // void setUniform(String& name, u8   value) const noexcept override final;
    // void setUniform(String& name, u16  value) const noexcept override final;
    // void setUniform(String& name, u32  value) const noexcept override final;
    // void setUniform(String& name, u64  value) const noexcept override final;
    // void setUniform(String& name, f32  value) const noexcept override final;
    // void setUniform(String& name, f64  value) const noexcept override final;
    // void setUniform(String& name, bool value) const noexcept override final;
    // void setUniform(String& name, const Vector2f&  value) const noexcept override final;
    // void setUniform(String& name, const Vector3f&  value) const noexcept override final;
    // void setUniform(String& name, const Vector3i&  value) const noexcept override final;
    // void setUniform(String& name, const Vector4f&  value) const noexcept override final;
    // void setUniform(String& name, const glm::mat4& value) const noexcept override final;
};
