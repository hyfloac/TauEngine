#pragma once

#include "graphics/Buffer.hpp"

#ifdef _WIN32
#include <d3d11.h>
#include "dx/DXUtils.hpp"

class DX11GraphicsInterface;
class DX11RenderingContext;

class TAU_DLL DX11VertexBuffer final : public IVertexBuffer
{
    VERTEX_BUFFER_IMPL(DX11VertexBuffer);
public:
    [[nodiscard]] static D3D11_USAGE getDXUsage(EBuffer::UsageType usage) noexcept;
    [[nodiscard]] static D3D11_CPU_ACCESS_FLAG getDXAccess(EBuffer::UsageType usage) noexcept;
    [[nodiscard]] static bool canReWrite(EBuffer::UsageType usage) noexcept;
private:
    ID3D11Buffer* _d3dBuffer;
    void* _currentMapping;
public:
    DX11VertexBuffer(const EBuffer::UsageType usage, const uSys bufferSize, const BufferDescriptor& descriptor, ID3D11Buffer* const d3dBuffer) noexcept
        : IVertexBuffer(usage, bufferSize, descriptor)
        , _d3dBuffer(d3dBuffer)
        , _currentMapping(null)
    { }

    ~DX11VertexBuffer() noexcept
    { RELEASE_DX(_d3dBuffer); }

    DX11VertexBuffer(const DX11VertexBuffer& copy) noexcept
        : IVertexBuffer(copy)
        , _d3dBuffer(copy._d3dBuffer)
        , _currentMapping(copy._currentMapping)
    { _d3dBuffer->AddRef(); }

    DX11VertexBuffer(DX11VertexBuffer&& move) noexcept
        : IVertexBuffer(::std::move(move))
        , _d3dBuffer(move._d3dBuffer)
        , _currentMapping(move._currentMapping)
    { move._d3dBuffer = null; }

    DX11VertexBuffer& operator=(const DX11VertexBuffer& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dBuffer);

        IVertexBuffer::operator=(copy);

        _d3dBuffer = copy._d3dBuffer;
        _currentMapping = copy._currentMapping;

        _d3dBuffer->AddRef();

