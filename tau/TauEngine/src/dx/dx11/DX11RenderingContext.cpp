#define __SDTC_WANT_LIB_EXT1__ 1
#include <cstdlib>
#include "dx/dx11/DX11RenderingContext.hpp"

#ifdef _WIN32
// #include <D3DX11Core.h>

#include <Utils.hpp>
#include "system/Window.hpp"
#include "system/SystemInterface.hpp"
#include "dx/dx11/DX11DepthStencilState.hpp"
#include "dx/dx11/DX11Buffer.hpp"
#include "dx/dx11/DX11Shader.hpp"
#include "dx/dx11/DX11VertexArray.hpp"

DX11RenderingContext::DX11RenderingContext(const RenderingMode& mode) noexcept
    : IRenderingContext(mode),
      _d3d11Device(null),
      _d3d11DeviceContext(null),
      _renderTargetView(null),
      _depthStencilBuffer(null), _depthStencilState(null), _depthStencilView(null),
      _rasterizerState(null),
      _swapChain(null),
      _vsync(false),
      _defaultDepthStencilState(null), _currentDepthStencilState(null),
      _bufferBuilder(new(::std::nothrow) DX11BufferBuilder(*this)),
      _indexBufferBuilder(new(::std::nothrow) DX11IndexBufferBuilder(*this)),
      _uniformBufferBuilder(new(::std::nothrow) DX11UniformBufferBuilder(*this)),
      _shaderBuilder(new(::std::nothrow) DX11ShaderBuilder(*this)),
      _depthStencilStateBuilder(new(::std::nothrow) DX11DepthStencilStateBuilder(*this))
{ }

DX11RenderingContext::~DX11RenderingContext() noexcept
{
#define RELEASE(_OBJ) do { \
    if(_OBJ) {\
        (_OBJ)->Release(); \
        (_OBJ) = null; \
    } } while(0)

    RELEASE(_d3d11Device);
    RELEASE(_d3d11DeviceContext);
    RELEASE(_renderTargetView);
    RELEASE(_depthStencilBuffer);
    RELEASE(_depthStencilState);
    RELEASE(_depthStencilView);
    RELEASE(_rasterizerState);
    RELEASE(_swapChain);
#undef RELEASE
#define RELEASE(_OBJ) do { \
    if(_OBJ) {\
        delete (_OBJ); \
        (_OBJ) = null; \
    } } while(0)

#undef RELEASE
}

