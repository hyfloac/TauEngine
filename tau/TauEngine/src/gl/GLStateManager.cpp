#include "gl/GLStateManager.hpp"
#include <cstring>

GLStateManager::GLStateManager() noexcept
    : _vao(0)
    , _arrayBuffer(0)
    , _elementArrayBuffer(0)
    , _uniformBuffer(0)
    , _shaderStorageBuffer(0)
    , _maxUniformBufferBindings(0)
    , _maxShaderStorageBufferBindings(0)
    , _maxTextureBindings(0)
    , _uniformBufferBaseBindings(null)
    , _shaderStorageBufferBaseBindings(null)
    , _textureBindings(null)
    , _activeTextureUnit(0)
    , _activeProgram(0)
    , _depthControl({ false, true, GL_LESS })
    , _stencilTestEnabled(false)
    , _scissorTestEnabled(false)
    , _faceCullingEnabled(false)
    , _frontStencil({ 0xFFFFFFFF, 0xFFFFFFFF, 0, GL_KEEP, GL_KEEP, GL_KEEP, GL_ALWAYS })
    , _backStencil({ 0xFFFFFFFF, 0xFFFFFFFF, 0, GL_KEEP, GL_KEEP, GL_KEEP, GL_ALWAYS })
    , _polygonOffset({ { 0.0f }, { 0.0f }, 0.0f, null })
    , _frontFace(GL_CCW)
    , _faceCullingMode(GL_BACK)
    , _polygonFillMode(GL_FILL)
    , _clearColor{ 0.0f, 0.0f, 0.0f, 0.0f }
    , _clearDepthF(1.0f)
    , _clearDepthD(1.0)
    , _clearStencil(0)
{
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, reinterpret_cast<GLint*>(&_maxUniformBufferBindings));
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, reinterpret_cast<GLint*>(&_maxShaderStorageBufferBindings));
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, reinterpret_cast<GLint*>(&_maxTextureBindings));

    _uniformBufferBaseBindings = new GLState::BufferBaseBinding[_maxUniformBufferBindings];
    _shaderStorageBufferBaseBindings = new GLState::BufferBaseBinding[_maxShaderStorageBufferBindings];
    _textureBindings = new GLState::TextureBinding[_maxTextureBindings];

    for(uSys i = 0; i < 8; ++i)
    {
        _blendingControls[0].active = false;
        _blendingControls[0].srcColor = GL_ONE;
        _blendingControls[0].srcAlpha = GL_ZERO;
        _blendingControls[0].destColor = GL_ONE;
        _blendingControls[0].destAlpha = GL_ZERO;
        _blendingControls[0].equationColor = GL_FUNC_ADD;
        _blendingControls[0].equationAlpha = GL_FUNC_ADD;
    }

    if(GLEW_ARB_polygon_offset_clamp && glPolygonOffsetClamp)
    {
        _polygonOffset.extFunc = glPolygonOffsetClamp;
    }
    else if(GLEW_EXT_polygon_offset_clamp && glPolygonOffsetClampEXT)
    {
        _polygonOffset.extFunc = glPolygonOffsetClampEXT;
    }
    else
    {
        _polygonOffset.extFunc = _glPolygonOffsetFallback;
    }
}

GLStateManager::~GLStateManager() noexcept
{
    delete[] _uniformBufferBaseBindings;
    delete[] _shaderStorageBufferBaseBindings;
    delete[] _textureBindings;
}

void GLStateManager::bindVertexArray(const GLuint vao) noexcept
{
    if(vao != _vao)
    {
        _vao = vao;
        glBindVertexArray(vao);
    }
}

void GLStateManager::bindArrayBuffer(const GLuint buffer) noexcept
{
    if(buffer != _arrayBuffer)
    {
        _arrayBuffer = buffer;
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
    }
}

void GLStateManager::bindElementArrayBuffer(const GLuint buffer) noexcept
{
    if(buffer != _elementArrayBuffer)
    {
        _elementArrayBuffer = buffer;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    }
}

void GLStateManager::bindUniformBuffer(const GLuint buffer) noexcept
{
    if(buffer != _uniformBuffer)
    {
        _uniformBuffer = buffer;
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    }
}

void GLStateManager::bindShaderStorageBuffer(const GLuint buffer) noexcept
{
    if(buffer != _shaderStorageBuffer)
    {
        _shaderStorageBuffer = buffer;
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    }
}

void GLStateManager::bindBuffer(const GLenum target, const GLuint buffer) noexcept
{
    switch(target)
    {
        case GL_ARRAY_BUFFER:          bindArrayBuffer(buffer); break;
        case GL_ELEMENT_ARRAY_BUFFER:  bindElementArrayBuffer(buffer); break;
        case GL_UNIFORM_BUFFER:        bindUniformBuffer(buffer); break;
        case GL_SHADER_STORAGE_BUFFER: bindShaderStorageBuffer(buffer); break;
        default:                       glBindBuffer(target, buffer); break;
    }
}

