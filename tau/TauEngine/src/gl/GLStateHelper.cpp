#include "gl/GLStateHelper.hpp"

GLStateHelper::GLStateHelper() noexcept
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
    , _blendingActive { false, false, false, false, false, false, false, false }
{
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, reinterpret_cast<GLint*>(&_maxUniformBufferBindings));
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, reinterpret_cast<GLint*>(&_maxShaderStorageBufferBindings));
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, reinterpret_cast<GLint*>(&_maxTextureBindings));

    _uniformBufferBaseBindings = new GLState::BufferBaseBinding[_maxUniformBufferBindings];
    _shaderStorageBufferBaseBindings = new GLState::BufferBaseBinding[_maxShaderStorageBufferBindings];
    _textureBindings = new GLState::TextureBinding[_maxTextureBindings];
}

GLStateHelper::~GLStateHelper() noexcept
{
    delete[] _uniformBufferBaseBindings;
    delete[] _shaderStorageBufferBaseBindings;
    delete[] _textureBindings;
}

void GLStateHelper::bindVertexArray(const GLuint vao) noexcept
{
    if(vao != _vao)
    {
        _vao = vao;
        glBindVertexArray(vao);
    }
}

void GLStateHelper::bindArrayBuffer(const GLuint buffer) noexcept
{
    if(buffer != _arrayBuffer)
    {
        _arrayBuffer = buffer;
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
    }
}

void GLStateHelper::bindElementArrayBuffer(const GLuint buffer) noexcept
{
    if(buffer != _elementArrayBuffer)
    {
        _elementArrayBuffer = buffer;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
    }
}

void GLStateHelper::bindUniformBuffer(const GLuint buffer) noexcept
{
    if(buffer != _uniformBuffer)
    {
        _uniformBuffer = buffer;
        glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    }
}

void GLStateHelper::bindShaderStorageBuffer(const GLuint buffer) noexcept
{
    if(buffer != _shaderStorageBuffer)
    {
        _shaderStorageBuffer = buffer;
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
    }
}

void GLStateHelper::bindBuffer(const GLenum target, const GLuint buffer) noexcept
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

void GLStateHelper::bindUniformBufferBase(const GLuint index, const GLuint buffer) noexcept
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

void GLStateHelper::bindShaderStorageBufferBase(const GLuint index, const GLuint buffer) noexcept
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

void GLStateHelper::bindUniformBufferRange(const GLuint index, const GLuint buffer, const GLintptr offset, const GLsizeiptr size) noexcept
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

void GLStateHelper::bindShaderStorageBufferRange(const GLuint index, const GLuint buffer, const GLintptr offset, const GLsizeiptr size) noexcept
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

void GLStateHelper::bindBufferBase(const GLenum target, const GLuint index, const GLuint buffer) noexcept
{
    switch(target)
    {
        case GL_UNIFORM_BUFFER:        bindUniformBufferBase(index, buffer); break;
        case GL_SHADER_STORAGE_BUFFER: bindShaderStorageBufferBase(index, buffer); break;
        default:                       glBindBufferBase(target, index, buffer); break;
    }
}

void GLStateHelper::bindBufferRange(const GLenum target, const GLuint index, const GLuint buffer, const GLintptr offset, const GLsizeiptr size) noexcept
{
    switch(target)
    {
        case GL_UNIFORM_BUFFER:        bindUniformBufferRange(index, buffer, offset, size); break;
        case GL_SHADER_STORAGE_BUFFER: bindShaderStorageBufferRange(index, buffer, offset, size); break;
        default:                       glBindBufferRange(target, index, buffer, offset, size); break;
    }
}

void GLStateHelper::activeTexture(const uSys unit) noexcept
{
    if(unit > _maxTextureBindings)
    { return; }

    if(unit != _activeTextureUnit)
    {
        _activeTextureUnit = unit;
        glActiveTexture(GL_TEXTURE0 + unit);
    }
}

void GLStateHelper::bindTexture(const GLenum target, const GLuint texture) noexcept
{
    GLState::TextureBinding& binding = _textureBindings[_activeTextureUnit];

    if(target != binding.target || texture != binding.texture)
    {
        binding.target = target;
        binding.texture = texture;
        glBindTexture(target, texture);
    }
}

void GLStateHelper::bindTexture(const GLenum target, const GLuint texture, const uSys unit) noexcept
{
    if(unit > _maxTextureBindings)
    { return; }

    GLState::TextureBinding& binding = _textureBindings[unit];

    if(target != binding.target || texture != binding.texture)
    {
        if(unit != _activeTextureUnit)
        {
            _activeTextureUnit = unit;
            glActiveTexture(GL_TEXTURE0 + unit);
        }

        binding.target = target;
        binding.texture = texture;
        glBindTexture(target, texture);
    }
}

void GLStateHelper::bindShaderProgram(const GLuint program) noexcept
{
    if(program != _activeProgram)
    {
        _activeProgram = program;
        glUseProgram(program);
    }
}

