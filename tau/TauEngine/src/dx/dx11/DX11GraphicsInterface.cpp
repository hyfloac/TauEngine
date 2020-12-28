#include "dx/dx11/DX11GraphicsInterface.hpp"

#ifdef _WIN32
#include <dxgi.h>
#include "dx/dxgi/DXGI13GraphicsAccelerator.hpp"
#include "dx/dx11/DX11Shader.hpp"
#include "dx/dx11/DX11InputLayout.hpp"
#include "dx/dx11/DX11VertexArray.hpp"
#include "dx/dx11/DX11Buffer.hpp"
#include "dx/dx11/DX11DepthStencilState.hpp"
#include "dx/dx11/DX11RasterizerState.hpp"
#include "dx/dx11/DX11BlendingState.hpp"
#include "dx/dx11/DX11RenderingContext.hpp"
#include "dx/dx11/DX11Texture.hpp"
#include "dx/dx11/DX11TextureSampler.hpp"
#include "dx/dx11/DX11TextureUploader.hpp"
#include "dx/dx11/DX11FrameBuffer.hpp"
#include "system/Window.hpp"

DX11GraphicsInterface::DX11GraphicsInterface(const RenderingMode& mode, ID3D11Device* const d3dDevice) noexcept
    : IGraphicsInterface(mode)
    , _d3d11Device(d3dDevice)
    , _shaderInfoExtractor(mode.currentMode())
    , _shaderBuilder(new(::std::nothrow) DX11ShaderBuilder(*this, &_shaderInfoExtractor))
    , _inputLayoutBuilder(new(::std::nothrow) DX11InputLayoutBuilder(*this))
    , _vertexArrayBuilder(new(::std::nothrow) DX11VertexArrayBuilder)
    , _bufferBuilder(new(::std::nothrow) DX11BufferBuilder(*this))
    , _depthStencilStateBuilder(new(::std::nothrow) DX11DepthStencilStateBuilder(*this))
    , _rasterizerStateBuilder(new(::std::nothrow) DX11RasterizerStateBuilder(*this))
    , _blendingStateBuilder(new(::std::nothrow) DX11BlendingStateBuilder(*this))
    , _textureBuilder(new(::std::nothrow) DX11TextureBuilder(*this))
    , _textureSamplerBuilder(new(::std::nothrow) DX11TextureSamplerBuilder(*this))
    , _textureUploaderBuilder(new(::std::nothrow) DX11TextureUploaderBuilder)
    , _frameBufferBuilder(new(::std::nothrow) DX11FrameBufferBuilder(*this))
    , _renderingContextBuilder(new(::std::nothrow) DX11RenderingContextBuilder(*this))
{ }

DX11GraphicsInterface::~DX11GraphicsInterface() noexcept
{
    _d3d11Device->Release();
    _d3d11Device = null;

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

RefDynArray<NullableRef<IGraphicsAccelerator>> DX11GraphicsInterface::graphicsAccelerators() noexcept
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
        accelerators[i] = RefCast<IGraphicsAccelerator>(builder.build());
        dxgiAdapter->Release();
    }

    dxgiFactory->Release();

    return accelerators;
}

IShaderBuilder& DX11GraphicsInterface::createShader() noexcept
{ return *_shaderBuilder; }

IInputLayoutBuilder& DX11GraphicsInterface::createInputLayout() noexcept
{ return *_inputLayoutBuilder; }

IVertexArrayBuilder& DX11GraphicsInterface::createVertexArray() noexcept
{ return *_vertexArrayBuilder; }

IBufferBuilder& DX11GraphicsInterface::createBuffer() noexcept
{ return *_bufferBuilder; }

IDepthStencilStateBuilder& DX11GraphicsInterface::createDepthStencilState() noexcept
{ return *_depthStencilStateBuilder; }

IRasterizerStateBuilder& DX11GraphicsInterface::createRasterizerState() noexcept
{ return *_rasterizerStateBuilder; }

IBlendingStateBuilder& DX11GraphicsInterface::createBlendingState() noexcept
{ return *_blendingStateBuilder; }

ITextureBuilder& DX11GraphicsInterface::createTexture() noexcept
{ return *_textureBuilder; }

ITextureSamplerBuilder& DX11GraphicsInterface::createTextureSampler() noexcept
{ return *_textureSamplerBuilder; }

ITextureUploaderBuilder& DX11GraphicsInterface::createTextureUploader() noexcept
{ return *_textureUploaderBuilder; }

IFrameBufferBuilder& DX11GraphicsInterface::createFrameBuffer() noexcept
{ return *_frameBufferBuilder; }

IRenderingContextBuilder& DX11GraphicsInterface::createRenderingContext() noexcept
{ return *_renderingContextBuilder; }

NullableRef<DX11GraphicsInterface> DX11GraphicsInterfaceBuilder::build(const GraphicsInterfaceArgs& args, TauAllocator& allocator) noexcept
{
    IDXGIAdapter1* dxgiAdapter = nullptr;

    if(args.graphicsAccelerator)
    {
        if(!rtt_check<DXGI13GraphicsAccelerator>(args.graphicsAccelerator))
        { return nullptr; }

        const DXGI13GraphicsAccelerator* const gpu = static_cast<const DXGI13GraphicsAccelerator*>(args.graphicsAccelerator.get());
        dxgiAdapter = gpu->dxgiAdapter();
    }

    D3D_FEATURE_LEVEL featureLevels[1] = { D3D_FEATURE_LEVEL_11_0 };

    ID3D11Device* device;
    const HRESULT h = D3D11CreateDevice(dxgiAdapter, D3D_DRIVER_TYPE_HARDWARE, NULL, args.renderingMode.debugMode() ? D3D11_CREATE_DEVICE_DEBUG : 0, featureLevels, 1, D3D11_SDK_VERSION, &device, nullptr, nullptr);

    if(FAILED(h))
    { return nullptr; }

    return NullableRef<DX11GraphicsInterface>(allocator, args.renderingMode, device);
}
#endif
