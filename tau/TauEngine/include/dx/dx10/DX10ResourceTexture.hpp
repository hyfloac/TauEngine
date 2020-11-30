#pragma once

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"
#include "DX10Resource.hpp"
#include "graphics/ResourceRawInterface.hpp"

class TAU_DLL DX10ResourceTexture1DRawInterface final : public IResourceRawInterface
{
private:
    ID3D10Texture1D* _d3dTexture;
public:
    DX10ResourceTexture1DRawInterface(ID3D10Texture1D* const d3dTexture) noexcept
        : _d3dTexture(d3dTexture)
    { }

    [[nodiscard]] void* rawHandle() const noexcept override { return _d3dTexture; }

    [[nodiscard]] ID3D10Resource* dx10Resource() const noexcept override { return _d3dTexture; }
    [[nodiscard]] ID3D10Texture1D* dx10Texture1D() const noexcept override { return _d3dTexture; }
private:
    friend class DX10ResourceTexture1D;
};

class TAU_DLL DX10ResourceTexture2DRawInterface final : public IResourceRawInterface
{
private:
    ID3D10Texture2D* _d3dTexture;
public:
    DX10ResourceTexture2DRawInterface(ID3D10Texture2D* const d3dTexture) noexcept
        : _d3dTexture(d3dTexture)
    { }

    [[nodiscard]] void* rawHandle() const noexcept override { return _d3dTexture; }

    [[nodiscard]] ID3D10Resource* dx10Resource() const noexcept override { return _d3dTexture; }
    [[nodiscard]] ID3D10Texture2D* dx10Texture2D() const noexcept override { return _d3dTexture; }
private:
    friend class DX10ResourceTexture2D;
};

class TAU_DLL DX10ResourceTexture3DRawInterface final : public IResourceRawInterface
{
private:
    ID3D10Texture3D* _d3dTexture;
public:
    DX10ResourceTexture3DRawInterface(ID3D10Texture3D* const d3dTexture) noexcept
        : _d3dTexture(d3dTexture)
    { }

    [[nodiscard]] void* rawHandle() const noexcept override { return _d3dTexture; }

    [[nodiscard]] ID3D10Resource* dx10Resource() const noexcept override { return _d3dTexture; }
    [[nodiscard]] ID3D10Texture3D* dx10Texture3D() const noexcept override { return _d3dTexture; }
private:
    friend class DX10ResourceTexture3D;
};

class TAU_DLL DX10ResourceTexture1D final : public DX10Resource
{
    DELETE_CM(DX10ResourceTexture1D);
private:
    ResourceTexture1DArgs _args;
    DX10ResourceTexture1DRawInterface _rawInterface;
    __declspec(property(get = d3dTexture, put = setD3DTexture)) ID3D10Texture1D* _d3dTexture;
public:
    DX10ResourceTexture1D(const ResourceTexture1DArgs& args, ID3D10Texture1D* const d3dTexture) noexcept
        : DX10Resource(ETexture::computeSizeMipArr(args.dataFormat, args.width, args.mipLevels, args.arrayCount), EResource::Type::Texture1D)
        , _args(args)
        , _rawInterface(d3dTexture)
    { }

    ~DX10ResourceTexture1D() noexcept override
    { RELEASE_DX(_d3dTexture); }

    [[nodiscard]] ID3D10Texture1D* d3dTexture() const noexcept { return _rawInterface._d3dTexture; }

    [[nodiscard]] void* map(ICommandList&, const EResource::MapType mapType, const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange* const mapReadRange, const ResourceMapRange* const mapWriteRange) noexcept override
    { return map(mapType, mipLevel, arrayIndex, mapReadRange, mapWriteRange); }

    void unmap(ICommandList&, const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange*) noexcept override
    { unmap(mipLevel, arrayIndex); }
    
