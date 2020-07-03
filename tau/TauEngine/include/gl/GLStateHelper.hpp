#pragma once

#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)

#include <Objects.hpp>
#include <NumTypes.hpp>

#include "DLL.hpp"

namespace GLState {
struct BufferBaseBinding final
{
    GLuint buffer;
    GLintptr offset;
    GLsizeiptr size;
};

struct TextureBinding final
{
    GLenum target;
    GLuint texture;
};
}

class TAU_DLL GLStateHelper final
{
    DELETE_CM(GLStateHelper);
private:
    static GLStateHelper _globalInstance;
public:
    static GLStateHelper& Instance() noexcept
    { return _globalInstance; }
private:
    GLuint _vao;

    GLuint _arrayBuffer;
    GLuint _elementArrayBuffer;
    GLuint _uniformBuffer;
    GLuint _shaderStorageBuffer;

    GLuint _maxUniformBufferBindings;
    GLuint _maxShaderStorageBufferBindings;
    GLuint _maxTextureBindings;

    GLState::BufferBaseBinding* _uniformBufferBaseBindings;
    GLState::BufferBaseBinding* _shaderStorageBufferBaseBindings;

    GLState::TextureBinding* _textureBindings;
    uSys _activeTextureUnit;
public:
    GLStateHelper() noexcept;

    ~GLStateHelper() noexcept;

    [[nodiscard]] GLuint maxUniformBufferBindings() const noexcept { return _maxUniformBufferBindings; }
    [[nodiscard]] GLuint maxShaderStorageBufferBindings() const noexcept { return _maxShaderStorageBufferBindings; }
    [[nodiscard]] GLuint maxTextureBindings() const noexcept { return _maxTextureBindings; }

    void bindVertexArray(GLuint vao) noexcept;

    void bindArrayBuffer(GLuint buffer) noexcept;
    void bindElementArrayBuffer(GLuint buffer) noexcept;
    void bindUniformBuffer(GLuint buffer) noexcept;
    void bindShaderStorageBuffer(GLuint buffer) noexcept;

    void bindBuffer(GLenum target, GLuint buffer) noexcept;

    void bindUniformBufferBase(GLuint index, GLuint buffer) noexcept;
    void bindShaderStorageBufferBase(GLuint index, GLuint buffer) noexcept;

    void bindUniformBufferRange(GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) noexcept;
    void bindShaderStorageBufferRange(GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) noexcept;

    void bindBufferBase(GLenum target, GLuint index, GLuint buffer) noexcept;
    void bindBufferRange(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size) noexcept;

    void activeTexture(uSys unit) noexcept;

    void bindTexture(GLenum target, GLuint texture) noexcept;
    void bindTexture(GLenum target, GLuint texture, uSys unit) noexcept;
};