bool DX11RenderingContext::createContext(Window& window) noexcept
{
#define CHECK(_VAL) do { if(FAILED(_VAL)) { return false; } } while(0)

    HWND hWnd = window.sysWindowContainer().windowHandle;

    DXGI_RATIONAL refreshRate = { 0, 1 };

    {
        NullableRef<IGraphicsInterface> graphicsInterface = SystemInterface::get()->createGraphicsInterface(_mode);
        auto gpuList = graphicsInterface->graphicsAccelerators();
        if(gpuList.count() == 0) { return false; }

        auto displayList = gpuList[0]->graphicsDisplays();
        if(displayList.count() == 0) { return false; }

        auto displayModeList = displayList[0]->displayModes();
        if(displayModeList.count() == 0) { return false; }

        for(uSys i = 0; i < displayModeList.length(); ++i)
        {
            if(displayModeList[i].width == window.width() &
               displayModeList[i].height == window.height())
            {
                refreshRate.Numerator = displayModeList[i].refreshRateNumerator;
                refreshRate.Denominator = displayModeList[i].refreshRateDenominator;
                break;
            }
        }
    }

    {
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

        swapChainDesc.BufferCount = 1;
        swapChainDesc.BufferDesc.Width = window.width();
        swapChainDesc.BufferDesc.Height = window.height();
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

        if(_vsync)
        {
            swapChainDesc.BufferDesc.RefreshRate = refreshRate;
        }
        else
        {
            swapChainDesc.BufferDesc.RefreshRate = { 0, 1 };
        }

        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

        swapChainDesc.OutputWindow = hWnd;

        // Disable MSAA
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;

        constexpr bool fullScreen = false;
        swapChainDesc.Windowed = !fullScreen;

        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDesc.Flags = 0;

        D3D_FEATURE_LEVEL featureLevels[1] = { D3D_FEATURE_LEVEL_11_0 };
        CHECK(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, _mode.debugMode() ? D3D11_CREATE_DEVICE_DEBUG : 0, featureLevels, 1, D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_d3d11Device, NULL, &_d3d11DeviceContext));
    }

    {
        // Get pointer to back buffer;
        ID3D11Texture2D* backBufferPtr;
        CHECK(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferPtr)));

        CHECK(_d3d11Device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView));

        backBufferPtr->Release();
    }

    {
        D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
        ZeroMemory(&depthStencilBufferDesc, sizeof(depthStencilBufferDesc));
        depthStencilBufferDesc.Width = window.width();
        depthStencilBufferDesc.Height = window.height();
        depthStencilBufferDesc.MipLevels = 1;
        depthStencilBufferDesc.ArraySize = 1;
        depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilBufferDesc.SampleDesc.Count = 1;
        depthStencilBufferDesc.SampleDesc.Quality = 0;
        depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilBufferDesc.CPUAccessFlags = 0;
        depthStencilBufferDesc.MiscFlags = 0;

        CHECK(_d3d11Device->CreateTexture2D(&depthStencilBufferDesc, NULL, &_depthStencilBuffer));
    }

    {
        DepthStencilArgs dsArgs;
        dsArgs.enableDepthTest = true;
        dsArgs.enableStencilTest = true;

        dsArgs.depthWriteMask = DepthStencilArgs::DepthWriteMask::All;
        dsArgs.depthCompareFunc = DepthStencilArgs::CompareFunc::LessThan;

        dsArgs.stencilReadMask = 0xFF;
        dsArgs.stencilWriteMask = 0xFF;

        dsArgs.frontFace.failOp = DepthStencilArgs::StencilOp::Keep;
        dsArgs.frontFace.stencilPassDepthFailOp = DepthStencilArgs::StencilOp::IncrementClamp;
        dsArgs.frontFace.passOp = DepthStencilArgs::StencilOp::Keep;
        dsArgs.frontFace.compareFunc = DepthStencilArgs::CompareFunc::Always;

        dsArgs.backFace.failOp = DepthStencilArgs::StencilOp::Keep;
        dsArgs.backFace.stencilPassDepthFailOp = DepthStencilArgs::StencilOp::DecrementClamp;
        dsArgs.backFace.passOp = DepthStencilArgs::StencilOp::Keep;
        dsArgs.backFace.compareFunc = DepthStencilArgs::CompareFunc::Always;

        NullableRef<IDepthStencilState> dsState = createDepthStencilState().buildTauRef(dsArgs, null);
        _defaultDepthStencilState = RefCast<DX11DepthStencilState>(dsState);
        _currentDepthStencilState = _defaultDepthStencilState;
        _currentDepthStencilState->apply(*this);
    }

    {
        D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc[2];
        ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
        depthStencilViewDesc[0].Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc[0].ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc[0].Texture2D.MipSlice = 0;
        depthStencilViewDesc[1].Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc[1].ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc[1].Texture2D.MipSlice = 0;

        CHECK(_d3d11Device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc[0], &_depthStencilView));
        _d3d11DeviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
    }

    {
        D3D11_RASTERIZER_DESC rasterDesc;
        rasterDesc.FillMode = D3D11_FILL_SOLID;
        rasterDesc.CullMode = D3D11_CULL_BACK;
        // rasterDesc.CullMode = D3D11_CULL_NONE;
        rasterDesc.FrontCounterClockwise = false;
        rasterDesc.DepthBias = 0;
        rasterDesc.DepthBiasClamp = 0.0f;
        rasterDesc.SlopeScaledDepthBias = 0.0f;
        rasterDesc.DepthClipEnable = true;
        rasterDesc.ScissorEnable = false;
        rasterDesc.MultisampleEnable = false;
        rasterDesc.AntialiasedLineEnable = false;

        CHECK(_d3d11Device->CreateRasterizerState(&rasterDesc, &_rasterizerState));
        _d3d11DeviceContext->RSSetState(_rasterizerState);
    }

    {
        D3D11_VIEWPORT viewport = { 0, 0, static_cast<FLOAT>(window.width()), static_cast<FLOAT>(window.height()), 0.0f, 1.0f };
        _d3d11DeviceContext->RSSetViewports(1, &viewport);
    }

    return true;
#undef CHECK
}

void DX11RenderingContext::updateViewport(const u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) noexcept
{
    D3D11_VIEWPORT viewport = { static_cast<FLOAT>(x), static_cast<FLOAT>(y), static_cast<FLOAT>(width), static_cast<FLOAT>(height), minZ, maxZ };
    _d3d11DeviceContext->RSSetViewports(1, &viewport);
}


