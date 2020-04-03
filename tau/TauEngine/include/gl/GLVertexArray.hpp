#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "model/VertexArray.hpp"
#include "model/BufferDescriptor.hpp"

class GLIndexBuffer;

class TAU_DLL GLVertexArray final : public IVertexArray
{
    DELETE_COPY(GLVertexArray);
public:
    static GLuint generate() noexcept;

    static void _bind(GLuint vao) noexcept;

    static void destroy(GLuint vao) noexcept;
    
    static GLenum getGLType(ShaderDataType::Type type) noexcept;
private:
    GLuint _vao;
    GLenum _glDrawType;
    CPPRef<GLIndexBuffer> _indexBuffer;
    GLuint _attribCount;
public:
    GLVertexArray(u32 drawCount, const RefDynArray<CPPRef<IBuffer>>& buffers, GLuint vao, GLenum drawType, const CPPRef<GLIndexBuffer>& indexBuffer, GLuint attribCount);

    ~GLVertexArray() noexcept override;

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    void preDraw(IRenderingContext& context) noexcept override;
    void postDraw(IRenderingContext& context) noexcept override;

    void draw(IRenderingContext& context, uSys drawCount = 0, uSys drawOffset = 0) noexcept override;
    void drawInstanced(IRenderingContext& context, uSys instanceCount, uSys drawCount = 0, uSys drawOffset = 0) noexcept override;
};

class GLRenderingContext;

class TAU_DLL GLVertexArrayBuilder final : public IVertexArrayBuilder
{
    DEFAULT_CONSTRUCT_PU(GLVertexArrayBuilder);
    DEFAULT_DESTRUCT(GLVertexArrayBuilder);
    DELETE_COPY(GLVertexArrayBuilder);
public:
    struct GLVertexArrayArgs final
    {
        GLuint vao;
        GLuint attribCount;
        CPPRef<GLIndexBuffer> indexBuffer;
        GLenum drawType;
    };
public:
    [[nodiscard]] GLVertexArray* build(const VertexArrayArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] GLVertexArray* build(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IVertexArray> buildCPPRef(const VertexArrayArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] NullableRef<IVertexArray> buildTauRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] NullableStrongRef<IVertexArray> buildTauSRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
private:
    [[nodiscard]] bool processArgs(const VertexArrayArgs& args, GLVertexArrayArgs* glArgs, [[tau::out]] Error* error) const noexcept;
};
