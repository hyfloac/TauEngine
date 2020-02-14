#pragma once

#include "model/Buffer.hpp"
#include "dx/dx9/DX9RenderingContext.hpp"

#ifdef _WIN32
#pragma warning(push, 0)
#include <d3d9.h>
#pragma warning(pop)

class TAU_DLL DX9Buffer final : public IBuffer
{
    BUFFER_IMPL(DX9Buffer);
private:
    IDirect3DVertexBuffer9* _buffer;
public:
    DX9Buffer(const EBuffer::Type type, const EBuffer::UsageType usage, const uSys bufferSize, const bool instanced, const BufferDescriptor& descriptor, IDirect3DVertexBuffer9* buffer)
        : IBuffer(type, usage, bufferSize, instanced, descriptor),
          _buffer(buffer)
    { }

    ~DX9Buffer() noexcept override final;


    void bind(IRenderingContext& context) noexcept override
    { /* No-Op */ }

    void unbind(IRenderingContext& context) noexcept override
    { /* No-Op */ }

    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;

    void beginModification(IRenderingContext& context) noexcept override { }

    void endModification(IRenderingContext& context) noexcept override { }

    void modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept override { }
};
#endif