void GLStateHelper::setBlending(const bool state) noexcept
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

void GLStateHelper::setBlending(const uSys index, const bool state) noexcept
{
    if(_blendingControls[index].active != state)
    {
        _blendingControls[index].active = state;
        if(state)
        {
            glEnablei(GL_BLEND, index);
        }
        else
        {
            glDisablei(GL_BLEND, index);
        }
    }
}

void GLStateHelper::enableBlending() noexcept
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

void GLStateHelper::enableBlending(const uSys index) noexcept
{
    if(_blendingControls[index].active != true)
    {
        _blendingControls[index].active = true;
        glEnablei(GL_BLEND, index);
    }
}

void GLStateHelper::disableBlending() noexcept
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

void GLStateHelper::disableBlending(const uSys index) noexcept
{
    if(_blendingControls[index].active != false)
    {
        _blendingControls[index].active = false;
        glDisablei(GL_BLEND, index);
    }
}

void GLStateHelper::set(const GLenum capability, const bool state) noexcept
{
    switch(capability)
    {
        case GL_BLEND: setBlending(state); break;
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

void GLStateHelper::set(const GLenum capability, const uSys index, const bool state) noexcept
{
    switch(capability)
    {
        case GL_BLEND: setBlending(index, state); break;
        default:
            if(state)
            {
                glEnablei(capability, index);
            }
            else
            {
                glDisablei(capability, index);
            }
            break;
    }
}

void GLStateHelper::enable(const GLenum capability) noexcept
{
    switch(capability)
    {
        case GL_BLEND: enableBlending(); break;
        default: glEnable(capability); break;
    }
}

void GLStateHelper::enable(const GLenum capability, const uSys index) noexcept
{
    switch(capability)
    {
        case GL_BLEND: enableBlending(index); break;
        default: glEnablei(capability, index); break;
    }
}

void GLStateHelper::disable(const GLenum capability) noexcept
{
    switch(capability)
    {
        case GL_BLEND: disableBlending(); break;
        default: glDisable(capability); break;
    }
}

void GLStateHelper::disable(const GLenum capability, const uSys index) noexcept
{
    switch(capability)
    {
        case GL_BLEND: disableBlending(index); break;
        default: glDisablei(capability, index); break;
    }
}

void GLStateHelper::blendFunc(const GLenum src, const GLenum dest) noexcept
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

void GLStateHelper::blendFunc(const GLenum srcColor, const GLenum srcAlpha, const GLenum destColor, const GLenum destAlpha) noexcept
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

void GLStateHelper::blendFunc(const uSys index, const GLenum src, const GLenum dest) noexcept
{
    if(_blendingControls[index].srcColor  != src  || _blendingControls[index].srcAlpha  != src ||
       _blendingControls[index].destColor != dest || _blendingControls[index].destAlpha != dest)
    {
        _blendingControls[index].srcColor = src;
        _blendingControls[index].srcAlpha = src;
        _blendingControls[index].destColor = dest;
        _blendingControls[index].destAlpha = dest;
        glBlendFunci(index, src, dest);
    }
}

void GLStateHelper::blendFunc(const uSys index, const GLenum srcColor, const GLenum srcAlpha, const GLenum destColor, const GLenum destAlpha) noexcept
{
    if(_blendingControls[index].srcColor  != srcColor  || _blendingControls[index].srcAlpha  != srcAlpha ||
       _blendingControls[index].destColor != destColor || _blendingControls[index].destAlpha != destAlpha)
    {
        _blendingControls[index].srcColor = srcColor;
        _blendingControls[index].srcAlpha = srcAlpha;
        _blendingControls[index].destColor = destColor;
        _blendingControls[index].destAlpha = destAlpha;
        glBlendFuncSeparatei(index, srcColor, srcAlpha, destColor, destAlpha);
    }
}

void GLStateHelper::blendEquation(const GLenum equation) noexcept
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

void GLStateHelper::blendEquation(const GLenum equationColor, const GLenum equationAlpha) noexcept
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

void GLStateHelper::blendEquation(const uSys index, const GLenum equation) noexcept
{
    if(_blendingControls[index].equationColor != equation || _blendingControls[index].equationAlpha != equation)
    {
        _blendingControls[index].equationColor = equation;
        _blendingControls[index].equationAlpha = equation;
        glBlendEquationi(index, equation);
    }
}

void GLStateHelper::blendEquation(const uSys index, const GLenum equationColor, const GLenum equationAlpha) noexcept
{
    if(_blendingControls[index].equationColor != equationColor || _blendingControls[index].equationAlpha != equationAlpha)
    {
        _blendingControls[index].equationColor = equationColor;
        _blendingControls[index].equationAlpha = equationAlpha;
        glBlendEquationSeparatei(index, equationColor, equationAlpha);
    }
}
