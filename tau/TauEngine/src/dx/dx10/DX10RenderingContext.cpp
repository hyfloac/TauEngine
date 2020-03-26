#define __SDTC_WANT_LIB_EXT1__ 1
#include <cstdlib>
#include "dx/dx10/DX10RenderingContext.hpp"

#include "TauEngine.hpp"

#ifdef _WIN32
// #include <D3DX10Core.h>

#include <Utils.hpp>
#include "system/Window.hpp"
#include "system/SystemInterface.hpp"
#include "dx/dx10/DX10Shader.hpp"
#include "dx/dx10/DX10VertexArray.hpp"
#include "dx/dx10/DX10Buffer.hpp"
#include "dx/dx10/DX10TextureSampler.hpp"
#include "dx/dx10/DX10Texture.hpp"
#include "dx/dx10/DX10TextureUploader.hpp"
#include "dx/dx10/DX10DepthStencilState.hpp"
#include "dx/dx10/DX10RasterizerState.hpp"
#include "dx/dx10/DX10GraphicsInterface.hpp"

DX10RenderingContext::DX10RenderingContext(DX10GraphicsInterface& gi, const DX10RenderingContextArgs& args) noexcept
    : IRenderingContext(gi.renderingMode()), _gi(gi),
      _renderTargetView(args.renderTargetView),
      _depthStencilBuffer(args.depthStencilBuffer), _depthStencilView(args.depthStencilView),
      _swapChain(args.swapChain),
      _vsync(false),
      _defaultDepthStencilState(null), _currentDepthStencilState(null),
      _defaultRasterizerState(null), _currentRasterizerState(null),
      _vertexArrayBuilder(new(::std::nothrow) DX10VertexArrayBuilder(gi)),
      _bufferBuilder(new(::std::nothrow) DX10BufferBuilder(*this)),
      _indexBufferBuilder(new(::std::nothrow) DX10IndexBufferBuilder(*this)),
      _uniformBufferBuilder(new(::std::nothrow) DX10UniformBufferBuilder(*this)),
      _textureSamplerBuilder(new(::std::nothrow) DX10TextureSamplerBuilder(*this)),
      _texture2DBuilder(new(::std::nothrow) DX10Texture2DBuilder(*this)),
      _textureNullBuilder(new(::std::nothrow) DX10NullTextureBuilder),
      _textureCubeBuilder(new(::std::nothrow) DX10TextureCubeBuilder(*this))
{ }

DX10RenderingContext::~DX10RenderingContext() noexcept
{
#define RELEASE(_OBJ) do { \
    if(_OBJ) {\
        (_OBJ)->Release(); \
        (_OBJ) = null; \
    } } while(0)

    RELEASE(_renderTargetView);
    RELEASE(_depthStencilBuffer);
    RELEASE(_depthStencilView);
    RELEASE(_swapChain);
#undef RELEASE
#define RELEASE(_OBJ) do { \
    if(_OBJ) {\
        delete (_OBJ); \
        (_OBJ) = null; \
    } } while(0)

    RELEASE(_vertexArrayBuilder);
    RELEASE(_bufferBuilder);
    RELEASE(_indexBufferBuilder);
    RELEASE(_uniformBufferBuilder);
    RELEASE(_textureSamplerBuilder);
    RELEASE(_texture2DBuilder);
    RELEASE(_textureNullBuilder);
    RELEASE(_textureCubeBuilder);
#undef RELEASE
}

const ID3D10Device* DX10RenderingContext::d3dDevice() const noexcept
{ return _gi.d3d10Device(); }

ID3D10Device* DX10RenderingContext::d3dDevice() noexcept
{ return _gi.d3d10Device(); }

