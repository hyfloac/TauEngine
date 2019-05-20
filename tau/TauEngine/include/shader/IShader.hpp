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
 * 
 * This is an interface, but with optimizations there shouldn't ever be a scenario where a VTable is generated
 */
interface_c TAU_DLL IShader
{
public:
    virtual ~IShader() noexcept = default;
    virtual ShaderType shaderType() const noexcept = 0;
    virtual bool loadShader(const char* src = nullptr) noexcept = 0;
    virtual void activateShader() const noexcept = 0;

    virtual i32 createUniform(String name) noexcept = 0;

    virtual void setUniform(i32 location, i8 value)          const noexcept = 0;
    virtual void setUniform(i32 location, i16 value)         const noexcept = 0;
    virtual void setUniform(i32 location, i32 value)         const noexcept = 0;
    virtual void setUniform(i32 location, i64 value)         const noexcept = 0;
    virtual void setUniform(i32 location, u8 value)          const noexcept = 0;
    virtual void setUniform(i32 location, u16 value)         const noexcept = 0;
    virtual void setUniform(i32 location, u32 value)         const noexcept = 0;
    virtual void setUniform(i32 location, u64 value)         const noexcept = 0;
    virtual void setUniform(i32 location, f32 value)         const noexcept = 0;
    virtual void setUniform(i32 location, f64 value)         const noexcept = 0;
    virtual void setUniform(i32 location, Vector3f& value)   const noexcept = 0;
    virtual void setUniform(i32 location, Vector3i& value)   const noexcept = 0;
    virtual void setUniform(i32 location, Vector4f& value)   const noexcept = 0;
    virtual void setUniform(i32 location, const  Matrix4x4f& value) const noexcept = 0;
    virtual void setUniform(i32 location, bool value)        const noexcept = 0;

    virtual void setUniform(String& name, i8 value)          const noexcept = 0;
    virtual void setUniform(String& name, i16 value)         const noexcept = 0;
    virtual void setUniform(String& name, i32 value)         const noexcept = 0;
    virtual void setUniform(String& name, i64 value)         const noexcept = 0;
    virtual void setUniform(String& name, u8 value)          const noexcept = 0;
    virtual void setUniform(String& name, u16 value)         const noexcept = 0;
    virtual void setUniform(String& name, u32 value)         const noexcept = 0;
    virtual void setUniform(String& name, u64 value)         const noexcept = 0;
    virtual void setUniform(String& name, f32 value)         const noexcept = 0;
    virtual void setUniform(String& name, f64 value)         const noexcept = 0;
    virtual void setUniform(String& name, Vector3f& value)   const noexcept = 0;
    virtual void setUniform(String& name, Vector3i& value)   const noexcept = 0;
    virtual void setUniform(String& name, Vector4f& value)   const noexcept = 0;
    virtual void setUniform(String& name, Matrix4x4f& value) const noexcept = 0;
    virtual void setUniform(String& name, bool value)        const noexcept = 0;
protected:
    IShader() noexcept = default;
    IShader(const IShader& copy) noexcept = default;
    IShader(IShader&& move) noexcept { }
public:
    IShader& operator =(const IShader& copy) noexcept = default;
    IShader& operator =(IShader&& move)      noexcept = default;
};
