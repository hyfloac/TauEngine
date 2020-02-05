#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include "gl/GLBuffer.hpp"

class TAU_DLL GLBuffer4_5Builder final : public GLBufferBuilder
{
    DEFAULT_CONSTRUCT_PU(GLBuffer4_5Builder);
    DEFAULT_DESTRUCT(GLBuffer4_5Builder);
    DELETE_COPY(GLBuffer4_5Builder);
protected:
    [[nodiscard]] GLuint createBuffer() const noexcept override;
};

class TAU_DLL GLIndexBuffer4_5Builder final : public GLIndexBufferBuilder
{
    DEFAULT_CONSTRUCT_PU(GLIndexBuffer4_5Builder);
    DEFAULT_DESTRUCT(GLIndexBuffer4_5Builder);
    DELETE_COPY(GLIndexBuffer4_5Builder);
protected:
    [[nodiscard]] GLuint createBuffer() const noexcept override;
};

class TAU_DLL GLUniformBuffer4_5Builder final : public GLUniformBufferBuilder
{
    DEFAULT_CONSTRUCT_PU(GLUniformBuffer4_5Builder);
    DEFAULT_DESTRUCT(GLUniformBuffer4_5Builder);
    DELETE_COPY(GLUniformBuffer4_5Builder);
protected:
    [[nodiscard]] GLuint createBuffer() const noexcept override;
};
