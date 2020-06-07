#pragma once

#include "graphics/ResourceView.hpp"
#include <Safeties.hpp>

class IRenderingContext;

#define TEXTURE_VIEW_IMPL_BASE(_TYPE) \
    RTTD_IMPL(_TYPE, ITextureView)

#define TEXTURE_VIEW_IMPL(_TYPE) TEXTURE_VIEW_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE ITextureView : public IResourceView
{
    DEFAULT_CONSTRUCT_PO(ITextureView);
    DEFAULT_DESTRUCT_VI(ITextureView);
    DEFAULT_CM_PO(ITextureView);
    RESOURCE_VIEW_IMPL(ITextureView);
public:
    virtual void generateMipmaps(IRenderingContext& context) noexcept = 0;

    RTTD_BASE_IMPL(ITextureView);
    RTTD_BASE_CHECK(ITextureView);
    RTTD_BASE_CAST(ITextureView);
};

struct TextureViewArgs final
{
    DEFAULT_DESTRUCT(TextureViewArgs);
    DEFAULT_CM_PU(TextureViewArgs);
public:
public:
    inline TextureViewArgs() noexcept
    { }
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
    [[nodiscard]] virtual ITextureView* build(const TextureViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual ITextureView* build(const TextureViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<ITextureView> buildCPPRef(const TextureViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<ITextureView> buildTauRef(const TextureViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<ITextureView> buildTauSRef(const TextureViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

};
