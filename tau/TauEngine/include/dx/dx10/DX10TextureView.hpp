#pragma once

#include "texture/TextureView.hpp"

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"

class DX10GraphicsInterface;

struct DXTextureViewArgs final
{
    ETexture::Format dataFormat;
    ID3D10ShaderResourceView* d3dSRV;
    u32 width;
    u32 height;
    u32 depth;
    u32 mipLevels;
    u32 arrayCount;
};

class TAU_DLL TAU_NOVTABLE DX10TextureView : public ITextureView
{
    TEXTURE_VIEW_IMPL(DX10TextureView);
protected:
    ETexture::Format _dataFormat;
    u32 _mipCount;
    ID3D10ShaderResourceView* _d3dSRV;
protected:
    DX10TextureView(const DXTextureViewArgs& args) noexcept
        : _dataFormat(args.dataFormat)
        , _mipCount(args.mipLevels)
        , _d3dSRV(args.d3dSRV)
    { }
public:
    virtual ~DX10TextureView() noexcept
    { RELEASE_DX(_d3dSRV); }

    DX10TextureView(const DX10TextureView& copy) noexcept
        : ITextureView(copy)
        , _dataFormat(copy._dataFormat)
        , _mipCount(copy._mipCount)
        , _d3dSRV(copy._d3dSRV)
    { _d3dSRV->AddRef(); }

    DX10TextureView(DX10TextureView&& move) noexcept
        : ITextureView(::std::move(move))
        , _dataFormat(move._dataFormat)
        , _mipCount(move._mipCount)
        , _d3dSRV(move._d3dSRV)
    { move._d3dSRV = null; }

    DX10TextureView& operator=(const DX10TextureView& copy) noexcept
    {
        if(this == &copy)
        { return *this; }

        RELEASE_DX(_d3dSRV);

        ITextureView::operator=(copy);

        _dataFormat = copy._dataFormat;
        _mipCount = copy._mipCount;
        _d3dSRV = copy._d3dSRV;

        _d3dSRV->AddRef();

        return *this;
    }

    DX10TextureView& operator=(DX10TextureView&& move) noexcept
    {
        if(this == &move)
        { return *this; }

        RELEASE_DX(_d3dSRV);

        ITextureView::operator=(::std::move(move));

        _dataFormat = move._dataFormat;
        _mipCount = move._mipCount;
        _d3dSRV = move._d3dSRV;

        move._d3dSRV = null;

        return *this;
    }

    [[nodiscard]] ETexture::Format dataFormat() const noexcept override { return _dataFormat; }

    [[nodiscard]] u32 mipLevels() const noexcept override { return _mipCount; }

    [[nodiscard]]       ID3D10ShaderResourceView* d3dSRV()       noexcept { return _d3dSRV; }
    [[nodiscard]] const ID3D10ShaderResourceView* d3dSRV() const noexcept { return _d3dSRV; }

    void generateMipmaps(IRenderingContext& context) noexcept override;
};

class TAU_DLL DX10Texture1DView : public DX10TextureView
{
    DEFAULT_DESTRUCT_VI(DX10Texture1DView);
    DEFAULT_CM_PU(DX10Texture1DView);
protected:
    u32 _width;
public:
    DX10Texture1DView(const DXTextureViewArgs& args) noexcept
        : DX10TextureView(args)
        , _width(args.width)
    { }

    [[nodiscard]] ETexture::Type textureType() const noexcept override { return ETexture::Type::Texture1D; }

    [[nodiscard]] u32 width() const noexcept override { return _width; }
};

class TAU_DLL DX10Texture1DArrayView : public DX10TextureView
{
    DEFAULT_DESTRUCT_VI(DX10Texture1DArrayView);
    DEFAULT_CM_PU(DX10Texture1DArrayView);
protected:
    u32 _width;
    u32 _arrayCount;
public:
    DX10Texture1DArrayView(const DXTextureViewArgs& args) noexcept
        : DX10TextureView(args)
        , _width(args.width)
        , _arrayCount(args.arrayCount)
    { }

    [[nodiscard]] ETexture::Type textureType() const noexcept override { return ETexture::Type::Texture1DArray; }

