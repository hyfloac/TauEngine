#include "dx/dx10/DX10GraphicsInterface.hpp"

#include "dx/dx10/DX10GraphicsAccelerator.hpp"

#ifdef _WIN32
#include <dxgi.h>
#include "dx/dx10/DX10Shader.hpp"
#include "dx/dx10/DX10ShaderProgram.hpp"
#include "dx/dx10/DX10InputLayout.hpp"
#include "dx/dx10/DX10VertexArray.hpp"
#include "dx/dx10/DX10Buffer.hpp"
#include "dx/dx10/DX10DepthStencilState.hpp"
#include "dx/dx10/DX10RasterizerState.hpp"
#include "dx/dx10/DX10BlendingState.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10Texture.hpp"
#include "dx/dx10/DX10TextureSampler.hpp"
#include "dx/dx10/DX10TextureUploader.hpp"
#include "dx/dx10/DX10FrameBuffer.hpp"
#include "system/Window.hpp"

DX10GraphicsInterface::DX10GraphicsInterface(const RenderingMode& mode, ID3D10Device* const d3dDevice) noexcept
    : IGraphicsInterface(mode)
    , _d3d10Device(d3dDevice)
    , _shaderInfoExtractor(mode.currentMode())
    , _shaderBuilder(new(::std::nothrow) DX10ShaderBuilder(*this, &_shaderInfoExtractor))
    , _shaderBuilder(new(::std::nothrow) DX10ShaderProgramBuilder(&_shaderInfoExtractor))
    , _inputLayoutBuilder(new(::std::nothrow) DX10InputLayoutBuilder(*this))
    , _vertexArrayBuilder(new(::std::nothrow) DX10VertexArrayBuilder)
    , _bufferBuilder(new(::std::nothrow) DX10BufferBuilder(*this))
    , _depthStencilStateBuilder(new(::std::nothrow) DX10DepthStencilStateBuilder(*this))
    , _rasterizerStateBuilder(new(::std::nothrow) DX10RasterizerStateBuilder(*this))
    , _blendingStateBuilder(new(::std::nothrow) DX10BlendingStateBuilder(*this))
    , _textureBuilder(new(::std::nothrow) DX10TextureBuilder(*this))
    , _textureSamplerBuilder(new(::std::nothrow) DX10TextureSamplerBuilder(*this))
    , _textureUploaderBuilder(new(::std::nothrow) DX10TextureUploaderBuilder)
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
    delete _textureBuilder;
    delete _textureSamplerBuilder;
    delete _textureUploaderBuilder;
    delete _frameBufferBuilder;
    delete _renderingContextBuilder;
}

RefDynArray<NullableRef<IGraphicsAccelerator>> DX10GraphicsInterface::graphicsAccelerators() noexcept
{
#define CHECK(_VAL) do { if(FAILED(_VAL)) { return RefDynArray<NullableRef<IGraphicsAccelerator>>(0); } } while(0)

    IDXGIFactory* dxgiFactory;
    CHECK(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory)));

    IDXGIAdapter* dxgiAdapter;
    UINT i;
    for(i = 0; dxgiFactory->EnumAdapters(i, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        dxgiAdapter->Release();
    }

    RefDynArray<NullableRef<IGraphicsAccelerator>> accelerators(i);

    DX10GraphicsAcceleratorBuilder builder{};

    const UINT maxAdapter = i;
    for(i = 0; i < maxAdapter; ++i)
    {
        CHECK(dxgiFactory->EnumAdapters(i, &dxgiAdapter));
        builder.setAdapter(dxgiAdapter);
        accelerators[i] = builder.build();
        dxgiAdapter->Release();
    }

    dxgiFactory->Release();

    return accelerators;
}

IShaderBuilder& DX10GraphicsInterface::createShader() noexcept
{ return *_shaderBuilder; }

IShaderProgramBuilder& DX10GraphicsInterface::createShaderProgram() noexcept
{ return *_shaderProgramBuilder; }

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

ITextureBuilder& DX10GraphicsInterface::createTexture() noexcept
{ return *_textureBuilder; }

ITextureSamplerBuilder& DX10GraphicsInterface::createTextureSampler() noexcept
{ return *_textureSamplerBuilder; }

ITextureUploaderBuilder& DX10GraphicsInterface::createTextureUploader() noexcept
{ return *_textureUploaderBuilder; }

IFrameBufferBuilder& DX10GraphicsInterface::createFrameBuffer() noexcept
{ return *_frameBufferBuilder; }

IRenderingContextBuilder& DX10GraphicsInterface::createRenderingContext() noexcept
{ return *_renderingContextBuilder; }

NullableRef<DX10GraphicsInterface> DX10GraphicsInterfaceBuilder::build(const GraphicsInterfaceArgs& args, TauAllocator& allocator) noexcept
{
    ID3D10Device* device;
    const HRESULT h = D3D10CreateDevice(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, args.renderingMode.debugMode() ? D3D10_CREATE_DEVICE_DEBUG : 0, D3D10_SDK_VERSION, &device);

    if(FAILED(h))
    { return null; }

    return NullableRef<DX10GraphicsInterface>(allocator, args.renderingMode, device);
}
#endif