void GLStateManager::bindUniformBufferBase(const GLuint index, const GLuint buffer) noexcept
{
    if(index > _maxUniformBufferBindings)
    { return; }

    GLState::BufferBaseBinding& binding = _uniformBufferBaseBindings[index];

    if(binding.buffer == buffer)
    {
        if(binding.offset != 0 || binding.size != 0)
        {
            binding.offset = 0;
            binding.size = 0;
            glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer);
        }
    }
    else
    {
        binding.buffer = buffer;
        binding.offset = 0;
        binding.size = 0;
        glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer);
    }
}

void GLStateManager::bindShaderStorageBufferBase(const GLuint index, const GLuint buffer) noexcept
{
    if(index > _maxShaderStorageBufferBindings)
    { return; }

    GLState::BufferBaseBinding& binding = _shaderStorageBufferBaseBindings[index];

    if(binding.buffer == buffer)
    {
        if(binding.offset != 0 || binding.size != 0)
        {
            binding.offset = 0;
            binding.size = 0;
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, buffer);
        }
    }
    else
    {
        binding.buffer = buffer;
        binding.offset = 0;
        binding.size = 0;
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, buffer);
    }
}

void GLStateManager::bindUniformBufferRange(const GLuint index, const GLuint buffer, const GLintptr offset, const GLsizeiptr size) noexcept
{
    if(index > _maxUniformBufferBindings)
    { return; }

    GLState::BufferBaseBinding& binding = _uniformBufferBaseBindings[index];

    if(binding.buffer == buffer)
    {
        if(binding.offset != offset || binding.size != size)
        {
            binding.offset = offset;
            binding.size = size;
            glBindBufferRange(GL_UNIFORM_BUFFER, index, buffer, offset, size);
        }
    }
    else
    {
        binding.buffer = buffer;
        binding.offset = offset;
        binding.size = size;
        glBindBufferRange(GL_UNIFORM_BUFFER, index, buffer, offset, size);
    }
}

void GLStateManager::bindShaderStorageBufferRange(const GLuint index, const GLuint buffer, const GLintptr offset, const GLsizeiptr size) noexcept
{
    if(index > _maxShaderStorageBufferBindings)
    { return; }

    GLState::BufferBaseBinding& binding = _shaderStorageBufferBaseBindings[index];

    if(binding.buffer == buffer)
    {
        if(binding.offset != offset || binding.size != size)
        {
            binding.offset = offset;
            binding.size = size;
            glBindBufferRange(GL_SHADER_STORAGE_BUFFER, index, buffer, offset, size);
        }
    }
    else
    {
        binding.buffer = buffer;
        binding.offset = offset;
        binding.size = size;
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, index, buffer, offset, size);
    }
}

void GLStateManager::bindBufferBase(const GLenum target, const GLuint index, const GLuint buffer) noexcept
{
    switch(target)
    {
        case GL_UNIFORM_BUFFER:        bindUniformBufferBase(index, buffer); break;
        case GL_SHADER_STORAGE_BUFFER: bindShaderStorageBufferBase(index, buffer); break;
        default:                       glBindBufferBase(target, index, buffer); break;
    }
}

void GLStateManager::bindBufferRange(const GLenum target, const GLuint index, const GLuint buffer, const GLintptr offset, const GLsizeiptr size) noexcept
{
    switch(target)
    {
        case GL_UNIFORM_BUFFER:        bindUniformBufferRange(index, buffer, offset, size); break;
        case GL_SHADER_STORAGE_BUFFER: bindShaderStorageBufferRange(index, buffer, offset, size); break;
        default:                       glBindBufferRange(target, index, buffer, offset, size); break;
    }
}

void GLStateManager::activeTexture(const uSys unit) noexcept
{
    if(unit > _maxTextureBindings)
    { return; }

    if(unit != _activeTextureUnit)
    {
        _activeTextureUnit = unit;
        glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + unit));
    }
}

void GLStateManager::bindTexture(const GLenum target, const GLuint texture) noexcept
{
    GLState::TextureBinding& binding = _textureBindings[_activeTextureUnit];

    if(target != binding.target || texture != binding.texture)
    {
        binding.target = target;
        binding.texture = texture;
        glBindTexture(target, texture);
    }
}

void GLStateManager::bindTexture(const GLenum target, const GLuint texture, const uSys unit) noexcept
{
    if(unit > _maxTextureBindings)
    { return; }

    GLState::TextureBinding& binding = _textureBindings[unit];

    if(target != binding.target || texture != binding.texture)
    {
        if(unit != _activeTextureUnit)
        {
            _activeTextureUnit = unit;
            glActiveTexture(static_cast<GLenum>(GL_TEXTURE0 + unit));
        }

        binding.target = target;
        binding.texture = texture;
        glBindTexture(target, texture);
    }
}

void GLStateManager::bindShaderProgram(const GLuint program) noexcept
{
    if(program != _activeProgram)
    {
        _activeProgram = program;
        glUseProgram(program);
    }
}

