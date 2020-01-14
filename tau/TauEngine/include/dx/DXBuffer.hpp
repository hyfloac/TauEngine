#pragma once

#include "model/IBuffer.hpp"
#include "dx/DX9RenderingContext.hpp"

#ifdef _WIN32
#pragma warning(push, 0)
#include <d3d9.h>
#pragma warning(pop)

class TAU_DLL DXBuffer final : public IBuffer
{
    DELETE_COPY(DXBuffer);
private:
    IDirect3DVertexBuffer9* _buffer;
public:
    DXBuffer(const EBuffer::Type type, const EBuffer::UsageType usage, const uSys bufferSize, const BufferDescriptor& descriptor, IDirect3DVertexBuffer9* buffer)
        : IBuffer(type, usage, bufferSize, descriptor),
          _buffer(buffer)
    { }

    ~DXBuffer() noexcept override final;


    void bind(IRenderingContext& context) noexcept override
    { /* No-Op */ }

    void unbind(IRenderingContext& context) noexcept override
    { /* No-Op */ }

    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;

    void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size,
                      const void* data) noexcept override;
};

class TAU_DLL DXBufferBuilder final : public IBufferBuilder
{
    DEFAULT_DESTRUCT(DXBufferBuilder);
    DELETE_COPY(DXBufferBuilder);
private:
    DX9RenderingContext& _context;
public:
    DXBufferBuilder(uSys descriptorCount, DX9RenderingContext& context) noexcept;

    [[nodiscard]] IBuffer* build([[tau::out]] Error* error) const noexcept override;
};
#endif
