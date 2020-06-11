/**
 * @file
 */
#pragma once

#include <Objects.hpp>
#include <Safeties.hpp>

#include "BufferDescriptor.hpp"
#include "ResourceView.hpp"
#include "BufferEnums.hpp"

class IResource;

#define VERTEX_BUFFER_VIEW_IMPL_BASE(_TYPE) \
    RTTD_IMPL(_TYPE, IVertexBufferView)

#define INDEX_BUFFER_VIEW_IMPL_BASE(_TYPE) \
    RTTD_IMPL(_TYPE, IIndexBufferView)

#define UNIFORM_BUFFER_VIEW_IMPL_BASE(_TYPE) \
    RTTD_IMPL(_TYPE, IUniformBufferView)

#define VERTEX_BUFFER_VIEW_IMPL(_TYPE) VERTEX_BUFFER_VIEW_IMPL_BASE(_TYPE)
#define INDEX_BUFFER_VIEW_IMPL(_TYPE) INDEX_BUFFER_VIEW_IMPL_BASE(_TYPE)
#define UNIFORM_BUFFER_VIEW_IMPL(_TYPE) UNIFORM_BUFFER_VIEW_IMPL_BASE(_TYPE)

/**
 * A view into a vertex buffer resource.
 *
 *   This contains the buffers descriptor. This is used by the
 * input assembler to decode the data for shaders.
 */
class TAU_DLL TAU_NOVTABLE IVertexBufferView : public IResourceView
{
    DEFAULT_DESTRUCT_VI(IVertexBufferView);
    DEFAULT_CM_PO(IVertexBufferView);
    RESOURCE_VIEW_IMPL(IVertexBufferView);
protected:
    BufferDescriptor _descriptor;
protected:
    IVertexBufferView(const BufferDescriptor& descriptor) noexcept
        : _descriptor(descriptor)
    { }
public:
    [[nodiscard]] const BufferDescriptor& descriptor() const noexcept { return _descriptor; }

    RTTD_BASE_IMPL(IVertexBufferView);
    RTTD_BASE_CHECK(IVertexBufferView);
    RTTD_BASE_CAST(IVertexBufferView);
};

/**
 * A view into a vertex buffer resource.
 *
 *   This contains information about the indice size. This is
 * used for decoding the indice stream.
 */
class TAU_DLL TAU_NOVTABLE IIndexBufferView : public IResourceView
{
    DEFAULT_DESTRUCT_VI(IIndexBufferView);
    DEFAULT_CM_PO(IIndexBufferView);
    RESOURCE_VIEW_IMPL(IIndexBufferView);
protected:
    EBuffer::IndexSize _indexSize;
protected:
    IIndexBufferView(const EBuffer::IndexSize indexSize) noexcept
        : _indexSize(indexSize)
    { }
public:
    [[nodiscard]] EBuffer::IndexSize indexSize() const noexcept { return _indexSize; }

    RTTD_BASE_IMPL(IIndexBufferView);
    RTTD_BASE_CHECK(IIndexBufferView);
    RTTD_BASE_CAST(IIndexBufferView);
};

/**
 * A view into a uniform buffer resource.
 *
 *   This is used for binding uniform buffers to the shader
 * pipeline.
 */
class TAU_DLL TAU_NOVTABLE IUniformBufferView : public IResourceView
{
    DEFAULT_CONSTRUCT_PO(IUniformBufferView);
    DEFAULT_DESTRUCT_VI(IUniformBufferView);
    DEFAULT_CM_PO(IUniformBufferView);
    RESOURCE_VIEW_IMPL(IUniformBufferView);
public:
    RTTD_BASE_IMPL(IUniformBufferView);
    RTTD_BASE_CHECK(IUniformBufferView);
    RTTD_BASE_CAST(IUniformBufferView);
};

struct VertexBufferViewArgs final
{
    DEFAULT_CONSTRUCT_PU(VertexBufferViewArgs);
    DEFAULT_DESTRUCT(VertexBufferViewArgs);
    DEFAULT_CM_PU(VertexBufferViewArgs);
public:
    IResource* buffer;
    BufferDescriptor descriptor;
public:
    VertexBufferViewArgs(IResource* const _buffer, const BufferDescriptor& _descriptor) noexcept
        : buffer(_buffer)
        , descriptor(_descriptor)
    { }

    VertexBufferViewArgs(IResource* const _buffer) noexcept
        : buffer(_buffer)
        , descriptor()
    { }

    VertexBufferViewArgs(const BufferDescriptor& _descriptor) noexcept
        : buffer(null)
        , descriptor(_descriptor)
    { }
};

struct IndexBufferViewArgs final
{
    DEFAULT_CONSTRUCT_PU(IndexBufferViewArgs);
    DEFAULT_DESTRUCT(IndexBufferViewArgs);
    DEFAULT_CM_PU(IndexBufferViewArgs);
public:
    IResource* buffer;
    EBuffer::IndexSize indexSize;
public:
    IndexBufferViewArgs(IResource* const _buffer, const EBuffer::IndexSize _indexSize) noexcept
        : buffer(_buffer)
        , indexSize(_indexSize)
    { }

    IndexBufferViewArgs(IResource* const _buffer) noexcept
        : buffer(_buffer)
        , indexSize(EBuffer::IndexSize::Uint32)
    { }

    IndexBufferViewArgs(const EBuffer::IndexSize _indexSize) noexcept
        : buffer(null)
        , indexSize(_indexSize)
    { }
};

struct UniformBufferViewArgs final
{
    DEFAULT_CONSTRUCT_PU(UniformBufferViewArgs);
    DEFAULT_DESTRUCT(UniformBufferViewArgs);
    DEFAULT_CM_PU(UniformBufferViewArgs);
public:
    IResource* buffer;
public:
    UniformBufferViewArgs(IResource* const _buffer) noexcept
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
    [[nodiscard]] virtual IVertexBufferView* build(const VertexBufferViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IVertexBufferView* build(const VertexBufferViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IVertexBufferView> buildCPPRef(const VertexBufferViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IVertexBufferView> buildTauRef(const VertexBufferViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IVertexBufferView> buildTauSRef(const VertexBufferViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual IIndexBufferView* build(const IndexBufferViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IIndexBufferView* build(const IndexBufferViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IIndexBufferView> buildCPPRef(const IndexBufferViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IIndexBufferView> buildTauRef(const IndexBufferViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IIndexBufferView> buildTauSRef(const IndexBufferViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;

    [[nodiscard]] virtual IUniformBufferView* build(const UniformBufferViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual IUniformBufferView* build(const UniformBufferViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept = 0;
    [[nodiscard]] virtual CPPRef<IUniformBufferView> buildCPPRef(const UniformBufferViewArgs& args, [[tau::out]] Error* error) const noexcept = 0;
    [[nodiscard]] virtual NullableRef<IUniformBufferView> buildTauRef(const UniformBufferViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
    [[nodiscard]] virtual NullableStrongRef<IUniformBufferView> buildTauSRef(const UniformBufferViewArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept = 0;
};
