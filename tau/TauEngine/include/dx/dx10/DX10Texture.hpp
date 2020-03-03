#pragma once

#include "texture/Texture.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10RenderingContext;

class TAU_DLL DX10NullTexture final : public ITexture
{
    DEFAULT_DESTRUCT(DX10NullTexture);
    TEXTURE_IMPL(DX10NullTexture);
public:
    DX10NullTexture() noexcept
        : ITexture(0, 0, ETexture::Format::Red8UnsignedInt)
    { }

    [[nodiscard]] inline ETexture::Type textureType() const noexcept override { return ETexture::Type::T2D; }

    void set(u32 level, const void* data) noexcept override { }
    void bind(u8 textureUnit, EShader::Stage) noexcept override { }
    void unbind(u8 textureUnit, EShader::Stage) noexcept override { }
    void generateMipmaps() noexcept override { }
};

class TAU_DLL DX10Texture2D final : public ITexture
{
    TEXTURE_IMPL(DX10Texture2D);
private:
    DX10RenderingContext& _ctx;
    ID3D10Texture2D* _d3dTexture;
    ID3D10ShaderResourceView* _d3dTextureView;
    UINT _mipLevels;
public:
    DX10Texture2D(const u32 width, const u32 height, const ETexture::Format dataFormat, DX10RenderingContext& ctx, ID3D10Texture2D* const d3dTexture, ID3D10ShaderResourceView* const d3dTextureView, const UINT mipLevels) noexcept
        : ITexture(width, height, dataFormat), _ctx(ctx), _d3dTexture(d3dTexture), _d3dTextureView(d3dTextureView), _mipLevels(mipLevels)
    { }

    ~DX10Texture2D() noexcept;

    [[nodiscard]] ID3D10Texture2D* d3dTexture() noexcept { return _d3dTexture; }
    [[nodiscard]] const ID3D10Texture2D* d3dTexture() const noexcept { return _d3dTexture; }

    [[nodiscard]] ID3D10ShaderResourceView* d3dTextureView() noexcept { return _d3dTextureView; }
    [[nodiscard]] const ID3D10ShaderResourceView* d3dTextureView() const noexcept { return _d3dTextureView; }

    [[nodiscard]] inline ETexture::Type textureType() const noexcept override { return ETexture::Type::T2D; }

    void set(u32 level, const void* data) noexcept override;
    void bind(u8 textureUnit, EShader::Stage stage) noexcept override;
    void unbind(u8 textureUnit, EShader::Stage stage) noexcept override;
    void generateMipmaps() noexcept override;
};

class TAU_DLL DX10TextureCube final : public ITextureCube
{
    TEXTURE_IMPL(DX10TextureCube);
public:
    static UINT dxCubeSide(ETexture::CubeSide side) noexcept;
private:
    DX10RenderingContext& _ctx;
    ID3D10Texture2D* _d3dTexture;
    ID3D10ShaderResourceView* _d3dTextureView;
    UINT _mipLevels;
public:
    DX10TextureCube(const u32 width, const u32 height, const ETexture::Format dataFormat, DX10RenderingContext& ctx, ID3D10Texture2D* const d3dTexture, ID3D10ShaderResourceView* const d3dTextureView, const UINT mipLevels) noexcept
        : ITextureCube(width, height, dataFormat), _ctx(ctx), _d3dTexture(d3dTexture), _d3dTextureView(d3dTextureView), _mipLevels(mipLevels)
    { }

    ~DX10TextureCube() noexcept;

    [[nodiscard]] ID3D10Texture2D* d3dTexture() noexcept { return _d3dTexture; }
    [[nodiscard]] const ID3D10Texture2D* d3dTexture() const noexcept { return _d3dTexture; }

    [[nodiscard]] ID3D10ShaderResourceView* d3dTextureView() noexcept { return _d3dTextureView; }
    [[nodiscard]] const ID3D10ShaderResourceView* d3dTextureView() const noexcept { return _d3dTextureView; }

    void set(u32 level, const void* data) noexcept override { }
    void bind(u8 textureUnit, EShader::Stage stage) noexcept override;
    void unbind(u8 textureUnit, EShader::Stage stage) noexcept override;
    void generateMipmaps() noexcept override;
    void setCube(u32 level, ETexture::CubeSide side, const void* data) noexcept override;
};

class TAU_DLL DX10NullTextureBuilder final : public ITextureBuilder
{
    DEFAULT_CONSTRUCT_PU(DX10NullTextureBuilder);
    DEFAULT_DESTRUCT(DX10NullTextureBuilder);
    DELETE_COPY(DX10NullTextureBuilder);
public:
    [[nodiscard]] DX10NullTexture* build(const TextureArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10NullTexture* build(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITexture> buildCPPRef(const TextureArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITexture> buildTauRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITexture> buildTauSRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
};

class TAU_DLL DX10Texture2DBuilder final : public ITextureBuilder
{
    DEFAULT_DESTRUCT(DX10Texture2DBuilder);
    DELETE_COPY(DX10Texture2DBuilder);
public:
    static DXGI_FORMAT dxTextureFormat(ETexture::Format format) noexcept;
public:
    struct DXTextureArgs final
    {
        ID3D10Texture2D* d3dTexture;
        ID3D10ShaderResourceView* d3dTextureView;
    };
private:
    DX10RenderingContext& _ctx;
public:
    DX10Texture2DBuilder(DX10RenderingContext& ctx) noexcept
        : _ctx(ctx)
    { }

    [[nodiscard]] DX10Texture2D* build(const TextureArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10Texture2D* build(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITexture> buildCPPRef(const TextureArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITexture> buildTauRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITexture> buildTauSRef(const TextureArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const TextureArgs& args, [[tau::out]] DXTextureArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};

class TAU_DLL DX10TextureCubeBuilder final : public ITextureCubeBuilder
{
    DEFAULT_DESTRUCT(DX10TextureCubeBuilder);
    DELETE_COPY(DX10TextureCubeBuilder);
public:
    using DXTextureCubeArgs = DX10Texture2DBuilder::DXTextureArgs;
private:
    DX10RenderingContext& _ctx;
public:
    DX10TextureCubeBuilder(DX10RenderingContext& ctx) noexcept
        : _ctx(ctx)
    { }

    [[nodiscard]] DX10TextureCube* build(const TextureCubeArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10TextureCube* build(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<ITextureCube> buildCPPRef(const TextureCubeArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<ITextureCube> buildTauRef(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<ITextureCube> buildTauSRef(const TextureCubeArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const TextureCubeArgs& args, [[tau::out]] DXTextureCubeArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