    [[nodiscard]] u32      width() const noexcept override { return _width;      }
    [[nodiscard]] u32 arrayCount() const noexcept override { return _arrayCount; }
};

class TAU_DLL DX10Texture2DView : public DX10TextureView
{
    DEFAULT_DESTRUCT_VI(DX10Texture2DView);
    DEFAULT_CM_PU(DX10Texture2DView);
protected:
    u32 _width;
    u32 _height;
public:
    DX10Texture2DView(const DXTextureViewArgs& args) noexcept
        : DX10TextureView(args)
        , _width(args.width)
        , _height(args.height)
    { }

    [[nodiscard]] ETexture::Type textureType() const noexcept override { return ETexture::Type::Texture2D; }

    [[nodiscard]] u32  width() const noexcept override { return _width;  }
    [[nodiscard]] u32 height() const noexcept override { return _height; }
};

class TAU_DLL DX10Texture2DArrayView : public DX10TextureView
{
    DEFAULT_DESTRUCT_VI(DX10Texture2DArrayView);
    DEFAULT_CM_PU(DX10Texture2DArrayView);
protected:
    u32 _width;
    u32 _height;
    u32 _arrayCount;
public:
    DX10Texture2DArrayView(const DXTextureViewArgs& args) noexcept
        : DX10TextureView(args)
        , _width(args.width)
        , _height(args.height)
        , _arrayCount(args.arrayCount)
    { }

    [[nodiscard]] ETexture::Type textureType() const noexcept override { return ETexture::Type::Texture2DArray; }

    [[nodiscard]] u32      width() const noexcept override { return _width;      }
    [[nodiscard]] u32     height() const noexcept override { return _height;     }
    [[nodiscard]] u32 arrayCount() const noexcept override { return _arrayCount; }
};

class TAU_DLL DX10Texture3DView : public DX10TextureView
{
    DEFAULT_DESTRUCT_VI(DX10Texture3DView);
    DEFAULT_CM_PU(DX10Texture3DView);
protected:
    u32 _width;
    u32 _height;
    u32 _depth;
public:
    DX10Texture3DView(const DXTextureViewArgs& args) noexcept
        : DX10TextureView(args)
        , _width(args.width)
        , _height(args.height)
        , _depth(args.depth)
    { }

    [[nodiscard]] ETexture::Type textureType() const noexcept override { return ETexture::Type::Texture3D; }

    [[nodiscard]] u32  width() const noexcept override { return _width;  }
    [[nodiscard]] u32 height() const noexcept override { return _height; }
    [[nodiscard]] u32  depth() const noexcept override { return _depth;  }
};

class TAU_DLL DX10TextureCubeView : public DX10TextureView
{
    DEFAULT_DESTRUCT_VI(DX10TextureCubeView);
    DEFAULT_CM_PU(DX10TextureCubeView);
protected:
    u32 _width;
    u32 _height;
public:
    DX10TextureCubeView(const DXTextureViewArgs& args) noexcept
        : DX10TextureView(args)
        , _width(args.width)
        , _height(args.height)
    { }

    [[nodiscard]] ETexture::Type textureType() const noexcept override { return ETexture::Type::TextureCube; }

    [[nodiscard]] u32  width() const noexcept override { return _width;  }
    [[nodiscard]] u32 height() const noexcept override { return _height; }
};

class TAU_DLL DX10TextureCubeArrayView : public DX10TextureView
{
    DEFAULT_DESTRUCT_VI(DX10TextureCubeArrayView);
    DEFAULT_CM_PU(DX10TextureCubeArrayView);
protected:
    u32 _width;
    u32 _height;
    u32 _arrayCount;
public:
    DX10TextureCubeArrayView(const DXTextureViewArgs& args) noexcept
        : DX10TextureView(args)
        , _width(args.width)
        , _height(args.height)
        , _arrayCount(args.arrayCount)
    { }

    [[nodiscard]] ETexture::Type textureType() const noexcept override { return ETexture::Type::TextureCubeArray; }

    [[nodiscard]] u32      width() const noexcept override { return _width;      }
    [[nodiscard]] u32     height() const noexcept override { return _height;     }
    [[nodiscard]] u32 arrayCount() const noexcept override { return _arrayCount; }
};

