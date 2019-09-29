#pragma once

#include <DLL.hpp>
#include <Objects.hpp>
#include <String.hpp>

class Vector3f;
class Vector3i;
class Vector4f;
class Matrix4x4f;

enum class ShaderType
{
    VERTEX = 0,
    PIXEL,
    FRAGMENT = PIXEL
};

/**
 * Represents an abstract, library independent shader.
 */
interface_c TAU_DLL IShader
{
protected:
    ShaderType _shaderType;
public:
    virtual ~IShader() noexcept = default;

    [[nodiscard]] ShaderType shaderType() const noexcept { return _shaderType; }

    virtual bool loadShader(const char* src = nullptr) noexcept = 0;
    virtual void activateShader() const noexcept = 0;

    virtual i32 createUniform(String name) noexcept = 0;

    virtual void setUniform(i32 location, const i8 value)          const noexcept = 0;
    virtual void setUniform(i32 location, const i16 value)         const noexcept = 0;
    virtual void setUniform(i32 location, const i32 value)         const noexcept = 0;
    virtual void setUniform(i32 location, const i64 value)         const noexcept = 0;
    virtual void setUniform(i32 location, const u8 value)          const noexcept = 0;
    virtual void setUniform(i32 location, const u16 value)         const noexcept = 0;
    virtual void setUniform(i32 location, const u32 value)         const noexcept = 0;
    virtual void setUniform(i32 location, const u64 value)         const noexcept = 0;
    virtual void setUniform(i32 location, const f32 value)         const noexcept = 0;
    virtual void setUniform(i32 location, const f64 value)         const noexcept = 0;
    virtual void setUniform(i32 location, const Vector3f& value)   const noexcept = 0;
    virtual void setUniform(i32 location, const Vector3i& value)   const noexcept = 0;
    virtual void setUniform(i32 location, const Vector4f& value)   const noexcept = 0;
    virtual void setUniform(i32 location, const Matrix4x4f& value) const noexcept = 0;
    virtual void setUniform(i32 location, const bool value)        const noexcept = 0;

    virtual void setUniform(String& name, const i8 value)          const noexcept = 0;
    virtual void setUniform(String& name, const i16 value)         const noexcept = 0;
    virtual void setUniform(String& name, const i32 value)         const noexcept = 0;
    virtual void setUniform(String& name, const i64 value)         const noexcept = 0;
    virtual void setUniform(String& name, const u8 value)          const noexcept = 0;
    virtual void setUniform(String& name, const u16 value)         const noexcept = 0;
    virtual void setUniform(String& name, const u32 value)         const noexcept = 0;
    virtual void setUniform(String& name, const u64 value)         const noexcept = 0;
    virtual void setUniform(String& name, const f32 value)         const noexcept = 0;
    virtual void setUniform(String& name, const f64 value)         const noexcept = 0;
    virtual void setUniform(String& name, const Vector3f& value)   const noexcept = 0;
    virtual void setUniform(String& name, const Vector3i& value)   const noexcept = 0;
    virtual void setUniform(String& name, const Vector4f& value)   const noexcept = 0;
    virtual void setUniform(String& name, const Matrix4x4f& value) const noexcept = 0;
    virtual void setUniform(String& name, const bool value)        const noexcept = 0;
protected:
    IShader(ShaderType shaderType) noexcept
        : _shaderType(shaderType)
    { }
private:
    IShader(const IShader& copy) noexcept = delete;
    IShader(IShader&& move) noexcept = delete;

    IShader& operator =(const IShader& copy) noexcept = delete;
    IShader& operator =(IShader&& move) noexcept = delete;
};