void GLStateManager::setBlending(const bool state) noexcept
{
    bool stateChangeRequired = false;
    for(uSys i = 0; i < 8; ++i)
    {
        if(_blendingControls[i].active != state)
        {
            stateChangeRequired = true;
            _blendingControls[i].active = state;
        }
    }

    if(stateChangeRequired)
    {
        if(state)
        {
            glEnable(GL_BLEND);
        }
        else
        {
            glDisable(GL_BLEND);
        }
    }
}

void GLStateManager::setBlending(const uSys index, const bool state) noexcept
{
    if(_blendingControls[index].active != state)
    {
        _blendingControls[index].active = state;
        if(state)
        {
            glEnablei(GL_BLEND, static_cast<GLuint>(index));
        }
        else
        {
            glDisablei(GL_BLEND, static_cast<GLuint>(index));
        }
    }
}

void GLStateManager::enableBlending() noexcept
{
    bool stateChangeRequired = false;
    for(uSys i = 0; i < 8; ++i)
    {
        if(_blendingControls[i].active != true)
        {
            stateChangeRequired = true;
            _blendingControls[i].active = true;
        }
    }

    if(stateChangeRequired)
    {
        glEnable(GL_BLEND);
    }
}

void GLStateManager::enableBlending(const uSys index) noexcept
{
    if(_blendingControls[index].active != true)
    {
        _blendingControls[index].active = true;
        glEnablei(GL_BLEND, static_cast<GLuint>(index));
    }
}

void GLStateManager::disableBlending() noexcept
{
    bool stateChangeRequired = false;
    for(uSys i = 0; i < 8; ++i)
    {
        if(_blendingControls[i].active != false)
        {
            stateChangeRequired = false;
            _blendingControls[i].active = false;
        }
    }

    if(stateChangeRequired)
    {
        glDisable(GL_BLEND);
    }
}

void GLStateManager::disableBlending(const uSys index) noexcept
{
    if(_blendingControls[index].active != false)
    {
        _blendingControls[index].active = false;
        glDisablei(GL_BLEND, static_cast<GLuint>(index));
    }
}

void GLStateManager::setDepthTest(const bool state) noexcept
{
    if(_depthControl.active != state)
    {
        _depthControl.active = state;
        if(state)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
    }
}

void GLStateManager::enableDepthTest() noexcept
{
    if(_depthControl.active != true)
    {
        _depthControl.active = true;
        glEnable(GL_DEPTH_TEST);
    }
}

void GLStateManager::disableDepthTest() noexcept
{
    if(_depthControl.active != false)
    {
        _depthControl.active = false;
        glDisable(GL_DEPTH_TEST);
    }
}

void GLStateManager::setStencilTest(const bool state) noexcept
{
    if(_stencilTestEnabled != state)
    {
        _stencilTestEnabled = state;
        if(state)
        {
            glEnable(GL_STENCIL_TEST);
        }
        else
        {
            glDisable(GL_STENCIL_TEST);
        }
    }
}

void GLStateManager::enableStencilTest() noexcept
{
    if(_stencilTestEnabled != true)
    {
        _stencilTestEnabled = true;
        glEnable(GL_STENCIL_TEST);
    }
}

void GLStateManager::disableStencilTest() noexcept
{
    if(_stencilTestEnabled != false)
    {
        _stencilTestEnabled = false;
        glDisable(GL_STENCIL_TEST);
    }
}

void GLStateManager::setScissorTest(const bool state) noexcept
{
    if(_scissorTestEnabled != state)
    {
        _scissorTestEnabled = state;
        if(state)
        {
            glEnable(GL_SCISSOR_TEST);
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }
    }
}

void GLStateManager::enableScissorTest() noexcept
{
    if(_scissorTestEnabled != true)
    {
        _scissorTestEnabled = true;
        glEnable(GL_SCISSOR_TEST);
    }
}

void GLStateManager::disableScissorTest() noexcept
{
    if(_scissorTestEnabled != false)
    {
        _scissorTestEnabled = false;
        glDisable(GL_SCISSOR_TEST);
    }
}

void GLStateManager::setFaceCulling(const bool state) noexcept
{
    if(_faceCullingEnabled != state)
    {
        _faceCullingEnabled = state;
        if(state)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
    }
}

void GLStateManager::enableFaceCulling() noexcept
{
    if(_faceCullingEnabled != true)
    {
        _faceCullingEnabled = true;
        glEnable(GL_CULL_FACE);
    }
}

void GLStateManager::disableFaceCulling() noexcept
{
    if(_faceCullingEnabled != false)
    {
        _faceCullingEnabled = false;
        glDisable(GL_CULL_FACE);
    }
}

void GLStateManager::set(const GLenum capability, const bool state) noexcept
{
    switch(capability)
    {
        case GL_BLEND: setBlending(state); break;
        case GL_DEPTH_TEST: setDepthTest(state); break;
        case GL_STENCIL_TEST: setStencilTest(state); break;
        case GL_SCISSOR_TEST: setScissorTest(state); break;
        case GL_CULL_FACE: setFaceCulling(state); break;
        default:
            if(state)
            {
                glEnable(capability);
            }
            else
            {
                glDisable(capability);
            }
            break;
    }
}

