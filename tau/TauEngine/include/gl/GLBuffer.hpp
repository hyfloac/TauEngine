#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "model/Buffer.hpp"
#include "Safeties.hpp"

class TAU_DLL GLBuffer final : public IBuffer
{
    BUFFER_IMPL(GLBuffer);
public:
    static GLenum getGLType(EBuffer::Type bt) noexcept;
    static EBuffer::Type getType(GLenum bt) noexcept;
    static GLenum getGLUsageType(EBuffer::UsageType usage) noexcept;
    static EBuffer::UsageType getUsageType(GLenum usage) noexcept;
    static GLuint createBuffer() noexcept;
private:
    GLuint _buffer;
    GLenum _glType;
    GLenum _glUsage;
public:
    GLBuffer(EBuffer::Type type, EBuffer::UsageType usage, uSys bufferSize, bool instanced, const BufferDescriptor& descriptor, GLuint buffer, GLenum glType, GLenum glUsage) noexcept;

    ~GLBuffer() noexcept override;

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL GLIndexBuffer final : public IIndexBuffer
{
    INDEX_BUFFER_IMPL(GLIndexBuffer);
private:
    GLuint _buffer;
    GLenum _glUsage;
public:
    GLIndexBuffer(EBuffer::UsageType usage, uSys bufferSize, GLuint buffer, GLenum glUsage) noexcept;

    ~GLIndexBuffer() noexcept override;

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
private:
    friend class GLIndexBufferBuilder;
};

class TAU_DLL GLUniformBuffer final : public IUniformBuffer
{
    UNIFORM_BUFFER_IMPL(GLUniformBuffer);
private:
    GLuint _buffer;
    GLenum _glUsage;
public:
    GLUniformBuffer(EBuffer::UsageType usage, uSys bufferSize, GLuint buffer, GLenum glUsage) noexcept;

    ~GLUniformBuffer() noexcept override;

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    void bind(IRenderingContext& context, EShader::Stage stage, u32 index) noexcept override;
    void unbind(IRenderingContext& context, EShader::Stage stage, u32 index) noexcept override;

    bool beginModification(IRenderingContext& context) noexcept override;
    void endModification(IRenderingContext& context) noexcept override;

    void modifyBuffer(::std::intptr_t offset, ::std::ptrdiff_t size, const void* data) noexcept override;
    void fillBuffer(IRenderingContext& context, const void* data) noexcept override;
private:
    friend class GLUniformBufferBuilder;
};

class TAU_DLL GLBufferBuilder : public IBufferBuilder
{
    DEFAULT_CONSTRUCT_PU(GLBufferBuilder);
    DEFAULT_DESTRUCT(GLBufferBuilder);
    DELETE_COPY(GLBufferBuilder);
private:
    struct GLBufferArgs final
    {
        GLenum glType;
        GLenum glUsage;
        GLuint bufferHandle;
    };
public:
    [[nodiscard]] GLBuffer* build(const BufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLBuffer* build(const BufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IBuffer> buildCPPRef(const BufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableReferenceCountingPointer<IBuffer> buildTauRef(const BufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongReferenceCountingPointer<IBuffer> buildTauSRef(const BufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
protected:
    [[nodiscard]] virtual GLuint createBuffer() const noexcept
    { return GLBuffer::createBuffer(); }
private:
    [[nodiscard]] bool processArgs(const BufferArgs& args, [[tau::out]] GLBufferArgs* glArgs, [[tau::out]] Error* error) const noexcept;

    static void initBuffer(const BufferArgs& args, const GLBufferArgs& glArgs) noexcept;
};

class TAU_DLL GLIndexBufferBuilder : public IIndexBufferBuilder
{
    DEFAULT_CONSTRUCT_PU(GLIndexBufferBuilder);
    DEFAULT_DESTRUCT(GLIndexBufferBuilder);
    DELETE_COPY(GLIndexBufferBuilder);
private:
    struct GLIndexBufferArgs final
    {
        GLenum glUsage;
        GLuint bufferHandle;
    };
public:
    [[nodiscard]] GLIndexBuffer* build(const IndexBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLIndexBuffer* build(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IIndexBuffer> buildCPPRef(const IndexBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableReferenceCountingPointer<IIndexBuffer> buildTauRef(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
    [[nodiscard]] NullableStrongReferenceCountingPointer<IIndexBuffer> buildTauSRef(const IndexBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator = DefaultTauAllocator::Instance()) const noexcept override;
protected:
    [[nodiscard]] virtual GLuint createBuffer() const noexcept
    { return GLBuffer::createBuffer(); }
private:
    [[nodiscard]] bool processArgs(const IndexBufferArgs & args, [[tau::out]] GLIndexBufferArgs * glArgs, [[tau::out]] Error * error) const noexcept;

    static void initBuffer(const IndexBufferArgs & args, const GLIndexBufferArgs & glArgs) noexcept;
private:
    friend class GLUniformBufferBuilder;
};

class TAU_DLL GLUniformBufferBuilder : public IUniformBufferBuilder
{
    DEFAULT_CONSTRUCT_PU(GLUniformBufferBuilder);
    DEFAULT_DESTRUCT(GLUniformBufferBuilder);
    DELETE_COPY(GLUniformBufferBuilder);
private:
    using GLUniformBufferArgs = GLIndexBufferBuilder::GLIndexBufferArgs;
public:
    [[nodiscard]] GLUniformBuffer* build(const UniformBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] GLUniformBuffer* build(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IUniformBuffer> buildCPPRef(const UniformBufferArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableReferenceCountingPointer<IUniformBuffer> buildTauRef(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongReferenceCountingPointer<IUniformBuffer> buildTauSRef(const UniformBufferArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
protected:
    [[nodiscard]] virtual GLuint createBuffer() const noexcept
    { return GLBuffer::createBuffer(); }
private:
    [[nodiscard]] bool processArgs(const UniformBufferArgs& args, [[tau::out]] GLUniformBufferArgs* glArgs, [[tau::out]] Error* error) const noexcept;

    static void initBuffer(const UniformBufferArgs& args, const GLUniformBufferArgs& glArgs) noexcept;
};

