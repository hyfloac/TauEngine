#pragma once

#include <Objects.hpp>
#include <DynArray.hpp>
#include <Safeties.hpp>
#include <RunTimeType.hpp>

#include "DLL.hpp"
#include "graphics/BufferView.hpp"

class IResource;

#define VERTEX_ARRAY_IMPL_BASE(_TYPE) \
    RTT_IMPL(_TYPE, IVertexArray)

#define VERTEX_ARRAY_IMPL(_TYPE) VERTEX_ARRAY_IMPL_BASE(_TYPE)

class TAU_DLL TAU_NOVTABLE IVertexArray
{
    DEFAULT_DESTRUCT_VI(IVertexArray);
    DEFAULT_CM_PO(IVertexArray);
protected:
    /**
     *   It is necessary to hold on to a pointer to the buffers to
     * ensure they don't get destroyed.
     */
    DynArray<NullableRef<IResource>> _buffers;
protected:
    IVertexArray(const DynArray<NullableRef<IResource>>& buffers) noexcept
        : _buffers(buffers)
    { }

    IVertexArray(DynArray<NullableRef<IResource>>&& buffers) noexcept
        : _buffers(::std::move(buffers))
    { }
public:
    [[nodiscard]] const DynArray<NullableRef<IResource>>& buffers() const noexcept { return _buffers; }
public:
    RTT_BASE_IMPL(IVertexArray);
    RTT_BASE_CHECK(IVertexArray);
    RTT_BASE_CAST(IVertexArray);
};

struct VertexArrayArgs final
{
    DEFAULT_CONSTRUCT_PU(VertexArrayArgs);
    DEFAULT_DESTRUCT(VertexArrayArgs);
    DEFAULT_CM_PU(VertexArrayArgs);
public:
    uSys bufferCount;
    VertexBufferView* bufferViews;
};

class TAU_DLL NOVTABLE IVertexArrayBuilder
{
    DEFAULT_CONSTRUCT_PO(IVertexArrayBuilder);
    DEFAULT_DESTRUCT_VI(IVertexArrayBuilder);
    DEFAULT_CM_PO(IVertexArrayBuilder);
public:
    enum class Error
    {
        NoError = 0,
        BuffersNotSet,
        ResourceIsNotBuffer,
        SystemMemoryAllocationFailure,
        DriverMemoryAllocationFailure,
        InternalError
    };
public:
    [[nodiscard]] virtual NullableRef<IVertexArray> buildTauRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) noexcept = 0;
};