void GLStateManager::set(const GLenum capability, const uSys index, const bool state) noexcept
{
    switch(capability)
    {
        case GL_BLEND: setBlending(index, state); break;
        case GL_DEPTH_TEST: setDepthTest(state); break;
        case GL_STENCIL_TEST: setStencilTest(state); break;
        case GL_SCISSOR_TEST: setScissorTest(state); break;
        case GL_CULL_FACE: setFaceCulling(state); break;
        default:
            if(state)
            {
                glEnablei(capability, static_cast<GLuint>(index));
            }
            else
            {
                glDisablei(capability, static_cast<GLuint>(index));
            }
            break;
    }
}

void GLStateManager::enable(const GLenum capability) noexcept
{
    switch(capability)
    {
        case GL_BLEND: enableBlending(); break;
        case GL_DEPTH_TEST: enableDepthTest(); break;
        case GL_STENCIL_TEST: enableStencilTest(); break;
        case GL_SCISSOR_TEST: enableScissorTest(); break;
        case GL_CULL_FACE: enableFaceCulling(); break;
        default: glEnable(capability); break;
    }
}

void GLStateManager::enable(const GLenum capability, const uSys index) noexcept
{
    switch(capability)
    {
        case GL_BLEND: enableBlending(index); break;
        case GL_DEPTH_TEST: enableDepthTest(); break;
        case GL_STENCIL_TEST: enableStencilTest(); break;
        case GL_SCISSOR_TEST: enableScissorTest(); break;
        case GL_CULL_FACE: enableFaceCulling(); break;
        default: glEnablei(capability, static_cast<GLuint>(index)); break;
    }
}

void GLStateManager::disable(const GLenum capability) noexcept
{
    switch(capability)
    {
        case GL_BLEND: disableBlending(); break;
        case GL_DEPTH_TEST: disableDepthTest(); break;
        case GL_STENCIL_TEST: disableStencilTest(); break;
        case GL_SCISSOR_TEST: disableScissorTest(); break;
        case GL_CULL_FACE: disableFaceCulling(); break;
        default: glDisable(capability); break;
    }
}

void GLStateManager::disable(const GLenum capability, const uSys index) noexcept
{
    switch(capability)
    {
        case GL_BLEND: disableBlending(index); break;
        case GL_DEPTH_TEST: disableDepthTest(); break;
        case GL_STENCIL_TEST: disableStencilTest(); break;
        case GL_SCISSOR_TEST: disableScissorTest(); break;
        case GL_CULL_FACE: disableFaceCulling(); break;
        default: glDisablei(capability, static_cast<GLuint>(index)); break;
    }
}

void GLStateManager::blendFunc(const GLenum src, const GLenum dest) noexcept
{
    bool stateChangeRequired = false;
    for(uSys i = 0; i < 8; ++i)
    {
        if(_blendingControls[i].srcColor  != src  || _blendingControls[i].srcAlpha  != src || 
           _blendingControls[i].destColor != dest || _blendingControls[i].destAlpha != dest)
        {
            stateChangeRequired = true;
            _blendingControls[i].srcColor = src;
            _blendingControls[i].srcAlpha = src;
            _blendingControls[i].destColor = dest;
            _blendingControls[i].destAlpha = dest;
        }
    }

    if(stateChangeRequired)
    {
        glBlendFunc(src, dest);
    }
}

void GLStateManager::blendFunc(const GLenum srcColor, const GLenum srcAlpha, const GLenum destColor, const GLenum destAlpha) noexcept
{
    bool stateChangeRequired = false;
    for(uSys i = 0; i < 8; ++i)
    {
        if(_blendingControls[i].srcColor  != srcColor  || _blendingControls[i].srcAlpha  != srcAlpha || 
           _blendingControls[i].destColor != destColor || _blendingControls[i].destAlpha != destAlpha)
        {
            stateChangeRequired = true;
            _blendingControls[i].srcColor = srcColor;
            _blendingControls[i].srcAlpha = srcAlpha;
            _blendingControls[i].destColor = destColor;
            _blendingControls[i].destAlpha = destAlpha;
        }
    }

    if(stateChangeRequired)
    {
        glBlendFuncSeparate(srcColor, srcAlpha, destColor, destAlpha);
    }
}

void GLStateManager::blendFunc(const uSys index, const GLenum src, const GLenum dest) noexcept
{
    if(_blendingControls[index].srcColor  != src  || _blendingControls[index].srcAlpha  != src ||
       _blendingControls[index].destColor != dest || _blendingControls[index].destAlpha != dest)
    {
        _blendingControls[index].srcColor = src;
        _blendingControls[index].srcAlpha = src;
        _blendingControls[index].destColor = dest;
        _blendingControls[index].destAlpha = dest;
        glBlendFunci(static_cast<GLuint>(index), src, dest);
    }
}

