#pragma once

#include <Objects.hpp>
#include <NumTypes.hpp>
#include <Safeties.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"
#include "TextureEnums.hpp"
#include "RenderTarget.hpp"
#include "TextureView.hpp"

#define TEXTURE_IMPL_BASE(_TYPE) \
    DELETE_COPY(_TYPE); \
    RTT_IMPL(_TYPE, ITexture)

#define TEXTURE_IMPL(_TYPE) TEXTURE_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE ITexture
{
    DEFAULT_CONSTRUCT_PO(ITexture);
    DEFAULT_DESTRUCT_VI(ITexture);
    DELETE_COPY(ITexture);
public:
    [[nodiscard]] virtual const IRenderTarget* renderTarget() const noexcept { return null; }
    [[nodiscard]] virtual       IRenderTarget* renderTarget()       noexcept { return null; }

    [[nodiscard]] virtual ETexture::Format dataFormat() const noexcept = 0;
    [[nodiscard]] virtual ETexture::Type  textureType() const noexcept = 0;

    [[nodiscard]] virtual u64 _getHandle() const noexcept { return 0; }

    RTT_BASE_IMPL(ITexture);
    RTT_BASE_CHECK(ITexture);
    RTT_BASE_CAST(ITexture);
};

class TAU_DLL TAU_NOVTABLE ITexture2D : public ITexture
{
    DEFAULT_DESTRUCT_VI(ITexture2D);
    DELETE_COPY(ITexture2D);
protected:
    u32 _width;
    u32 _height;
    ETexture::Format _dataFormat;
protected:
    inline ITexture2D(const u32 width, const u32 height, const ETexture::Format dataFormat) noexcept
        : ITexture()
        , _width(width)
        , _height(height)
        , _dataFormat(dataFormat)
    { }
public:
    [[nodiscard]] inline u32  width() const noexcept { return _width;  }
    [[nodiscard]] inline u32 height() const noexcept { return _height; }

    [[nodiscard]] virtual const ITextureView* textureView() const noexcept { return null; }
    [[nodiscard]] virtual       ITextureView* textureView()       noexcept { return null; }

    [[nodiscard]] ETexture::Format dataFormat() const noexcept override       { return _dataFormat;         }
    [[nodiscard]] ETexture::Type  textureType() const noexcept override final { return ETexture::Type::T2D; }

    virtual void set(IRenderingContext& context, u32 mipLevel, const void* data) noexcept = 0;
};

class TAU_DLL TAU_NOVTABLE ITexture3D : public ITexture
{
    DEFAULT_DESTRUCT_VI(ITexture3D);
    DELETE_COPY(ITexture3D);
protected:
    u32 _width;
    u32 _height;
    u32 _depth;
    ETexture::Format _dataFormat;
protected:
    inline ITexture3D(const u32 width, const u32 height, const u32 depth, const ETexture::Format dataFormat) noexcept
        : ITexture()
        , _width(width)
        , _height(height)
        , _depth(depth)
        , _dataFormat(dataFormat)
    { }
public:
    [[nodiscard]] inline u32  width() const noexcept { return _width;  }
    [[nodiscard]] inline u32 height() const noexcept { return _height; }
    [[nodiscard]] inline u32  depth() const noexcept { return _depth;  }

    [[nodiscard]] virtual const ITextureView* textureView() const noexcept { return null; }
    [[nodiscard]] virtual       ITextureView* textureView()       noexcept { return null; }
    
    [[nodiscard]] ETexture::Format dataFormat() const noexcept override       { return _dataFormat;         }
    [[nodiscard]] ETexture::Type  textureType() const noexcept override final { return ETexture::Type::T3D; }

    virtual void set(IRenderingContext& context, u32 depthLevel, u32 mipLevel, const void* data) noexcept = 0;
};

class TAU_DLL TAU_NOVTABLE ITextureCube : public ITexture
{
    DEFAULT_DESTRUCT_VI(ITextureCube);
    DELETE_COPY(ITextureCube);
protected:
    u32 _width;
    u32 _height;
    ETexture::Format _dataFormat;
protected:
    inline ITextureCube(const u32 width, const u32 height, const ETexture::Format dataFormat) noexcept
        : ITexture()
        , _width(width)
        , _height(height)
        , _dataFormat(dataFormat)
    { }
public:
    [[nodiscard]] inline u32  width() const noexcept { return _width;  }
    [[nodiscard]] inline u32 height() const noexcept { return _height; }

    [[nodiscard]] virtual const ITextureView* textureView() const noexcept { return null; }
    [[nodiscard]] virtual       ITextureView* textureView()       noexcept { return null; }
    
    [[nodiscard]] ETexture::Format dataFormat() const noexcept override       { return _dataFormat;          }
    [[nodiscard]] ETexture::Type  textureType() const noexcept override final { return ETexture::Type::Cube; }

    virtual void set(IRenderingContext& context, u32 mipLevel, ETexture::CubeSide side, const void* data) noexcept = 0;
};

class TAU_DLL TAU_NOVTABLE ITextureDepthStencil : public ITexture
{
    DEFAULT_DESTRUCT_VI(ITextureDepthStencil);
    DELETE_COPY(ITextureDepthStencil);
protected:
    u32 _width;
    u32 _height;
protected:
    inline ITextureDepthStencil(const u32 width, const u32 height) noexcept
        : ITexture()
        , _width(width)
        , _height(height)
    { }
public:
    [[nodiscard]] inline u32  width() const noexcept { return _width;  }
    [[nodiscard]] inline u32 height() const noexcept { return _height; }

