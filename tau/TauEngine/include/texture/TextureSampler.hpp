#pragma once

#include <Objects.hpp>

#include "DLL.hpp"
#include "TextureEnums.hpp"
#include "Color.hpp"
#include "graphics/DescriptorHeap.hpp"

DECL_OPAQUE_TYPE(TextureSampler);

struct TextureSamplerArgs final
{
    DEFAULT_DESTRUCT(TextureSamplerArgs);
    DEFAULT_CM_PU(TextureSamplerArgs);
public:
    ETexture::Filter magnificationFilter;
    ETexture::Filter minificationFilter;
    ETexture::Filter mipmapMinificationFilter;
    ETexture::WrapMode wrapU;
    ETexture::WrapMode wrapV;
    ETexture::WrapMode wrapW;
    ETexture::CompareFunc depthCompareFunc;
    RGBAColor borderColor;
public:
    TextureSamplerArgs() noexcept
        : magnificationFilter(static_cast<ETexture::Filter>(0))
        , minificationFilter(static_cast<ETexture::Filter>(0))
        , mipmapMinificationFilter(static_cast<ETexture::Filter>(0))
        , wrapU(static_cast<ETexture::WrapMode>(0))
        , wrapV(static_cast<ETexture::WrapMode>(0))
        , wrapW(static_cast<ETexture::WrapMode>(0))
        , depthCompareFunc(static_cast<ETexture::CompareFunc>(0))
        , borderColor { 0, 0, 0, 0 }
    { }

    [[nodiscard]] const ETexture::Filter&    magFilter() const noexcept { return magnificationFilter;      }
    [[nodiscard]] const ETexture::Filter&    minFilter() const noexcept { return minificationFilter;       }
    [[nodiscard]] const ETexture::Filter&    mipFilter() const noexcept { return mipmapMinificationFilter; }
    [[nodiscard]] const ETexture::Filter& mipMinFilter() const noexcept { return mipmapMinificationFilter; }

    [[nodiscard]] ETexture::Filter&    magFilter() noexcept { return magnificationFilter;      }
    [[nodiscard]] ETexture::Filter&    minFilter() noexcept { return minificationFilter;       }
    [[nodiscard]] ETexture::Filter&    mipFilter() noexcept { return mipmapMinificationFilter; }
    [[nodiscard]] ETexture::Filter& mipMinFilter() noexcept { return mipmapMinificationFilter; }
};

class TAU_DLL TAU_NOVTABLE ITextureSamplerBuilder
{
    DEFAULT_CONSTRUCT_PO(ITextureSamplerBuilder);
    DEFAULT_DESTRUCT_VI(ITextureSamplerBuilder);
    DEFAULT_CM_PO(ITextureSamplerBuilder);
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
        DriverMemoryAllocationFailure,
        FilterIsUnset,
        WrapModeIsUnset,
        DepthComparisonIsUnset,
        DescriptorTableIsNull
    };
public:
    [[nodiscard]] virtual TextureSampler build(const TextureSamplerArgs& args, DescriptorSamplerTable table, uSys tableIndex, [[tau::out]] Error* error = null) const noexcept = 0;
};
