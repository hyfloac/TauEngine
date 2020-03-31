#include "dx/dx10/DX10GraphicsInterface.hpp"

#ifdef _WIN32
#include <dxgi.h>
#include "dx/dx10/DX10GraphicsAccelerator.hpp"
#include "dx/dx10/DX10Shader.hpp"
#include "dx/dx10/DX10DepthStencilState.hpp"
#include "dx/dx10/DX10RasterizerState.hpp"
#include "dx/dx10/DX10RenderingContext.hpp"
#include "dx/dx10/DX10Texture.hpp"
#include "system/Window.hpp"

DX10GraphicsInterface::DX10GraphicsInterface(const RenderingMode& mode, ID3D10Device* const d3dDevice) noexcept
    : IGraphicsInterface(mode), _d3d10Device(d3dDevice),
      _shaderBuilder(new(::std::nothrow) DX10ShaderBuilder(*this)),
      _depthStencilStateBuilder(new(::std::nothrow) DX10DepthStencilStateBuilder(*this)),
      _rasterizerStateBuilder(new(::std::nothrow) DX10RasterizerStateBuilder(*this)),
      _depthTextureBuilder(new(::std::nothrow) DX10DepthTextureBuilder(*this)),
      _renderingContextBuilder(new(::std::nothrow) DX10RenderingContextBuilder(*this))
{ }

DX10GraphicsInterface::~DX10GraphicsInterface() noexcept
{
    _d3d10Device->Release();
    _d3d10Device = null;

    delete _shaderBuilder;
    delete _depthStencilStateBuilder;
    delete _rasterizerStateBuilder;
    delete _depthTextureBuilder;
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
        accelerators[i] = RefCast<IGraphicsAccelerator>(builder.build());
        dxgiAdapter->Release();
    }

    dxgiFactory->Release();

    return accelerators;
}

IShaderBuilder& DX10GraphicsInterface::createShader() noexcept
{ return *_shaderBuilder; }

IDepthStencilStateBuilder& DX10GraphicsInterface::createDepthStencilState() noexcept
{ return *_depthStencilStateBuilder; }

IRasterizerStateBuilder& DX10GraphicsInterface::createRasterizerState() noexcept
{ return *_rasterizerStateBuilder; }

ITextureBuilder& DX10GraphicsInterface::createDepthTexture() noexcept
{ return *_depthTextureBuilder; }

IRenderingContextBuilder& DX10GraphicsInterface::createRenderingContext() noexcept
{ return *_renderingContextBuilder; }

NullableRef<DX10GraphicsInterface> DX10GraphicsInterfaceBuilder::build(const DX10GraphicsInterfaceArgs& args, TauAllocator& allocator) noexcept
{
    ID3D10Device* device;
    const HRESULT h = D3D10CreateDevice(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, args.mode.debugMode() ? D3D10_CREATE_DEVICE_DEBUG : 0, D3D10_SDK_VERSION, &device);

    if(FAILED(h))
    { return null; }

    return NullableRef<DX10GraphicsInterface>(allocator, args.mode, device);
}
#endif