void GLStateManager::blendFunc(const uSys index, const GLenum srcColor, const GLenum srcAlpha, const GLenum destColor, const GLenum destAlpha) noexcept
{
    if(_blendingControls[index].srcColor  != srcColor  || _blendingControls[index].srcAlpha  != srcAlpha ||
       _blendingControls[index].destColor != destColor || _blendingControls[index].destAlpha != destAlpha)
    {
        _blendingControls[index].srcColor = srcColor;
        _blendingControls[index].srcAlpha = srcAlpha;
        _blendingControls[index].destColor = destColor;
        _blendingControls[index].destAlpha = destAlpha;
        glBlendFuncSeparatei(static_cast<GLuint>(index), srcColor, srcAlpha, destColor, destAlpha);
    }
}

void GLStateManager::blendEquation(const GLenum equation) noexcept
{
    bool stateChangeRequired = false;
    for(uSys i = 0; i < 8; ++i)
    {
        if(_blendingControls[i].equationColor != equation || _blendingControls[i].equationAlpha != equation)
        {
            stateChangeRequired = true;
            _blendingControls[i].equationColor = equation;
            _blendingControls[i].equationAlpha = equation;
        }
    }

    if(stateChangeRequired)
    {
        glBlendEquation(equation);
    }
}

void GLStateManager::blendEquation(const GLenum equationColor, const GLenum equationAlpha) noexcept
{
    bool stateChangeRequired = false;
    for(uSys i = 0; i < 8; ++i)
    {
        if(_blendingControls[i].equationColor != equationColor || _blendingControls[i].equationAlpha != equationAlpha)
        {
            stateChangeRequired = true;
            _blendingControls[i].equationColor = equationColor;
            _blendingControls[i].equationAlpha = equationAlpha;
        }
    }

    if(stateChangeRequired)
    {
        glBlendEquationSeparate(equationColor, equationAlpha);
    }
}

void GLStateManager::blendEquation(const uSys index, const GLenum equation) noexcept
{
    if(_blendingControls[index].equationColor != equation || _blendingControls[index].equationAlpha != equation)
    {
        _blendingControls[index].equationColor = equation;
        _blendingControls[index].equationAlpha = equation;
        glBlendEquationi(static_cast<GLuint>(index), equation);
    }
}

void GLStateManager::blendEquation(const uSys index, const GLenum equationColor, const GLenum equationAlpha) noexcept
{
    if(_blendingControls[index].equationColor != equationColor || _blendingControls[index].equationAlpha != equationAlpha)
    {
        _blendingControls[index].equationColor = equationColor;
        _blendingControls[index].equationAlpha = equationAlpha;
        glBlendEquationSeparatei(static_cast<GLuint>(index), equationColor, equationAlpha);
    }
}

void GLStateManager::depthMask(const GLboolean depthMask) noexcept
{
    if(_depthControl.depthMask != depthMask)
    {
        _depthControl.depthMask = depthMask;
        glDepthMask(depthMask);
    }
}

void GLStateManager::depthFunc(const GLenum depthCompareFunc) noexcept
{
    if(_depthControl.depthCompareFunc != depthCompareFunc)
    {
        _depthControl.depthCompareFunc = depthCompareFunc;
        glDepthFunc(depthCompareFunc);
    }
}

void GLStateManager::stencilMaskFront(const GLuint writeMask) noexcept
{
    if(_frontStencil.writeMask != writeMask)
    {
        _frontStencil.writeMask = writeMask;
        glStencilMaskSeparate(GL_FRONT, writeMask);
    }
}

void GLStateManager::stencilMaskBack(const GLuint writeMask) noexcept
{
    if(_backStencil.writeMask != writeMask)
    {
        _backStencil.writeMask = writeMask;
        glStencilMaskSeparate(GL_BACK, writeMask);
    }
}

void GLStateManager::stencilMaskFrontBack(const GLuint writeMask) noexcept
{
    int update = 0;
    if(_frontStencil.writeMask != writeMask)
    {
        _frontStencil.writeMask = writeMask;
        update |= 0x01;
    }

    if(_backStencil.writeMask != writeMask)
    {
        _backStencil.writeMask = writeMask;
        update |= 0x02;
    }

    if(update == 0x01)
    {
        glStencilMaskSeparate(GL_FRONT, writeMask);
    }
    else if(update == 0x02)
    {
        glStencilMaskSeparate(GL_BACK, writeMask);
    }
    else if(update == 0x03)
    {
        glStencilMaskSeparate(GL_FRONT_AND_BACK, writeMask);
    }
}

void GLStateManager::stencilMask(const GLuint writeMask) noexcept
{
    stencilMaskFrontBack(writeMask);
}

void GLStateManager::stencilMask(const GLenum face, const GLuint writeMask) noexcept
{
    switch(face)
    {
        case GL_FRONT:          stencilMaskFront(writeMask); break;
        case GL_BACK:           stencilMaskBack(writeMask); break;
        case GL_FRONT_AND_BACK: stencilMaskFrontBack(writeMask); break;
        default:                glStencilMaskSeparate(face, writeMask); break;
    }
}

