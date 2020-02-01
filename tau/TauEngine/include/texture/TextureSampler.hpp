#pragma once

#include <Objects.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"
#include "TextureEnums.hpp"
#include "Color.hpp"

#define TEXTURE_SAMPLER_IMPL_BASE(_TYPE) DELETE_COPY(_TYPE); \
                                         RTT_IMPL(_TYPE, ITextureSampler)

#define TEXTURE_SAMPLER_IMPL(_TYPE) TEXTURE_SAMPLER_IMPL_BASE(_TYPE)

class TAU_DLL NOVTABLE ITextureSampler
{
    DEFAULT_CONSTRUCT_PO(ITextureSampler);
    DEFAULT_DESTRUCT_VI(ITextureSampler);
    DELETE_COPY(ITextureSampler);
public:
    RTT_BASE_IMPL(ITextureSampler);
    RTT_BASE_CHECK(ITextureSampler);
    RTT_BASE_CAST(ITextureSampler);
};

class TAU_DLL NOVTABLE ITextureSamplerBuilder
{
    DEFAULT_CONSTRUCT_PO(ITextureSamplerBuilder);
    DEFAULT_DESTRUCT_VI(ITextureSamplerBuilder);
    DELETE_COPY(ITextureSamplerBuilder);
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
    virtual void setFilterMode(ETexture::Filter magnificationFilter, ETexture::Filter minificationFilter, ETexture::Filter mipmapMinificationFilter) noexcept = 0;

    virtual void setWrapMode(ETexture::WrapMode u, ETexture::WrapMode v, ETexture::WrapMode w) noexcept = 0;

    virtual void setDepthComparison(ETexture::DepthCompareFunc compareFunc) noexcept = 0;

    virtual void setBorderColor(RGBAColor color) noexcept = 0;

    [[nodiscard]] virtual ITextureSampler* build([[tau::out]] Error* error = null) noexcept = 0;
};
