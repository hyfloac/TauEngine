#pragma once

#include "model/Buffer.hpp"

#ifdef _WIN32
#include <d3d11.h>

class DX11GraphicsInterface;
class DX11RenderingContext;

class TAU_DLL DX11Buffer final : public IBuffer
{
    BUFFER_IMPL(DX11Buffer);
public:
    static D3D11_USAGE getDXUsage(EBuffer::UsageType usage) noexcept;
    static D3D11_CPU_ACCESS_FLAG getDXAccess(EBuffer::UsageType usage) noexcept;
    static bool canReWrite(EBuffer::UsageType usage) noexcept;
private:
    ID3D11Buffer* _d3dBuffer;
    void* _currentMapping;
public:
    DX11Buffer(const EBuffer::Type type, const EBuffer::UsageType usage, const uSys bufferSize, const bool instanced, const BufferDescriptor& descriptor, ID3D11Buffer* const d3dBuffer) noexcept
        : IBuffer(type, usage, bufferSize, instanced, descriptor)
        , _d3dBuffer(d3dBuffer)
        , _currentMapping(null)
    { }

    ~DX11Buffer() noexcept
    {
        _d3dBuffer->Release();
        _d3dBuffer = null;
    }

    [[nodiscard]] const ID3D11Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }
    [[nodiscard]] ID3D11Buffer* d3dBuffer() noexcept { return _d3dBuffer; }

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL DX11IndexBuffer final : public IIndexBuffer
{
    INDEX_BUFFER_IMPL(DX11IndexBuffer);
private:
    ID3D11Buffer* _d3dBuffer;
    void* _currentMapping;
public:
    DX11IndexBuffer(const EBuffer::UsageType usage, const uSys bufferSize, ID3D11Buffer* const d3dBuffer) noexcept
        : IIndexBuffer(usage, bufferSize)
        , _d3dBuffer(d3dBuffer)
        , _currentMapping(null)
    { }

    ~DX11IndexBuffer() noexcept
    {
        _d3dBuffer->Release();
        _d3dBuffer = null;
    }

    [[nodiscard]] const ID3D11Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }
    [[nodiscard]] ID3D11Buffer* d3dBuffer() noexcept { return _d3dBuffer; }

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL DX11UniformBuffer final : public IUniformBuffer
{
    UNIFORM_BUFFER_IMPL(DX11UniformBuffer);
private:
    ID3D11Buffer* _d3dBuffer;
    void* _currentMapping;
public:
    DX11UniformBuffer(const EBuffer::UsageType usage, const uSys bufferSize, ID3D11Buffer* const d3dBuffer) noexcept
        : IUniformBuffer(usage, bufferSize)
        , _d3dBuffer(d3dBuffer)
        , _currentMapping(null)
    { }

    ~DX11UniformBuffer() noexcept
    {
        _d3dBuffer->Release();
        _d3dBuffer = null;
    }

    [[nodiscard]] const ID3D11Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }
    [[nodiscard]] ID3D11Buffer* d3dBuffer() noexcept { return _d3dBuffer; }

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    void bind(IRenderingContext& context, EShader::Stage stage, u32 index) noexcept override;
    void unbind(IRenderingContext& context, EShader::Stage, u32 index) noexcept override;

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL DX11BufferBuilder final : public IBufferBuilder
{
    DEFAULT_DESTRUCT(DX11BufferBuilder);
    DELETE_COPY(DX11BufferBuilder);
private:
    DX11GraphicsInterface& _gi;
public:
    DX11BufferBuilder(DX11GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX11Buffer* build(const BufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11Buffer* build(const BufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IBuffer> buildCPPRef(const BufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IBuffer> buildTauRef(const BufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
    [[nodiscard]] NullableStrongRef<IBuffer> buildTauSRef(const BufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const BufferArgs& args, [[tau::out]] ID3D11Buffer** d3dBuffer, [[tau::out]] Error* error) const noexcept;
};

class TAU_DLL DX11IndexBufferBuilder final : public IIndexBufferBuilder
{
    DEFAULT_DESTRUCT(DX11IndexBufferBuilder);
    DELETE_COPY(DX11IndexBufferBuilder);
private:
    DX11GraphicsInterface& _gi;
public:
    DX11IndexBufferBuilder(DX11GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX11IndexBuffer* build(const IndexBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11IndexBuffer* build(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IIndexBuffer> buildCPPRef(const IndexBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IIndexBuffer> buildTauRef(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
    [[nodiscard]] NullableStrongRef<IIndexBuffer> buildTauSRef(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const IndexBufferArgs& args, [[tau::out]] ID3D11Buffer** d3dBuffer, [[tau::out]] Error* error) const noexcept;
};

class TAU_DLL DX11UniformBufferBuilder final : public IUniformBufferBuilder
{
    DEFAULT_DESTRUCT(DX11UniformBufferBuilder);
    DELETE_COPY(DX11UniformBufferBuilder);
private:
    DX11GraphicsInterface& _gi;
public:
    DX11UniformBufferBuilder(DX11GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX11UniformBuffer* build(const UniformBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11UniformBuffer* build(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IUniformBuffer> buildCPPRef(const UniformBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IUniformBuffer> buildTauRef(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
    [[nodiscard]] NullableStrongRef<IUniformBuffer> buildTauSRef(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const UniformBufferArgs& args, [[tau::out]] ID3D11Buffer** d3dBuffer, [[tau::out]] Error* error) const noexcept;
};
#endif
