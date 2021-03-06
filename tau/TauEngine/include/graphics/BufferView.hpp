/**
 * @file
 */
#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>

#include "BufferEnums.hpp"
#include "BufferDescriptor.hpp"
#include "DescriptorHeap.hpp"
#include "_GraphicsOpaqueObjects.hpp"

class IResource;

/**
 * A view into a vertex buffer resource.
 *
 *   This contains the buffers descriptor. This is used by the
 * input assembler to decode the data for shaders.
 */
struct VertexBufferView final
{
    DEFAULT_CONSTRUCT_PU(VertexBufferView);
    DEFAULT_DESTRUCT(VertexBufferView);
    DEFAULT_CM_PU(VertexBufferView);
public:
    NullableRef<IResource> buffer;
    BufferDescriptor descriptor;
public:
    VertexBufferView(const NullableRef<IResource>& _buffer, const BufferDescriptor& _descriptor) noexcept
        : buffer(_buffer)
        , descriptor(_descriptor)
    { }
};

/**
 * A view into a vertex buffer resource.
 *
 *   This contains information about the indice size. This is
 * used for decoding the indice stream.
 */
struct IndexBufferView final
{
    DEFAULT_CONSTRUCT_PU(IndexBufferView);
    DEFAULT_DESTRUCT(IndexBufferView);
    DEFAULT_CM_PU(IndexBufferView);
public:
    NullableRef<IResource> buffer;
    EBuffer::IndexSize indexSize;
public:
    IndexBufferView(const NullableRef<IResource>& _buffer, const EBuffer::IndexSize _indexSize) noexcept
        : buffer(_buffer)
        , indexSize(_indexSize)
    { }
};

struct UniformBufferViewArgs final
{
    DEFAULT_CONSTRUCT_PU(UniformBufferViewArgs);
    DEFAULT_DESTRUCT(UniformBufferViewArgs);
    DEFAULT_CM_PU(UniformBufferViewArgs);
public:
    NullableRef<IResource> buffer;
public:
    UniformBufferViewArgs(const NullableRef<IResource>& _buffer) noexcept
        : buffer(_buffer)
    { }
};

class TAU_DLL TAU_NOVTABLE IBufferViewBuilder
{
    DEFAULT_CONSTRUCT_PO(IBufferViewBuilder);
    DEFAULT_DESTRUCT_VI(IBufferViewBuilder);
    DEFAULT_CM_PO(IBufferViewBuilder);
public:
    enum class Error
    {
        NoError = 0,
        BufferIsNull,
        ResourceIsNotBuffer,
        DescriptorTableIsNull,
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
    [[nodiscard]] virtual UniformBufferView build(const UniformBufferViewArgs& args, CPUDescriptorHandle handle, [[tau::out]] Error* error) const noexcept = 0;
};
