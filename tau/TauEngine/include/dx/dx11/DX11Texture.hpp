#pragma once

#include "texture/Texture.hpp"

#ifdef _WIN32
#include <d3d11.h>
#include "dx/DXUtils.hpp"

#include "DX11RenderTarget.hpp"
#include "DX11TextureView.hpp"
#include "DX11TextureRawInterface.hpp"

class DX11GraphicsInterface;
class DX11RenderingContext;

class TAU_DLL DX11Texture2D final : public ITexture2D
{
    DELETE_CM(DX11Texture2D);
    TEXTURE_IMPL(DX11Texture2D);
private:
    ID3D11Texture2D* _d3dTexture;
    DX11RenderTarget _renderTarget;
    DX11TextureView* _textureView;
    UINT _mipLevels;
    DX11TextureRawInterface _rawInterface;
public:
    DX11Texture2D(const u32 width, const u32 height, const ETexture::Format dataFormat, ID3D11Texture2D* const d3dTexture, ID3D11RenderTargetView* const renderTargetView, DX11TextureView* const textureView, const UINT mipLevels) noexcept
        : ITexture2D(width, height, dataFormat)
        , _d3dTexture(d3dTexture)
        , _renderTarget(renderTargetView)
        , _textureView(textureView)
        , _mipLevels(mipLevels)
        , _rawInterface(d3dTexture)
    { }

    ~DX11Texture2D() noexcept
    {
        RELEASE_DX(_d3dTexture);
        delete _textureView;
    }

    [[nodiscard]] const ID3D11Texture2D* d3dTexture() const noexcept { return _d3dTexture; }
    [[nodiscard]]       ID3D11Texture2D* d3dTexture()       noexcept { return _d3dTexture; }

    [[nodiscard]] const DX11RenderTarget* renderTarget() const noexcept override { return &_renderTarget; }
    [[nodiscard]]       DX11RenderTarget* renderTarget()       noexcept override { return &_renderTarget; }
    [[nodiscard]] const DX11TextureView*   textureView() const noexcept override { return  _textureView;  }
    [[nodiscard]]       DX11TextureView*   textureView()       noexcept override { return  _textureView;  }

    [[nodiscard]] const ITextureRawInterface& _getRawHandle() const noexcept override { return _rawInterface; }

    void set(IRenderingContext& context, u32 mipLevel, const void* data) noexcept override;
};

class TAU_DLL DX11Texture3D final : public ITexture3D
{
    DELETE_CM(DX11Texture3D);
    TEXTURE_IMPL(DX11Texture3D);
private:
    ID3D11Texture3D* _d3dTexture;
    DX11RenderTarget _renderTarget;
    DX11TextureView* _textureView;
    UINT _mipLevels;
    DX11TextureRawInterface _rawInterface;
public:
    DX11Texture3D(const u32 width, const u32 height, const u32 depth, const ETexture::Format dataFormat, ID3D11Texture3D* const d3dTexture, ID3D11RenderTargetView* const renderTargetView, DX11TextureView* const textureView, const UINT mipLevels) noexcept
        : ITexture3D(width, height, depth, dataFormat)
        , _d3dTexture(d3dTexture)
        , _renderTarget(renderTargetView)
        , _textureView(textureView)
        , _mipLevels(mipLevels)
        , _rawInterface(d3dTexture)
    { }

    ~DX11Texture3D() noexcept
    {
        RELEASE_DX(_d3dTexture);
        delete _textureView;
    }

    [[nodiscard]] const ID3D11Texture3D* d3dTexture() const noexcept { return _d3dTexture; }
    [[nodiscard]]       ID3D11Texture3D* d3dTexture()       noexcept { return _d3dTexture; }

    [[nodiscard]] const DX11RenderTarget* renderTarget() const noexcept override { return &_renderTarget; }
    [[nodiscard]]       DX11RenderTarget* renderTarget()       noexcept override { return &_renderTarget; }
    [[nodiscard]] const DX11TextureView*   textureView() const noexcept override { return  _textureView;  }
    [[nodiscard]]       DX11TextureView*   textureView()       noexcept override { return  _textureView;  }