    [[nodiscard]] virtual const ITextureView*   depthView() const noexcept { return null; }
    [[nodiscard]] virtual       ITextureView*   depthView()       noexcept { return null; }

    [[nodiscard]] virtual const ITextureView* stencilView() const noexcept { return null; }
    [[nodiscard]] virtual       ITextureView* stencilView()       noexcept { return null; }

    [[nodiscard]] ETexture::Format dataFormat() const noexcept override final { return ETexture::Format::Depth24Stencil8; }
    [[nodiscard]] ETexture::Type  textureType() const noexcept override final { return ETexture::Type::DepthStencil;      }

    virtual void set(IRenderingContext& context, const void* data) noexcept = 0;
};

struct Texture2DArgs final
{
    DEFAULT_DESTRUCT(Texture2DArgs);
    DEFAULT_COPY(Texture2DArgs);
public:
    u32 width;
    u32 height;
    i32 mipmapLevels;
    ETexture::Format dataFormat;
    ETexture::BindFlags flags;
    const void* initialBuffer;
public:
    inline Texture2DArgs() noexcept
        : width(0)
        , height(0)
        , mipmapLevels(0)
        , dataFormat(static_cast<ETexture::Format>(0))
        , flags(ETexture::BindFlags::None)
        , initialBuffer(null)
    { }
};

struct Texture3DArgs final
{
    DEFAULT_DESTRUCT(Texture3DArgs);
    DEFAULT_COPY(Texture3DArgs);
public:
    u32 width;
    u32 height;
    u32 depth;
    i32 mipmapLevels;
    ETexture::Format dataFormat;
    ETexture::BindFlags flags;
public:
    inline Texture3DArgs() noexcept
        : width(0)
        , height(0)
        , depth(0)
        , mipmapLevels(0)
        , dataFormat(static_cast<ETexture::Format>(0))
        , flags(ETexture::BindFlags::None)
    { }
};

struct TextureCubeArgs final
{
    DEFAULT_DESTRUCT(TextureCubeArgs);
    DEFAULT_COPY(TextureCubeArgs);
public:
    u32 width;
    u32 height;
    i32 mipmapLevels;
    ETexture::Format dataFormat;
    ETexture::BindFlags flags;
    const void* initialBuffers[6];
public:
    inline TextureCubeArgs() noexcept
        : width(0)
        , height(0)
        , mipmapLevels(0)
        , dataFormat(static_cast<ETexture::Format>(0))
        , flags(ETexture::BindFlags::None)
        , initialBuffers{ null, null, null, null, null, null }
    { }
};

struct TextureDepthStencilArgs final
{
    DEFAULT_DESTRUCT(TextureDepthStencilArgs);
    DEFAULT_COPY(TextureDepthStencilArgs);
public:
    u32 width;
    u32 height;
    ETexture::DepthStencilBindFlags flags;
    const void* initialBuffer;
public:
    inline TextureDepthStencilArgs() noexcept
        : width(0)
        , height(0)
        , flags(ETexture::DepthStencilBindFlags::None)
        , initialBuffer(null)
    { }
};

class TAU_DLL TAU_NOVTABLE ITextureBuilder
{
    DEFAULT_CONSTRUCT_PO(ITextureBuilder);
    DEFAULT_DESTRUCT_VI(ITextureBuilder);
    DELETE_COPY(ITextureBuilder);
public:
    enum class Error
    {
        NoError = 0,
        WidthIsZero,
        HeightIsZero,
        /**
         * Used only for 3D textures.
         */
        DepthIsZero,
        MipMapLevelsIsUnset,
        DataFormatIsUnset,
        /**
         *   Usually occurs when attempting to use the Depth24Stencil format
         * on non-depth-stencil formats.
         */
        DataFormatIsInvalid,
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
        InternalError
    };
public:
    [[nodiscard]] virtual ITexture2D* build(const Texture2DArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITexture2D* build(const Texture2DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITexture2D> buildCPPRef(const Texture2DArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITexture2D> buildTauRef(const Texture2DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITexture2D> buildTauSRef(const Texture2DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITexture3D* build(const Texture3DArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITexture3D* build(const Texture3DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITexture3D> buildCPPRef(const Texture3DArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITexture3D> buildTauRef(const Texture3DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITexture3D> buildTauSRef(const Texture3DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITextureCube* build(const TextureCubeArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITextureCube* build(const TextureCubeArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITextureCube> buildCPPRef(const TextureCubeArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITextureCube> buildTauRef(const TextureCubeArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITextureCube> buildTauSRef(const TextureCubeArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITextureDepthStencil* build(const TextureDepthStencilArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITextureDepthStencil* build(const TextureDepthStencilArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITextureDepthStencil> buildCPPRef(const TextureDepthStencilArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITextureDepthStencil> buildTauRef(const TextureDepthStencilArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITextureDepthStencil> buildTauSRef(const TextureDepthStencilArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
