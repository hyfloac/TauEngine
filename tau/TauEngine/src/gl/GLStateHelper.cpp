#include "gl/GLStateHelper.hpp"

GLStateHelper GLStateHelper::_globalInstance;

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
        case GL_UNIFORM_BUFFER:        bindUniformBufferBase(index, buffer);
        case GL_SHADER_STORAGE_BUFFER: bindShaderStorageBufferBase(index, buffer);
        default:                       glBindBufferBase(target, index, buffer);
    }
}

void GLStateHelper::bindBufferRange(const GLenum target, const GLuint index, const GLuint buffer, const GLintptr offset, const GLsizeiptr size) noexcept
{
    switch(target)
    {
        case GL_UNIFORM_BUFFER:        bindUniformBufferRange(index, buffer, offset, size);
        case GL_SHADER_STORAGE_BUFFER: bindShaderStorageBufferRange(index, buffer, offset, size);
        default:                       glBindBufferRange(target, index, buffer, offset, size);
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
