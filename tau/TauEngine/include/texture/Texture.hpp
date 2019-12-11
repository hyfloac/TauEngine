#pragma once

#include <DLL.hpp>
#include <NumTypes.hpp>
#include <Objects.hpp>
#include "TextureEnums.hpp"

class RenderingMode;
class IRenderingContext;

class TAU_DLL ITexture
{
    DEFAULT_DESTRUCT_VI(ITexture);
    DELETE_COPY(ITexture);
public:
    static ITexture* create(IRenderingContext& context, u32 width, u32 height, ETexture::Format format, ETexture::Type textureType = ETexture::Type::T2D) noexcept;
protected:
    u32 _width;
    u32 _height;
    ETexture::Format _dataFormat;
protected:
    inline ITexture(u32 width, u32 height, ETexture::Format dataFormat) noexcept
        : _width(width), _height(height), _dataFormat(dataFormat)
    { }
public:
    [[nodiscard]] u32 width() const noexcept { return _width; }
    [[nodiscard]] u32 height() const noexcept { return _height; }

    [[nodiscard]] ETexture::Format dataFormat() const noexcept { return _dataFormat; }

    [[nodiscard]] virtual inline ETexture::Type textureType() const noexcept = 0;

    virtual void setFilterMode(ETexture::Filter minificationFilter, ETexture::Filter magnificationFilter) noexcept = 0;

    virtual void setWrapMode(ETexture::WrapMode s, ETexture::WrapMode t) noexcept = 0;

    virtual void setDepthComparison(bool enableDepthTest, ETexture::DepthCompareFunc compareFunc) noexcept { }

    virtual void set(const void* data) noexcept = 0;

    virtual void bind(u8 textureUnit) noexcept = 0;

    virtual void unbind(u8 textureUnit) noexcept = 0;

    virtual void generateMipmaps() noexcept = 0;
};

class TAU_DLL ITextureCube : public ITexture
{
    DEFAULT_DESTRUCT_VI(ITextureCube);
    DELETE_COPY(ITextureCube);
public:
    [[nodiscard]] static ITextureCube* create(IRenderingContext& context, u32 width, u32 height, ETexture::Format format) noexcept;
protected:
    inline ITextureCube(u32 width, u32 height, ETexture::Format dataFormat) noexcept
        : ITexture(width, height, dataFormat)
    { }
public:
    [[nodiscard]] inline ETexture::Type textureType() const noexcept override { return ETexture::Type::Cube; }

    virtual void setWrapModeCube(ETexture::WrapMode s, ETexture::WrapMode t, ETexture::WrapMode r) noexcept = 0;

    virtual void setCube(ETexture::CubeSide side, const void* data) noexcept = 0;
};
