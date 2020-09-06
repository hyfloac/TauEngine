#include "gl/GLGraphicsInterface.hpp"

#include "gl/gl4_5/GLBuffer4_5.hpp"
#include "gl/GLShader.hpp"
#include "gl/GLShaderProgram.hpp"
#include "gl/GLInputLayout.hpp"
#include "gl/GLVertexArray.hpp"
#include "gl/GLBuffer.hpp"
#include "gl/GLTextureSampler.hpp"
#include "gl/GLDepthStencilState.hpp"
#include "gl/GLRenderingContext.hpp"
#include "gl/GLRasterizerState.hpp"
#include "gl/GLTexture.hpp"
#include "gl/GLTextureUploader.hpp"
#include "gl/GLFrameBuffer.hpp"
#include "gl/gl4_0/GL4_0BlendingState.hpp"
#include "gl/gl3_0/GL3_0BlendingState.hpp"
#include "gl/gl4_3/GL4_3InputLayout.hpp"
#include "gl/gl4_3/GL4_3VertexArray.hpp"
#include "system/Window.hpp"

GLGraphicsInterface::GLGraphicsInterface(const RenderingMode& mode, const int majorVersion, const int minorVersion, const GLProfile compat, const bool forwardCompatible)
    : IGraphicsInterface(mode)
    , _majorVersion(majorVersion)
    , _minorVersion(minorVersion)
    , _compat(compat)
    , _forwardCompatible(forwardCompatible)
    , _shaderInfoExtractor(mode.currentMode())
    , _shaderBuilder(new(::std::nothrow) GLShaderBuilder(&_shaderInfoExtractor))
    , _shaderProgramBuilder(new(::std::nothrow) GLShaderProgramBuilder(&_shaderInfoExtractor))
    , _depthStencilStateBuilder(new(::std::nothrow) GLDepthStencilStateBuilder)
    , _rasterizerStateBuilder(new(::std::nothrow) GLRasterizerStateBuilder)
    , _textureBuilder(new(::std::nothrow) GLTextureBuilder)
    , _textureSamplerBuilder(new(::std::nothrow) GLTextureSamplerBuilder)
    , _textureUploaderBuilder(new(::std::nothrow) GLTextureUploaderBuilder)
    , _frameBufferBuilder(new(::std::nothrow) GLFrameBufferBuilder)
    , _renderingContextBuilder(new(::std::nothrow) GLRenderingContextBuilder(*this))
{
    switch(_mode.currentMode())
    {
        case RenderingMode::Mode::OpenGL4_2:
            _inputLayoutBuilder = new(::std::nothrow) GLInputLayoutBuilder;
            _vertexArrayBuilder = new(::std::nothrow) GLVertexArrayBuilder;
            _bufInterface = new(::std::nothrow) GLBufInterface;
            _blendingStateBuilder = new(::std::nothrow) GL4_0BlendingStateBuilder;
            break;
        case RenderingMode::Mode::OpenGL4_3:
        case RenderingMode::Mode::OpenGL4_4:
            _inputLayoutBuilder = new(::std::nothrow) GL4_3InputLayoutBuilder;
            _vertexArrayBuilder = new(::std::nothrow) GL4_3VertexArrayBuilder;
            _bufInterface = new(::std::nothrow) GLBufInterface;
            _blendingStateBuilder = new(::std::nothrow) GL4_0BlendingStateBuilder;
            break;
        case RenderingMode::Mode::OpenGL4_5:
        case RenderingMode::Mode::OpenGL4_6:
            _inputLayoutBuilder = new(::std::nothrow) GL4_3InputLayoutBuilder;
            _vertexArrayBuilder = new(::std::nothrow) GL4_3VertexArrayBuilder;
            _bufInterface = new(::std::nothrow) GLBuf4_5Interface;
            _blendingStateBuilder = new(::std::nothrow) GL4_0BlendingStateBuilder;
            break;
        default: break;
    }

    _bufferBuilder = new(::std::nothrow) GLBufferBuilder(*_bufInterface);
}

GLGraphicsInterface::~GLGraphicsInterface() noexcept
{
    delete _shaderBuilder;
    delete _shaderProgramBuilder;
    delete _inputLayoutBuilder;
    delete _vertexArrayBuilder;
    delete _bufferBuilder;
    delete _depthStencilStateBuilder;
    delete _rasterizerStateBuilder;
    delete _blendingStateBuilder;
    delete _textureBuilder;
    delete _textureSamplerBuilder;
    delete _textureUploaderBuilder;
    delete _frameBufferBuilder;
    delete _renderingContextBuilder;
}

RefDynArray<NullableRef<IGraphicsAccelerator>> GLGraphicsInterface::graphicsAccelerators() noexcept
{ return RefDynArray<NullableRef<IGraphicsAccelerator>>(0); }

IShaderBuilder& GLGraphicsInterface::createShader() noexcept
{ return *_shaderBuilder; }

IShaderProgramBuilder& GLGraphicsInterface::createShaderProgram() noexcept
{ return *_shaderProgramBuilder; }

IInputLayoutBuilder& GLGraphicsInterface::createInputLayout() noexcept
{ return *_inputLayoutBuilder; }

IVertexArrayBuilder& GLGraphicsInterface::createVertexArray() noexcept
{ return *_vertexArrayBuilder; }

IBufferBuilder& GLGraphicsInterface::createBuffer() noexcept
{ return *_bufferBuilder; }

IDepthStencilStateBuilder& GLGraphicsInterface::createDepthStencilState() noexcept
{ return *_depthStencilStateBuilder; }

IRasterizerStateBuilder& GLGraphicsInterface::createRasterizerState() noexcept
{ return *_rasterizerStateBuilder; }

IBlendingStateBuilder& GLGraphicsInterface::createBlendingState() noexcept
{ return *_blendingStateBuilder; }

ITextureBuilder& GLGraphicsInterface::createTexture() noexcept
{ return *_textureBuilder; }

ITextureSamplerBuilder& GLGraphicsInterface::createTextureSampler() noexcept
{ return *_textureSamplerBuilder; }

ITextureUploaderBuilder& GLGraphicsInterface::createTextureUploader() noexcept
{ return *_textureUploaderBuilder; }

IFrameBufferBuilder& GLGraphicsInterface::createFrameBuffer() noexcept
{ return *_frameBufferBuilder; }

IRenderingContextBuilder& GLGraphicsInterface::createRenderingContext() noexcept
{ return *_renderingContextBuilder; }

NullableRef<GLGraphicsInterface> GLGraphicsInterfaceBuilder::build(const GLGraphicsInterfaceArgs& args, TauAllocator& allocator) noexcept
{ return NullableRef<GLGraphicsInterface>(allocator, args.mode, args.majorVersion, args.minorVersion, args.compat, args.forwardCompatible); }
