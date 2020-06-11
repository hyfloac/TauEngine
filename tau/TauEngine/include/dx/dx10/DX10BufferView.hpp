#pragma once

#include "graphics/BufferView.hpp"

#ifdef _WIN32
#include "DX10Resource.hpp"

class TAU_DLL DX10VertexBufferView final : public IVertexBufferView
{
    DEFAULT_DESTRUCT(DX10VertexBufferView);
    DEFAULT_CM_PU(DX10VertexBufferView);
    VERTEX_BUFFER_VIEW_IMPL(DX10VertexBufferView);
private:
    DX10ResourceBuffer& _buffer;
public:
    DX10VertexBufferView(const BufferDescriptor& descriptor, DX10ResourceBuffer& buffer) noexcept
        : IVertexBufferView(descriptor)
        , _buffer(buffer)
    { }

    [[nodiscard]]       DX10ResourceBuffer& buffer()       noexcept { return _buffer; }
    [[nodiscard]] const DX10ResourceBuffer& buffer() const noexcept { return _buffer; }
};

class TAU_DLL DX10IndexBufferView final : public IIndexBufferView
{
    DEFAULT_DESTRUCT(DX10IndexBufferView);
    DEFAULT_CM_PU(DX10IndexBufferView);
    INDEX_BUFFER_VIEW_IMPL(DX10IndexBufferView);
private:
    DX10ResourceBuffer& _buffer;
public:
    DX10IndexBufferView(const EBuffer::IndexSize indexSize, DX10ResourceBuffer& buffer) noexcept
        : IIndexBufferView(indexSize)
        , _buffer(buffer)
    { }

    [[nodiscard]]       DX10ResourceBuffer& buffer()       noexcept { return _buffer; }
    [[nodiscard]] const DX10ResourceBuffer& buffer() const noexcept { return _buffer; }
};

class TAU_DLL DX10UniformBufferView final : public IUniformBufferView
{
    DEFAULT_DESTRUCT(DX10UniformBufferView);
    DEFAULT_CM_PU(DX10UniformBufferView);
    UNIFORM_BUFFER_VIEW_IMPL(DX10UniformBufferView);
private:
    DX10ResourceBuffer& _buffer;
public:
    DX10UniformBufferView(DX10ResourceBuffer& buffer) noexcept
        : _buffer(buffer)
    { }

    [[nodiscard]]       DX10ResourceBuffer& buffer()       noexcept { return _buffer; }
    [[nodiscard]] const DX10ResourceBuffer& buffer() const noexcept { return _buffer; }
};

class TAU_DLL DX10BufferViewBuilder final : public IBufferViewBuilder
{
    DEFAULT_CONSTRUCT_PU(DX10BufferViewBuilder);
    DEFAULT_DESTRUCT(DX10BufferViewBuilder);
    DEFAULT_CM_PU(DX10BufferViewBuilder);
public:
    struct DXBufferViewArgs final
    {
        DX10ResourceBuffer* buffer;
    };
public:
    [[nodiscard]] DX10VertexBufferView* build(const VertexBufferViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10VertexBufferView* build(const VertexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IVertexBufferView> buildCPPRef(const VertexBufferViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IVertexBufferView> buildTauRef(const VertexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IVertexBufferView> buildTauSRef(const VertexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX10IndexBufferView* build(const IndexBufferViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10IndexBufferView* build(const IndexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IIndexBufferView> buildCPPRef(const IndexBufferViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IIndexBufferView> buildTauRef(const IndexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IIndexBufferView> buildTauSRef(const IndexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX10UniformBufferView* build(const UniformBufferViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10UniformBufferView* build(const UniformBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IUniformBufferView> buildCPPRef(const UniformBufferViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IUniformBufferView> buildTauRef(const UniformBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IUniformBufferView> buildTauSRef(const UniformBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] static bool processArgs(const VertexBufferViewArgs& args, [[tau::out]] DXBufferViewArgs* dxArgs, [[tau::out]] Error* error) noexcept;
    [[nodiscard]] static bool processArgs(const IndexBufferViewArgs& args, [[tau::out]] DXBufferViewArgs* dxArgs, [[tau::out]] Error* error) noexcept;
    [[nodiscard]] static bool processArgs(const UniformBufferViewArgs& args, [[tau::out]] DXBufferViewArgs* dxArgs, [[tau::out]] Error* error) noexcept;
};
#endif
