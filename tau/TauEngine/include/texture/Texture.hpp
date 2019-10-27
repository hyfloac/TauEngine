#pragma once

#include <DLL.hpp>
#include <NumTypes.hpp>
#include <Objects.hpp>

class RenderingMode;

enum class TextureType : u8
{
    TEXTURE_2D = 1,
    TEXTURE_3D,
    TEXTURE_CUBE
};

class TAU_DLL ITexture
{
    DEFAULT_DESTRUCT_VI(ITexture);
    DELETE_COPY(ITexture);
public:
    static ITexture* create(const RenderingMode& mode, TextureType textureType = TextureType::TEXTURE_2D) noexcept;
protected:
    TextureType _textureType;
protected:
    ITexture(const TextureType textureType) noexcept
        : _textureType(textureType)
    { }
public:
    [[nodiscard]] inline TextureType textureType() const noexcept { return _textureType; }
    inline operator TextureType() const noexcept { return _textureType; }

    virtual void bind(u8 textureUnit) noexcept = 0;

    virtual void unbind(u8 textureUnit) noexcept = 0;
};
