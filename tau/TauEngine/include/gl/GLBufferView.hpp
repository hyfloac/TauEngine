#pragma once

#include "graphics/BufferView.hpp"

#include "GLResource.hpp"

#include <d3d12.h>

class TAU_DLL GLVertexBufferView final : public IVertexBufferView
{
    DEFAULT_DESTRUCT(GLVertexBufferView);
    DEFAULT_CM_PU(GLVertexBufferView);
    VERTEX_BUFFER_VIEW_IMPL(GLVertexBufferView);
private:
    GLResourceBuffer& _buffer;
public:
    GLVertexBufferView(const BufferDescriptor& descriptor, GLResourceBuffer& buffer) noexcept
        : IVertexBufferView(descriptor)
        , _buffer(buffer)
    { }

    [[nodiscard]]       GLResourceBuffer& buffer()       noexcept { return _buffer; }
    [[nodiscard]] const GLResourceBuffer& buffer() const noexcept { return _buffer; }
};

class TAU_DLL GLIndexBufferView final : public IIndexBufferView
{
    DEFAULT_DESTRUCT(GLIndexBufferView);
    DEFAULT_CM_PU(GLIndexBufferView);
    INDEX_BUFFER_VIEW_IMPL(GLIndexBufferView);
private:
    GLResourceBuffer& _buffer;
public:
    GLIndexBufferView(const EBuffer::IndexSize indexSize, GLResourceBuffer& buffer) noexcept
        : IIndexBufferView(indexSize)
        , _buffer(buffer)
    { }

    [[nodiscard]]       GLResourceBuffer& buffer()       noexcept { return _buffer; }
    [[nodiscard]] const GLResourceBuffer& buffer() const noexcept { return _buffer; }
};

class TAU_DLL GLUniformBufferView final : public IUniformBufferView
{
    DEFAULT_DESTRUCT(GLUniformBufferView);
    DEFAULT_CM_PU(GLUniformBufferView);
    UNIFORM_BUFFER_VIEW_IMPL(GLUniformBufferView);
private:
    GLResourceBuffer& _buffer;
public:
    GLUniformBufferView(GLResourceBuffer& buffer) noexcept
        : _buffer(buffer)
    { }

    [[nodiscard]]       GLResourceBuffer& buffer()       noexcept { return _buffer; }
    [[nodiscard]] const GLResourceBuffer& buffer() const noexcept { return _buffer; }
};

class TAU_DLL GLBufferViewBuilder final : public IBufferViewBuilder
{
    DEFAULT_CONSTRUCT_PU(GLBufferViewBuilder);
    DEFAULT_DESTRUCT(GLBufferViewBuilder);
    DEFAULT_CM_PU(GLBufferViewBuilder);
public:
    struct GLBufferViewArgs final
    {
        GLResourceBuffer* buffer;
    };
public:
    [[nodiscard]] GLVertexBufferView* build(const VertexBufferViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLVertexBufferView* build(const VertexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IVertexBufferView> buildCPPRef(const VertexBufferViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IVertexBufferView> buildTauRef(const VertexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IVertexBufferView> buildTauSRef(const VertexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] GLIndexBufferView* build(const IndexBufferViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLIndexBufferView* build(const IndexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IIndexBufferView> buildCPPRef(const IndexBufferViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IIndexBufferView> buildTauRef(const IndexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IIndexBufferView> buildTauSRef(const IndexBufferViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] GLUniformBufferView* build(const UniformBufferViewArgs& args, Error* error, DescriptorTable table, uSys tableIndex) const noexcept override;
private:
    [[nodiscard]] static bool processArgs(const VertexBufferViewArgs& args, [[tau::out]] GLBufferViewArgs* glArgs, [[tau::out]] Error* error) noexcept;
    [[nodiscard]] static bool processArgs(const IndexBufferViewArgs& args, [[tau::out]] GLBufferViewArgs* glArgs, [[tau::out]] Error* error) noexcept;
    [[nodiscard]] static bool processArgs(const UniformBufferViewArgs& args, [[tau::out]] GLBufferViewArgs* glArgs, [[tau::out]] Error* error) noexcept;
};