    [[nodiscard]] void* map(EResource::MapType mapType, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept;
    void unmap(uSys mipLevel, uSys arrayIndex) noexcept;

    [[nodiscard]] const IResourceRawInterface& _getRawHandle() const noexcept override { return _rawInterface; }
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }

#if defined(_MSVC_LANG) || 1
    void setD3DTexture(ID3D10Texture1D* const texture) noexcept { _rawInterface._d3dTexture = texture; }
#endif
};

class TAU_DLL DX10ResourceTexture2D final : public DX10Resource
{
    DELETE_CM(DX10ResourceTexture2D);
private:
    ResourceTexture2DArgs _args;
    DX10ResourceTexture2DRawInterface _rawInterface;
    __declspec(property(get = d3dTexture, put = setD3DTexture)) ID3D10Texture2D* _d3dTexture;
public:
    DX10ResourceTexture2D(const ResourceTexture2DArgs& args, ID3D10Texture2D* const d3dTexture) noexcept
        : DX10Resource(ETexture::computeSizeMipArr(args.dataFormat, args.width, args.height, args.mipLevels, args.arrayCount), EResource::Type::Texture2D)
        , _args(args)
        , _rawInterface(d3dTexture)
    { }

    ~DX10ResourceTexture2D() noexcept override
    { RELEASE_DX(_d3dTexture); }

    [[nodiscard]] ID3D10Texture2D* d3dTexture() const noexcept { return _rawInterface._d3dTexture; }
    
    [[nodiscard]] void* map(ICommandList&, const EResource::MapType mapType, const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange* const mapReadRange, const ResourceMapRange*) noexcept override
    { return map(mapType, mipLevel, arrayIndex, mapReadRange); }
    
    void unmap(ICommandList&, const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange*) noexcept override
    { unmap(mipLevel, arrayIndex); }
    
    [[nodiscard]] void* map(EResource::MapType mapType, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange) noexcept;
    void unmap(uSys mipLevel, uSys arrayIndex) noexcept;

    [[nodiscard]] const IResourceRawInterface& _getRawHandle() const noexcept override { return _rawInterface; }
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }

#if defined(_MSVC_LANG) || 1
    void setD3DTexture(ID3D10Texture2D* const texture) noexcept { _rawInterface._d3dTexture = texture; }
#endif
};

class TAU_DLL DX10ResourceTexture3D final : public DX10Resource
{
    DELETE_CM(DX10ResourceTexture3D);
private:
    ResourceTexture3DArgs _args;
    DX10ResourceTexture3DRawInterface _rawInterface;
    __declspec(property(get = d3dTexture, put = setD3DTexture)) ID3D10Texture3D* _d3dTexture;
public:
    DX10ResourceTexture3D(const ResourceTexture3DArgs& args, ID3D10Texture3D* const d3dTexture) noexcept
        : DX10Resource(ETexture::computeSizeMipArr(args.dataFormat, args.width, args.height, args.depth, args.mipLevels), EResource::Type::Texture3D)
        , _args(args)
        , _rawInterface(d3dTexture)
    { }

    ~DX10ResourceTexture3D() noexcept override
    { RELEASE_DX(_d3dTexture); }

    [[nodiscard]] ID3D10Texture3D* d3dTexture() const noexcept { return _rawInterface._d3dTexture; }
    
    [[nodiscard]] void* map(ICommandList&, const EResource::MapType mapType, const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange* const mapReadRange, const ResourceMapRange*) noexcept override
    { return map(mapType, mipLevel, arrayIndex, mapReadRange); }
    
    void unmap(ICommandList&, const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange*) noexcept override
    { unmap(mipLevel, arrayIndex); }
    
    [[nodiscard]] void* map(EResource::MapType mapType, uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange) noexcept;
    void unmap(uSys mipLevel, uSys arrayIndex) noexcept;

    [[nodiscard]] const IResourceRawInterface& _getRawHandle() const noexcept override { return _rawInterface; }
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }

#if defined(_MSVC_LANG) || 1
    void setD3DTexture(ID3D10Texture3D* const texture) noexcept { _rawInterface._d3dTexture = texture; }
#endif
};
#endif
