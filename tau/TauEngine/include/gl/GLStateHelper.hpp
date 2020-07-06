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

struct BlendingControl final
{
    bool active;
    GLenum srcColor;
    GLenum srcAlpha;
    GLenum destColor;
    GLenum destAlpha;
    GLenum equationColor;
    GLenum equationAlpha;
};
}

class TAU_DLL GLStateHelper final
{
    DELETE_CM(GLStateHelper);
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

    GLuint _activeProgram;

    GLState::BlendingControl _blendingControls[8];
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

    void bindShaderProgram(GLuint program) noexcept;

    void setBlending(bool state) noexcept;
    void setBlending(uSys index, bool state) noexcept;

    void enableBlending() noexcept;
    void enableBlending(uSys index) noexcept;

    void disableBlending() noexcept;
    void disableBlending(uSys index) noexcept;

    void set(GLenum capability, bool state) noexcept;
    void set(GLenum capability, uSys index, bool state) noexcept;

    void enable(GLenum capability) noexcept;
    void enable(GLenum capability, uSys index) noexcept;

    void disable(GLenum capability) noexcept;
    void disable(GLenum capability, uSys index) noexcept;

    void blendFunc(GLenum src, GLenum dest) noexcept;
    void blendFunc(GLenum srcColor, GLenum srcAlpha, GLenum destColor, GLenum destAlpha) noexcept;

    void blendFunc(uSys index, GLenum src, GLenum dest) noexcept;
    void blendFunc(uSys index, GLenum srcColor, GLenum srcAlpha, GLenum destColor, GLenum destAlpha) noexcept;

    void blendEquation(GLenum equation) noexcept;
    void blendEquation(GLenum equationColor, GLenum equationAlpha) noexcept;

    void blendEquation(uSys index, GLenum equation) noexcept;
    void blendEquation(uSys index, GLenum equationColor, GLenum equationAlpha) noexcept;
};
