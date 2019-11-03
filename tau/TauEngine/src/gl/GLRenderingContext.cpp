#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)
#include <gl/GLRenderingContext.hpp>
#include <Utils.hpp>
#include "gl/GLBufferDescriptor.hpp"
#include "gl/GLVertexArray.hpp"

GLRenderingContext::GLRenderingContext(const RenderingMode& mode, const bool debug, const int majorVersion, const int minorVersion, const GLProfile core, const bool forwardCompatible) noexcept
    : IRenderingContext(mode, debug),
      _device(null), _context(null),
      _vaos(),
      _majorVersion(majorVersion),
      _minorVersion(minorVersion),
      _compat(core),
      _forwardCompatible(forwardCompatible)
{ }

Ref<IVertexArray> GLRenderingContext::createVertexArray(std::size_t attribCount) noexcept
{
    return Ref<IVertexArray>(new(std::nothrow) GLVertexArray(getVAUid(), attribCount));
}

void* GLRenderingContext::getVertexArrayHandle(IVertexArray* vertexArray) noexcept
{
    const auto iter = _vaos.find(vertexArray);

    if(iter == _vaos.end())
    {
        GLuint vao = GLVertexArray::generate();
        _vaos.insert(std::make_pair(vertexArray, vao));

        GLVertexArray::_bind(vao);

        vertexArray->internalSetup(*this);

        // for(GLuint i = 0; i < vertexArray->attribs().count(); ++i)
        // {
        //     const auto attrib = vertexArray->attribs()[i];
        //     // glBindBuffer(GL_ARRAY_BUFFER, attrib.buffer);
        //     attrib.buffer->bind(*this);
        //     GLBufferDescriptor::attribPointer(i, attrib.size, attrib.type, attrib.normalized, attrib.stride, attrib.pointer);
        //     attrib.buffer->unbind(*this);
        //     // glBindBuffer(GL_ARRAY_BUFFER, 0);
        // }
    }

    return &_vaos[vertexArray];
}

// void GLRenderingContext::initBufferDescriptor(IBufferDescriptor* bufferDescriptor) noexcept
// {
//     const auto iter = _vaos.find(bufferDescriptor);
//
//     if(iter == _vaos.end())
//     {
//         GLuint vao = GLBufferDescriptor::generate();
//         _vaos.insert(std::make_pair(bufferDescriptor, vao));
//
//         GLBufferDescriptor::_bind(vao);
//
//         for(GLuint i = 0; i < bufferDescriptor->attribs().count(); ++i)
//         {
//             const auto attrib = bufferDescriptor->attribs()[i];
//             glBindBuffer(GL_ARRAY_BUFFER, attrib.vbo);
//             GLBufferDescriptor::attribPointer(i, attrib.size, attrib.type, attrib.normalized, attrib.stride, attrib.pointer);
//             glBindBuffer(GL_ARRAY_BUFFER, 0);
//         }
//     }
// }
//
// void GLRenderingContext::bindBD(IBufferDescriptor* bufferDescriptor) noexcept
// {
//     const auto iter = _vaos.find(bufferDescriptor);
//     
//     if(iter == _vaos.end())
//     {
//         GLuint vao = GLBufferDescriptor::generate();
//         _vaos.insert(std::make_pair(bufferDescriptor, vao));
//
//         GLBufferDescriptor::_bind(vao);
//
//         for(GLuint i = 0; i < bufferDescriptor->attribs().count(); ++i)
//         {
//             const auto attrib = bufferDescriptor->attribs()[i];
//             glBindBuffer(GL_ARRAY_BUFFER, attrib.vbo);
//             GLBufferDescriptor::attribPointer(i, attrib.size, attrib.type, attrib.normalized, attrib.stride, attrib.pointer);
//             glBindBuffer(GL_ARRAY_BUFFER, 0);
//         }
//     }
//     else
//     {
//         GLBufferDescriptor::_bind(_vaos[bufferDescriptor]);
//     }
// }
//
// void GLRenderingContext::unbindBD(IBufferDescriptor* bufferDescriptor) noexcept
// {
//     GLBufferDescriptor::_unbind(0);
// }

void GLRenderingContext::destroyVA(IVertexArray* vertexArray) noexcept
{
    const auto iter = _vaos.find(vertexArray);

    if(iter != _vaos.end())
    {
        GLVertexArray::destroy(_vaos[vertexArray]);
    }
}

void GLRenderingContext::clearVAs() noexcept
{
    _vaos.clear();
}

void GLRenderingContext::updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) noexcept
{
    glViewport(x, y, width, height);
}

void GLRenderingContext::clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, int stencilValue) noexcept
{
    GLbitfield flags = 0;
    if(clearColorBuffer) { flags = GL_COLOR_BUFFER_BIT; }
    if(clearDepthBuffer) { flags |= GL_DEPTH_BUFFER_BIT; }
    if(clearStencilBuffer) { flags |= GL_STENCIL_BUFFER_BIT; }

    glClearDepth(depthValue);
    glClearStencil(stencilValue);
    glClearColor(static_cast<float>(color.r) / 255.0f,
                 static_cast<float>(color.g) / 255.0f,
                 static_cast<float>(color.b) / 255.0f,
                 static_cast<float>(color.a) / 255.0f);
    glClear(flags);
}

u32 GLRenderingContext::getVAUid() noexcept
{
    static u32 uid = 0;
    return ++uid;
}
