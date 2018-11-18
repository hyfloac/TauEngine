#pragma once

#include <DLL.hpp>
#include <Objects.hpp>

enum ShaderType
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
interface TAU_DLL IShader
{
public:
    virtual ~IShader() noexcept = default;
    virtual ShaderType shaderType() const noexcept = 0;
    virtual bool loadShader() noexcept = 0;
    virtual void activateShader() const noexcept = 0;
protected:
    IShader() noexcept = default;
    IShader(const IShader& copy) noexcept = default;
    IShader(IShader&& move) noexcept { }

    IShader& operator =(const IShader& copy) noexcept = default;
    IShader& operator =(IShader&& move)      noexcept { return *this; }
};
