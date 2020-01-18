#pragma once

#include "model/IBuffer.hpp"
#include "dx/dx9/DX9RenderingContext.hpp"

#ifdef _WIN32
#pragma warning(push, 0)
#include <d3d9.h>
#pragma warning(pop)

class TAU_DLL DX9Buffer final : public IBuffer
{
    DELETE_COPY(DX9Buffer);
private:
    IDirect3DVertexBuffer9* _buffer;
public:
    DX9Buffer(const EBuffer::Type type, const EBuffer::UsageType usage, const uSys bufferSize, const BufferDescriptor& descriptor, IDirect3DVertexBuffer9* buffer)
        : IBuffer(type, usage, bufferSize, descriptor),
          _buffer(buffer)
    { }

    ~DX9Buffer() noexcept override final;


    void bind(IRenderingContext& context) noexcept override
    { /* No-Op */ }

    void unbind(IRenderingContext& context) noexcept override
    { /* No-Op */ }

    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;

    void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size,
                      const void* data) noexcept override;
};

class TAU_DLL DX9BufferBuilder final : public IBufferBuilder
{
    DEFAULT_DESTRUCT(DX9BufferBuilder);
    DELETE_COPY(DX9BufferBuilder);
private:
    DX9RenderingContext& _context;
public:
    DX9BufferBuilder(uSys descriptorCount, DX9RenderingContext& context) noexcept;

    [[nodiscard]] IBuffer* build([[tau::out]] Error* error) const noexcept override;
};
#endif
