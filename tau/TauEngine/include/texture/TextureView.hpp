#pragma once

#include <Safeties.hpp>

#include "graphics/ResourceView.hpp"
#include "texture/TextureEnums.hpp"

class IRenderingContext;
class IResource;

#define TEXTURE_VIEW_IMPL_BASE(_TYPE) \
    RTTD_IMPL(_TYPE, ITextureView)

#define TEXTURE_VIEW_IMPL(_TYPE) TEXTURE_VIEW_IMPL_BASE(_TYPE)

/**
 * A view into a texture.
 */
class TAU_DLL TAU_NOVTABLE ITextureView : public IResourceView
{
    DEFAULT_CONSTRUCT_PO(ITextureView);
    DEFAULT_DESTRUCT_VI(ITextureView);
    DEFAULT_CM_PO(ITextureView);
    RESOURCE_VIEW_IMPL(ITextureView);
public:
    [[nodiscard]] virtual ETexture::Format dataFormat() const noexcept = 0;
    [[nodiscard]] virtual ETexture::Type  textureType() const noexcept = 0;

    [[nodiscard]] virtual u32 mipCount() const noexcept = 0;

    virtual void generateMipmaps(IRenderingContext& context) noexcept = 0;

    RTTD_BASE_IMPL(ITextureView);
    RTTD_BASE_CHECK(ITextureView);
    RTTD_BASE_CAST(ITextureView);
};

/**
 * A view into a 1 dimensional texture.
 */
class TAU_DLL TAU_NOVTABLE ITexture1DView : public ITextureView
{
    DEFAULT_DESTRUCT_VI(ITexture1DView);
    DEFAULT_CM_PO(ITexture1DView);
    TEXTURE_VIEW_IMPL(ITexture1DView);
protected:
    u32 _width;
    ETexture::Format _dataFormat;
protected:
    inline ITexture1DView(const u32 width, const ETexture::Format dataFormat) noexcept
        : _width(width)
        , _dataFormat(dataFormat)
    { }
public:
    [[nodiscard]] inline u32 width() const noexcept { return _width; }

    [[nodiscard]] ETexture::Format dataFormat() const noexcept override       { return _dataFormat;               }
    [[nodiscard]] ETexture::Type  textureType() const noexcept override final { return ETexture::Type::Texture1D; }
};

/**
 * A view into an array of 1 dimensional textures.
 */
class TAU_DLL TAU_NOVTABLE ITexture1DArrayView : public ITextureView
{
    DEFAULT_DESTRUCT_VI(ITexture1DArrayView);
    DEFAULT_CM_PO(ITexture1DArrayView);
    TEXTURE_VIEW_IMPL(ITexture1DArrayView);
protected:
    u32 _width;
    u32 _count;
    ETexture::Format _dataFormat;
protected:
    inline ITexture1DArrayView(const u32 width, const u32 count, const ETexture::Format dataFormat) noexcept
        : _width(width)
        , _count(count)
        , _dataFormat(dataFormat)
    { }
public:
    [[nodiscard]] inline u32 width() const noexcept { return _width; }
    [[nodiscard]] inline u32 count() const noexcept { return _count; }

    [[nodiscard]] ETexture::Format dataFormat() const noexcept override       { return _dataFormat;                    }
    [[nodiscard]] ETexture::Type  textureType() const noexcept override final { return ETexture::Type::Texture1DArray; }
};

/**
 * A view into a 2 dimensional texture.
 */
class TAU_DLL TAU_NOVTABLE ITexture2DView : public ITextureView
{
    DEFAULT_DESTRUCT_VI(ITexture2DView);
    DEFAULT_CM_PO(ITexture2DView);
    TEXTURE_VIEW_IMPL(ITexture2DView);
protected:
    u32 _width;
    u32 _height;
    ETexture::Format _dataFormat;
protected:
    inline ITexture2DView(const u32 width, const u32 height, const ETexture::Format dataFormat) noexcept
        : _width(width)
        , _height(height)
        , _dataFormat(dataFormat)
    { }
public:
    [[nodiscard]] inline u32  width() const noexcept { return _width;  }
    [[nodiscard]] inline u32 height() const noexcept { return _height; }

    [[nodiscard]] ETexture::Format dataFormat() const noexcept override       { return _dataFormat;               }
    [[nodiscard]] ETexture::Type  textureType() const noexcept override final { return ETexture::Type::Texture2D; }
};

/**
 * A view into an array of 2 dimensional textures.
 */
