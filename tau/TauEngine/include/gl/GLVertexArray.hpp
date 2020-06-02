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
    static GLenum getGLType(ShaderDataType::Type type) noexcept;
private:
    GLuint _vao;
    GLenum _glDrawType;
    NullableRef<GLIndexBuffer> _indexBuffer;
public:
    GLVertexArray(const u32 drawCount, const DynArray<NullableRef<IVertexBuffer>>& buffers, const GLuint vao, const GLenum drawType, const NullableRef<GLIndexBuffer>& indexBuffer)
        : IVertexArray(drawCount, buffers)
        , _vao(vao)
        , _glDrawType(drawType)
        , _indexBuffer(indexBuffer)
    { }

    ~GLVertexArray() noexcept override
    { glDeleteVertexArrays(1, &_vao); }

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;

    void draw(IRenderingContext& context, uSys drawCount = 0, uSys drawOffset = 0) noexcept override;
    void drawInstanced(IRenderingContext& context, uSys instanceCount, uSys drawCount = 0, uSys drawOffset = 0) noexcept override;
};

class GLRenderingContext;

class TAU_DLL GLVertexArrayBuilder : public IVertexArrayBuilder
{
    DEFAULT_CONSTRUCT_PU(GLVertexArrayBuilder);
    DEFAULT_DESTRUCT(GLVertexArrayBuilder);
    DELETE_COPY(GLVertexArrayBuilder);
public:
    struct GLVertexArrayArgs final
    {
        GLuint vao;
        GLuint attribCount;
        NullableRef<GLIndexBuffer> indexBuffer;
        GLenum drawType;
    };
public:
    [[nodiscard]] IVertexArray* build(const VertexArrayArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] IVertexArray* build(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IVertexArray> buildCPPRef(const VertexArrayArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] NullableRef<IVertexArray> buildTauRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] NullableStrongRef<IVertexArray> buildTauSRef(const VertexArrayArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
private:
    [[nodiscard]] bool processArgs(const VertexArrayArgs& args, GLVertexArrayArgs* glArgs, [[tau::out]] Error* error) const noexcept;
};
