#include "gl/GLGraphicsInterface.hpp"

#include "gl/GLShader.hpp"
#include "gl/GLDepthStencilState.hpp"
#include "gl/GLRenderingContext.hpp"
#include "gl/GLRasterizerState.hpp"
#include "gl/GLTexture.hpp"
#include "system/Window.hpp"

GLGraphicsInterface::GLGraphicsInterface(const RenderingMode& mode, const int majorVersion, const int minorVersion, const GLProfile compat, const bool forwardCompatible)
    : IGraphicsInterface(mode),
      _majorVersion(majorVersion),
      _minorVersion(minorVersion),
      _compat(compat),
      _forwardCompatible(forwardCompatible),
      _shaderBuilder(new(::std::nothrow) GLShaderBuilder(*this)),
      _depthStencilStateBuilder(new(::std::nothrow) GLDepthStencilStateBuilder),
      _rasterizerStateBuilder(new(::std::nothrow) GLRasterizerStateBuilder),
      _depthTextureBuilder(new(::std::nothrow) GLTextureDepthBuilder),
      _renderingContextBuilder(new(::std::nothrow) GLRenderingContextBuilder(*this))
{ }

GLGraphicsInterface::~GLGraphicsInterface() noexcept
{
    delete _shaderBuilder;
    delete _depthStencilStateBuilder;
    delete _rasterizerStateBuilder;
    delete _renderingContextBuilder;
}

RefDynArray<NullableRef<IGraphicsAccelerator>> GLGraphicsInterface::graphicsAccelerators() noexcept
{
    return RefDynArray<NullableRef<IGraphicsAccelerator>>(0);
}

IShaderBuilder& GLGraphicsInterface::createShader() noexcept
{ return *_shaderBuilder; }

IDepthStencilStateBuilder& GLGraphicsInterface::createDepthStencilState() noexcept
{ return *_depthStencilStateBuilder; }

IRasterizerStateBuilder& GLGraphicsInterface::createRasterizerState() noexcept
{ return *_rasterizerStateBuilder; }

ITextureBuilder& GLGraphicsInterface::createDepthTexture() noexcept
{ return *_depthTextureBuilder; }

IRenderingContextBuilder& GLGraphicsInterface::createRenderingContext() noexcept
{ return *_renderingContextBuilder; }

NullableRef<GLGraphicsInterface> GLGraphicsInterfaceBuilder::build(const GLGraphicsInterfaceArgs& args, TauAllocator& allocator) noexcept
{ return NullableRef<GLGraphicsInterface>(allocator, args.mode, args.majorVersion, args.minorVersion, args.compat, args.forwardCompatible); }
