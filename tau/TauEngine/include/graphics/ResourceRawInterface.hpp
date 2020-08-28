#pragma once

#include <Objects.hpp>

#include "DLL.hpp"

#pragma warning(push, 0)
#include <GL/glew.h>

#ifdef _WIN32
  #include <d3d12.h>
  #include <d3d11.h>
  #include <d3d10.h>
#endif
#pragma warning(pop)

class TAU_DLL TAU_NOVTABLE IResourceRawInterface
{
    DEFAULT_CONSTRUCT_PO(IResourceRawInterface);
    DEFAULT_DESTRUCT_VI(IResourceRawInterface);
    DEFAULT_CM_PO(IResourceRawInterface);
public:
    [[nodiscard]] virtual void* rawHandle() const noexcept = 0;

    [[nodiscard]] virtual GLuint glTexture() const noexcept { return 0; }
    [[nodiscard]] virtual GLuint glBuffer() const noexcept { return 0; }

#ifdef _WIN32
    [[nodiscard]] virtual ID3D10Texture1D* dx10Texture1D() const noexcept { return null; }
    [[nodiscard]] virtual ID3D10Texture2D* dx10Texture2D() const noexcept { return null; }
    [[nodiscard]] virtual ID3D10Texture3D* dx10Texture3D() const noexcept { return null; }

    [[nodiscard]] virtual ID3D10Buffer* dx10Buffer() const noexcept { return null; }

    [[nodiscard]] virtual ID3D11Texture1D* dx11Texture1D() const noexcept { return null; }
    [[nodiscard]] virtual ID3D11Texture2D* dx11Texture2D() const noexcept { return null; }
    [[nodiscard]] virtual ID3D11Texture3D* dx11Texture3D() const noexcept { return null; }

    [[nodiscard]] virtual ID3D11Buffer* dx11Buffer() const noexcept { return null; }

    [[nodiscard]] virtual ID3D12Resource* dx12Resource() const noexcept { return null; }
#endif
};