class TAU_DLL TAU_NOVTABLE ITexture2DArrayView : public ITextureView
{
    DEFAULT_DESTRUCT_VI(ITexture2DArrayView);
    DEFAULT_CM_PO(ITexture2DArrayView);
    TEXTURE_VIEW_IMPL(ITexture2DArrayView);
protected:
    u32 _width;
    u32 _height;
    u32 _count;
    ETexture::Format _dataFormat;
protected:
    inline ITexture2DArrayView(const u32 width, const u32 height, const u32 count, const ETexture::Format dataFormat) noexcept
        : _width(width)
        , _height(height)
        , _count(count)
        , _dataFormat(dataFormat)
    { }
public:
    [[nodiscard]] inline u32  width() const noexcept { return _width;  }
    [[nodiscard]] inline u32 height() const noexcept { return _height; }
    [[nodiscard]] inline u32  count() const noexcept { return _count;  }

    [[nodiscard]] ETexture::Format dataFormat() const noexcept override       { return _dataFormat;                    }
    [[nodiscard]] ETexture::Type  textureType() const noexcept override final { return ETexture::Type::Texture2DArray; }
};

/**
 * A view into a 3 dimensional texture.
 */
class TAU_DLL TAU_NOVTABLE ITexture3DView : public ITextureView
{
    DEFAULT_DESTRUCT_VI(ITexture3DView);
    DEFAULT_CM_PO(ITexture3DView);
    TEXTURE_VIEW_IMPL(ITexture3DView);
protected:
    u32 _width;
    u32 _height;
    u32 _depth;
    ETexture::Format _dataFormat;
protected:
    inline ITexture3DView(const u32 width, const u32 height, const u32 depth, const ETexture::Format dataFormat) noexcept
        : _width(width)
        , _height(height)
        , _depth(depth)
        , _dataFormat(dataFormat)
    { }
public:
    [[nodiscard]] inline u32  width() const noexcept { return _width;  }
    [[nodiscard]] inline u32 height() const noexcept { return _height; }
    [[nodiscard]] inline u32  depth() const noexcept { return _depth;  }

    [[nodiscard]] ETexture::Format dataFormat() const noexcept override       { return _dataFormat;               }
    [[nodiscard]] ETexture::Type  textureType() const noexcept override final { return ETexture::Type::Texture3D; }
};

/**
 * A view into a cube map texture.
 */
class TAU_DLL TAU_NOVTABLE ITextureCubeView : public ITextureView
{
    DEFAULT_DESTRUCT_VI(ITextureCubeView);
    DEFAULT_CM_PO(ITextureCubeView);
    TEXTURE_VIEW_IMPL(ITextureCubeView);
protected:
    u32 _width;
    u32 _height;
    ETexture::Format _dataFormat;
protected:
    inline ITextureCubeView(const u32 width, const u32 height, const ETexture::Format dataFormat) noexcept
        : _width(width)
        , _height(height)
        , _dataFormat(dataFormat)
    { }
public:
    [[nodiscard]] inline u32  width() const noexcept { return _width; }
    [[nodiscard]] inline u32 height() const noexcept { return _height; }

    [[nodiscard]] ETexture::Format dataFormat() const noexcept override       { return _dataFormat;                 }
    [[nodiscard]] ETexture::Type  textureType() const noexcept override final { return ETexture::Type::TextureCube; }
};

/**
 * A view into an array of cube map textures.
 */
class TAU_DLL TAU_NOVTABLE ITextureCubeArrayView : public ITextureView
{
    DEFAULT_DESTRUCT_VI(ITextureCubeArrayView);
    DEFAULT_CM_PO(ITextureCubeArrayView);
    TEXTURE_VIEW_IMPL(ITextureCubeArrayView);
protected:
    u32 _width;
    u32 _height;
    u32 _count;
    ETexture::Format _dataFormat;
protected:
    inline ITextureCubeArrayView(const u32 width, const u32 height, const u32 count, const ETexture::Format dataFormat) noexcept
        : _width(width)
        , _height(height)
        , _count(count)
        , _dataFormat(dataFormat)
    { }
public:
    [[nodiscard]] inline u32  width() const noexcept { return _width; }
    [[nodiscard]] inline u32 height() const noexcept { return _height; }

    [[nodiscard]] ETexture::Format dataFormat() const noexcept override       { return _dataFormat;                      }
    [[nodiscard]] ETexture::Type  textureType() const noexcept override final { return ETexture::Type::TextureCubeArray; }
};

struct Texture1DViewArgs final
{
    DEFAULT_CONSTRUCT_PU(Texture1DViewArgs);
    DEFAULT_DESTRUCT(Texture1DViewArgs);
    DEFAULT_CM_PU(Texture1DViewArgs);
public:
    IResource* texture;
    u32 width;
    ETexture::Format dataFormat;
};

struct Texture1DArrayViewArgs final
{
    DEFAULT_CONSTRUCT_PU(Texture1DArrayViewArgs);
    DEFAULT_DESTRUCT(Texture1DArrayViewArgs);
    DEFAULT_CM_PU(Texture1DArrayViewArgs);
public:
    IResource* texture;
    u32 width;
    u32 count;
    ETexture::Format dataFormat;
};

