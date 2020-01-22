#define __SDTC_WANT_LIB_EXT1__ 1
#include <cstdlib>
#include "dx/dx10/DX10RenderingContext.hpp"

#ifdef _WIN32
#include <Utils.hpp>
#include "system/Window.hpp"
#include "system/SystemInterface.hpp"
#include "dx/dx10/DX10Shader.hpp"
#include "dx/dx10/DX10InputLayout.hpp"
#include "dx/dx10/DX10VertexArray.hpp"

DX10RenderingContext::DX10RenderingContext(const RenderingMode& mode) noexcept
    : IRenderingContext(mode),
      _d3d10Device(null),
      _renderTargetView(null),
      _depthStencilBuffer(null), _depthStencilState(null), _depthStencilView(null),
      _rasterizerState(null),
      _swapChain(null),
      _vsync(false)//,
      // _tmpShaderBlob(null), _tmpInputLayout(null)
{ }

DX10RenderingContext::~DX10RenderingContext() noexcept
{
#define RELEASE(_OBJ) do { \
    if(_OBJ) {\
        (_OBJ)->Release(); \
        (_OBJ) = null; \
    } } while(0)

    RELEASE(_d3d10Device);
    RELEASE(_renderTargetView);
    RELEASE(_depthStencilBuffer);
    RELEASE(_depthStencilState);
    RELEASE(_depthStencilView);
    RELEASE(_rasterizerState);
    RELEASE(_swapChain);
#undef RELEASE
}

bool DX10RenderingContext::createContext(Window& window) noexcept
{
#define CHECK(_VAL) do { if(FAILED(_VAL)) { return false; } } while(0)

    HWND hWnd = window.sysWindowContainer().windowHandle;

    DXGI_RATIONAL refreshRate = { 0, 1 };

    {
        Ref<IGraphicsInterface> graphicsInterface = SystemInterface::get()->createGraphicsInterface(_mode);
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

        CHECK(D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION, &swapChainDesc, &_swapChain, &_d3d10Device));
    }

    {
        // Get pointer to back buffer;
        ID3D10Texture2D* backBufferPtr;
        CHECK(_swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBufferPtr)));

        CHECK(_d3d10Device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView));

        backBufferPtr->Release();
    }

    {
        D3D10_TEXTURE2D_DESC depthStencilBufferDesc;
        ZeroMemory(&depthStencilBufferDesc, sizeof(depthStencilBufferDesc));
        depthStencilBufferDesc.Width = window.width();
        depthStencilBufferDesc.Height = window.height();
        depthStencilBufferDesc.MipLevels = 1;
        depthStencilBufferDesc.ArraySize = 1;
        depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilBufferDesc.SampleDesc.Count = 1;
        depthStencilBufferDesc.SampleDesc.Quality = 0;
        depthStencilBufferDesc.Usage = D3D10_USAGE_DEFAULT;
        depthStencilBufferDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
        depthStencilBufferDesc.CPUAccessFlags = 0;
        depthStencilBufferDesc.MiscFlags = 0;

        CHECK(_d3d10Device->CreateTexture2D(&depthStencilBufferDesc, NULL, &_depthStencilBuffer));
    }

    {
        D3D10_DEPTH_STENCIL_DESC depthStencilDesc;
        ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
        depthStencilDesc.DepthEnable = true;
        depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;

        depthStencilDesc.StencilEnable = true;
        depthStencilDesc.StencilReadMask = 0xFF;
        depthStencilDesc.StencilWriteMask = 0xFF;

        depthStencilDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
        depthStencilDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
        depthStencilDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

        depthStencilDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
        depthStencilDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
        depthStencilDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

        CHECK(_d3d10Device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState));
        _d3d10Device->OMSetDepthStencilState(_depthStencilState, 1);
    }

    {
        D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
        ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
        depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Texture2D.MipSlice = 0;

        CHECK(_d3d10Device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView));
        _d3d10Device->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
    }

    {
        D3D10_RASTERIZER_DESC rasterDesc;
        rasterDesc.AntialiasedLineEnable = false;
        rasterDesc.CullMode = D3D10_CULL_BACK;
        rasterDesc.DepthBias = 0;
        rasterDesc.DepthBiasClamp = 0.0f;
        rasterDesc.DepthClipEnable = true;
        rasterDesc.FillMode = D3D10_FILL_SOLID;
        rasterDesc.FrontCounterClockwise = false;
        rasterDesc.MultisampleEnable = false;
        rasterDesc.ScissorEnable = false;
        rasterDesc.SlopeScaledDepthBias = 0.0f;

        CHECK(_d3d10Device->CreateRasterizerState(&rasterDesc, &_rasterizerState));
        _d3d10Device->RSSetState(_rasterizerState);
    }

    {
        D3D10_VIEWPORT viewport = { 0, 0, window.width(), window.height(), 0.0f, 1.0f };
        _d3d10Device->RSSetViewports(1, &viewport);
    }

    return true;
#undef CHECK
}

void DX10RenderingContext::updateViewport(u32 x, u32 y, u32 width, u32 height, float minZ, float maxZ) noexcept
{
    D3D10_VIEWPORT viewport = { static_cast<INT>(x), static_cast<INT>(y), width, height, minZ, maxZ };
    _d3d10Device->RSSetViewports(1, &viewport);
}

void DX10RenderingContext::clearScreen(bool clearColorBuffer, bool clearDepthBuffer, bool clearStencilBuffer, RGBAColor color, float depthValue, u8 stencilValue) noexcept
{
    float colorF[4];

    colorF[0] = static_cast<float>(color.r) / 255.0f;
    colorF[1] = static_cast<float>(color.g) / 255.0f;
    colorF[2] = static_cast<float>(color.b) / 255.0f;
    colorF[3] = static_cast<float>(color.a) / 255.0f;

    if(clearColorBuffer)
    {
        _d3d10Device->ClearRenderTargetView(_renderTargetView, colorF);
    }

    UINT clearFlags = 0;
    if(clearDepthBuffer)
    {
        clearFlags |= D3D10_CLEAR_DEPTH;
    }
    if(clearStencilBuffer)
    {
        clearFlags |= D3D10_CLEAR_STENCIL;
    }

    _d3d10Device->ClearDepthStencilView(_depthStencilView, clearFlags, depthValue, stencilValue);
}

void DX10RenderingContext::setVSync(bool vsync) noexcept
{
    _vsync = vsync;
    // TODO: Implement DX vsync changing.
}

void DX10RenderingContext::setFaceWinding(bool clockwise) noexcept
{
}

void DX10RenderingContext::enableDepthWriting(bool writing) noexcept
{
}

void DX10RenderingContext::beginFrame() noexcept
{
    // No-Op
}

void DX10RenderingContext::endFrame() noexcept
{
    // No-Op
}

void DX10RenderingContext::swapFrame() noexcept
{
    _swapChain->Present(_vsync ? 1 : 0, 0);
}

Ref<IInputLayoutBuilder> DX10RenderingContext::createInputLayout(const uSys numDescriptors) noexcept
{
    return Ref<IInputLayoutBuilder>(new(::std::nothrow) DX10InputLayoutBuilder(numDescriptors));
}

Ref<IVertexArrayBuilder> DX10RenderingContext::createVertexArray(const uSys bufferCount) noexcept
{
    return Ref<IVertexArrayBuilder>(new(::std::nothrow) DX10VertexArrayBuilder(bufferCount));
}

Ref<IShaderBuilder> DX10RenderingContext::createShader() noexcept
{
    return Ref<IShaderBuilder>(new(::std::nothrow) DX10ShaderBuilder(this));
}
#endif
