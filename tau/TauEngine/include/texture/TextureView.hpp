#pragma once

#include <Safeties.hpp>

#include "graphics/ResourceView.hpp"
#include "texture/TextureEnums.hpp"
#include "graphics/DescriptorHeap.hpp"

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
    [[nodiscard]] virtual u32 arrayCount() const noexcept { return 0; }
    [[nodiscard]] virtual u32  mipLevels() const noexcept { return 0; }

    virtual void generateMipmaps(IRenderingContext& context) noexcept = 0;

    RTTD_BASE_IMPL(ITextureView);
    RTTD_BASE_CHECK(ITextureView);
    RTTD_BASE_CAST(ITextureView);
};

struct TextureViewArgs final
{
    DEFAULT_CONSTRUCT_PU(TextureViewArgs);
    DEFAULT_DESTRUCT(TextureViewArgs);
    DEFAULT_CM_PU(TextureViewArgs);
public:
    IResource* texture;
    ETexture::Format dataFormat;
    ETexture::Type type;
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
        DescriptorTableIsNull,
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
    [[nodiscard]] virtual ITextureView* build(const TextureViewArgs& args, [[tau::out]] Error* error, DescriptorTable table, uSys tableIndex) const noexcept = 0;
};