bool DX10RenderingContext::createContext(Window& window) noexcept
{
#define CHECK(_VAL) do { if(FAILED(_VAL)) { return false; } } while(0)

    HWND hWnd = window.sysWindowContainer().windowHandle;

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

        // CHECK(D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, _mode.debugMode() ? D3D10_CREATE_DEVICE_DEBUG : 0, D3D10_SDK_VERSION, &swapChainDesc, &_swapChain, &_d3dDevice));
        IDXGIFactory* dxgiFactory;
        CHECK(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&dxgiFactory)));
        dxgiFactory->CreateSwapChain(_gi.d3d10Device(), &swapChainDesc, &_swapChain);
        dxgiFactory->Release();

        // D3DX10DebugMute(TRUE);
    }

    {
        // Get pointer to back buffer;
        ID3D10Texture2D* backBufferPtr;
        CHECK(_swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBufferPtr)));

        CHECK(_gi.d3d10Device()->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView));

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

        CHECK(_gi.d3d10Device()->CreateTexture2D(&depthStencilBufferDesc, NULL, &_depthStencilBuffer));
    }

    {
        // DepthStencilParams depthStencilParams {};
        // depthStencilParams.enableDepthTest = true;
        // depthStencilParams.enableStencilTest = true;
        //
        // depthStencilParams.depthWriteMask = DepthStencilParams::DepthWriteMask::All;
        // depthStencilParams.depthCompareFunc = DepthStencilParams::CompareFunc::LessThan;
        //
        // depthStencilParams.stencilReadMask = 0xFF;
        // depthStencilParams.stencilWriteMask = 0xFF;
        //
        // depthStencilParams.frontFace.failOp = DepthStencilParams::StencilOp::Keep;
        // depthStencilParams.frontFace.stencilPassDepthFailOp = DepthStencilParams::StencilOp::IncrementClamp;
        // depthStencilParams.frontFace.passOp = DepthStencilParams::StencilOp::Keep;
        // depthStencilParams.frontFace.compareFunc = DepthStencilParams::CompareFunc::Always;
        //
        // depthStencilParams.backFace.failOp = DepthStencilParams::StencilOp::Keep;
        // depthStencilParams.backFace.stencilPassDepthFailOp = DepthStencilParams::StencilOp::DecrementClamp;
        // depthStencilParams.backFace.passOp = DepthStencilParams::StencilOp::Keep;
        // depthStencilParams.backFace.compareFunc = DepthStencilParams::CompareFunc::Always;
        //
        // NullableRef<IDepthStencilState> dsState = createDepthStencilState().buildTauRef(depthStencilParams, null);
        // _defaultDepthStencilState = RefCast<DX10DepthStencilState>(dsState);
        // _currentDepthStencilState = _defaultDepthStencilState;
        _currentDepthStencilState->apply(*this);
    }

    {
        D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc[2];
        ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
        depthStencilViewDesc[0].Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc[0].ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc[0].Texture2D.MipSlice = 0;
        depthStencilViewDesc[1].Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc[1].ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc[1].Texture2D.MipSlice = 0;

        CHECK(_gi.d3d10Device()->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc[0], &_depthStencilView));
        _gi.d3d10Device()->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
    }

    // {
    //     D3D10_RASTERIZER_DESC rasterDesc;
    //     rasterDesc.FillMode = D3D10_FILL_SOLID;
    //     rasterDesc.CullMode = D3D10_CULL_BACK;
    //     // rasterDesc.CullMode = D3D10_CULL_NONE;
    //     rasterDesc.FrontCounterClockwise = true;
    //     rasterDesc.DepthBias = 0;
    //     rasterDesc.DepthBiasClamp = 0.0f;
    //     rasterDesc.SlopeScaledDepthBias = 0.0f;
    //     rasterDesc.DepthClipEnable = true;
    //     rasterDesc.ScissorEnable = false;
    //     rasterDesc.MultisampleEnable = false;
    //     rasterDesc.AntialiasedLineEnable = false;
    //
    //     CHECK(_gi.d3d10Device()->CreateRasterizerState(&rasterDesc, &_rasterizerState));
    //     _gi.d3d10Device()->RSSetState(_rasterizerState);
    // }

    {
        D3D10_VIEWPORT viewport = { 0, 0, window.width(), window.height(), 0.0f, 1.0f };
        _gi.d3d10Device()->RSSetViewports(1, &viewport);
    }

    return true;
#undef CHECK
}

void DX10RenderingContext::updateViewport(const u32 x, const u32 y, const u32 width, const u32 height, const float minZ, const float maxZ) noexcept
{
    D3D10_VIEWPORT viewport = { static_cast<INT>(x), static_cast<INT>(y), width, height, minZ, maxZ };
    _gi.d3d10Device()->RSSetViewports(1, &viewport);
}

