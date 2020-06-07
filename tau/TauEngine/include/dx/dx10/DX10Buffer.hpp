#pragma once

#include "model/Buffer.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"

class DX10GraphicsInterface;
class DX10RenderingContext;

class TAU_DLL DX10VertexBuffer final : public IVertexBuffer
{
    VERTEX_BUFFER_IMPL(DX10VertexBuffer);
public:
    [[nodiscard]] static D3D10_USAGE getDXUsage(EBuffer::UsageType usage) noexcept;
    [[nodiscard]] static D3D10_CPU_ACCESS_FLAG getDXAccess(EBuffer::UsageType usage) noexcept;
    [[nodiscard]] static bool canReWrite(EBuffer::UsageType usage) noexcept;
private:
    ID3D10Buffer* _d3dBuffer;
    void* _currentMapping;
public:
    DX10VertexBuffer(const EBuffer::UsageType usage, const uSys bufferSize, const BufferDescriptor& descriptor, ID3D10Buffer* const d3dBuffer) noexcept
        : IVertexBuffer(usage, bufferSize, descriptor)
        , _d3dBuffer(d3dBuffer)
        , _currentMapping(null)
    { }

    ~DX10VertexBuffer() noexcept
    { RELEASE_DX(_d3dBuffer); }

    DX10VertexBuffer(const DX10VertexBuffer& copy) noexcept
        : IVertexBuffer(copy)
        , _d3dBuffer(copy._d3dBuffer)
        , _currentMapping(copy._currentMapping)
    { _d3dBuffer->AddRef(); }

    DX10VertexBuffer(DX10VertexBuffer&& move) noexcept
        : IVertexBuffer(::std::move(move))
        , _d3dBuffer(move._d3dBuffer)
        , _currentMapping(move._currentMapping)
    { _d3dBuffer = null; }

    DX10VertexBuffer& operator=(const DX10VertexBuffer& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        IVertexBuffer::operator=(copy);

        _d3dBuffer = copy._d3dBuffer;
        _currentMapping = copy._currentMapping;

        _d3dBuffer->AddRef();

        return *this;
    }

    DX10VertexBuffer& operator=(DX10VertexBuffer&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        IVertexBuffer::operator=(::std::move(move));

        _d3dBuffer = move._d3dBuffer;
        _currentMapping = move._currentMapping;

        move._d3dBuffer = null;

        return *this;
    }

