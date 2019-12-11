#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)
#include <Utils.hpp>
#include "gl/GLRenderingContext.hpp"
#include "gl/GLBufferDescriptor.hpp"
#include "gl/GLVertexArray.hpp"
#include "gl/GLBuffer.hpp"
#include "gl/GLFrameBuffer.hpp"
#include "Timings.hpp"

GLRenderingContext::GLRenderingContext(const RenderingMode& mode, const bool debug, const int majorVersion, const int minorVersion, const GLProfile core, const bool forwardCompatible) noexcept
    : IRenderingContext(mode, debug),
      _device(null), _context(null),
      _vaos(),
      _majorVersion(majorVersion),
      _minorVersion(minorVersion),
      _compat(core),
      _forwardCompatible(forwardCompatible)
{ }

Ref<IVertexArray> GLRenderingContext::createVertexArray(const std::size_t attribCount, const DrawType drawType) noexcept
{
    return Ref<IVertexArray>(new(std::nothrow) GLVertexArray(attribCount, drawType));
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
    }

    return &_vaos[vertexArray];
}

void GLRenderingContext::destroyVA(IVertexArray* vertexArray) noexcept
{
    PERF();
    const auto iter = _vaos.find(vertexArray);

    if(iter != _vaos.end())
    {
        GLVertexArray::destroy(_vaos[vertexArray]);
    }
}

void GLRenderingContext::clearVAs() noexcept
{
    PERF();
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

void GLRenderingContext::setFaceWinding(const bool clockwise) noexcept
{
    glFrontFace(clockwise ? GL_CW : GL_CCW);
}

void GLRenderingContext::enableDepthWriting(bool writing) noexcept
{
    glDepthMask(writing ? GL_TRUE : GL_FALSE);
}

Ref<IBuffer> GLRenderingContext::createBuffer(const std::size_t descriptorCount, const IBuffer::Type type, const IBuffer::UsageType usage) noexcept
{
    const GLuint buffer = GLBuffer::createBuffer();
    return Ref<GLBuffer>(new(std::nothrow) GLBuffer(type, usage, descriptorCount, buffer));
}

Ref<IIndexBuffer> GLRenderingContext::createIndexBuffer(const IBuffer::UsageType usage) noexcept
{
    const GLuint buffer = GLBuffer::createBuffer();
    return Ref<GLIndexBuffer>(new(std::nothrow) GLIndexBuffer(usage, buffer));
}

Ref<IFrameBufferBuilder> GLRenderingContext::createFrameBuffer() noexcept
{
    return Ref<GLFrameBufferBuilder>(new(std::nothrow) GLFrameBufferBuilder);
}
