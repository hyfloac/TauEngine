#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "gl/GLVertexArray.hpp"
#include "model/BufferDescriptor.hpp"

class GLIndexBuffer;

struct GL4_3VertexArrayArgs final
{
    DEFAULT_CONSTRUCT_PU(GL4_3VertexArrayArgs);
    DEFAULT_DESTRUCT(GL4_3VertexArrayArgs);
    DEFAULT_COPY(GL4_3VertexArrayArgs);
public:
    GLenum drawType;
    GLuint* buffers;
    NullableRef<GLIndexBuffer> indexBuffer;
};

class TAU_DLL GL4_3VertexArray final : public IVertexArray
{
    DEFAULT_DESTRUCT(GL4_3VertexArray);
    DELETE_COPY(GL4_3VertexArray);
public:
    static GLenum getGLType(ShaderDataType::Type type) noexcept;
private:
    GLenum _glDrawType;
    GLuint* _glBuffers;
    NullableRef<GLIndexBuffer> _indexBuffer;
public:
    GL4_3VertexArray(const VertexArrayArgs& args, const GL4_3VertexArrayArgs& glArgs)
        : IVertexArray(args.drawCount, args.buffers)
        , _glDrawType(glArgs.drawType)
        , _glBuffers(glArgs.buffers)
        , _indexBuffer(glArgs.indexBuffer)
    { }

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    void draw(IRenderingContext& context, uSys drawCount = 0, uSys drawOffset = 0) noexcept override;
    void drawInstanced(IRenderingContext& context, uSys instanceCount, uSys drawCount = 0, uSys drawOffset = 0) noexcept override;
};

class GLRenderingContext;

class TAU_DLL GL4_3VertexArrayBuilder final : public GLVertexArrayBuilder
{
    DEFAULT_CONSTRUCT_PU(GL4_3VertexArrayBuilder);
    DEFAULT_DESTRUCT(GL4_3VertexArrayBuilder);
    DELETE_COPY(GL4_3VertexArrayBuilder);
public:
    [[nodiscard]] GL4_3VertexArray* build(const VertexArrayArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] GL4_3VertexArray* build(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IVertexArray> buildCPPRef(const VertexArrayArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] NullableRef<IVertexArray> buildTauRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] NullableStrongRef<IVertexArray> buildTauSRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
private:
    [[nodiscard]] bool processArgs(const VertexArrayArgs& args, GL4_3VertexArrayArgs* glArgs, [[tau::out]] Error* error) const noexcept;
};
