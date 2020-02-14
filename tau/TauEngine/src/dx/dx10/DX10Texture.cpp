#include "dx/dx10/DX10Texture.hpp"
#include <d3d9.h>

#ifdef _WIN32
#include "dx/dx10/DX10RenderingContext.hpp"

DX10Texture2D::~DX10Texture2D() noexcept
{
    _d3dTexture->Release();
    _d3dTextureView->Release();
    _d3dTexture = null;
    _d3dTextureView = null;
}

void DX10Texture2D::set(u32 level, const void* data) noexcept
{
    _ctx.d3d10Device()->UpdateSubresource(_d3dTexture, 0, NULL, data, _width * ETexture::bytesPerPixel(_dataFormat), 0);
}

void DX10Texture2D::bind(u8 textureUnit) noexcept
{
    _ctx.d3d10Device()->PSSetShaderResources(textureUnit, 1, &_d3dTextureView);
}

void DX10Texture2D::unbind(u8 textureUnit) noexcept
{
}

void DX10Texture2D::generateMipmaps() noexcept
{
    _ctx.d3d10Device()->GenerateMips(_d3dTextureView);
}

ITexture* DX10Texture2DBuilder::build(Error* error) const noexcept
{
    ERROR_CODE_COND_N(_width == 0, Error::WidthIsZero);
    ERROR_CODE_COND_N(_height == 0, Error::HeightIsZero);
    ERROR_CODE_COND_N(_mipmapLevels < 0, Error::MipMapLevelsIsUnset);
    ERROR_CODE_COND_N(_dataFormat == static_cast<ETexture::Format>(0), Error::DataFormatIsUnset);

    D3D10_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = _width;
    textureDesc.Height = _height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = dxTextureFormat(_dataFormat);
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D10_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D10_BIND_SHADER_RESOURCE | D3D10_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D10_RESOURCE_MISC_GENERATE_MIPS;

    D3D10_SUBRESOURCE_DATA initialDesc;
    initialDesc.pSysMem = _initialBuffer;
    initialDesc.SysMemPitch = _width * ETexture::bytesPerPixel(_dataFormat);
    initialDesc.SysMemSlicePitch = 0;
	
    ID3D10Texture2D* d3dTexture;
    HRESULT res = _ctx.d3d10Device()->CreateTexture2D(&textureDesc, _initialBuffer ? &initialDesc : NULL, &d3dTexture);
	
    ERROR_CODE_COND_N(FAILED(res), Error::DriverMemoryAllocationFailure);

    D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
    viewDesc.Format = textureDesc.Format;
    viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MostDetailedMip = 0;
    viewDesc.Texture2D.MipLevels = -1;

    ID3D10ShaderResourceView* d3dTextureView;
    res = _ctx.d3d10Device()->CreateShaderResourceView(d3dTexture, &viewDesc, &d3dTextureView);

    if(FAILED(res))
    {
        d3dTexture->Release();
        ERROR_CODE_N(Error::DriverMemoryAllocationFailure);
    }

    DX10Texture2D* const texture = new(std::nothrow) DX10Texture2D(_width, _height, _dataFormat, _ctx, d3dTexture, d3dTextureView);

    if(!texture)
    {
        d3dTexture->Release();
        d3dTextureView->Release();
        ERROR_CODE_N(Error::SystemMemoryAllocationFailure);
    }

    ERROR_CODE_V(Error::NoError, texture);
}

DXGI_FORMAT DX10Texture2DBuilder::dxTextureFormat(const ETexture::Format format) noexcept
{
    switch(format)
    {
        case ETexture::Format::Red8UnsignedInt:                 return DXGI_FORMAT_R8_UNORM;
        case ETexture::Format::Red16UnsignedInt:                return DXGI_FORMAT_R16_UNORM;
        case ETexture::Format::Red32UnsignedInt:                return DXGI_FORMAT_R32_UINT;
        case ETexture::Format::RedGreen8UnsignedInt:            return DXGI_FORMAT_R8G8_UNORM;
        case ETexture::Format::RedGreen16UnsignedInt:           return DXGI_FORMAT_R16G16_UNORM;
        case ETexture::Format::RedGreen32UnsignedInt:           return DXGI_FORMAT_R32G32_UINT;
        // case ETexture::Format::RedGreenBlue8UnsignedInt:        return DXGI_FORMAT_R8G8B8_UINT;
        // case ETexture::Format::RedGreenBlue16UnsignedInt:       return DXGI_FORMAT_R16G16B16_UINT;
        case ETexture::Format::RedGreenBlue32UnsignedInt:       return DXGI_FORMAT_R32G32B32_UINT;
        case ETexture::Format::RedGreenBlueAlpha8UnsignedInt:   return DXGI_FORMAT_R8G8B8A8_UNORM;
        case ETexture::Format::RedGreenBlueAlpha16UnsignedInt:  return DXGI_FORMAT_R16G16B16A16_UNORM;
        case ETexture::Format::RedGreenBlueAlpha32UnsignedInt:  return DXGI_FORMAT_R32G32B32A32_UINT;
        case ETexture::Format::Red16Float:                      return DXGI_FORMAT_R16_FLOAT;
        case ETexture::Format::Red32Float:                      return DXGI_FORMAT_R32_FLOAT;
        case ETexture::Format::RedGreen16Float:                 return DXGI_FORMAT_R16G16_FLOAT;
        case ETexture::Format::RedGreen32Float:                 return DXGI_FORMAT_R32G32_FLOAT;
        // case ETexture::Format::RedGreenBlue16Float:             return DXGI_FORMAT_R16G16B16_FLOAT;
        case ETexture::Format::RedGreenBlue32Float:             return DXGI_FORMAT_R32G32B32_FLOAT;
        case ETexture::Format::RedGreenBlueAlpha16Float:        return DXGI_FORMAT_R16G16B16A16_FLOAT;
        case ETexture::Format::RedGreenBlueAlpha32Float:        return DXGI_FORMAT_R32G32B32A32_FLOAT;
        case ETexture::Format::Depth16UnsignedInt:              return DXGI_FORMAT_D16_UNORM;
        // case ETexture::Format::Depth32UnsignedInt:              return DXGI_FORMAT_D32;
        case ETexture::Format::Depth32Float:                    return DXGI_FORMAT_D32_FLOAT;
        // case ETexture::Format::Stencil8UnsignedInt:             return DXGI_FORMAT_S8;
        // case ETexture::Format::Stencil16UnsignedInt:            return DXGI_FORMAT_;
        case ETexture::Format::Depth24Stencil8:                 return DXGI_FORMAT_D24_UNORM_S8_UINT;
        case ETexture::Format::Depth32FloatStencil8UnsignedInt: return DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
        default: return DXGI_FORMAT_UNKNOWN;
    }
}
#endif