void GLStateManager::stencilOpFront(const GLenum stencilFail, const GLenum depthFail, const GLenum pass) noexcept
{
    if(_frontStencil.stencilFailOp != stencilFail ||
       _frontStencil.stencilDepthFailOp != depthFail ||
       _frontStencil.stencilPassOp != pass)
    {
        _frontStencil.stencilFailOp = stencilFail;
        _frontStencil.stencilDepthFailOp = depthFail;
        _frontStencil.stencilPassOp = pass;
        glStencilOpSeparate(GL_FRONT, stencilFail, depthFail, pass);
    }
}

void GLStateManager::stencilOpBack(const GLenum stencilFail, const GLenum depthFail, const GLenum pass) noexcept
{
    if(_backStencil.stencilFailOp != stencilFail ||
       _backStencil.stencilDepthFailOp != depthFail ||
       _backStencil.stencilPassOp != pass)
    {
        _backStencil.stencilFailOp = stencilFail;
        _backStencil.stencilDepthFailOp = depthFail;
        _backStencil.stencilPassOp = pass;
        glStencilOpSeparate(GL_BACK, stencilFail, depthFail, pass);
    }
}

void GLStateManager::stencilOpFrontBack(const GLenum stencilFail, const GLenum depthFail, const GLenum pass) noexcept
{
    int update = 0;
    if(_frontStencil.stencilFailOp != stencilFail ||
       _frontStencil.stencilDepthFailOp != depthFail ||
       _frontStencil.stencilPassOp != pass)
    {
        _frontStencil.stencilFailOp = stencilFail;
        _frontStencil.stencilDepthFailOp = depthFail;
        _frontStencil.stencilPassOp = pass;
        update |= 0x01;
    }

    if(_backStencil.stencilFailOp != stencilFail ||
       _backStencil.stencilDepthFailOp != depthFail ||
       _backStencil.stencilPassOp != pass)
    {
        _backStencil.stencilFailOp = stencilFail;
        _backStencil.stencilDepthFailOp = depthFail;
        _backStencil.stencilPassOp = pass;
        update |= 0x02;
    }

    if(update == 0x01)
    {
        glStencilOpSeparate(GL_FRONT, stencilFail, depthFail, pass);
    }
    else if(update == 0x02)
    {
        glStencilOpSeparate(GL_BACK, stencilFail, depthFail, pass);
    }
    else if(update == 0x03)
    {
        glStencilOpSeparate(GL_FRONT_AND_BACK, stencilFail, depthFail, pass);
    }
}

void GLStateManager::stencilOp(const GLenum stencilFail, const GLenum depthFail, const GLenum pass) noexcept
{
    stencilOpFrontBack(stencilFail, depthFail, pass);
}

void GLStateManager::stencilOp(const GLenum face, const GLenum stencilFail, const GLenum depthFail, const GLenum pass) noexcept
{
    switch(face)
    {
        case GL_FRONT:          stencilOpFront(stencilFail, depthFail, pass); break;
        case GL_BACK:           stencilOpBack(stencilFail, depthFail, pass); break;
        case GL_FRONT_AND_BACK: stencilOpFrontBack(stencilFail, depthFail, pass); break;
        default:                glStencilOpSeparate(face, stencilFail, depthFail, pass); break;
    }
}

void GLStateManager::stencilFuncFront(const GLenum func, const GLint reference, const GLuint readMask) noexcept
{
    if(_frontStencil.compareFunc != func ||
       _frontStencil.reference != reference ||
       _frontStencil.readMask != readMask)
    {
        _frontStencil.compareFunc = func;
        _frontStencil.reference = reference;
        _frontStencil.readMask = readMask;
        glStencilFuncSeparate(GL_FRONT, func, reference, readMask);
    }
}

void GLStateManager::stencilFuncBack(const GLenum func, const GLint reference, const GLuint readMask) noexcept
{
    if(_backStencil.compareFunc != func ||
       _backStencil.reference != reference ||
       _backStencil.readMask != readMask)
    {
        _backStencil.compareFunc = func;
        _backStencil.reference = reference;
        _backStencil.readMask = readMask;
        glStencilFuncSeparate(GL_BACK, func, reference, readMask);
    }
}

void GLStateManager::stencilFuncFrontBack(const GLenum func, const GLint reference, const GLuint readMask) noexcept
{
    int update = 0;
    if(_frontStencil.compareFunc != func ||
       _frontStencil.reference != reference ||
       _frontStencil.readMask != readMask)
    {
        _frontStencil.compareFunc = func;
        _frontStencil.reference = reference;
        _frontStencil.readMask = readMask;
        update |= 0x01;
    }

    if(_backStencil.compareFunc != func ||
       _backStencil.reference != reference ||
       _backStencil.readMask != readMask)
    {
        _backStencil.compareFunc = func;
        _backStencil.reference = reference;
        _backStencil.readMask = readMask;
        update |= 0x02;
    }

    if(update == 0x01)
    {
        glStencilFuncSeparate(GL_FRONT, func, reference, readMask);
    }
    else if(update == 0x02)
    {
        glStencilFuncSeparate(GL_BACK, func, reference, readMask);
    }
    else if(update == 0x03)
    {
        glStencilFuncSeparate(GL_FRONT_AND_BACK, func, reference, readMask);
    }
}

