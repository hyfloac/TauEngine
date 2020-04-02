#include "dx/dx11/DX11GraphicsInterface.hpp"

#ifdef _WIN32
#include <dxgi.h>
#include "dx/dx10/DX10GraphicsAccelerator.hpp"
#include "dx/dx11/DX11Shader.hpp"
#include "dx/dx11/DX11DepthStencilState.hpp"
#include "dx/dx11/DX11RasterizerState.hpp"
#include "dx/dx11/DX11RenderingContext.hpp"
#include "system/Window.hpp"

DX11GraphicsInterface::DX11GraphicsInterface(const RenderingMode& mode, ID3D11Device* const d3dDevice) noexcept
    : IGraphicsInterface(mode), _d3d11Device(d3dDevice),
      _shaderBuilder(new(::std::nothrow) DX11ShaderBuilder(*this)),
      _depthStencilStateBuilder(new(::std::nothrow) DX11DepthStencilStateBuilder(*this)),
      _rasterizerStateBuilder(new(::std::nothrow) DX11RasterizerStateBuilder(*this)),
      _renderingContextBuilder(new(::std::nothrow) DX11RenderingContextBuilder(*this))
{ }

DX11GraphicsInterface::~DX11GraphicsInterface() noexcept
{
    _d3d11Device->Release();
    _d3d11Device = null;

    delete _shaderBuilder;
    delete _depthStencilStateBuilder;
    delete _rasterizerStateBuilder;
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

IDepthStencilStateBuilder& DX11GraphicsInterface::createDepthStencilState() noexcept
{ return *_depthStencilStateBuilder; }

IRasterizerStateBuilder& DX11GraphicsInterface::createRasterizerState() noexcept
{ return *_rasterizerStateBuilder; }

ITextureBuilder& DX11GraphicsInterface::createDepthTexture() noexcept
{ return *static_cast<ITextureBuilder*>(null); }

ISingleTextureUploaderBuilder& DX11GraphicsInterface::createSingleTextureUploader() noexcept
{ return *static_cast<ISingleTextureUploaderBuilder*>(null); }

ITextureUploaderBuilder& DX11GraphicsInterface::createTextureUploader() noexcept
{ return *static_cast<ITextureUploaderBuilder*>(null); }

IRenderingContextBuilder& DX11GraphicsInterface::createRenderingContext() noexcept
{ return *_renderingContextBuilder; }

NullableRef<DX11GraphicsInterface> DX11GraphicsInterfaceBuilder::build(const DX11GraphicsInterfaceArgs& args, TauAllocator& allocator) noexcept
{
    D3D_FEATURE_LEVEL featureLevels[1] = { D3D_FEATURE_LEVEL_11_0 };

    ID3D11Device* device;
    const HRESULT h = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, args.mode.debugMode() ? D3D11_CREATE_DEVICE_DEBUG : 0, featureLevels, 1, D3D11_SDK_VERSION, &device, NULL, NULL);

    if(FAILED(h))
    { return null; }

    return NullableRef<DX11GraphicsInterface>(allocator, args.mode, device);
}
#endif
