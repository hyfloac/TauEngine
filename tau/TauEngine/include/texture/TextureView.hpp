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

    [[nodiscard]] virtual u32      width() const noexcept { return 0; }
    [[nodiscard]] virtual u32     height() const noexcept { return 0; }
    [[nodiscard]] virtual u32      depth() const noexcept { return 0; }
    [[nodiscard]] virtual u32   mipCount() const noexcept { return 0; }
    [[nodiscard]] virtual u32 arrayCount() const noexcept { return 0; }

    virtual void generateMipmaps(IRenderingContext& context) noexcept = 0;

    RTTD_BASE_IMPL(ITextureView);
    RTTD_BASE_CHECK(ITextureView);
    RTTD_BASE_CAST(ITextureView);
};

struct Texture1DViewArgs final
{
    DEFAULT_CONSTRUCT_PU(Texture1DViewArgs);
    DEFAULT_DESTRUCT(Texture1DViewArgs);
    DEFAULT_CM_PU(Texture1DViewArgs);
public:
    IResource* texture;
    ETexture::Format dataFormat;
};

struct Texture1DArrayViewArgs final
{
    DEFAULT_CONSTRUCT_PU(Texture1DArrayViewArgs);
    DEFAULT_DESTRUCT(Texture1DArrayViewArgs);
    DEFAULT_CM_PU(Texture1DArrayViewArgs);
public:
    IResource* texture;
    ETexture::Format dataFormat;
};

struct Texture2DViewArgs final
{
    DEFAULT_CONSTRUCT_PU(Texture2DViewArgs);
    DEFAULT_DESTRUCT(Texture2DViewArgs);
    DEFAULT_CM_PU(Texture2DViewArgs);
public:
    IResource* texture;
    ETexture::Format dataFormat;
};

struct Texture2DArrayViewArgs final
{
    DEFAULT_CONSTRUCT_PU(Texture2DArrayViewArgs);
    DEFAULT_DESTRUCT(Texture2DArrayViewArgs);
    DEFAULT_CM_PU(Texture2DArrayViewArgs);
public:
    IResource* texture;
    ETexture::Format dataFormat;
};

struct Texture3DViewArgs final
{
    DEFAULT_CONSTRUCT_PU(Texture3DViewArgs);
    DEFAULT_DESTRUCT(Texture3DViewArgs);
    DEFAULT_CM_PU(Texture3DViewArgs);
public:
    IResource* texture;
    ETexture::Format dataFormat;
};

struct TextureCubeViewArgs final
{
    DEFAULT_CONSTRUCT_PU(TextureCubeViewArgs);
    DEFAULT_DESTRUCT(TextureCubeViewArgs);
    DEFAULT_CM_PU(TextureCubeViewArgs);
public:
    IResource* texture;
    ETexture::Format dataFormat;
};

struct TextureCubeArrayViewArgs final
{
    DEFAULT_CONSTRUCT_PU(TextureCubeArrayViewArgs);
    DEFAULT_DESTRUCT(TextureCubeArrayViewArgs);
    DEFAULT_CM_PU(TextureCubeArrayViewArgs);
public:
    IResource* texture;
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
        TextureIsNull,
        InvalidTexture,
        TextureDoesNotSupportView,
        InvalidDataFormat,
        TextureIsNotArray,
        /**
         * Indicates that a specific view type is not supported.
         *
         * This will most likely be cube map arrays on DX10.
         */
        UnsupportedType,
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
        InternalError
    };
public:
    [[nodiscard]] virtual ITextureView* build(const Texture1DViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITextureView* build(const Texture1DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITextureView> buildCPPRef(const Texture1DViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITextureView> buildTauRef(const Texture1DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITextureView> buildTauSRef(const Texture1DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITextureView* build(const Texture1DArrayViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITextureView* build(const Texture1DArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITextureView> buildCPPRef(const Texture1DArrayViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITextureView> buildTauRef(const Texture1DArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITextureView> buildTauSRef(const Texture1DArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITextureView* build(const Texture2DViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITextureView* build(const Texture2DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITextureView> buildCPPRef(const Texture2DViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITextureView> buildTauRef(const Texture2DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITextureView> buildTauSRef(const Texture2DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITextureView* build(const Texture2DArrayViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITextureView* build(const Texture2DArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITextureView> buildCPPRef(const Texture2DArrayViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITextureView> buildTauRef(const Texture2DArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITextureView> buildTauSRef(const Texture2DArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITextureView* build(const Texture3DViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITextureView* build(const Texture3DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITextureView> buildCPPRef(const Texture3DViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITextureView> buildTauRef(const Texture3DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITextureView> buildTauSRef(const Texture3DViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITextureView* build(const TextureCubeViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITextureView* build(const TextureCubeViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITextureView> buildCPPRef(const TextureCubeViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITextureView> buildTauRef(const TextureCubeViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITextureView> buildTauSRef(const TextureCubeViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual ITextureView* build(const TextureCubeArrayViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITextureView* build(const TextureCubeArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITextureView> buildCPPRef(const TextureCubeArrayViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITextureView> buildTauRef(const TextureCubeArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITextureView> buildTauSRef(const TextureCubeArrayViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