void GLStateManager::stencilFunc(const GLenum func, const GLint reference, const GLuint readMask) noexcept
{
    stencilFuncFrontBack(func, reference, readMask);
}

void GLStateManager::stencilFunc(const GLenum face, const GLenum func, const GLint reference, const GLuint readMask) noexcept
{
    switch(face)
    {
        case GL_FRONT:          stencilFuncFront(func, reference, readMask); break;
        case GL_BACK:           stencilFuncBack(func, reference, readMask); break;
        case GL_FRONT_AND_BACK: stencilFuncFrontBack(func, reference, readMask); break;
        default:                glStencilFuncSeparate(face, func, reference, readMask); break;
    }
}

void GLStateManager::stencilFuncFront(const GLenum func, const GLuint readMask) noexcept
{
    if(_frontStencil.compareFunc != func ||
        _frontStencil.readMask != readMask)
    {
        _frontStencil.compareFunc = func;
        _frontStencil.readMask = readMask;
        glStencilFuncSeparate(GL_FRONT, func, _frontStencil.reference, readMask);
    }
}

void GLStateManager::stencilFuncBack(const GLenum func, const GLuint readMask) noexcept
{
    if(_backStencil.compareFunc != func ||
        _backStencil.readMask != readMask)
    {
        _backStencil.compareFunc = func;
        _backStencil.readMask = readMask;
        glStencilFuncSeparate(GL_BACK, func, _backStencil.reference, readMask);
    }
}

void GLStateManager::stencilFuncFrontBack(const GLenum func, const GLuint readMask) noexcept
{
    int update = 0;
    if(_frontStencil.compareFunc != func ||
        _frontStencil.readMask != readMask)
    {
        _frontStencil.compareFunc = func;
        _frontStencil.readMask = readMask;
        update |= 0x01;
    }

    if(_backStencil.compareFunc != func ||
        _backStencil.readMask != readMask)
    {
        _backStencil.compareFunc = func;
        _backStencil.readMask = readMask;
        update |= 0x02;
    }

    if(update == 0x01)
    {
        glStencilFuncSeparate(GL_FRONT, func, _frontStencil.reference, readMask);
    }
    else if(update == 0x02)
    {
        glStencilFuncSeparate(GL_BACK, func, _backStencil.reference, readMask);
    }
    else if(update == 0x03)
    {
        glStencilFuncSeparate(GL_FRONT, func, _frontStencil.reference, readMask);
        glStencilFuncSeparate(GL_BACK, func, _backStencil.reference, readMask);
    }
}

void GLStateManager::stencilFunc(const GLenum func, const GLuint readMask) noexcept
{
    stencilFuncFrontBack(func, readMask);
}

void GLStateManager::stencilFunc(const GLenum face, const GLenum func, const GLuint readMask) noexcept
{
    switch(face)
    {
        case GL_FRONT:          stencilFuncFront(func, readMask); break;
        case GL_BACK:           stencilFuncBack(func, readMask); break;
        case GL_FRONT_AND_BACK: stencilFuncFrontBack(func, readMask); break;
        default: break;
    }
}

void GLStateManager::stencilRefFront(const GLint reference) noexcept
{
    if(_frontStencil.reference != reference)
    {
        _frontStencil.reference = reference;
        glStencilFuncSeparate(GL_FRONT, _frontStencil.compareFunc, reference, _frontStencil.readMask);
    }
}

void GLStateManager::stencilRefBack(const GLint reference) noexcept
{
    if(_backStencil.reference != reference)
    {
        _backStencil.reference = reference;
        glStencilFuncSeparate(GL_BACK, _backStencil.compareFunc, reference, _backStencil.readMask);
    }
}

void GLStateManager::stencilRefFrontBack(const GLint reference) noexcept
{
    int update = 0;
    if(_frontStencil.reference != reference)
    {
        _frontStencil.reference = reference;
        update |= 0x01;
    }

    if(_backStencil.reference != reference)
    {
        _backStencil.reference = reference;
        update |= 0x02;
    }

    if(update == 0x01)
    {
        glStencilFuncSeparate(GL_FRONT, _frontStencil.compareFunc, reference, _frontStencil.readMask);
    }
    else if(update == 0x02)
    {
        glStencilFuncSeparate(GL_BACK, _backStencil.compareFunc, reference, _backStencil.readMask);
    }
    else if(update == 0x03)
    {
        glStencilFuncSeparate(GL_FRONT, _frontStencil.compareFunc, reference, _frontStencil.readMask);
        glStencilFuncSeparate(GL_BACK, _backStencil.compareFunc, reference, _backStencil.readMask);
    }
}

void GLStateManager::stencilRef(const GLint reference) noexcept
{
    stencilRefFrontBack(reference);
}

void GLStateManager::stencilRef(const GLenum face, const GLint reference) noexcept
{
    switch(face)
    {
        case GL_FRONT:          stencilRefFront(reference); break;
        case GL_BACK:           stencilRefBack(reference); break;
        case GL_FRONT_AND_BACK: stencilRefFrontBack(reference); break;
        default: break;
    }
}