    [[nodiscard]] const ITextureRawInterface& _getRawHandle() const noexcept override { return _rawInterface; }

    void set(IRenderingContext& context, u32 depthLevel, u32 mipLevel, const void* data) noexcept override;
};

class TAU_DLL DX11TextureCube final : public ITextureCube
{
    DELETE_CM(DX11TextureCube);
    TEXTURE_IMPL(DX11TextureCube);
public:
    static UINT dxCubeSide(ETexture::CubeSide side) noexcept;
private:
    ID3D11Texture2D* _d3dTexture;
    DX11RenderTarget _renderTarget;
    DX11TextureView* _textureView;
    UINT _mipLevels;
    DX11TextureRawInterface _rawInterface;
public:
    DX11TextureCube(const u32 width, const u32 height, const ETexture::Format dataFormat, ID3D11Texture2D* const d3dTexture, ID3D11RenderTargetView* const renderTargetView, DX11TextureView* const textureView, const UINT mipLevels) noexcept
        : ITextureCube(width, height, dataFormat)
        , _d3dTexture(d3dTexture)
        , _renderTarget(renderTargetView)
        , _textureView(textureView)
        , _mipLevels(mipLevels)
        , _rawInterface(d3dTexture)
    { }

    ~DX11TextureCube() noexcept
    {
        RELEASE_DX(_d3dTexture);
        delete _textureView;
    }

    [[nodiscard]] const ID3D11Texture2D* d3dTexture() const noexcept { return _d3dTexture; }
    [[nodiscard]]       ID3D11Texture2D* d3dTexture()       noexcept { return _d3dTexture; }

    [[nodiscard]] const DX11RenderTarget* renderTarget() const noexcept override { return &_renderTarget; }
    [[nodiscard]]       DX11RenderTarget* renderTarget()       noexcept override { return &_renderTarget; }
    [[nodiscard]] const DX11TextureView*   textureView() const noexcept override { return  _textureView;  }
    [[nodiscard]]       DX11TextureView*   textureView()       noexcept override { return  _textureView;  }

    [[nodiscard]] const ITextureRawInterface& _getRawHandle() const noexcept override { return _rawInterface; }

    void set(IRenderingContext& context, u32 mipLevel, ETexture::CubeSide side, const void* data) noexcept override;
};

class TAU_DLL DX11TextureDepthStencil final : public ITextureDepthStencil
{
    DELETE_CM(DX11TextureDepthStencil);
    TEXTURE_IMPL(DX11TextureDepthStencil);
private:
    ID3D11Texture2D* _d3dTexture;
    DX11DepthStencilTarget _renderTarget;
    DX11NoMipmapTextureView _depthView;
    DX11NoMipmapTextureView _stencilView;
    DX11TextureRawInterface _rawInterface;
public:
    DX11TextureDepthStencil(const u32 width, const u32 height, ID3D11Texture2D* const d3dTexture, ID3D11DepthStencilView* const depthStencilView, ID3D11ShaderResourceView* const depthView, ID3D11ShaderResourceView* const stencilView) noexcept
        : ITextureDepthStencil(width, height)
        , _d3dTexture(d3dTexture)
        , _renderTarget(depthStencilView)
        , _depthView(depthView)
        , _stencilView(stencilView)
        , _rawInterface(d3dTexture)
    { }

    ~DX11TextureDepthStencil() noexcept
    { RELEASE_DX(_d3dTexture); }

    [[nodiscard]] const ID3D11Texture2D* d3dTexture() const noexcept { return _d3dTexture; }
    [[nodiscard]]       ID3D11Texture2D* d3dTexture()       noexcept { return _d3dTexture; }

