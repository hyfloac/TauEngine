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
#include "gl/GLTextureUtils.hpp"
#include "gl/GLFrameBuffer.hpp"
#include "gl/gl4_0/GL4_0BlendingState.hpp"
#include "gl/gl3_0/GL3_0BlendingState.hpp"
#include "gl/gl4_3/GL4_3InputLayout.hpp"
#include "gl/gl4_3/GL4_3VertexArray.hpp"
#include "system/Window.hpp"

namespace tau::gl {

GLGraphicsInterface::GLGraphicsInterface(
    const C8DynString& mode,
    const int majorVersion,
    const int minorVersion,
    const GLProfile compat,
    const bool forwardCompatible
)
    : _majorVersion(majorVersion)
    , _minorVersion(minorVersion)
    , _compat(compat)
    , _forwardCompatible(forwardCompatible)
{
    (void) mode;
    // switch(_mode.currentMode())
    // {
    //     case RenderingMode::Mode::OpenGL4_2:
    //         _inputLayoutBuilder = new(::std::nothrow) GLInputLayoutBuilder;
    //         _vertexArrayBuilder = new(::std::nothrow) GLVertexArrayBuilder;
    //         _bufInterface = new(::std::nothrow) GLBufInterface;
    //         _blendingStateBuilder = new(::std::nothrow) GL4_0BlendingStateBuilder;
    //         break;
    //     case RenderingMode::Mode::OpenGL4_3:
    //     case RenderingMode::Mode::OpenGL4_4:
    //         _inputLayoutBuilder = new(::std::nothrow) GL4_3InputLayoutBuilder;
    //         _vertexArrayBuilder = new(::std::nothrow) GL4_3VertexArrayBuilder;
    //         _bufInterface = new(::std::nothrow) GLBufInterface;
    //         _blendingStateBuilder = new(::std::nothrow) GL4_0BlendingStateBuilder;
    //         break;
    //     case RenderingMode::Mode::OpenGL4_5:
    //     case RenderingMode::Mode::OpenGL4_6:
    //         _inputLayoutBuilder = new(::std::nothrow) GL4_3InputLayoutBuilder;
    //         _vertexArrayBuilder = new(::std::nothrow) GL4_3VertexArrayBuilder;
    //         _bufInterface = new(::std::nothrow) GLBuf4_5Interface;
    //         _blendingStateBuilder = new(::std::nothrow) GL4_0BlendingStateBuilder;
    //         break;
    //     default: break;
    // }
    //
    // _bufferBuilder = new(::std::nothrow) GLBufferBuilder(*_bufInterface);
}

GLGraphicsInterface::~GLGraphicsInterface() noexcept
{
}

void GLGraphicsInterface::DestroySelf() noexcept
{
    TAU_COM_DEFAULT_DESTROY(this);
}

com::EResultCode GLGraphicsInterface::QueryInterface(const com::UUID& iid, void** const pInterface) noexcept
{
    using namespace com;

    if(!pInterface)
    {
        return RC_NullParam;
    }

    if(iid == iid_of<IUnknown>)
    {
        *pInterface = static_cast<IGraphicsInterface*>(this);
    }
    else
    {
        return RC_InterfaceNotFound;
    }

    AddReference();
    return RC_Success;
}

com::EResultCode GLGraphicsInterface::GetPrivateData(const com::UUID& uuid, u32* pDataSize, void* pData) noexcept
{
}

com::EResultCode GLGraphicsInterface::SetPrivateData(const com::UUID& uuid, u32 dataSize, const void* pData) noexcept
{
}

com::EResultCode GLGraphicsInterface::SetPrivateDataInterface(const com::UUID& uuid, IUnknown* pInterface) noexcept
{
}

com::EResultCode GLGraphicsInterface::SetName(const C8DynString& name) noexcept
{
}

const C8DynString& GLGraphicsInterface::RenderingMode() const noexcept
{
}

com::EResultCode GLGraphicsInterface::CreateCommandList(graphics::ICommandList** pCommandList,
    const graphics::ICommandList::ConstructionInfo* constructionInfo)
{
}

}

// IShaderBuilder& GLGraphicsInterface::createShader() noexcept
// { return *_shaderBuilder; }
//
// IShaderProgramBuilder& GLGraphicsInterface::createShaderProgram() noexcept
// { return *_shaderProgramBuilder; }
//
// IInputLayoutBuilder& GLGraphicsInterface::createInputLayout() noexcept
// { return *_inputLayoutBuilder; }
//
// IVertexArrayBuilder& GLGraphicsInterface::createVertexArray() noexcept
// { return *_vertexArrayBuilder; }
//
// IBufferBuilder& GLGraphicsInterface::createBuffer() noexcept
// { return *_bufferBuilder; }
//
// IDepthStencilStateBuilder& GLGraphicsInterface::createDepthStencilState() noexcept
// { return *_depthStencilStateBuilder; }
//
// IRasterizerStateBuilder& GLGraphicsInterface::createRasterizerState() noexcept
// { return *_rasterizerStateBuilder; }
//
// IBlendingStateBuilder& GLGraphicsInterface::createBlendingState() noexcept
// { return *_blendingStateBuilder; }
//
// ITextureBuilder& GLGraphicsInterface::createTexture() noexcept
// { return *_textureBuilder; }
//
// ITextureSamplerBuilder& GLGraphicsInterface::createTextureSampler() noexcept
// { return *_textureSamplerBuilder; }
//
// // TODO: port to ICommandQueue upload path
// // ITextureUploaderBuilder& GLGraphicsInterface::createTextureUploader() noexcept
// // { return *_textureUploaderBuilder; }
//
// IFrameBufferBuilder& GLGraphicsInterface::createFrameBuffer() noexcept
// { return *_frameBufferBuilder; }
//
// IRenderingContextBuilder& GLGraphicsInterface::createRenderingContext() noexcept
// { return *_renderingContextBuilder; }
//
// NullableRef<GLGraphicsInterface> GLGraphicsInterfaceBuilder::build(const GLGraphicsInterfaceArgs& args, TauAllocator& allocator) noexcept
// { return NullableRef<GLGraphicsInterface>(allocator, args.mode, args.majorVersion, args.minorVersion, args.compat, args.forwardCompatible); }
