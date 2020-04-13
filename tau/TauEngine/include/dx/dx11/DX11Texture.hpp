#pragma once

#include "texture/Texture.hpp"

#ifdef _WIN32
#include <d3d11.h>

#include "DX11RenderTarget.hpp"
#include "DX11TextureView.hpp"

class DX11GraphicsInterface;
class DX11RenderingContext;

class TAU_DLL DX11Texture2D final : public ITexture2D
{
    TEXTURE_IMPL(DX11Texture2D);
private:
    ID3D11Texture2D* _d3dTexture;
    DX11RenderTarget _renderTarget;
    DX11TextureView* _textureView;
    UINT _mipLevels;
public:
    DX11Texture2D(const u32 width, const u32 height, const ETexture::Format dataFormat, ID3D11Texture2D* const d3dTexture, ID3D11RenderTargetView* const renderTargetView, DX11TextureView* const textureView, const UINT mipLevels) noexcept
        : ITexture2D(width, height, dataFormat)
        , _d3dTexture(d3dTexture)
        , _renderTarget(renderTargetView)
        , _textureView(textureView)
        , _mipLevels(mipLevels)
    { }

    ~DX11Texture2D() noexcept;

    [[nodiscard]] const ID3D11Texture2D* d3dTexture() const noexcept { return _d3dTexture; }
    [[nodiscard]]       ID3D11Texture2D* d3dTexture()       noexcept { return _d3dTexture; }

    [[nodiscard]] const DX11RenderTarget* renderTarget() const noexcept override { return &_renderTarget; }
    [[nodiscard]]       DX11RenderTarget* renderTarget()       noexcept override { return &_renderTarget; }
    [[nodiscard]] const DX11TextureView*   textureView() const noexcept override { return  _textureView;  }
    [[nodiscard]]       DX11TextureView*   textureView()       noexcept override { return  _textureView;  }

    [[nodiscard]] u64 _getHandle() const noexcept override { return reinterpret_cast<uintptr_t>(_d3dTexture); }

    void set(IRenderingContext& context, u32 mipLevel, const void* data) noexcept override;
};

class TAU_DLL DX11Texture3D final : public ITexture3D
{
    TEXTURE_IMPL(DX11Texture3D);
private:
    ID3D11Texture3D* _d3dTexture;
    DX11RenderTarget _renderTarget;
    DX11TextureView* _textureView;
    UINT _mipLevels;
public:
    DX11Texture3D(const u32 width, const u32 height, const u32 depth, const ETexture::Format dataFormat, ID3D11Texture3D* const d3dTexture, ID3D11RenderTargetView* const renderTargetView, DX11TextureView* const textureView, const UINT mipLevels) noexcept
        : ITexture3D(width, height, depth, dataFormat)
        , _d3dTexture(d3dTexture)
        , _renderTarget(renderTargetView)
        , _textureView(textureView)
        , _mipLevels(mipLevels)
    { }

    ~DX11Texture3D() noexcept;

    [[nodiscard]] const ID3D11Texture3D* d3dTexture() const noexcept { return _d3dTexture; }
    [[nodiscard]]       ID3D11Texture3D* d3dTexture()       noexcept { return _d3dTexture; }

    [[nodiscard]] const DX11RenderTarget* renderTarget() const noexcept override { return &_renderTarget; }
    [[nodiscard]]       DX11RenderTarget* renderTarget()       noexcept override { return &_renderTarget; }
    [[nodiscard]] const DX11TextureView*   textureView() const noexcept override { return  _textureView;  }
    [[nodiscard]]       DX11TextureView*   textureView()       noexcept override { return  _textureView;  }

    [[nodiscard]] u64 _getHandle() const noexcept override { return reinterpret_cast<uintptr_t>(_d3dTexture); }

    void set(IRenderingContext& context, u32 depthLevel, u32 mipLevel, const void* data) noexcept override;
};

class TAU_DLL DX11TextureCube final : public ITextureCube
{
    TEXTURE_IMPL(DX11TextureCube);
public:
    static UINT dxCubeSide(ETexture::CubeSide side) noexcept;
private:
    ID3D11Texture2D* _d3dTexture;
    DX11RenderTarget _renderTarget;
    DX11TextureView* _textureView;
    UINT _mipLevels;
public:
    DX11TextureCube(const u32 width, const u32 height, const ETexture::Format dataFormat, ID3D11Texture2D* const d3dTexture, ID3D11RenderTargetView* const renderTargetView, DX11TextureView* const textureView, const UINT mipLevels) noexcept
        : ITextureCube(width, height, dataFormat)
        , _d3dTexture(d3dTexture)
        , _renderTarget(renderTargetView)
        , _textureView(textureView)
        , _mipLevels(mipLevels)
    { }

    ~DX11TextureCube() noexcept;

    [[nodiscard]] const ID3D11Texture2D* d3dTexture() const noexcept { return _d3dTexture; }
    [[nodiscard]]       ID3D11Texture2D* d3dTexture()       noexcept { return _d3dTexture; }

