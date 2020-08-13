#pragma once

#include "texture/TextureEnums.hpp"
#include "graphics/_GraphicsOpaqueObjects.hpp"
#include "graphics/DescriptorHeap.hpp"
#include "DLL.hpp"

class IRenderingContext;
class IResource;

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
    [[nodiscard]] virtual TextureView build(const TextureViewArgs& args, CPUDescriptorHandle handle, [[tau::out]] Error* error) const noexcept = 0;
};