#define DX10_NO_MIPMAP_TEXTURE_VIEW(_BASE, _TYPE) \
    class TAU_DLL _TYPE final : public _BASE { \
        DEFAULT_DESTRUCT(_TYPE); \
        DEFAULT_CM_PU(_TYPE); \
    public: \
        _TYPE(const DXTextureViewArgs& args) noexcept : _BASE(args) { } \
        void generateMipmaps(IRenderingContext&) noexcept override { } \
    }

DX10_NO_MIPMAP_TEXTURE_VIEW(DX10Texture1DView, DX10NoMipmapTexture1DView);
DX10_NO_MIPMAP_TEXTURE_VIEW(DX10Texture1DArrayView, DX10NoMipmapTexture1DArrayView);
DX10_NO_MIPMAP_TEXTURE_VIEW(DX10Texture2DView, DX10NoMipmapTexture2DView);
DX10_NO_MIPMAP_TEXTURE_VIEW(DX10Texture2DArrayView, DX10NoMipmapTexture2DArrayView);
DX10_NO_MIPMAP_TEXTURE_VIEW(DX10Texture3DView, DX10NoMipmapTexture3DView);
DX10_NO_MIPMAP_TEXTURE_VIEW(DX10TextureCubeView, DX10NoMipmapTextureCubeDView);
DX10_NO_MIPMAP_TEXTURE_VIEW(DX10TextureCubeArrayView, DX10NoMipmapTextureCubeArrayView);

class TAU_DLL DX10TextureViewBuilder final : public ITextureViewBuilder
{
    DEFAULT_DESTRUCT(DX10TextureViewBuilder);
    DEFAULT_CM_PU(DX10TextureViewBuilder);
public:
    static DXGI_FORMAT dxTextureFormat(ETexture::Format format) noexcept;
    static D3D10_BIND_FLAG dxBindFlags(ETexture::BindFlags flags) noexcept;
    static D3D10_RESOURCE_MISC_FLAG dxMiscFlags(ETexture::BindFlags flags) noexcept;
private:
    DX10GraphicsInterface& _gi;
public:
    DX10TextureViewBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10Texture1DView* build(const Texture1DViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10Texture1DView* build(const Texture1DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureView> buildCPPRef(const Texture1DViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureView> buildTauRef(const Texture1DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureView> buildTauSRef(const Texture1DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX10Texture1DArrayView* build(const Texture1DArrayViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10Texture1DArrayView* build(const Texture1DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureView> buildCPPRef(const Texture1DArrayViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureView> buildTauRef(const Texture1DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureView> buildTauSRef(const Texture1DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX10Texture2DView* build(const Texture2DViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10Texture2DView* build(const Texture2DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureView> buildCPPRef(const Texture2DViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureView> buildTauRef(const Texture2DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureView> buildTauSRef(const Texture2DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX10Texture2DArrayView* build(const Texture2DArrayViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10Texture2DArrayView* build(const Texture2DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureView> buildCPPRef(const Texture2DArrayViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureView> buildTauRef(const Texture2DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureView> buildTauSRef(const Texture2DArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX10Texture3DView* build(const Texture3DViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10Texture3DView* build(const Texture3DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureView> buildCPPRef(const Texture3DViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureView> buildTauRef(const Texture3DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureView> buildTauSRef(const Texture3DViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX10TextureCubeView* build(const TextureCubeViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10TextureCubeView* build(const TextureCubeViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureView> buildCPPRef(const TextureCubeViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureView> buildTauRef(const TextureCubeViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureView> buildTauSRef(const TextureCubeViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX10TextureCubeArrayView* build(const TextureCubeArrayViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10TextureCubeArrayView* build(const TextureCubeArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureView> buildCPPRef(const TextureCubeArrayViewArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureView> buildTauRef(const TextureCubeArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureView> buildTauSRef(const TextureCubeArrayViewArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const Texture1DViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const Texture1DArrayViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const Texture2DViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const Texture2DArrayViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const Texture3DViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const TextureCubeViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const TextureCubeArrayViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
