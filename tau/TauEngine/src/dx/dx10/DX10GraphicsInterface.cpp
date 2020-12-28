#include "dx/dx10/DX10GraphicsInterface.hpp"

#ifdef _WIN32
#include <dxgi.h>
#include "dx/dxgi/DXGI11GraphicsAccelerator.hpp"
#include "dx/dx10/DX10Shader.hpp"
#include "dx/dx10/DX10ShaderProgram.hpp"
#include "dx/dx10/DX10Resource.hpp"
#include "dx/dx10/DX10InputLayout.hpp"
#include "dx/dx10/DX10VertexArray.hpp"
#include "dx/dx10/DX10Buffer.hpp"
#include "dx/dx10/DX10DepthStencilState.hpp"
#include "dx/dx10/DX10RasterizerState.hpp"
#include "dx/dx10/DX10BlendingState.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10TextureSampler.hpp"
#include "dx/dx10/DX10FrameBuffer.hpp"
#include "system/Window.hpp"

DX10GraphicsInterface::DX10GraphicsInterface(const RenderingMode& mode, ID3D10Device* const d3dDevice) noexcept
    : IGraphicsInterface(mode)
    , _d3d10Device(d3dDevice)
    , _shaderInfoExtractor(mode.currentMode())
    , _shaderBuilder(new(::std::nothrow) DX10ShaderBuilder(*this, &_shaderInfoExtractor))
    , _shaderProgramBuilder(new(::std::nothrow) DX10ShaderProgramBuilder(&_shaderInfoExtractor))
    , _resourceBuilder(new(::std::nothrow) DX10ResourceBuilder(*this))
    , _inputLayoutBuilder(new(::std::nothrow) DX10InputLayoutBuilder(*this))
    , _vertexArrayBuilder(new(::std::nothrow) DX10VertexArrayBuilder)
    , _bufferBuilder(new(::std::nothrow) DX10BufferBuilder(*this))
    , _depthStencilStateBuilder(new(::std::nothrow) DX10DepthStencilStateBuilder(*this))
    , _rasterizerStateBuilder(new(::std::nothrow) DX10RasterizerStateBuilder(*this))
    , _blendingStateBuilder(new(::std::nothrow) DX10BlendingStateBuilder(*this))
    , _textureSamplerBuilder(new(::std::nothrow) DX10TextureSamplerBuilder(*this))
    , _frameBufferBuilder(new(::std::nothrow) DX10FrameBufferBuilder(*this))
    , _renderingContextBuilder(new(::std::nothrow) DX10RenderingContextBuilder(*this))
{ }

DX10GraphicsInterface::~DX10GraphicsInterface() noexcept
{
    _d3d10Device->Release();
    _d3d10Device = null;

    delete _shaderBuilder;
    delete _inputLayoutBuilder;
    delete _vertexArrayBuilder;
    delete _bufferBuilder;
    delete _depthStencilStateBuilder;
    delete _rasterizerStateBuilder;
    delete _blendingStateBuilder;
    delete _textureSamplerBuilder;
    delete _frameBufferBuilder;
    delete _renderingContextBuilder;
}

IShaderBuilder& DX10GraphicsInterface::createShader() noexcept
{ return *_shaderBuilder; }

IShaderProgramBuilder& DX10GraphicsInterface::createShaderProgram() noexcept
{ return *_shaderProgramBuilder; }

IResourceBuilder& DX10GraphicsInterface::createResource() noexcept
{ return *_resourceBuilder; }

IInputLayoutBuilder& DX10GraphicsInterface::createInputLayout() noexcept
{ return *_inputLayoutBuilder; }

IVertexArrayBuilder& DX10GraphicsInterface::createVertexArray() noexcept
{ return *_vertexArrayBuilder; }

IBufferBuilder& DX10GraphicsInterface::createBuffer() noexcept
{ return *_bufferBuilder; }

IDepthStencilStateBuilder& DX10GraphicsInterface::createDepthStencilState() noexcept
{ return *_depthStencilStateBuilder; }

IRasterizerStateBuilder& DX10GraphicsInterface::createRasterizerState() noexcept
{ return *_rasterizerStateBuilder; }

IBlendingStateBuilder& DX10GraphicsInterface::createBlendingState() noexcept
{ return *_blendingStateBuilder; }

ITextureSamplerBuilder& DX10GraphicsInterface::createTextureSampler() noexcept
{ return *_textureSamplerBuilder; }

IFrameBufferBuilder& DX10GraphicsInterface::createFrameBuffer() noexcept
{ return *_frameBufferBuilder; }

IRenderingContextBuilder& DX10GraphicsInterface::createRenderingContext() noexcept
{ return *_renderingContextBuilder; }

NullableRef<DX10GraphicsInterface> DX10GraphicsInterfaceBuilder::build(const GraphicsInterfaceArgs& args, TauAllocator& allocator) noexcept
{
    IDXGIAdapter1* dxgiAdapter = nullptr;

    if(args.graphicsAccelerator)
    {
        if(!rtt_check<DXGI11GraphicsAccelerator>(args.graphicsAccelerator))
        { return nullptr; }

        const DXGI11GraphicsAccelerator* const gpu = static_cast<const DXGI11GraphicsAccelerator*>(args.graphicsAccelerator.get());
        dxgiAdapter = gpu->dxgiAdapter();
    }

    ID3D10Device* device;
    const HRESULT h = D3D10CreateDevice(dxgiAdapter, D3D10_DRIVER_TYPE_HARDWARE, nullptr, args.renderingMode.debugMode() ? D3D10_CREATE_DEVICE_DEBUG : 0, D3D10_SDK_VERSION, &device);

    if(FAILED(h))
    { return nullptr; }

    return NullableRef<DX10GraphicsInterface>(allocator, args.renderingMode, device);
}
#endif
