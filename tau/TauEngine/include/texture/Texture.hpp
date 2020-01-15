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
    // [[deprecated]] static ITexture* create(IRenderingContext& context, u32 width, u32 height, ETexture::Format format, ETexture::Type textureType = ETexture::Type::T2D) noexcept;
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

    virtual void set(u32 level, const void* data) noexcept = 0;

    virtual void bind(u8 textureUnit) noexcept = 0;

    virtual void unbind(u8 textureUnit) noexcept = 0;

    virtual void generateMipmaps() noexcept = 0;
};

class TAU_DLL ITextureCube : public ITexture
{
    DEFAULT_DESTRUCT_VI(ITextureCube);
    DELETE_COPY(ITextureCube);
public:
    // [[nodiscard]] static ITextureCube* create(IRenderingContext& context, u32 width, u32 height, ETexture::Format format) noexcept;
protected:
    inline ITextureCube(u32 width, u32 height, ETexture::Format dataFormat) noexcept
        : ITexture(width, height, dataFormat)
    { }
public:
    [[nodiscard]] inline ETexture::Type textureType() const noexcept override { return ETexture::Type::Cube; }

    virtual void setWrapModeCube(ETexture::WrapMode s, ETexture::WrapMode t, ETexture::WrapMode r) noexcept = 0;

    virtual void setCube(u32 level, ETexture::CubeSide side, const void* data) noexcept = 0;
};

class TAU_DLL ITextureBuilder
{
    DEFAULT_DESTRUCT_VI(ITextureBuilder);
    DELETE_COPY(ITextureBuilder);
public:
    enum class Error
    {
        NoError = 0,
        WidthIsZero,
        HeightIsZero,
        MipMapLevelsIsUnset,
        DataFormatIsUnset,
        /**
         * Failed to allocate system memory.
         *
         *   This error is produced when a call to the system memory
         * allocator (malloc, new, new[], HeapAlloc, LocalAlloc,
         * GlobalAlloc) fails (returns null). This generally occurs
         * when the system has run out of memory. It may also mean
         * that the process itself has exceed its maximum allotted
         * memory. It may also occur when allocating a buffer that
         * is too large.
         *
         *   If this occurs its probably safe to assume that the
         * program or system will crash shortly. As such I prescribe to
         * the philosophy of simply exiting, or outright ignoring the
         * error, given that it likely won't matter in a couple of seconds
         * at most.
         */
        SystemMemoryAllocationFailure,
        /**
         * The driver failed to allocate system memory.
         *
         *   This occurs when the driver failed to allocate memory on
         * the system. This is likely caused by the same reasons as
         * Error::SystemMemoryAllocationFailure.
         */
        DriverMemoryAllocationFailure,
        /**
         * The driver failed to allocate memory on the GPU.
         *
         *   This occurs when the GPU runs out of memory. It may also mean
         * that there isn't a GPU installed, or at the very least, one that
         * isn't suitable. This may be more resolvable by scaling back
         * texture sizes and model sizes. As well as deallocating unnecessary
         * objects. Depending on the backing API it may also be possible to
         * move certain allocation from the GPU back into system memory.
         */
        GPUMemoryAllocationFailure,
        /**
         * TauEngine screwed up somehow.
         *
         * I fucked up. Tell me about.
         *
         *   This shouldn't ever occur. If it does I must have done something
         * very wrong. Please, tell me about it, including any relevant
         * information - things like the call stack (I mostly care about within
         * TauEngine), the model (or at the very least, the model size), etc.
         */
    };
protected:
    u32 _width;
    u32 _height;
    i32 _mipmapLevels;
    ETexture::Format _dataFormat;
public:
    inline ITextureBuilder() noexcept
        : _width(0), _height(0), _mipmapLevels(0),
          _dataFormat(static_cast<ETexture::Format>(0))
    { }

    void width(const u32 width) noexcept { _width = width; }
    void height(const u32 height) noexcept { _height = height; }
    virtual void mipmapLevels(const i32 mipmapLevels) noexcept { _mipmapLevels = mipmapLevels; }
    virtual void dataFormat(const ETexture::Format dataFormat) noexcept { _dataFormat = dataFormat; }

    [[nodiscard]] virtual ITexture* build([[tau::out]] Error* error) const noexcept = 0;
};

class TAU_DLL ITextureCubeBuilder
{
    DEFAULT_DESTRUCT_VI(ITextureCubeBuilder);
    DELETE_COPY(ITextureCubeBuilder);
public:
    using Error = ITextureBuilder::Error;
protected:
    u32 _width;
    u32 _height;
    i32 _mipmapLevels;
    ETexture::Format _dataFormat;
public:
    inline ITextureCubeBuilder() noexcept
        : _width(0), _height(0), _mipmapLevels(0),
        _dataFormat(static_cast<ETexture::Format>(0))
    { }

    void width(const u32 width) noexcept { _width = width; }
    void height(const u32 height) noexcept { _height = height; }
    virtual void mipmapLevels(const i32 mipmapLevels) noexcept { _mipmapLevels = mipmapLevels; }
    virtual void dataFormat(const ETexture::Format dataFormat) noexcept { _dataFormat = dataFormat; }

    [[nodiscard]] virtual ITextureCube* build([[tau::out]] Error* error) const noexcept = 0;
};
