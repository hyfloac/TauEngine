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
#include "dx/dx11/DX11GraphicsInterface.hpp"
#include "dx/dx11/DX11RasterizerState.hpp"

DX11RenderingContext::DX11RenderingContext(DX11GraphicsInterface& gi, const DX11RenderingContextArgs& args) noexcept
    : IRenderingContext(gi.renderingMode()), _gi(gi),
      _d3d11DeviceContext(args.d3d11DeviceContext),
      _renderTargetView(args.renderTargetView),
      _depthStencilBuffer(args.depthStencilBuffer), _depthStencilView(args.depthStencilView),
      _swapChain(args.swapChain),
      _vsync(false),
      _defaultDepthStencilState(null), _currentDepthStencilState(null),
      _defaultRasterizerState(null), _currentRasterizerState(null)
{ }

DX11RenderingContext::~DX11RenderingContext() noexcept
{
#define RELEASE(_OBJ) do { \
    if(_OBJ) {\
        (_OBJ)->Release(); \
        (_OBJ) = null; \
    } } while(0)

    RELEASE(_d3d11DeviceContext);
    RELEASE(_renderTargetView);
    RELEASE(_depthStencilBuffer);
    RELEASE(_depthStencilView);
    RELEASE(_swapChain);
#undef RELEASE
}

const ID3D11Device* DX11RenderingContext::d3d11Device() const noexcept
{ return _gi.d3d11Device(); }

ID3D11Device* DX11RenderingContext::d3d11Device() noexcept
{ return _gi.d3d11Device(); }

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

const DepthStencilArgs& DX11RenderingContext::getDefaultDepthStencilArgs() noexcept
{ return _defaultDepthStencilState->args(); }

NullableRef<IDepthStencilState> DX11RenderingContext::getDefaultDepthStencilState() noexcept
{ return RefCast<IDepthStencilState>(_defaultDepthStencilState); }

NullableRef<IRasterizerState> DX11RenderingContext::setRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept
{
    NullableRef<IRasterizerState> ret = RefCast<IRasterizerState>(_currentRasterizerState);

    if(!rsState || !RTT_CHECK(rsState.get(), DX11RasterizerState))
    { return ret; }

    _currentRasterizerState = RefCast<DX11RasterizerState>(rsState);
    _currentRasterizerState->apply(*this);

    return ret;
}

void DX11RenderingContext::setDefaultRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept
{
    if(!rsState || !RTT_CHECK(rsState.get(), DX11RasterizerState))
    { return; }

    _defaultRasterizerState = RefCast<DX11RasterizerState>(rsState);
}

void DX11RenderingContext::resetRasterizerState() noexcept
{
    _currentRasterizerState = _defaultRasterizerState;
    _currentRasterizerState->apply(*this);
}

const RasterizerArgs& DX11RenderingContext::getDefaultRasterizerArgs() noexcept
{ return _defaultRasterizerState->args(); }

NullableRef<IRasterizerState> DX11RenderingContext::getDefaultRasterizerState() noexcept
{ return RefCast<IRasterizerState>(_defaultRasterizerState); }

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

DX11RenderingContext* DX11RenderingContextBuilder::build(const RenderingContextArgs& args, Error* error) noexcept
{
    DX11RenderingContextArgs dxArgs{};
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX11RenderingContext* const context = new(::std::nothrow) DX11RenderingContext(_gi, dxArgs);

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, context);
}

DX11RenderingContext* DX11RenderingContextBuilder::build(const RenderingContextArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    DX11RenderingContextArgs dxArgs{};
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX11RenderingContext* const context = allocator.allocateT<DX11RenderingContext>(_gi, dxArgs);

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, context);
}

CPPRef<IRenderingContext> DX11RenderingContextBuilder::buildCPPRef(const RenderingContextArgs& args, Error* error) noexcept
{
    DX11RenderingContextArgs dxArgs{};
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX11RenderingContext> context = CPPRef<DX11RenderingContext>(new(::std::nothrow) DX11RenderingContext(_gi, dxArgs));

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, context);
}

NullableRef<IRenderingContext> DX11RenderingContextBuilder::buildTauRef(const RenderingContextArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    DX11RenderingContextArgs dxArgs{};
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX11RenderingContext> context(allocator, _gi, dxArgs);

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, RefCast<IRenderingContext>(context));
}

