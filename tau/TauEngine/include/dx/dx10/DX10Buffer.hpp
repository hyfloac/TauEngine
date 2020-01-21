#pragma once

#include "model/IBuffer.hpp"

#ifdef _WIN32
#include <d3d10.h>

class TAU_DLL DX10Buffer final : public IBuffer
{
    DEFAULT_CONSTRUCT(DX10Buffer);
    DEFAULT_DESTRUCT(DX10Buffer);
    DELETE_COPY(DX10Buffer);
private:
public:
    [[nodiscard]] const ID3D10Buffer* d3dBuffer() const noexcept { return null; }
    [[nodiscard]] ID3D10Buffer* d3dBuffer() noexcept { return null; }


    void bind(IRenderingContext& context) noexcept override { }
    void unbind(IRenderingContext& context) noexcept override { }
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override { }
    void modifyBuffer(IRenderingContext& context, intptr_t offset, std::ptrdiff_t size, const void* data) noexcept override { }
};

class TAU_DLL DX10IndexBuffer final : public IIndexBuffer
{
public:
    [[nodiscard]] const ID3D10Buffer * d3dBuffer() const noexcept { return null; }
    [[nodiscard]] ID3D10Buffer * d3dBuffer() noexcept { return null; }

};
#endif
