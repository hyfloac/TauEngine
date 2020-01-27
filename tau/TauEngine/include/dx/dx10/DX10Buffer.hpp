#pragma once

#include "model/Buffer.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10RenderingContext;

class TAU_DLL DX10Buffer final : public IBuffer
{
    BUFFER_IMPL(DX10Buffer);
public:
    static D3D10_USAGE getDXUsage(EBuffer::UsageType usage) noexcept;
    static bool canReWrite(EBuffer::UsageType usage) noexcept;
private:
    ID3D10Buffer* _d3dBuffer;
public:
    DX10Buffer(EBuffer::Type type, EBuffer::UsageType usage, uSys bufferSize, bool instanced, const BufferDescriptor& descriptor, ID3D10Buffer* d3dBuffer) noexcept;

    ~DX10Buffer() noexcept;

    [[nodiscard]] const ID3D10Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }
    [[nodiscard]] ID3D10Buffer* d3dBuffer() noexcept { return _d3dBuffer; }

    void bind(IRenderingContext& context) noexcept override { /* NO-OP */ }
    void unbind(IRenderingContext& context) noexcept override { /* NO-OP */ }
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
    void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept override;

    [[nodiscard]] void* mapBuffer(IRenderingContext& context) noexcept override;
    void unmapBuffer(IRenderingContext& context) noexcept override;
};

class TAU_DLL DX10IndexBuffer final : public IIndexBuffer
{
    INDEX_BUFFER_IMPL(DX10IndexBuffer);
private:
    ID3D10Buffer* _d3dBuffer;
public:
    DX10IndexBuffer(EBuffer::UsageType usage, uSys bufferSize, ID3D10Buffer* d3dBuffer) noexcept;

    ~DX10IndexBuffer() noexcept;

    [[nodiscard]] const ID3D10Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }
    [[nodiscard]] ID3D10Buffer* d3dBuffer() noexcept { return _d3dBuffer; }

    void bind(IRenderingContext& context) noexcept override { }
    void unbind(IRenderingContext& context) noexcept override { }
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
    void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept override;

    [[nodiscard]] void* mapBuffer(IRenderingContext& context) noexcept override;
    void unmapBuffer(IRenderingContext& context) noexcept override;
};

class TAU_DLL DX10BufferBuilder final : public IBufferBuilder
{
    DEFAULT_DESTRUCT(DX10BufferBuilder);
    DELETE_COPY(DX10BufferBuilder);
private:
    DX10RenderingContext& _context;
public:
    DX10BufferBuilder(uSys descriptorCount, DX10RenderingContext& context) noexcept;

    [[nodiscard]] DX10Buffer* build(Error* error) const noexcept override;
};

class TAU_DLL DX10IndexBufferBuilder final : public IIndexBufferBuilder
{
    DEFAULT_DESTRUCT(DX10IndexBufferBuilder);
    DELETE_COPY(DX10IndexBufferBuilder);
private:
    DX10RenderingContext& _context;
public:
    DX10IndexBufferBuilder(DX10RenderingContext& context) noexcept;

    [[nodiscard]] DX10IndexBuffer* build(Error* error) const noexcept override;
};
#endif
