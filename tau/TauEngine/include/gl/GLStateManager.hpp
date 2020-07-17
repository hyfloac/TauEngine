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

struct DepthControl final
{
    bool active;
    GLboolean depthMask;
    GLenum depthCompareFunc;
};

struct StencilControl final
{
    GLuint writeMask;
    GLuint readMask;
    GLint reference;
    GLenum stencilFailOp;
    GLenum stencilDepthFailOp;
    GLenum stencilPassOp;
    GLenum compareFunc;
};
}

class TAU_DLL GLStateManager final
{
    DELETE_CM(GLStateManager);
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

    GLState::DepthControl _depthControl;
    bool _stencilTestEnabled;

    GLState::StencilControl _frontStencil;
    GLState::StencilControl _backStencil;
public:
    GLStateManager() noexcept;

    ~GLStateManager() noexcept;

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

    void setDepthTest(bool state) noexcept;
    void enableDepthTest() noexcept;
    void disableDepthTest() noexcept;

    void setStencilTest(bool state) noexcept;
    void enableStencilTest() noexcept;
    void disableStencilTest() noexcept;

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

    void depthMask(GLboolean depthMask) noexcept;
    void depthFunc(GLenum depthCompareFunc) noexcept;

    void stencilMaskFront(GLuint writeMask) noexcept;
    void stencilMaskBack(GLuint writeMask) noexcept;
    void stencilMaskFrontBack(GLuint writeMask) noexcept;

    void stencilMask(GLuint writeMask) noexcept;
    void stencilMask(GLenum face, GLuint writeMask) noexcept;

    void stencilOpFront(GLenum stencilFail, GLenum depthFail, GLenum pass) noexcept;
    void stencilOpBack(GLenum stencilFail, GLenum depthFail, GLenum pass) noexcept;
    void stencilOpFrontBack(GLenum stencilFail, GLenum depthFail, GLenum pass) noexcept;

    void stencilOp(GLenum stencilFail, GLenum depthFail, GLenum pass) noexcept;
    void stencilOp(GLenum face, GLenum stencilFail, GLenum depthFail, GLenum pass) noexcept;

    void stencilFuncFront(GLenum func, GLint reference, GLuint readMask) noexcept;
    void stencilFuncBack(GLenum func, GLint reference, GLuint readMask) noexcept;
    void stencilFuncFrontBack(GLenum func, GLint reference, GLuint readMask) noexcept;

    void stencilFunc(GLenum func, GLint reference, GLuint readMask) noexcept;
    void stencilFunc(GLenum face, GLenum func, GLint reference, GLuint readMask) noexcept;

    void stencilFuncFront(GLenum func, GLuint readMask) noexcept;
    void stencilFuncBack(GLenum func, GLuint readMask) noexcept;
    void stencilFuncFrontBack(GLenum func, GLuint readMask) noexcept;

    void stencilFunc(GLenum func, GLuint readMask) noexcept;
    void stencilFunc(GLenum face, GLenum func, GLuint readMask) noexcept;

    void stencilRefFront(GLint reference) noexcept;
    void stencilRefBack(GLint reference) noexcept;
    void stencilRefFrontBack(GLint reference) noexcept;

    void stencilRef(GLint reference) noexcept;
    void stencilRef(GLenum face, GLint reference) noexcept;
};