void GLStateManager::polygonOffset(const GLfloat factor, const GLfloat units) noexcept
{
    if(_polygonOffset.factor != factor || _polygonOffset.units != units)  // NOLINT(clang-diagnostic-float-equal)
    {
        _polygonOffset.factor = factor;
        _polygonOffset.units = units;
        glPolygonOffset(factor, units);
    }
}

void GLStateManager::polygonOffset(const GLfloat factor, const GLfloat units, const GLfloat clamp) noexcept
{
    if(_polygonOffset.factor != factor || _polygonOffset.units != units || _polygonOffset.depthClamp != clamp)  // NOLINT(clang-diagnostic-float-equal)
    {
        _polygonOffset.factor = factor;
        _polygonOffset.units = units;
        _polygonOffset.depthClamp = clamp;
        _polygonOffset.extFunc(factor, units, clamp);
    }
}

void GLStateManager::_glPolygonOffsetFallback(const GLfloat factor, const GLfloat units, GLfloat) noexcept
{
    glPolygonOffset(factor, units);
}

void GLStateManager::frontFaceCW() noexcept
{
    if(_frontFace != GL_CW)
    {
        _frontFace = GL_CW;
        glFrontFace(GL_CW);
    }
}

void GLStateManager::frontFaceCCW() noexcept
{
    if(_frontFace != GL_CCW)
    {
        _frontFace = GL_CCW;
        glFrontFace(GL_CCW);
    }
}

void GLStateManager::frontFace(const GLenum mode) noexcept
{
    if(_frontFace != mode)
    {
        _frontFace = mode;
        glFrontFace(mode);
    }
}

void GLStateManager::frontFaceB(const bool counterClockWise) noexcept
{
    frontFace(counterClockWise ? GL_CCW : GL_CW);
}

void GLStateManager::cullFront() noexcept
{
    if(_faceCullingMode != GL_FRONT)
    {
        _faceCullingMode = GL_FRONT;
        glCullFace(GL_FRONT);
    }
}

void GLStateManager::cullBack() noexcept
{
    if(_faceCullingMode != GL_BACK)
    {
        _faceCullingMode = GL_BACK;
        glCullFace(GL_BACK);
    }
}

void GLStateManager::cullFrontBack() noexcept
{
    if(_faceCullingMode != GL_FRONT_AND_BACK)
    {
        _faceCullingMode = GL_FRONT_AND_BACK;
        glCullFace(GL_FRONT_AND_BACK);
    }
}

void GLStateManager::cullMode(const GLenum mode) noexcept
{
    if(_faceCullingMode != mode)
    {
        _faceCullingMode = mode;
        glCullFace(mode);
    }
}

void GLStateManager::polygonModeFill() noexcept
{
    if(_polygonFillMode != GL_FILL)
    {
        _polygonFillMode = GL_FILL;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void GLStateManager::polygonModeLines() noexcept
{
    if(_polygonFillMode != GL_LINE)
    {
        _polygonFillMode = GL_LINE;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

void GLStateManager::polygonModePoints() noexcept
{
    if(_polygonFillMode != GL_POINT)
    {
        _polygonFillMode = GL_POINT;
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }
}

void GLStateManager::polygonMode(const GLenum mode) noexcept
{
    if(_polygonFillMode != mode)
    {
        _polygonFillMode = mode;
        glPolygonMode(GL_FRONT_AND_BACK, mode);
    }
}

void GLStateManager::clearColor(const GLfloat color[4]) noexcept
{
    if(color)
    {
        if(::std::memcmp(_clearColor, color, sizeof(GLfloat) * 4) != 0)
        {
            (void) ::std::memcpy(_clearColor, color, sizeof(GLfloat) * 4);
            glClearColor(color[0], color[1], color[2], color[3]);
        }
    }
}

void GLStateManager::clearColor(const GLfloat r, const GLfloat g, const GLfloat b, const GLfloat a) noexcept
{
    const GLfloat color[] { r, g, b, a };

    if(::std::memcmp(_clearColor, color, sizeof(GLfloat) * 4) != 0)
    {
        (void) ::std::memcpy(_clearColor, color, sizeof(GLfloat) * 4);
        glClearColor(r, g, b, a);
    }
}

void GLStateManager::clearDepth(const GLfloat depth) noexcept
{
    if(_clearDepth != static_cast<GLdouble>(depth))  // NOLINT(clang-diagnostic-float-equal)
    {
        _clearDepth = static_cast<GLdouble>(depth);
        glClearDepthf(depth);
    }
}

void GLStateManager::clearDepth(const GLdouble depth) noexcept
{
    if(_clearDepth != depth)  // NOLINT(clang-diagnostic-float-equal)
    {
        _clearDepth = depth;
        glClearDepth(depth);
    }
}

void GLStateManager::clearStencil(const GLint stencil) noexcept
{
    if(_clearStencil != stencil)
    {
        _clearStencil = stencil;
        glClearStencil(stencil);
    }
}
