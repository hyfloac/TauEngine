#pragma warning(push, 0)
#include <GL/glew.h>
#pragma warning(pop)
#include <Utils.hpp>
#include "gl/GLRenderingContext.hpp"
#include "gl/GLBufferDescriptor.hpp"
#include "gl/GLVertexArray.hpp"
#include "gl/GLFrameBuffer.hpp"
#include "gl/gl4_5/GLBuffer4_5.hpp"
#include "gl/GLTexture.hpp"
#include "gl/GLShader.hpp"
#include "gl/GLTextureSampler.hpp"
#include "gl/GLTextureUploader.hpp"
#include "Timings.hpp"

GLRenderingContext::GLRenderingContext(const RenderingMode& mode, const int majorVersion, const int minorVersion, const GLProfile core, const bool forwardCompatible) noexcept
    : IRenderingContext(mode),
      _device(null), _context(null),
      _majorVersion(majorVersion),
      _minorVersion(minorVersion),
      _compat(core),
      _forwardCompatible(forwardCompatible)
{
    switch(_mode.currentMode())
    {
        case RenderingMode::Mode::OpenGL2:
        case RenderingMode::Mode::OpenGL3:
        case RenderingMode::Mode::OpenGL3_1:
        case RenderingMode::Mode::OpenGL3_2:
        case RenderingMode::Mode::OpenGL3_3:
        case RenderingMode::Mode::OpenGL4:
        case RenderingMode::Mode::OpenGL4_2:
        case RenderingMode::Mode::OpenGL4_3:
        case RenderingMode::Mode::OpenGL4_4:
            _bufferBuilder = new(::std::nothrow) GLBufferBuilder;
            _indexBufferBuilder = new(::std::nothrow) GLIndexBufferBuilder;
            _uniformBufferBuilder = new(::std::nothrow) GLUniformBufferBuilder;
            break;
        case RenderingMode::Mode::OpenGL4_5:
        case RenderingMode::Mode::OpenGL4_6:
            _bufferBuilder = new(::std::nothrow) GLBuffer4_5Builder;
            _indexBufferBuilder = new(::std::nothrow) GLIndexBuffer4_5Builder;
            _uniformBufferBuilder = new(::std::nothrow) GLUniformBuffer4_5Builder;
            break;
        default: break;
    }

    _textureSamplerBuilder = new(::std::nothrow) GLTextureSamplerBuilder;
}

void GLRenderingContext::updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) noexcept
{
    glViewport(x, y, width, height);
}

void GLRenderingContext::clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, u8 stencilValue) noexcept
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

Ref<IVertexArrayBuilder> GLRenderingContext::createVertexArray(const uSys bufferCount) noexcept
{
    return Ref<IVertexArrayBuilder>(new(::std::nothrow) GLVertexArrayBuilder(bufferCount, *this));
}

Ref<IFrameBufferBuilder> GLRenderingContext::createFrameBuffer() noexcept
{
    return Ref<GLFrameBufferBuilder>(new(::std::nothrow) GLFrameBufferBuilder);
}

Ref<ITextureBuilder> GLRenderingContext::createTexture2D() noexcept
{
    return Ref<GLTexture2DBuilder>(new(::std::nothrow) GLTexture2DBuilder);
}

Ref<ITextureBuilder> GLRenderingContext::createNullTexture() noexcept
{
    return Ref<GLTextureNullBuilder>(new(::std::nothrow) GLTextureNullBuilder);
}

Ref<ITextureBuilder> GLRenderingContext::createTextureDepth() noexcept
{
    return Ref<GLTextureDepthBuilder>(new(::std::nothrow) GLTextureDepthBuilder);
}

Ref<ITextureCubeBuilder> GLRenderingContext::createTextureCube() noexcept
{
    return Ref<GLTextureCubeBuilder>(new(::std::nothrow) GLTextureCubeBuilder);
}

Ref<ITextureUploaderBuilder> GLRenderingContext::createTextureUploader(const uSys textureCount) noexcept
{
    return Ref<GLTextureUploaderBuilder>(new(::std::nothrow) GLTextureUploaderBuilder(textureCount, *this));
}

Ref<ISingleTextureUploaderBuilder> GLRenderingContext::createSingleTextureUploader() noexcept
{
    return Ref<GLSingleTextureUploaderBuilder>(new(::std::nothrow) GLSingleTextureUploaderBuilder(*this));
}

Ref<IShaderBuilder> GLRenderingContext::createShader() noexcept
{
    return Ref<GLShaderBuilder>(new(::std::nothrow) GLShaderBuilder);
}
