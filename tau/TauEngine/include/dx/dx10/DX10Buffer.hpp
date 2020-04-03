#pragma once

#include "model/Buffer.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10GraphicsInterface;
class DX10RenderingContext;

class TAU_DLL DX10Buffer final : public IBuffer
{
    BUFFER_IMPL(DX10Buffer);
public:
    static D3D10_USAGE getDXUsage(EBuffer::UsageType usage) noexcept;
    static D3D10_CPU_ACCESS_FLAG getDXAccess(EBuffer::UsageType usage) noexcept;
    static bool canReWrite(EBuffer::UsageType usage) noexcept;
private:
    ID3D10Buffer* _d3dBuffer;
    void* _currentMapping;
public:
    DX10Buffer(EBuffer::Type type, EBuffer::UsageType usage, uSys bufferSize, bool instanced, const BufferDescriptor& descriptor, ID3D10Buffer* d3dBuffer) noexcept;

    ~DX10Buffer() noexcept;

    [[nodiscard]] const ID3D10Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }
    [[nodiscard]] ID3D10Buffer* d3dBuffer() noexcept { return _d3dBuffer; }

    void bind(IRenderingContext&) noexcept override;
    void unbind(IRenderingContext&) noexcept override;

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL DX10IndexBuffer final : public IIndexBuffer
{
    INDEX_BUFFER_IMPL(DX10IndexBuffer);
private:
    ID3D10Buffer* _d3dBuffer;
    void* _currentMapping;
public:
    DX10IndexBuffer(EBuffer::UsageType usage, uSys bufferSize, ID3D10Buffer* d3dBuffer) noexcept;

    ~DX10IndexBuffer() noexcept;

    [[nodiscard]] const ID3D10Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }
    [[nodiscard]] ID3D10Buffer* d3dBuffer() noexcept { return _d3dBuffer; }

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL DX10UniformBuffer final : public IUniformBuffer
{
    UNIFORM_BUFFER_IMPL(DX10UniformBuffer);
private:
    ID3D10Buffer* _d3dBuffer;
    void* _currentMapping;
public:
    DX10UniformBuffer(EBuffer::UsageType usage, uSys bufferSize, ID3D10Buffer* d3dBuffer) noexcept;

    ~DX10UniformBuffer() noexcept;

    [[nodiscard]] const ID3D10Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }
    [[nodiscard]] ID3D10Buffer* d3dBuffer() noexcept { return _d3dBuffer; }

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    void bind(IRenderingContext& context, EShader::Stage stage, u32 index) noexcept override;
    void unbind(IRenderingContext& context, EShader::Stage, u32 index) noexcept override;

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL DX10BufferBuilder final : public IBufferBuilder
{
    DEFAULT_DESTRUCT(DX10BufferBuilder);
    DELETE_COPY(DX10BufferBuilder);
private:
    DX10GraphicsInterface& _gi;
public:
    DX10BufferBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10Buffer* build(const BufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10Buffer* build(const BufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IBuffer> buildCPPRef(const BufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IBuffer> buildTauRef(const BufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
    [[nodiscard]] NullableStrongRef<IBuffer> buildTauSRef(const BufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
private:
    [[nodiscard]] bool processBufferArgs(const BufferArgs& args, [[tau::out]] ID3D10Buffer** d3dBuffer, [[tau::out]] Error* error) const noexcept;
};

class TAU_DLL DX10IndexBufferBuilder final : public IIndexBufferBuilder
{
    DEFAULT_DESTRUCT(DX10IndexBufferBuilder);
    DELETE_COPY(DX10IndexBufferBuilder);
private:
    DX10GraphicsInterface& _gi;
public:
    DX10IndexBufferBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10IndexBuffer* build(const IndexBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10IndexBuffer* build(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IIndexBuffer> buildCPPRef(const IndexBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IIndexBuffer> buildTauRef(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
    [[nodiscard]] NullableStrongRef<IIndexBuffer> buildTauSRef(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
private:
    [[nodiscard]] bool processBufferArgs(const IndexBufferArgs& args, [[tau::out]] ID3D10Buffer** d3dBuffer, [[tau::out]] Error* error) const noexcept;
};

class TAU_DLL DX10UniformBufferBuilder final : public IUniformBufferBuilder
{
    DEFAULT_DESTRUCT(DX10UniformBufferBuilder);
    DELETE_COPY(DX10UniformBufferBuilder);
private:
    DX10GraphicsInterface& _gi;
public:
    DX10UniformBufferBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10UniformBuffer* build(const UniformBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10UniformBuffer* build(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IUniformBuffer> buildCPPRef(const UniformBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IUniformBuffer> buildTauRef(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
    [[nodiscard]] NullableStrongRef<IUniformBuffer> buildTauSRef(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
private:
    [[nodiscard]] bool processBufferArgs(const UniformBufferArgs& args, [[tau::out]] ID3D10Buffer** d3dBuffer, [[tau::out]] Error* error) const noexcept;
};
#endif
