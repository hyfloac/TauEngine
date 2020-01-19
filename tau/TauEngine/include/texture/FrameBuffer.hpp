#pragma once

#include "DLL.hpp"
#include "texture/Texture.hpp"
#include <Objects.hpp>
#include <NumTypes.hpp>
#include <Safeties.hpp>

class IRenderingContext;

class TAU_DLL IRenderTexture : public ITexture
{
    DEFAULT_DESTRUCT_VI(IRenderTexture);
    DELETE_COPY(IRenderTexture);
protected:
    IRenderTexture(const u32 width, const u32 height, const ETexture::Format dataFormat)
        : ITexture(width, height, dataFormat)
    { }
public:
    [[nodiscard]] inline ETexture::Type textureType() const noexcept override { return ETexture::Type::T2D; }
    void setFilterMode(ETexture::Filter minificationFilter, ETexture::Filter magnificationFilter) noexcept override final { }
    void setWrapMode(ETexture::WrapMode s, ETexture::WrapMode t) noexcept override final { }
    void setDepthComparison(bool enableDepthTest, ETexture::DepthCompareFunc compareFunc) noexcept override final { }
    void set(u32 level, const void* data) noexcept override final { }
    void generateMipmaps() noexcept override final { }
};

class TAU_DLL IFrameBufferAttachment
{
    DELETE_COPY(IFrameBufferAttachment);
public:
    enum Type
    {
        Color,
        Depth,
        Stencil,
        DepthStencil
    };

    static IFrameBufferAttachment* create(IRenderingContext& context, Type type, u32 width, u32 height) noexcept;
protected:
    u32 _width;
    u32 _height;
    Ref<ITexture> _texture;
    IFrameBufferAttachment* _next;
protected:
    inline IFrameBufferAttachment(const u32 width, const u32 height, const Ref<ITexture>& texture) noexcept
        : _width(width), _height(height), _texture(texture), _next(nullptr)
    { }
public:
    virtual ~IFrameBufferAttachment() noexcept;

    [[nodiscard]] const Ref<ITexture>& texture() const noexcept { return _texture; }
    [[nodiscard]] Ref<ITexture>& texture() noexcept { return _texture; }

    [[nodiscard]] const IFrameBufferAttachment* next() const noexcept { return _next; }
    [[nodiscard]] IFrameBufferAttachment*& next() noexcept { return _next; }

    [[nodiscard]] virtual Type type() const noexcept = 0;

    virtual void attach() noexcept = 0;
};

class TAU_DLL IFrameBuffer
{
    DELETE_COPY(IFrameBuffer);
public:
    enum AccessMode
    {
        Read,
        Write,
        ReadWrite
    };
protected:
    IFrameBufferAttachment* _attachment;
protected:
    inline IFrameBuffer(IFrameBufferAttachment* attachment) noexcept
        : _attachment(attachment)
    { }
public:
    virtual ~IFrameBuffer() noexcept;

    [[nodiscard]] inline const IFrameBufferAttachment* color() const noexcept { return _attachment; }

    virtual void bind(IRenderingContext& context, AccessMode mode = AccessMode::ReadWrite) noexcept = 0;

    virtual void unbind(IRenderingContext& context) noexcept = 0;
};

class TAU_DLL IFrameBufferBuilder
{
    DEFAULT_DESTRUCT_VI(IFrameBufferBuilder);
    DELETE_COPY(IFrameBufferBuilder);
public:
    enum Error : u8
    {
        NoError = 0,
        NoAttachment,
        NoColorAttachment,
        DepthAlreadyExists,
        StencilAlreadyExists,
        DS_DepthAlreadyExists,
        DS_StencilAlreadyExists,
        DSAlreadyExists,
        MemoryAllocationFailure
    };
public:
    // static IFrameBufferBuilder* create(IRenderingContext& context) noexcept;
protected:
    IFrameBufferAttachment* _attachment;
    u32 _colorCount;
    bool _hasDepth;
    bool _hasStencil;
protected:
    inline IFrameBufferBuilder() noexcept
        : _attachment(nullptr), _colorCount(0),
          _hasDepth(false), _hasStencil(false)
    { }
public:
    [[nodiscard]] virtual IFrameBuffer* build([[tau::out]] Error* error = nullptr) const noexcept = 0;

    void attach([[tau::in]] IFrameBufferAttachment* attachment, [[tau::out]] Error* error = nullptr) noexcept;
};