void DX10RenderingContext::clearScreen(const bool clearColorBuffer, const bool clearDepthBuffer, const bool clearStencilBuffer, const RGBAColor color, const float depthValue, const u8 stencilValue) noexcept
{
    float colorF[4];

    colorF[0] = static_cast<float>(color.r) / 255.0f;
    colorF[1] = static_cast<float>(color.g) / 255.0f;
    colorF[2] = static_cast<float>(color.b) / 255.0f;
    colorF[3] = static_cast<float>(color.a) / 255.0f;

    if(clearColorBuffer)
    {
        _gi.d3d10Device()->ClearRenderTargetView(_renderTargetView, colorF);
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

    _gi.d3d10Device()->ClearDepthStencilView(_depthStencilView, clearFlags, depthValue, stencilValue);
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

NullableRef<IDepthStencilState> DX10RenderingContext::setDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept
{
    NullableRef<IDepthStencilState> ret = RefCast<IDepthStencilState>(_currentDepthStencilState);

    if(!dsState || !RTT_CHECK(dsState.get(), DX10DepthStencilState))
    { return ret; }

    _currentDepthStencilState = RefCast<DX10DepthStencilState>(dsState);
    _currentDepthStencilState->apply(*this);

    return ret;
}

void DX10RenderingContext::setDefaultDepthStencilState(const NullableRef<IDepthStencilState>& dsState) noexcept
{
    if(!dsState || !RTT_CHECK(dsState.get(), DX10DepthStencilState))
    { return; }

    _defaultDepthStencilState = RefCast<DX10DepthStencilState>(dsState);
}

void DX10RenderingContext::resetDepthStencilState() noexcept
{
    _currentDepthStencilState = _defaultDepthStencilState;
    _currentDepthStencilState->apply(*this);
}

const DepthStencilArgs& DX10RenderingContext::getDefaultDepthStencilArgs() noexcept
{ return _defaultDepthStencilState->args(); }

NullableRef<IRasterizerState> DX10RenderingContext::setRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept
{
    NullableRef<IRasterizerState> ret = RefCast<IRasterizerState>(_currentRasterizerState);

    if(!rsState || !RTT_CHECK(rsState.get(), DX10RasterizerState))
    { return ret; }

    _currentRasterizerState = RefCast<DX10RasterizerState>(rsState);
    _currentRasterizerState->apply(*this);

    return ret;
}

void DX10RenderingContext::setDefaultRasterizerState(const NullableRef<IRasterizerState>& rsState) noexcept
{
    if(!rsState || !RTT_CHECK(rsState.get(), DX10RasterizerState))
    { return; }

    _defaultRasterizerState = RefCast<DX10RasterizerState>(rsState);
}

void DX10RenderingContext::resetRasterizerState() noexcept
{
    _currentRasterizerState = _defaultRasterizerState;
    _currentRasterizerState->apply(*this);
}

const RasterizerArgs& DX10RenderingContext::getDefaultRasterizerArgs() noexcept
{ return _defaultRasterizerState->args(); }

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

IVertexArrayBuilder& DX10RenderingContext::createVertexArray() noexcept
{ return *_vertexArrayBuilder; }

IBufferBuilder& DX10RenderingContext::createBuffer() noexcept
{ return *_bufferBuilder; }

IIndexBufferBuilder& DX10RenderingContext::createIndexBuffer() noexcept
{ return *_indexBufferBuilder; }

IUniformBufferBuilder& DX10RenderingContext::createUniformBuffer() noexcept
{ return *_uniformBufferBuilder; }

ITextureBuilder& DX10RenderingContext::createTexture2D() noexcept
{ return *_texture2DBuilder; }

ITextureBuilder& DX10RenderingContext::createNullTexture() noexcept
{ return *_textureNullBuilder; }

ITextureBuilder& DX10RenderingContext::createTextureDepth() noexcept
{ return *static_cast<ITextureBuilder*>(null); }

ITextureCubeBuilder& DX10RenderingContext::createTextureCube() noexcept
{ return *_textureCubeBuilder; }

ITextureSamplerBuilder& DX10RenderingContext::createTextureSampler() noexcept
{ return *_textureSamplerBuilder; }

CPPRef<ITextureUploaderBuilder> DX10RenderingContext::createTextureUploader(uSys textureCount) noexcept
{
    return CPPRef<ITextureUploaderBuilder>(new(::std::nothrow) DX10TextureUploaderBuilder(textureCount, *this));
}

CPPRef<ISingleTextureUploaderBuilder> DX10RenderingContext::createSingleTextureUploader() noexcept
{
    return CPPRef<ISingleTextureUploaderBuilder>(new(::std::nothrow) DX10SingleTextureUploaderBuilder(*this));
}

IShaderBuilder& DX10RenderingContext::createShader() noexcept
{ return _gi.createShader(); }

DX10RenderingContext* DX10RenderingContextBuilder::build(const RenderingContextArgs& args, Error* error) noexcept
{
    DX10RenderingContextArgs dxArgs{};
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10RenderingContext* const context = new(::std::nothrow) DX10RenderingContext(_gi, dxArgs);

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, context);
}

DX10RenderingContext* DX10RenderingContextBuilder::build(const RenderingContextArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    DX10RenderingContextArgs dxArgs{};
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    DX10RenderingContext* const context = allocator.allocateT<DX10RenderingContext>(_gi, dxArgs);

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, context);
}

CPPRef<IRenderingContext> DX10RenderingContextBuilder::buildCPPRef(const RenderingContextArgs& args, Error* error) noexcept
{
    DX10RenderingContextArgs dxArgs{};
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const CPPRef<DX10RenderingContext> context = CPPRef<DX10RenderingContext>(new(::std::nothrow) DX10RenderingContext(_gi, dxArgs));

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, context);
}

