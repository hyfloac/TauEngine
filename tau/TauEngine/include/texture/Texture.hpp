#pragma once

#include <DLL.hpp>
#include <NumTypes.hpp>
#include <Objects.hpp>
#include "TextureEnums.hpp"

class RenderingMode;

class TAU_DLL ITexture
{
    DEFAULT_DESTRUCT_VI(ITexture);
    DELETE_COPY(ITexture);
public:
    static ITexture* create(const RenderingMode& mode, u32 width, u32 height, ETexture::Format format, ETexture::Type textureType = ETexture::Type::TEXTURE_2D) noexcept;
protected:
    u32 _width;
    u32 _height;
    ETexture::Format _dataFormat;
protected:
    ITexture(u32 width, u32 height, ETexture::Format dataFormat) noexcept;
public:
    [[nodiscard]] u32 width() const noexcept { return _width; }
    [[nodiscard]] u32 height() const noexcept { return _height; }

    [[nodiscard]] ETexture::Format dataFormat() const noexcept { return _dataFormat; }

    [[nodiscard]] virtual inline ETexture::Type textureType() const noexcept = 0;

    virtual void setFilterMode(ETexture::Filter minificationFilter, ETexture::Filter magnificationFilter) noexcept = 0;

    virtual void set(const void* data) noexcept = 0;

    virtual void bind(u8 textureUnit) noexcept = 0;

    virtual void unbind(u8 textureUnit) noexcept = 0;
};