void DX11RenderingContext::clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, u8 stencilValue) noexcept
{
    float colorF[4];

    colorF[0] = static_cast<float>(color.r) / 255.0f;
    colorF[1] = static_cast<float>(color.g) / 255.0f;
    colorF[2] = static_cast<float>(color.b) / 255.0f;
    colorF[3] = static_cast<float>(color.a) / 255.0f;

    if(clearColorBuffer)
    {
        _d3d11DeviceContext->ClearRenderTargetView(_renderTargetView, colorF);
    }

    UINT clearFlags = 0;
    if(clearDepthBuffer)
    {
        clearFlags |= D3D11_CLEAR_DEPTH;
    }
    if(clearStencilBuffer)
    {
        clearFlags |= D3D11_CLEAR_STENCIL;
    }

    _d3d11DeviceContext->ClearDepthStencilView(_depthStencilView, clearFlags, depthValue, stencilValue);
}

void DX11RenderingContext::setVSync(bool vsync) noexcept
{
    _vsync = vsync;
    // TODO: Implement DX vsync changing.
}

void DX11RenderingContext::setFaceWinding(bool clockwise) noexcept
{
}

void DX11RenderingContext::enableDepthWriting(bool writing) noexcept
{
}

NullableRef<IDepthStencilState> DX11RenderingContext::setDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept
{
    NullableRef<IDepthStencilState> ret = RefCast<IDepthStencilState>(_currentDepthStencilState);

    if(!dsState || !RTT_CHECK(dsState.get(), DX11DepthStencilState))
    { return ret; }

    _currentDepthStencilState = RefCast<DX11DepthStencilState>(dsState);
    _currentDepthStencilState->apply(*this);

    return ret;
}

void DX11RenderingContext::setDefaultDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept
{
    if(!dsState || !RTT_CHECK(dsState.get(), DX11DepthStencilState))
    { return; }

    _defaultDepthStencilState = RefCast<DX11DepthStencilState>(dsState);
}

void DX11RenderingContext::resetDepthStencilState() noexcept
{
    _currentDepthStencilState = _defaultDepthStencilState;
    _currentDepthStencilState->apply(*this);
}

const DepthStencilArgs& DX11RenderingContext::getDefaultDepthStencilStateParams() noexcept
{
    return _defaultDepthStencilState->args();
}

void DX11RenderingContext::beginFrame() noexcept
{
    // No-Op
}

void DX11RenderingContext::endFrame() noexcept
{
    // No-Op
}

void DX11RenderingContext::swapFrame() noexcept
{
    _swapChain->Present(_vsync ? 1 : 0, 0);
}

CPPRef<IVertexArrayBuilder> DX11RenderingContext::createVertexArray(uSys bufferCount) noexcept
{
    return CPPRef<IVertexArrayBuilder>(new(::std::nothrow) DX11VertexArrayBuilder(bufferCount, *this));
}

IBufferBuilder& DX11RenderingContext::createBuffer() noexcept
{ return *_bufferBuilder; }

IIndexBufferBuilder& DX11RenderingContext::createIndexBuffer() noexcept
{ return *_indexBufferBuilder; }

IUniformBufferBuilder& DX11RenderingContext::createUniformBuffer() noexcept
{ return *_uniformBufferBuilder; }

ITextureBuilder& DX11RenderingContext::createTexture2D() noexcept
{ return *static_cast<ITextureBuilder*>(null); }

ITextureBuilder& DX11RenderingContext::createNullTexture() noexcept
{ return *static_cast<ITextureBuilder*>(null); }

ITextureBuilder& DX11RenderingContext::createTextureDepth() noexcept
{ return *static_cast<ITextureBuilder*>(null); }

ITextureCubeBuilder& DX11RenderingContext::createTextureCube() noexcept
{ return *static_cast<ITextureCubeBuilder*>(null); }

ITextureSamplerBuilder& DX11RenderingContext::createTextureSampler() noexcept
{ return *static_cast<ITextureSamplerBuilder*>(null); }

CPPRef<ITextureUploaderBuilder> DX11RenderingContext::createTextureUploader(uSys textureCount) noexcept
{ return null; }

CPPRef<ISingleTextureUploaderBuilder> DX11RenderingContext::createSingleTextureUploader() noexcept
{ return null; }

IShaderBuilder& DX11RenderingContext::createShader() noexcept
{ return *_shaderBuilder; }

IDepthStencilStateBuilder& DX11RenderingContext::createDepthStencilState() noexcept
{ return *_depthStencilStateBuilder; }
#endif
