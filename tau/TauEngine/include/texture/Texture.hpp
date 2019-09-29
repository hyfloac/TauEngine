#pragma once

#include <DLL.hpp>
#include <NumTypes.hpp>

class RenderingMode;

enum class TextureType : u8
{
    TEXTURE_2D = 1,
    TEXTURE_3D,
    TEXTURE_CUBE
};

class TAU_DLL ITexture
{
public:
    static ITexture* create(const RenderingMode& mode, const TextureType textureType = TextureType::TEXTURE_2D) noexcept;
protected:
    TextureType _textureType;
protected:
    ITexture(const TextureType textureType) noexcept
        : _textureType(textureType)
    { }
public:
    virtual ~ITexture() noexcept = default;

    ITexture(const ITexture& copy) noexcept = delete;
    ITexture(ITexture&& move) noexcept = delete;
    
    ITexture& operator=(const ITexture& copy) noexcept = delete;
    ITexture& operator=(ITexture&& move) noexcept = delete;

    [[nodiscard]] inline TextureType textureType() const noexcept { return _textureType; }
    inline operator TextureType() const noexcept { return _textureType; }

    virtual void bind(u8 textureUnit) noexcept = 0;

    virtual void unbind(u8 textureUnit) noexcept = 0;
};