        return *this;
    }

    DX11VertexBuffer& operator=(DX11VertexBuffer&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dBuffer);

        IVertexBuffer::operator=(::std::move(move));

        _d3dBuffer = move._d3dBuffer;
        _currentMapping = move._currentMapping;

        move._d3dBuffer = null;

        return *this;
    }

    [[nodiscard]]       ID3D11Buffer* d3dBuffer()       noexcept { return _d3dBuffer; }
    [[nodiscard]] const ID3D11Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(uSys offset, uSys size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL DX11IndexBuffer final : public IIndexBuffer
{
    INDEX_BUFFER_IMPL(DX11IndexBuffer);
public:
    [[nodiscard]] static DXGI_FORMAT dxIndexSize(EBuffer::IndexSize indexSize) noexcept;
private:
    DXGI_FORMAT _dxIndexSize;
    ID3D11Buffer* _d3dBuffer;
    void* _currentMapping;
public:
    DX11IndexBuffer(const EBuffer::UsageType usage, const EBuffer::IndexSize indexSize, const uSys bufferSize, DXGI_FORMAT dxIndexSize, ID3D11Buffer* const d3dBuffer) noexcept
        : IIndexBuffer(usage, indexSize, bufferSize)
        , _dxIndexSize(dxIndexSize)
        , _d3dBuffer(d3dBuffer)
        , _currentMapping(null)
    { }

    ~DX11IndexBuffer() noexcept
    { RELEASE_DX(_d3dBuffer); }

    DX11IndexBuffer(const DX11IndexBuffer& copy) noexcept
        : IIndexBuffer(copy)
        , _dxIndexSize(copy._dxIndexSize)
        , _d3dBuffer(copy._d3dBuffer)
        , _currentMapping(copy._currentMapping)
    { _d3dBuffer->AddRef(); }

    DX11IndexBuffer(DX11IndexBuffer&& move) noexcept
        : IIndexBuffer(::std::move(move))
        , _dxIndexSize(move._dxIndexSize)
        , _d3dBuffer(move._d3dBuffer)
        , _currentMapping(move._currentMapping)
    { move._d3dBuffer = null; }

    DX11IndexBuffer& operator=(const DX11IndexBuffer& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dBuffer);

        IIndexBuffer::operator=(copy);

        _dxIndexSize = copy._dxIndexSize;
        _d3dBuffer = copy._d3dBuffer;
        _currentMapping = copy._currentMapping;

        _d3dBuffer->AddRef();

        return *this;
    }

    DX11IndexBuffer& operator=(DX11IndexBuffer&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dBuffer);

        IIndexBuffer::operator=(::std::move(move));

        _dxIndexSize = move._dxIndexSize;
        _d3dBuffer = move._d3dBuffer;
        _currentMapping = move._currentMapping;

        move._d3dBuffer = null;

        return *this;
    }

    [[nodiscard]] DXGI_FORMAT dxIndexSize() const noexcept { return _dxIndexSize; }

    [[nodiscard]]       ID3D11Buffer* d3dBuffer()       noexcept { return _d3dBuffer; }
    [[nodiscard]] const ID3D11Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(uSys offset, uSys size, const void* data) noexcept override;
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
    { RELEASE_DX(_d3dBuffer); }

    DX11UniformBuffer(const DX11UniformBuffer& copy) noexcept
        : IUniformBuffer(copy)
        , _d3dBuffer(copy._d3dBuffer)
        , _currentMapping(copy._currentMapping)
    { _d3dBuffer->AddRef(); }

    DX11UniformBuffer(DX11UniformBuffer&& move) noexcept
        : IUniformBuffer(::std::move(move))
        , _d3dBuffer(move._d3dBuffer)
        , _currentMapping(move._currentMapping)
    { move._d3dBuffer = null; }

    DX11UniformBuffer& operator=(const DX11UniformBuffer& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dBuffer);

        IUniformBuffer::operator=(copy);

        _d3dBuffer = copy._d3dBuffer;
        _currentMapping = copy._currentMapping;

        _d3dBuffer->AddRef();

        return *this;
    }

    DX11UniformBuffer& operator=(DX11UniformBuffer&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dBuffer);

        IUniformBuffer::operator=(::std::move(move));

        _d3dBuffer = move._d3dBuffer;
        _currentMapping = move._currentMapping;

        move._d3dBuffer = null;

        return *this;
    }

    [[nodiscard]]       ID3D11Buffer* d3dBuffer()       noexcept { return _d3dBuffer; }
    [[nodiscard]] const ID3D11Buffer* d3dBuffer() const noexcept { return _d3dBuffer; }

    void bind(IRenderingContext& context, EShader::Stage stage, u32 index) const noexcept override;
    void unbind(IRenderingContext& context, EShader::Stage, u32 index) const noexcept override;
    void fastUnbind() const noexcept override;

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(uSys offset, uSys size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL DX11BufferBuilder final : public IBufferBuilder
{
    DEFAULT_DESTRUCT(DX11BufferBuilder);
    DEFAULT_CM_PU(DX11BufferBuilder);
public:
    struct DXIndexBufferArgs final
    {
        DEFAULT_CONSTRUCT_PU(DXIndexBufferArgs);
        DEFAULT_DESTRUCT(DXIndexBufferArgs);
        DEFAULT_COPY(DXIndexBufferArgs);

        DXGI_FORMAT indexSize;
        ID3D11Buffer* d3dBuffer;
    };
private:
    DX11GraphicsInterface& _gi;
public:
    inline DX11BufferBuilder(DX11GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX11VertexBuffer* build(const VertexBufferArgs & args, [[tau::out]] Error * error) const noexcept override;
    [[nodiscard]] DX11VertexBuffer* build(const VertexBufferArgs & args, [[tau::out]] Error * error, TauAllocator & allocator) const noexcept override;
    [[nodiscard]] CPPRef<IVertexBuffer> buildCPPRef(const VertexBufferArgs & args, [[tau::out]] Error * error) const noexcept override;
    [[nodiscard]] NullableRef<IVertexBuffer> buildTauRef(const VertexBufferArgs & args, [[tau::out]] Error * error, TauAllocator & allocator = DefaultTauAllocator::Instance()) const noexcept override;
    [[nodiscard]] NullableStrongRef<IVertexBuffer> buildTauSRef(const VertexBufferArgs & args, [[tau::out]] Error * error, TauAllocator & allocator = DefaultTauAllocator::Instance()) const noexcept override;

    [[nodiscard]] DX11IndexBuffer* build(const IndexBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11IndexBuffer* build(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IIndexBuffer> buildCPPRef(const IndexBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IIndexBuffer> buildTauRef(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
    [[nodiscard]] NullableStrongRef<IIndexBuffer> buildTauSRef(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;

    [[nodiscard]] DX11UniformBuffer* build(const UniformBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11UniformBuffer* build(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IUniformBuffer> buildCPPRef(const UniformBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IUniformBuffer> buildTauRef(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
    [[nodiscard]] NullableStrongRef<IUniformBuffer> buildTauSRef(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const VertexBufferArgs& args, [[tau::out]] ID3D11Buffer** d3dBuffer, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const IndexBufferArgs& args, [[tau::out]] DXIndexBufferArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const UniformBufferArgs& args, [[tau::out]] ID3D11Buffer** d3dBuffer, [[tau::out]] Error* error) const noexcept;
};
#endif