    [[nodiscard]] const DX11RenderTarget* renderTarget() const noexcept override { return &_renderTarget; }
    [[nodiscard]]       DX11RenderTarget* renderTarget()       noexcept override { return &_renderTarget; }
    [[nodiscard]] const DX11TextureView*   textureView() const noexcept override { return  _textureView;  }
    [[nodiscard]]       DX11TextureView*   textureView()       noexcept override { return  _textureView;  }

    [[nodiscard]] u64 _getHandle() const noexcept override { return reinterpret_cast<uintptr_t>(_d3dTexture); }

    void set(IRenderingContext& context, u32 mipLevel, ETexture::CubeSide side, const void* data) noexcept override;
};

class TAU_DLL DX11TextureDepthStencil final : public ITextureDepthStencil
{
    TEXTURE_IMPL(DX11TextureDepthStencil);
private:
    ID3D11Texture2D* _d3dTexture;
    DX11DepthStencilTarget _renderTarget;
    DX11NoMipmapTextureView _depthView;
    DX11NoMipmapTextureView _stencilView;
public:
    DX11TextureDepthStencil(const u32 width, const u32 height, ID3D11Texture2D* const d3dTexture, ID3D11DepthStencilView* const depthStencilView, ID3D11ShaderResourceView* const depthView, ID3D11ShaderResourceView* const stencilView) noexcept
        : ITextureDepthStencil(width, height)
        , _d3dTexture(d3dTexture)
        , _renderTarget(depthStencilView)
        , _depthView(depthView)
        , _stencilView(stencilView)
    { }

    ~DX11TextureDepthStencil() noexcept;

    [[nodiscard]] const ID3D11Texture2D* d3dTexture() const noexcept { return _d3dTexture; }
    [[nodiscard]]       ID3D11Texture2D* d3dTexture()       noexcept { return _d3dTexture; }

    [[nodiscard]] const DX11DepthStencilTarget* renderTarget() const noexcept override { return &_renderTarget; }
    [[nodiscard]]       DX11DepthStencilTarget* renderTarget()       noexcept override { return &_renderTarget; }
    [[nodiscard]] const DX11NoMipmapTextureView*   depthView() const noexcept override { return &_depthView;    }
    [[nodiscard]]       DX11NoMipmapTextureView*   depthView()       noexcept override { return &_depthView;    }
    [[nodiscard]] const DX11NoMipmapTextureView* stencilView() const noexcept override { return &_stencilView;  }
    [[nodiscard]]       DX11NoMipmapTextureView* stencilView()       noexcept override { return &_stencilView;  }

    [[nodiscard]] u64 _getHandle() const noexcept override { return reinterpret_cast<uintptr_t>(_d3dTexture); }

    void set(IRenderingContext& context, const void* data) noexcept override;
};

class TAU_DLL DX11TextureBuilder final : public ITextureBuilder
{
    DEFAULT_DESTRUCT(DX11TextureBuilder);
    DELETE_COPY(DX11TextureBuilder);
public:
    static DXGI_FORMAT dxTextureFormat(ETexture::Format format) noexcept;
    static D3D11_BIND_FLAG dxBindFlags(ETexture::BindFlags flags) noexcept;
    static D3D11_BIND_FLAG dxBindFlags(ETexture::DepthStencilBindFlags flags) noexcept;
    static D3D11_RESOURCE_MISC_FLAG dxMiscFlags(ETexture::BindFlags flags) noexcept;
    static D3D11_RESOURCE_MISC_FLAG dxMiscFlags(ETexture::DepthStencilBindFlags flags) noexcept;
public:
#define RELEASE_DX(_X) \
    if((_X)) { \
        (_X)->Release(); \
        (_X) = null; }
#define RELEASE_TAU(_X) \
    if((_X)) { \
        delete (_X); \
        (_X) = null; }

    struct DXTexture2DArgs final
    {
        DEFAULT_CONSTRUCT_PU(DXTexture2DArgs);
        DEFAULT_COPY(DXTexture2DArgs);
    public:
        ID3D11Texture2D* d3dTexture;
        ID3D11RenderTargetView* renderTarget;
        DX11TextureView* textureView;

        ~DXTexture2DArgs() noexcept
        {
            RELEASE_DX(d3dTexture);
            RELEASE_DX(renderTarget);
            RELEASE_TAU(textureView);
        }
    };

    struct DXTexture3DArgs final
    {
        DEFAULT_CONSTRUCT_PU(DXTexture3DArgs);
        DEFAULT_COPY(DXTexture3DArgs);
    public:
        ID3D11Texture3D* d3dTexture;
        ID3D11RenderTargetView* renderTarget;
        DX11TextureView* textureView;

        ~DXTexture3DArgs() noexcept
        {
            RELEASE_DX(d3dTexture);
            RELEASE_DX(renderTarget);
            RELEASE_TAU(textureView);
        }
    };

    using DXTextureCubeArgs = DXTexture2DArgs;

    struct DXTextureDepthStencilArgs final
    {
        DEFAULT_CONSTRUCT_PU(DXTextureDepthStencilArgs);
        DEFAULT_COPY(DXTextureDepthStencilArgs);
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
    };

#undef RELEASE_DX
#undef RELEASE_TAU
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