    [[nodiscard]]       ID3D10Buffer* d3dBuffer()       noexcept { return _d3dBuffer; }
    [[nodiscard]] const ID3D10Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(uSys offset, uSys size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL DX10IndexBuffer final : public IIndexBuffer
{
    INDEX_BUFFER_IMPL(DX10IndexBuffer);
public:
    [[nodiscard]] static DXGI_FORMAT dxIndexSize(EBuffer::IndexSize indexSize) noexcept;
private:
    DXGI_FORMAT _dxIndexSize;
    ID3D10Buffer* _d3dBuffer;
    void* _currentMapping;
public:
    DX10IndexBuffer(const EBuffer::UsageType usage, const EBuffer::IndexSize indexSize, const uSys bufferSize, DXGI_FORMAT dxIndexSize, ID3D10Buffer* const d3dBuffer) noexcept
        : IIndexBuffer(usage, indexSize, bufferSize)
        , _dxIndexSize(dxIndexSize)
        , _d3dBuffer(d3dBuffer)
        , _currentMapping(null)
    { }

    ~DX10IndexBuffer() noexcept
    { RELEASE_DX(_d3dBuffer); }

    DX10IndexBuffer(const DX10IndexBuffer& copy) noexcept
        : IIndexBuffer(copy)
        , _dxIndexSize(copy._dxIndexSize)
        , _d3dBuffer(copy._d3dBuffer)
        , _currentMapping(copy._currentMapping)
    { _d3dBuffer->AddRef(); }

    DX10IndexBuffer(DX10IndexBuffer&& move) noexcept
        : IIndexBuffer(::std::move(move))
        , _dxIndexSize(move._dxIndexSize)
        , _d3dBuffer(move._d3dBuffer)
        , _currentMapping(move._currentMapping)
    { _d3dBuffer = null; }

    DX10IndexBuffer& operator=(const DX10IndexBuffer& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        IIndexBuffer::operator=(copy);

        _dxIndexSize = copy._dxIndexSize;
        _d3dBuffer = copy._d3dBuffer;
        _currentMapping = copy._currentMapping;

        _d3dBuffer->AddRef();

        return *this;
    }

    DX10IndexBuffer& operator=(DX10IndexBuffer&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        IIndexBuffer::operator=(::std::move(move));

        _dxIndexSize = move._dxIndexSize;
        _d3dBuffer = move._d3dBuffer;
        _currentMapping = move._currentMapping;

        move._d3dBuffer = null;

        return *this;
    }

    [[nodiscard]] DXGI_FORMAT dxIndexSize() const noexcept { return _dxIndexSize; }

    [[nodiscard]]       ID3D10Buffer* d3dBuffer()       noexcept { return _d3dBuffer; }
    [[nodiscard]] const ID3D10Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(uSys offset, uSys size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL DX10UniformBuffer final : public IUniformBuffer
{
    UNIFORM_BUFFER_IMPL(DX10UniformBuffer);
private:
    ID3D10Buffer* _d3dBuffer;
    void* _currentMapping;
public:
    DX10UniformBuffer(const EBuffer::UsageType usage, const uSys bufferSize, ID3D10Buffer* const d3dBuffer) noexcept
        : IUniformBuffer(usage, bufferSize)
        , _d3dBuffer(d3dBuffer)
        , _currentMapping(null)
    { }

    ~DX10UniformBuffer() noexcept
    { RELEASE_DX(_d3dBuffer); }

    DX10UniformBuffer(const DX10UniformBuffer& copy) noexcept
        : IUniformBuffer(copy)
        , _d3dBuffer(copy._d3dBuffer)
        , _currentMapping(copy._currentMapping)
    { _d3dBuffer->AddRef(); }

    DX10UniformBuffer(DX10UniformBuffer&& move) noexcept
        : IUniformBuffer(::std::move(move))
        , _d3dBuffer(move._d3dBuffer)
        , _currentMapping(move._currentMapping)
    { _d3dBuffer = null; }

    DX10UniformBuffer& operator=(const DX10UniformBuffer& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        IUniformBuffer::operator=(copy);

        _d3dBuffer = copy._d3dBuffer;
        _currentMapping = copy._currentMapping;

        _d3dBuffer->AddRef();

        return *this;
    }

    DX10UniformBuffer& operator=(DX10UniformBuffer&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        IUniformBuffer::operator=(::std::move(move));

        _d3dBuffer = move._d3dBuffer;
        _currentMapping = move._currentMapping;

        move._d3dBuffer = null;

        return *this;
    }

    [[nodiscard]]       ID3D10Buffer* d3dBuffer()       noexcept { return _d3dBuffer; }
    [[nodiscard]] const ID3D10Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }

    void bind(IRenderingContext& context, EShader::Stage stage, u32 index) const noexcept override;
    void unbind(IRenderingContext& context, EShader::Stage stage, u32 index) const noexcept override;
    void fastUnbind() const noexcept override;

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(uSys offset, uSys size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL DX10BufferBuilder final : public IBufferBuilder
{
    DEFAULT_DESTRUCT(DX10BufferBuilder);
    DEFAULT_CM_PU(DX10BufferBuilder);
public:
    struct DXIndexBufferArgs final
    {
        DEFAULT_CONSTRUCT_PU(DXIndexBufferArgs);
        DEFAULT_DESTRUCT(DXIndexBufferArgs);
        DEFAULT_CM_PU(DXIndexBufferArgs);

        DXGI_FORMAT indexSize;
        ID3D10Buffer* d3dBuffer;
    };
private:
    DX10GraphicsInterface& _gi;
public:
    inline DX10BufferBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10VertexBuffer* build(const VertexBufferArgs & args, [[tau::out]] Error * error) const noexcept override;
    [[nodiscard]] DX10VertexBuffer* build(const VertexBufferArgs & args, [[tau::out]] Error * error, TauAllocator & allocator) const noexcept override;
    [[nodiscard]] CPPRef<IVertexBuffer> buildCPPRef(const VertexBufferArgs & args, [[tau::out]] Error * error) const noexcept override;
    [[nodiscard]] NullableRef<IVertexBuffer> buildTauRef(const VertexBufferArgs & args, [[tau::out]] Error * error, TauAllocator & allocator = DefaultTauAllocator::Instance()) const noexcept override;
    [[nodiscard]] NullableStrongRef<IVertexBuffer> buildTauSRef(const VertexBufferArgs & args, [[tau::out]] Error * error, TauAllocator & allocator = DefaultTauAllocator::Instance()) const noexcept override;

    [[nodiscard]] DX10IndexBuffer* build(const IndexBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10IndexBuffer* build(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IIndexBuffer> buildCPPRef(const IndexBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IIndexBuffer> buildTauRef(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
    [[nodiscard]] NullableStrongRef<IIndexBuffer> buildTauSRef(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;

    [[nodiscard]] DX10UniformBuffer* build(const UniformBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX10UniformBuffer* build(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IUniformBuffer> buildCPPRef(const UniformBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IUniformBuffer> buildTauRef(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
    [[nodiscard]] NullableStrongRef<IUniformBuffer> buildTauSRef(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const VertexBufferArgs& args, [[tau::out]] ID3D10Buffer** d3dBuffer, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const IndexBufferArgs& args, [[tau::out]] DXIndexBufferArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const UniformBufferArgs& args, [[tau::out]] ID3D10Buffer** d3dBuffer, [[tau::out]] Error* error) const noexcept;
};
#endif
