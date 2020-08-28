#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "graphics/VertexArray.hpp"
#include "graphics/BufferDescriptor.hpp"

class GLIndexBuffer;

class TAU_DLL GLVertexArray final : public IVertexArray
{
    DELETE_CM(GLVertexArray);
    VERTEX_ARRAY_IMPL(GLVertexArray);
public:
    static GLenum getGLType(ShaderDataType::Type type) noexcept;
private:
    GLuint _vao;
public:
    GLVertexArray(const DynArray<NullableRef<IResource>>& buffers, const GLuint vao)
        : IVertexArray(buffers)
        , _vao(vao)
    { }
    
    GLVertexArray(DynArray<NullableRef<IResource>>&& buffers, const GLuint vao)
        : IVertexArray(::std::move(buffers))
        , _vao(vao)
    { }

    ~GLVertexArray() noexcept override
    { glDeleteVertexArrays(1, &_vao); }

    [[nodiscard]] GLuint vao() const noexcept { return _vao; }
};

class TAU_DLL GLVertexArrayBuilder : public IVertexArrayBuilder
{
    DEFAULT_CONSTRUCT_PU(GLVertexArrayBuilder);
    DEFAULT_DESTRUCT(GLVertexArrayBuilder);
    DEFAULT_CM_PU(GLVertexArrayBuilder);
public:
    struct GLVertexArrayArgs final
    {
        GLuint vao;
        GLuint attribCount;
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
