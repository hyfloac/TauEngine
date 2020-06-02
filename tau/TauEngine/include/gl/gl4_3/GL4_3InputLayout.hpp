#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "gl/GLInputLayout.hpp"

struct GL4_3InputLayoutArgs final
{
    DELETE_COPY(GL4_3InputLayoutArgs);
public:
    GLuint vao;
    GLintptr* offsets;
    GLsizei* strides;

    GL4_3InputLayoutArgs() noexcept
        : vao(0)
        , offsets(nullptr)
        , strides(nullptr)
    { }

    ~GL4_3InputLayoutArgs() noexcept
    {
        delete[] strides;
        delete[] offsets;
    }
};

class TAU_DLL GL4_3InputLayout final : public GLInputLayout
{
private:
    // This is the OpenGL 4.3 separate VAO.
    GLuint _vao;
    GLintptr* _offsets;
    GLsizei* _strides;
public:
    GL4_3InputLayout(const GL4_3InputLayoutArgs& glArgs) noexcept
        : _vao(glArgs.vao)
        , _offsets(glArgs.offsets)
        , _strides(glArgs.strides)
    { }

    void bind(IRenderingContext& context) noexcept override;
    void unbind(IRenderingContext& context) noexcept override;
};

class TAU_DLL GL4_3InputLayoutBuilder final : public GLInputLayoutBuilder
{
    DEFAULT_CONSTRUCT_PU(GL4_3InputLayoutBuilder);
    DEFAULT_DESTRUCT(GL4_3InputLayoutBuilder);
    DELETE_COPY(GL4_3InputLayoutBuilder);
public:
    static GLenum getGLType(ShaderDataType::Type type) noexcept;
public:
    [[nodiscard]] GL4_3InputLayout* build(const InputLayoutArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] GL4_3InputLayout* build(const InputLayoutArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] CPPRef<IInputLayout> buildCPPRef(const InputLayoutArgs& args, [[tau::out]] Error* error) noexcept override;
    [[nodiscard]] NullableRef<IInputLayout> buildTauRef(const InputLayoutArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
    [[nodiscard]] NullableStrongRef<IInputLayout> buildTauSRef(const InputLayoutArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) noexcept override;
private:
    [[nodiscard]] bool processArgs(const InputLayoutArgs& args, GL4_3InputLayoutArgs* glArgs, [[tau::out]] Error* error) const noexcept;
};