NullableStrongRef<IRenderingContext> DX11RenderingContextBuilder::buildTauSRef(const RenderingContextArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    DX11RenderingContextArgs dxArgs{};
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX11RenderingContext> context(allocator, _gi, dxArgs);

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, RefCast<IRenderingContext>(context));
}

bool DX11RenderingContextBuilder::processArgs(const RenderingContextArgs& args, DX11RenderingContextArgs* const dxArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(!args.window, Error::NullWindow);

#define CHECK(_VAL) do { if(FAILED(_VAL)) { return false; } } while(0)

    const HWND& hWnd = args.window->sysWindowContainer().windowHandle;

    DXGI_RATIONAL refreshRate = { 0, 1 };

    {
        auto gpuList = _gi.graphicsAccelerators();
        if(gpuList.count() == 0) { return false; }

        auto displayList = gpuList[0]->graphicsDisplays();
        if(displayList.count() == 0) { return false; }

        auto displayModeList = displayList[0]->displayModes();
        if(displayModeList.count() == 0) { return false; }

        for(uSys i = 0; i < displayModeList.length(); ++i)
        {
            if(displayModeList[i].width  == args.window->width() &
               displayModeList[i].height == args.window->height())
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
        swapChainDesc.BufferDesc.Width = args.window->width();
        swapChainDesc.BufferDesc.Height = args.window->height();
        swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

        if(args.vsync)
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

        IDXGIFactory* dxgiFactory;
        CHECK(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory)));
        dxgiFactory->CreateSwapChain(_gi.d3d11Device(), &swapChainDesc, &dxArgs->swapChain);
        dxgiFactory->Release();

        _gi.d3d11Device()->GetImmediateContext(&dxArgs->d3d11DeviceContext);
    }

    {
        // Get pointer to back buffer;
        ID3D11Texture2D* backBufferPtr;
        CHECK(dxArgs->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBufferPtr)));

        CHECK(_gi.d3d11Device()->CreateRenderTargetView(backBufferPtr, NULL, &dxArgs->renderTargetView));

        backBufferPtr->Release();
    }

    {
        D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
        ZeroMemory(&depthStencilBufferDesc, sizeof(depthStencilBufferDesc));
        depthStencilBufferDesc.Width = args.window->width();
        depthStencilBufferDesc.Height = args.window->height();
        depthStencilBufferDesc.MipLevels = 1;
        depthStencilBufferDesc.ArraySize = 1;
        depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilBufferDesc.SampleDesc.Count = 1;
        depthStencilBufferDesc.SampleDesc.Quality = 0;
        depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilBufferDesc.CPUAccessFlags = 0;
        depthStencilBufferDesc.MiscFlags = 0;

        CHECK(_gi.d3d11Device()->CreateTexture2D(&depthStencilBufferDesc, NULL, &dxArgs->depthStencilBuffer));
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

        CHECK(_gi.d3d11Device()->CreateDepthStencilView(dxArgs->depthStencilBuffer, &depthStencilViewDesc[0], &dxArgs->depthStencilView));
        dxArgs->d3d11DeviceContext->OMSetRenderTargets(1, &dxArgs->renderTargetView, dxArgs->depthStencilView);
    }

    // {
    //     D3D11_RASTERIZER_DESC rasterDesc;
    //     rasterDesc.FillMode = D3D11_FILL_SOLID;
    //     rasterDesc.CullMode = D3D11_CULL_BACK;
    //     // rasterDesc.CullMode = D3D11_CULL_NONE;
    //     rasterDesc.FrontCounterClockwise = true;
    //     rasterDesc.DepthBias = 0;
    //     rasterDesc.DepthBiasClamp = 0.0f;
    //     rasterDesc.SlopeScaledDepthBias = 0.0f;
    //     rasterDesc.DepthClipEnable = true;
    //     rasterDesc.ScissorEnable = false;
    //     rasterDesc.MultisampleEnable = false;
    //     rasterDesc.AntialiasedLineEnable = false;
    //
    //     CHECK(_gi.d3d11Device()->CreateRasterizerState(&rasterDesc, &dxArgs->rasterizerState));
    //     _gi.d3d11Device()->RSSetState(dxArgs->rasterizerState);
    // }

    {
         D3D11_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<FLOAT>(args.window->width()), static_cast<FLOAT>(args.window->height()), 0.0f, 1.0f };
        dxArgs->d3d11DeviceContext->RSSetViewports(1, &viewport);
    }

    return true;
#undef CHECK
}
#endif