struct Texture2DViewArgs final
{
    DEFAULT_CONSTRUCT_PU(Texture2DViewArgs);
    DEFAULT_DESTRUCT(Texture2DViewArgs);
    DEFAULT_CM_PU(Texture2DViewArgs);
public:
    IResource* texture;
    u32 width;
    u32 height;
    ETexture::Format dataFormat;
};

struct Texture2DArrayViewArgs final
{
    DEFAULT_CONSTRUCT_PU(Texture2DArrayViewArgs);
    DEFAULT_DESTRUCT(Texture2DArrayViewArgs);
    DEFAULT_CM_PU(Texture2DArrayViewArgs);
public:
    IResource* texture;
    u32 width;
    u32 height;
    u32 count;
    ETexture::Format dataFormat;
};

struct Texture3DViewArgs final
{
    DEFAULT_CONSTRUCT_PU(Texture3DViewArgs);
    DEFAULT_DESTRUCT(Texture3DViewArgs);
    DEFAULT_CM_PU(Texture3DViewArgs);
public:
    IResource* texture;
    u32 width;
    u32 height;
    u32 depth;
    ETexture::Format dataFormat;
};

struct TextureCubeViewArgs final
{
    DEFAULT_CONSTRUCT_PU(TextureCubeViewArgs);
    DEFAULT_DESTRUCT(TextureCubeViewArgs);
    DEFAULT_CM_PU(TextureCubeViewArgs);
public:
    IResource* texture;
    u32 width;
    u32 height;
    ETexture::Format dataFormat;
};

struct TextureCubeArrayViewArgs final
{
    DEFAULT_CONSTRUCT_PU(TextureCubeArrayViewArgs);
    DEFAULT_DESTRUCT(TextureCubeArrayViewArgs);
    DEFAULT_CM_PU(TextureCubeArrayViewArgs);
public:
    IResource* texture;
    u32 width;
    u32 height;
    u32 count;
    ETexture::Format dataFormat;
};

class TAU_DLL TAU_NOVTABLE ITextureViewBuilder
{
    DEFAULT_CONSTRUCT_PO(ITextureViewBuilder);
    DEFAULT_DESTRUCT_VI(ITextureViewBuilder);
    DEFAULT_CM_PO(ITextureViewBuilder);
public:
    enum class Error
    {
        NoError = 0,
        InvalidTexture,
        InvalidWidth,
        InvalidHeight,
        InvalidDepth,
        InvalidCount,
        InvalidDataFormat,
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
        DriverMemoryAllocationFailure
    };
public:
    [[nodiscard]] virtual ITexture1DView* build(const Texture1DViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITexture1DView* build(const Texture1DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITexture1DView> buildCPPRef(const Texture1DViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITexture1DView> buildTauRef(const Texture1DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITexture1DView> buildTauSRef(const Texture1DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITexture1DArrayView* build(const Texture1DArrayViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITexture1DArrayView* build(const Texture1DArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITexture1DArrayView> buildCPPRef(const Texture1DArrayViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITexture1DArrayView> buildTauRef(const Texture1DArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITexture1DArrayView> buildTauSRef(const Texture1DArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITexture2DView* build(const Texture2DViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITexture2DView* build(const Texture2DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITexture2DView> buildCPPRef(const Texture2DViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITexture2DView> buildTauRef(const Texture2DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITexture2DView> buildTauSRef(const Texture2DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITexture2DArrayView* build(const Texture2DArrayViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITexture2DArrayView* build(const Texture2DArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITexture2DArrayView> buildCPPRef(const Texture2DArrayViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITexture2DArrayView> buildTauRef(const Texture2DArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITexture2DArrayView> buildTauSRef(const Texture2DArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITexture3DView* build(const Texture3DViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITexture3DView* build(const Texture3DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITexture3DView> buildCPPRef(const Texture3DViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITexture3DView> buildTauRef(const Texture3DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITexture3DView> buildTauSRef(const Texture3DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITextureCubeView* build(const TextureCubeViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITextureCubeView* build(const TextureCubeViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITextureCubeView> buildCPPRef(const TextureCubeViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITextureCubeView> buildTauRef(const TextureCubeViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITextureCubeView> buildTauSRef(const TextureCubeViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITextureCubeArrayView* build(const TextureCubeArrayViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITextureCubeArrayView* build(const TextureCubeArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITextureCubeArrayView> buildCPPRef(const TextureCubeArrayViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITextureCubeArrayView> buildTauRef(const TextureCubeArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITextureCubeArrayView> buildTauSRef(const TextureCubeArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
