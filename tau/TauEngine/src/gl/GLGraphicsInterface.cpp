#include "gl/GLGraphicsInterface.hpp"


#include "gl/gl4_5/GLBuffer4_5.hpp"
#include "gl/GLShader.hpp"
#include "gl/GLVertexArray.hpp"
#include "gl/GLBuffer.hpp"
#include "gl/GLTextureSampler.hpp"
#include "gl/GLDepthStencilState.hpp"
#include "gl/GLRenderingContext.hpp"
#include "gl/GLRasterizerState.hpp"
#include "gl/GLTexture.hpp"
#include "gl/GLTextureUploader.hpp"
#include "gl/GLFrameBuffer.hpp"
#include "system/Window.hpp"

GLGraphicsInterface::GLGraphicsInterface(const RenderingMode& mode, const int majorVersion, const int minorVersion, const GLProfile compat, const bool forwardCompatible)
    : IGraphicsInterface(mode)
    , _majorVersion(majorVersion)
    , _minorVersion(minorVersion)
    , _compat(compat)
    , _forwardCompatible(forwardCompatible)
    , _shaderBuilder(new(::std::nothrow) GLShaderBuilder(*this))
    , _vertexArrayBuilder(new(::std::nothrow) GLVertexArrayBuilder)
    , _depthStencilStateBuilder(new(::std::nothrow) GLDepthStencilStateBuilder)
    , _rasterizerStateBuilder(new(::std::nothrow) GLRasterizerStateBuilder)
    , _textureBuilder(new(::std::nothrow) GLTextureBuilder)
    , _textureSamplerBuilder(new(::std::nothrow) GLTextureSamplerBuilder)
    , _singleTextureUploaderBuilder(new(::std::nothrow) GLSingleTextureUploaderBuilder)
    , _textureUploaderBuilder(new(::std::nothrow) GLTextureUploaderBuilder)
    , _frameBufferBuilder(new(::std::nothrow) GLFrameBufferBuilder)
    , _renderingContextBuilder(new(::std::nothrow) GLRenderingContextBuilder(*this))
{
    switch(_mode.currentMode())
    {
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
}

GLGraphicsInterface::~GLGraphicsInterface() noexcept
{
    delete _shaderBuilder;
    delete _vertexArrayBuilder;
    delete _bufferBuilder;
    delete _indexBufferBuilder;
    delete _uniformBufferBuilder;
    delete _depthStencilStateBuilder;
    delete _rasterizerStateBuilder;
    delete _textureBuilder;
    delete _textureSamplerBuilder;
    delete _singleTextureUploaderBuilder;
    delete _textureUploaderBuilder;
    delete _frameBufferBuilder;
    delete _renderingContextBuilder;
}

RefDynArray<NullableRef<IGraphicsAccelerator>> GLGraphicsInterface::graphicsAccelerators() noexcept
{ return RefDynArray<NullableRef<IGraphicsAccelerator>>(0); }

IShaderBuilder& GLGraphicsInterface::createShader() noexcept
{ return *_shaderBuilder; }

IVertexArrayBuilder& GLGraphicsInterface::createVertexArray() noexcept
{ return *_vertexArrayBuilder; }

IBufferBuilder& GLGraphicsInterface::createBuffer() noexcept
{ return *_bufferBuilder; }

IIndexBufferBuilder& GLGraphicsInterface::createIndexBuffer() noexcept
{ return *_indexBufferBuilder; }

IUniformBufferBuilder& GLGraphicsInterface::createUniformBuffer() noexcept
{ return *_uniformBufferBuilder; }

IDepthStencilStateBuilder& GLGraphicsInterface::createDepthStencilState() noexcept
{ return *_depthStencilStateBuilder; }

IRasterizerStateBuilder& GLGraphicsInterface::createRasterizerState() noexcept
{ return *_rasterizerStateBuilder; }

ITextureBuilder& GLGraphicsInterface::createTexture() noexcept
{ return *_textureBuilder; }

ITextureSamplerBuilder& GLGraphicsInterface::createTextureSampler() noexcept
{ return *_textureSamplerBuilder; }

ISingleTextureUploaderBuilder& GLGraphicsInterface::createSingleTextureUploader() noexcept
{ return *_singleTextureUploaderBuilder; }

ITextureUploaderBuilder& GLGraphicsInterface::createTextureUploader() noexcept
{ return *_textureUploaderBuilder; }

IFrameBufferBuilder& GLGraphicsInterface::createFrameBuffer() noexcept
{ return *_frameBufferBuilder; }

IRenderingContextBuilder& GLGraphicsInterface::createRenderingContext() noexcept
{ return *_renderingContextBuilder; }

NullableRef<GLGraphicsInterface> GLGraphicsInterfaceBuilder::build(const GLGraphicsInterfaceArgs& args, TauAllocator& allocator) noexcept
{ return NullableRef<GLGraphicsInterface>(allocator, args.mode, args.majorVersion, args.minorVersion, args.compat, args.forwardCompatible); }