    [[nodiscard]] const DX11DepthStencilTarget* renderTarget() const noexcept override { return &_renderTarget; }
    [[nodiscard]]       DX11DepthStencilTarget* renderTarget()       noexcept override { return &_renderTarget; }
    [[nodiscard]] const DX11NoMipmapTextureView*   depthView() const noexcept override { return &_depthView;    }
    [[nodiscard]]       DX11NoMipmapTextureView*   depthView()       noexcept override { return &_depthView;    }
    [[nodiscard]] const DX11NoMipmapTextureView* stencilView() const noexcept override { return &_stencilView;  }
    [[nodiscard]]       DX11NoMipmapTextureView* stencilView()       noexcept override { return &_stencilView;  }

    [[nodiscard]] const ITextureRawInterface& _getRawHandle() const noexcept override { return _rawInterface; }

    void set(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL DX11TextureBuilder final : public ITextureBuilder
{
    DEFAULT_DESTRUCT(DX11TextureBuilder);
    DEFAULT_CM_PU(DX11TextureBuilder);
public:
    static DXGI_FORMAT dxTextureFormat(ETexture::Format format) noexcept;
    static D3D11_BIND_FLAG dxBindFlags(ETexture::BindFlags flags) noexcept;
    static D3D11_BIND_FLAG dxBindFlags(ETexture::DepthStencilBindFlags flags) noexcept;
    static D3D11_RESOURCE_MISC_FLAG dxMiscFlags(ETexture::BindFlags flags) noexcept;
    static D3D11_RESOURCE_MISC_FLAG dxMiscFlags(ETexture::DepthStencilBindFlags flags) noexcept;
public:
    struct DXTexture2DArgs final
    {
        DEFAULT_CONSTRUCT_PU(DXTexture2DArgs);
        DELETE_COPY(DXTexture2DArgs);
    public:
        ID3D11Texture2D* d3dTexture;
        ID3D11RenderTargetView* renderTarget;
        DX11TextureView* textureView;

        ~DXTexture2DArgs() noexcept
        {
            RELEASE_DX(d3dTexture);
            RELEASE_DX(renderTarget);
            delete textureView;
        }

        DXTexture2DArgs(DXTexture2DArgs&& move) noexcept
            : d3dTexture(move.d3dTexture)
            , renderTarget(move.renderTarget)
            , textureView(move.textureView)
        {
            move.d3dTexture = null;
            move.renderTarget = null;
            move.textureView = null;
        }

        DXTexture2DArgs& operator=(DXTexture2DArgs&& move) noexcept
        {
            if(this == &move)
            { return *this; }

            d3dTexture = move.d3dTexture;
            renderTarget = move.renderTarget;
            textureView = move.textureView;

            move.d3dTexture = null;
            move.renderTarget = null;
            move.textureView = null;

            return *this;
        }
    };

    struct DXTexture3DArgs final
    {
        DEFAULT_CONSTRUCT_PU(DXTexture3DArgs);
        DELETE_COPY(DXTexture3DArgs);
    public:
        ID3D11Texture3D* d3dTexture;
        ID3D11RenderTargetView* renderTarget;
        DX11TextureView* textureView;

        ~DXTexture3DArgs() noexcept
        {
            RELEASE_DX(d3dTexture);
            RELEASE_DX(renderTarget);
            delete textureView;
        }

        DXTexture3DArgs(DXTexture3DArgs&& move) noexcept
            : d3dTexture(move.d3dTexture)
            , renderTarget(move.renderTarget)
            , textureView(move.textureView)
        {
            move.d3dTexture = null;
            move.renderTarget = null;
            move.textureView = null;
        }

        DXTexture3DArgs& operator=(DXTexture3DArgs&& move) noexcept
        {
            if(this == &move)
            { return *this; }

            d3dTexture = move.d3dTexture;
            renderTarget = move.renderTarget;
            textureView = move.textureView;

            move.d3dTexture = null;
            move.renderTarget = null;
            move.textureView = null;

            return *this;
        }
    };

    using DXTextureCubeArgs = DXTexture2DArgs;

    struct DXTextureDepthStencilArgs final
    {
        DEFAULT_CONSTRUCT_PU(DXTextureDepthStencilArgs);
    public:
        ID3D11Texture2D* d3dTexture;
        ID3D11DepthStencilView* renderTarget;
        ID3D11ShaderResourceView* depthView;
        ID3D11ShaderResourceView* stencilView;

        ~DXTextureDepthStencilArgs() noexcept
        {
            RELEASE_DX(d3dTexture);
            RELEASE_DX(renderTarget);
            RELEASE_DX(depthView);
            RELEASE_DX(stencilView);
        }

        DXTextureDepthStencilArgs(const DXTextureDepthStencilArgs& copy) noexcept
            : d3dTexture(copy.d3dTexture)
            , renderTarget(copy.renderTarget)
            , depthView(copy.depthView)
            , stencilView(copy.stencilView)
        {
            d3dTexture->AddRef();
            renderTarget->AddRef();
            depthView->AddRef();
            stencilView->AddRef();
        }

        DXTextureDepthStencilArgs(DXTextureDepthStencilArgs&& move) noexcept
            : d3dTexture(move.d3dTexture)
            , renderTarget(move.renderTarget)
            , depthView(move.depthView)
            , stencilView(move.stencilView)
        {
            move.d3dTexture = null;
            move.renderTarget = null;
            move.depthView = null;
            move.stencilView = null;
        }

        DXTextureDepthStencilArgs& operator=(const DXTextureDepthStencilArgs& copy) noexcept
        {
            if(this == &copy)
            { return *this; }

            d3dTexture = copy.d3dTexture;
            renderTarget = copy.renderTarget;
            depthView = copy.depthView;
            stencilView = copy.stencilView;

            d3dTexture->AddRef();
            renderTarget->AddRef();
            depthView->AddRef();
            stencilView->AddRef();

            return *this;
        }

        DXTextureDepthStencilArgs& operator=(DXTextureDepthStencilArgs&& copy) noexcept
        {
            if(this == &copy)
            { return *this; }

            d3dTexture = copy.d3dTexture;
            renderTarget = copy.renderTarget;
            depthView = copy.depthView;
            stencilView = copy.stencilView;

            copy.d3dTexture = null;
            copy.renderTarget = null;
            copy.depthView = null;
            copy.stencilView = null;

            return *this;
        }
    };
private:
    DX11GraphicsInterface& _gi;
public:
    DX11TextureBuilder(DX11GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX11Texture2D* build(const Texture2DArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11Texture2D* build(const Texture2DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITexture2D> buildCPPRef(const Texture2DArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITexture2D> buildTauRef(const Texture2DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITexture2D> buildTauSRef(const Texture2DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX11Texture3D* build(const Texture3DArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11Texture3D* build(const Texture3DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITexture3D> buildCPPRef(const Texture3DArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITexture3D> buildTauRef(const Texture3DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITexture3D> buildTauSRef(const Texture3DArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX11TextureCube* build(const TextureCubeArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11TextureCube* build(const TextureCubeArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureCube> buildCPPRef(const TextureCubeArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureCube> buildTauRef(const TextureCubeArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureCube> buildTauSRef(const TextureCubeArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX11TextureDepthStencil* build(const TextureDepthStencilArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] DX11TextureDepthStencil* build(const TextureDepthStencilArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureDepthStencil> buildCPPRef(const TextureDepthStencilArgs& args, [[tau::out]] Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureDepthStencil> buildTauRef(const TextureDepthStencilArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureDepthStencil> buildTauSRef(const TextureDepthStencilArgs& args, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const Texture2DArgs& args, [[tau::out]] DXTexture2DArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const Texture3DArgs& args, [[tau::out]] DXTexture3DArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const TextureCubeArgs& args, [[tau::out]] DXTextureCubeArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const TextureDepthStencilArgs& args, [[tau::out]] DXTextureDepthStencilArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
