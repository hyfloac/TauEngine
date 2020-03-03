#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "shader/IShader.hpp"
#include <Safeties.hpp>

class GLRenderingContext;

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
    GLShader(GLuint shaderID, EShader::Stage shaderType) noexcept;
	
    ~GLShader() noexcept override final;

    [[nodiscard]] EShader::Stage shaderStage() const noexcept override { return _shaderType; }

    [[nodiscard]] GLuint shaderId() const noexcept { return _shaderID; }

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
private:
    friend class GLShaderBuilder;
};

class TAU_DLL GLShaderBuilder final : public IShaderBuilder
{
    DEFAULT_DESTRUCT(GLShaderBuilder);
    DELETE_COPY(GLShaderBuilder);
private:
	struct GLShaderArgs final
	{
        GLuint shaderID;
	};
private:
    IResourceSelectorTransformer::ResIndex _resIndex;
public:
    GLShaderBuilder(GLRenderingContext& ctx) noexcept;

    [[nodiscard]] GLShader* build(const ShaderArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLShader* build(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IShader> buildCPPRef(const ShaderArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableReferenceCountingPointer<IShader> buildTauRef(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongReferenceCountingPointer<IShader> buildTauSRef(const ShaderArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] static bool processArgs(const ShaderArgs& args, [[tau::out]] GLShaderArgs* glArgs, [[tau::out]] Error* error) noexcept;

    [[nodiscard]] bool compileShader(const ShaderArgs& args, GLShaderArgs& glArgs, [[tau::out]] Error* error) const noexcept;
};