NullableRef<IRenderingContext> DX10RenderingContextBuilder::buildTauRef(const RenderingContextArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    DX10RenderingContextArgs dxArgs{};
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableRef<DX10RenderingContext> context(allocator, _gi, dxArgs);

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, RefCast<IRenderingContext>(context));
}

NullableStrongRef<IRenderingContext> DX10RenderingContextBuilder::buildTauSRef(const RenderingContextArgs& args, Error* error, TauAllocator& allocator) noexcept
{
    DX10RenderingContextArgs dxArgs{};
    if(!processArgs(args, &dxArgs, error))
    { return null; }

    const NullableStrongRef<DX10RenderingContext> context(allocator, _gi, dxArgs);

    ERROR_CODE_COND_N(!context, Error::SystemMemoryAllocationError);
    ERROR_CODE_V(Error::NoError, RefCast<IRenderingContext>(context));
}

bool DX10RenderingContextBuilder::processArgs(const RenderingContextArgs& args, DX10RenderingContextArgs* const dxArgs, Error* const error) const noexcept
{
    ERROR_CODE_COND_F(!args.window, Error::NullWindow);

#define CHECK(_VAL) do { if(FAILED(_VAL)) { return false; } } while(0)

    HWND hWnd = args.window->sysWindowContainer().windowHandle;

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
        dxgiFactory->CreateSwapChain(_gi.d3d10Device(), &swapChainDesc, &dxArgs->swapChain);
        dxgiFactory->Release();
    }

    {
        // Get pointer to back buffer;
        ID3D10Texture2D* backBufferPtr;
        CHECK(dxArgs->swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBufferPtr)));

        CHECK(_gi.d3d10Device()->CreateRenderTargetView(backBufferPtr, NULL, &dxArgs->renderTargetView));

        backBufferPtr->Release();
    }

    {
        D3D10_TEXTURE2D_DESC depthStencilBufferDesc;
        ZeroMemory(&depthStencilBufferDesc, sizeof(depthStencilBufferDesc));
        depthStencilBufferDesc.Width = args.window->width();
        depthStencilBufferDesc.Height = args.window->height();
        depthStencilBufferDesc.MipLevels = 1;
        depthStencilBufferDesc.ArraySize = 1;
        depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilBufferDesc.SampleDesc.Count = 1;
        depthStencilBufferDesc.SampleDesc.Quality = 0;
        depthStencilBufferDesc.Usage = D3D10_USAGE_DEFAULT;
        depthStencilBufferDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
        depthStencilBufferDesc.CPUAccessFlags = 0;
        depthStencilBufferDesc.MiscFlags = 0;

        CHECK(_gi.d3d10Device()->CreateTexture2D(&depthStencilBufferDesc, NULL, &dxArgs->depthStencilBuffer));
    }

    {
        D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc[2];
        ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
        depthStencilViewDesc[0].Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc[0].ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc[0].Texture2D.MipSlice = 0;
        depthStencilViewDesc[1].Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilViewDesc[1].ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc[1].Texture2D.MipSlice = 0;

        CHECK(_gi.d3d10Device()->CreateDepthStencilView(dxArgs->depthStencilBuffer, &depthStencilViewDesc[0], &dxArgs->depthStencilView));
        _gi.d3d10Device()->OMSetRenderTargets(1, &dxArgs->renderTargetView, dxArgs->depthStencilView);
    }

    // {
    //     D3D10_RASTERIZER_DESC rasterDesc;
    //     rasterDesc.FillMode = D3D10_FILL_SOLID;
    //     rasterDesc.CullMode = D3D10_CULL_BACK;
    //     // rasterDesc.CullMode = D3D10_CULL_NONE;
    //     rasterDesc.FrontCounterClockwise = true;
    //     rasterDesc.DepthBias = 0;
    //     rasterDesc.DepthBiasClamp = 0.0f;
    //     rasterDesc.SlopeScaledDepthBias = 0.0f;
    //     rasterDesc.DepthClipEnable = true;
    //     rasterDesc.ScissorEnable = false;
    //     rasterDesc.MultisampleEnable = false;
    //     rasterDesc.AntialiasedLineEnable = false;
    //
    //     CHECK(_gi.d3d10Device()->CreateRasterizerState(&rasterDesc, &dxArgs->rasterizerState));
    //     _gi.d3d10Device()->RSSetState(dxArgs->rasterizerState);
    // }

    {
        D3D10_VIEWPORT viewport = { 0, 0, args.window->width(), args.window->height(), 0.0f, 1.0f };
        _gi.d3d10Device()->RSSetViewports(1, &viewport);
    }

    return true;
#undef CHECK
}
#endif
