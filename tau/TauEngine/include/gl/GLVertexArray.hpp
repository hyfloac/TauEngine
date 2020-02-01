#pragma once

#include "model/VertexArray.hpp"
#include <GL/glew.h>
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
    // RefDynArray<Ref<IBuffer>> _buffers;
    Ref<GLIndexBuffer> _indexBuffer;
    GLuint _attribCount;
public:
    GLVertexArray(u32 drawCount, const RefDynArray<Ref<IBuffer>>& buffers, GLuint vao, DrawType drawType, /*const RefDynArray<Ref<IBuffer>>& buffers,*/ const Ref<GLIndexBuffer>& indexBuffer, GLuint attribCount);

    ~GLVertexArray() noexcept override;

    void bind(IRenderingContext& context) noexcept override;

    void unbind(IRenderingContext& context) noexcept override;

    // void internalSetup(IRenderingContext& context) noexcept override;

    void preDraw(IRenderingContext& context) noexcept override;

    void postDraw(IRenderingContext& context) noexcept override;

    void draw(IRenderingContext& context) noexcept override;

    void drawInstanced(IRenderingContext& context, uSys instanceCount) noexcept override;
};

class GLRenderingContext;

class TAU_DLL GLVertexArrayBuilder final : public IVertexArrayBuilder
{
    DEFAULT_DESTRUCT(GLVertexArrayBuilder);
    DELETE_COPY(GLVertexArrayBuilder);
private:
    GLRenderingContext& _ctx;
public:
    GLVertexArrayBuilder(const uSys bufferCount, GLRenderingContext& ctx) noexcept
        : IVertexArrayBuilder(bufferCount), _ctx(ctx)
    { }

    void setVertexBuffer(uSys index, const Ref<IBuffer>& vertexBuffer) noexcept override;
    void indexBuffer(const Ref<IIndexBuffer>& indexBuffer) noexcept override;
    // void inputLayout(const Ref<IInputLayout>& inputLayout) noexcept override;

    [[nodiscard]] GLVertexArray* build([[tau::out]] Error* error) noexcept override;
};
