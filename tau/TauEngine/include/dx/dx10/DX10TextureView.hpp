#pragma once

#include "texture/TextureView.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10GraphicsInterface;
class DX10Resource;

class TAU_DLL DX10TextureViewBuilder final : public ITextureViewBuilder
{
    DEFAULT_DESTRUCT(DX10TextureViewBuilder);
    DEFAULT_CM_PU(DX10TextureViewBuilder);
public:
    static DXGI_FORMAT dxTextureFormat(ETexture::Format format) noexcept;
    static D3D10_BIND_FLAG dxBindFlags(ETexture::BindFlags flags) noexcept;
    static D3D10_RESOURCE_MISC_FLAG dxMiscFlags(ETexture::BindFlags flags) noexcept;
public:
    struct DXTextureViewArgs final
    {
        ID3D10ShaderResourceView** d3dSRV;
    };
private:
    DX10GraphicsInterface& _gi;
public:
    DX10TextureViewBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] TextureView build(const TextureViewArgs& args, CPUDescriptorHandle handle, Error* error) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const TextureViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs1D(const TextureViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, DX10Resource* dxResource, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs1DArray(const TextureViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, DX10Resource* dxResource, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs2D(const TextureViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, DX10Resource* dxResource, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs2DArray(const TextureViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, DX10Resource* dxResource, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs3D(const TextureViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, DX10Resource* dxResource, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgsCube(const TextureViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, DX10Resource* dxResource, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgsCubeArray(const TextureViewArgs& args, [[tau::out]] DXTextureViewArgs* dxArgs, DX10Resource* dxResource, [[tau::out]] Error* error) const noexcept;
};
#endif
