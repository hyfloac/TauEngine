#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "model/Buffer.hpp"
#include "Safeties.hpp"

struct GLBufferArgs final
{
    DEFAULT_CONSTRUCT_PU(GLBufferArgs);
    DEFAULT_DESTRUCT(GLBufferArgs);
    DEFAULT_COPY(GLBufferArgs);

    GLenum glUsage;
    GLuint buffer;
};

struct GLIndexBufferArgs final
{
    DEFAULT_CONSTRUCT_PU(GLIndexBufferArgs);
    DEFAULT_DESTRUCT(GLIndexBufferArgs);
    DEFAULT_COPY(GLIndexBufferArgs);

    GLenum glUsage;
    GLenum glIndexSize;
    GLuint buffer;
};

class TAU_DLL GLVertexBuffer final : public IVertexBuffer
{
    VERTEX_BUFFER_IMPL(GLVertexBuffer);
public:
    static GLenum getGLUsageType(EBuffer::UsageType usage) noexcept;
    static EBuffer::UsageType getUsageType(GLenum usage) noexcept;
private:
    GLenum _glUsage;
    GLuint _buffer;
public:
    GLVertexBuffer(const EBuffer::UsageType usage, const uSys bufferSize, const BufferDescriptor& descriptor, const GLBufferArgs& glArgs) noexcept
        : IVertexBuffer(usage, bufferSize, descriptor)
        , _glUsage(glArgs.glUsage)
        , _buffer(glArgs.buffer)
    { }

    ~GLVertexBuffer() noexcept override;

    [[nodiscard]] GLuint buffer() const noexcept { return _buffer; }

    void bind() const noexcept;
    void unbind() const noexcept;

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(uSys offset, uSys size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL GLIndexBuffer final : public IIndexBuffer
{
    INDEX_BUFFER_IMPL(GLIndexBuffer);
public:
    static GLenum glIndexSize(EBuffer::IndexSize indexSize) noexcept;
private:
    GLenum _glUsage;
    GLenum _glIndexSize;
    GLuint _buffer;
public:
    GLIndexBuffer(const EBuffer::UsageType usage, const EBuffer::IndexSize indexSize, const uSys bufferSize, const GLIndexBufferArgs& glArgs) noexcept
        : IIndexBuffer(usage, indexSize, bufferSize)
        , _glUsage(glArgs.glUsage)
        , _glIndexSize(glArgs.glIndexSize)
        , _buffer(glArgs.buffer)
    { }

    ~GLIndexBuffer() noexcept override;

    [[nodiscard]] GLuint      buffer() const noexcept { return _buffer;      }
    [[nodiscard]] GLenum glIndexSize() const noexcept { return _glIndexSize; }

    void bind() const noexcept;
    void unbind() const noexcept;

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(uSys offset, uSys size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
private:
    friend class GLIndexBufferBuilder;
};

class TAU_DLL GLUniformBuffer final : public IUniformBuffer
{
    UNIFORM_BUFFER_IMPL(GLUniformBuffer);
private:
    GLenum _glUsage;
    GLuint _buffer;
public:
    GLUniformBuffer(const EBuffer::UsageType usage, const uSys bufferSize, const GLBufferArgs& glArgs) noexcept
        : IUniformBuffer(usage, bufferSize)
        , _glUsage(glArgs.glUsage)
        , _buffer(glArgs.buffer)
    { }

    ~GLUniformBuffer() noexcept override;

    [[nodiscard]] GLuint buffer() const noexcept { return _buffer; }

    void bind() const noexcept;
    void unbind() const noexcept;

    void bind(u32 index) const noexcept;
    void unbind(u32 index) const noexcept;

    void bind(IRenderingContext& context, EShader::Stage stage, u32 index) noexcept override;
    void unbind(IRenderingContext& context, EShader::Stage stage, u32 index) noexcept override;
    void fastUnbind() noexcept override;

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(uSys offset, uSys size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
private:
    friend class GLUniformBufferBuilder;
};

class TAU_DLL GLBufInterface
{
    DEFAULT_CONSTRUCT_PU(GLBufInterface);
    DEFAULT_DESTRUCT_VI(GLBufInterface);
    DELETE_COPY(GLBufInterface);
public:
    [[nodiscard]] virtual GLuint createBuffer() const noexcept;
};

class TAU_DLL GLBufferBuilder final : public IBufferBuilder
{
    DEFAULT_DESTRUCT(GLBufferBuilder);
    DELETE_COPY(GLBufferBuilder);
private:
    GLBufInterface& _glInterface;
public:
    inline GLBufferBuilder(GLBufInterface& glInterface) noexcept
        : _glInterface(glInterface)
    { }

    [[nodiscard]] GLVertexBuffer* build(const VertexBufferArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLVertexBuffer* build(const VertexBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IVertexBuffer> buildCPPRef(const VertexBufferArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IVertexBuffer> buildTauRef(const VertexBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IVertexBuffer> buildTauSRef(const VertexBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] GLIndexBuffer* build(const IndexBufferArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLIndexBuffer* build(const IndexBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IIndexBuffer> buildCPPRef(const IndexBufferArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IIndexBuffer> buildTauRef(const IndexBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IIndexBuffer> buildTauSRef(const IndexBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] GLUniformBuffer* build(const UniformBufferArgs& args, Error* error) const noexcept override;
    [[nodiscard]] GLUniformBuffer* build(const UniformBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IUniformBuffer> buildCPPRef(const UniformBufferArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IUniformBuffer> buildTauRef(const UniformBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IUniformBuffer> buildTauSRef(const UniformBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const VertexBufferArgs& args, [[tau::out]] GLBufferArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const IndexBufferArgs& args, [[tau::out]] GLIndexBufferArgs* glArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const UniformBufferArgs& args, [[tau::out]] GLBufferArgs* glArgs, [[tau::out]] Error* error) const noexcept;

    static void initBuffer(const VertexBufferArgs& args, const GLBufferArgs& glArgs) noexcept;
    static void initBuffer(const IndexBufferArgs& args, const GLIndexBufferArgs& glArgs) noexcept;
    static void initBuffer(const UniformBufferArgs& args, const GLBufferArgs& glArgs) noexcept;
};
