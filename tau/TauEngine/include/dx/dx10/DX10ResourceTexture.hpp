#pragma once

#ifdef _WIN32
#include "graphics/ResourceRawInterface.hpp"
#include "dx/DXUtils.hpp"
#include "DX10Resource.hpp"

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
    friend class DX10ResourceDefaultTexture1D;
    friend class DX10ResourceTransferTexture1D;
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
    friend class DX10ResourceDefaultTexture2D;
    friend class DX10ResourceTransferTexture2D;
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
    friend class DX10ResourceDefaultTexture3D;
    friend class DX10ResourceTransferTexture3D;
};

class TAU_DLL TAU_NOVTABLE DX10ResourceTexture1D : public DX10Resource
{
    DELETE_CM(DX10ResourceTexture1D);
protected:
    ResourceTexture1DArgs _args;
    DX10ResourceTexture1DRawInterface _rawInterface;
    __declspec(property(get = d3dTexture, put = setD3DTexture)) ID3D10Texture1D* _d3dTexture;
protected:
    DX10ResourceTexture1D(const ResourceTexture1DArgs& args, ID3D10Texture1D* const d3dTexture) noexcept
        : DX10Resource(ETexture::computeSizeMipArr(args.dataFormat, args.width, args.mipLevels, args.arrayCount), EResource::Type::Texture1D, args.usageType)
        , _args(args)
        , _rawInterface(d3dTexture)
    { }
public:
    ~DX10ResourceTexture1D() noexcept override
    { RELEASE_DX(_d3dTexture); }

    [[nodiscard]] ID3D10Texture1D* d3dTexture() const noexcept { return _rawInterface._d3dTexture; }

    [[nodiscard]] const IResourceRawInterface& _getRawHandle() const noexcept override final { return _rawInterface; }
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override final { return &_args; }

#if defined(_MSVC_LANG) || 1
    void setD3DTexture(ID3D10Texture1D* const texture) noexcept { _rawInterface._d3dTexture = texture; }
#endif
};

class TAU_DLL TAU_NOVTABLE DX10ResourceTexture2D : public DX10Resource
{
    DELETE_CM(DX10ResourceTexture2D);
protected:
    ResourceTexture2DArgs _args;
    DX10ResourceTexture2DRawInterface _rawInterface;
    __declspec(property(get = d3dTexture, put = setD3DTexture)) ID3D10Texture2D* _d3dTexture;
protected:
    DX10ResourceTexture2D(const ResourceTexture2DArgs& args, ID3D10Texture2D* const d3dTexture) noexcept
        : DX10Resource(ETexture::computeSizeMipArr(args.dataFormat, args.width, args.height, args.mipLevels, args.arrayCount), EResource::Type::Texture2D, args.usageType)
        , _args(args)
        , _rawInterface(d3dTexture)
    { }
public:
    ~DX10ResourceTexture2D() noexcept override
    { RELEASE_DX(_d3dTexture); }

    [[nodiscard]] ID3D10Texture2D* d3dTexture() const noexcept { return _rawInterface._d3dTexture; }

    [[nodiscard]] const IResourceRawInterface& _getRawHandle() const noexcept override final { return _rawInterface; }
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override final { return &_args; }

#if defined(_MSVC_LANG) || 1
    void setD3DTexture(ID3D10Texture2D* const texture) noexcept { _rawInterface._d3dTexture = texture; }
#endif
};

class TAU_DLL TAU_NOVTABLE DX10ResourceTexture3D : public DX10Resource
{
    DELETE_CM(DX10ResourceTexture3D);
protected:
    ResourceTexture3DArgs _args;
    DX10ResourceTexture3DRawInterface _rawInterface;
    __declspec(property(get = d3dTexture, put = setD3DTexture)) ID3D10Texture3D* _d3dTexture;
protected:
    DX10ResourceTexture3D(const ResourceTexture3DArgs& args, ID3D10Texture3D* const d3dTexture) noexcept
        : DX10Resource(ETexture::computeSizeMipArr(args.dataFormat, args.width, args.height, args.depth, args.mipLevels), EResource::Type::Texture3D, args.usageType)
        , _args(args)
        , _rawInterface(d3dTexture)
    { }
public:
    ~DX10ResourceTexture3D() noexcept override
    { RELEASE_DX(_d3dTexture); }

    [[nodiscard]] ID3D10Texture3D* d3dTexture() const noexcept { return _rawInterface._d3dTexture; }
    
    [[nodiscard]] const IResourceRawInterface& _getRawHandle() const noexcept override final { return _rawInterface; }
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override final { return &_args; }

#if defined(_MSVC_LANG) || 1
    void setD3DTexture(ID3D10Texture3D* const texture) noexcept { _rawInterface._d3dTexture = texture; }
#endif
};

class TAU_DLL DX10ResourceDefaultTexture1D final : public DX10ResourceTexture1D
{
    DEFAULT_DESTRUCT(DX10ResourceDefaultTexture1D);
    DELETE_CM(DX10ResourceDefaultTexture1D);
public:
    DX10ResourceDefaultTexture1D(const ResourceTexture1DArgs& args, ID3D10Texture1D* const d3dTexture) noexcept
        : DX10ResourceTexture1D(args, d3dTexture)
    { }
    
    [[nodiscard]] void* map(const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange* const mapReadRange, const ResourceMapRange* const mapWriteRange) noexcept override { return nullptr; }
    void unmap(const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange*) noexcept override { }
};

class TAU_DLL DX10ResourceDefaultTexture2D final : public DX10ResourceTexture2D
{
    DEFAULT_DESTRUCT(DX10ResourceDefaultTexture2D);
    DELETE_CM(DX10ResourceDefaultTexture2D);
public:
    DX10ResourceDefaultTexture2D(const ResourceTexture2DArgs& args, ID3D10Texture2D* const d3dTexture) noexcept
        : DX10ResourceTexture2D(args, d3dTexture)
    { }

    [[nodiscard]] void* map(const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange* const mapReadRange, const ResourceMapRange* const mapWriteRange) noexcept override { return nullptr; }
    void unmap(const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange*) noexcept override { }
};

class TAU_DLL DX10ResourceDefaultTexture3D final : public DX10ResourceTexture3D
{
    DEFAULT_DESTRUCT(DX10ResourceDefaultTexture3D);
    DELETE_CM(DX10ResourceDefaultTexture3D);
public:
    DX10ResourceDefaultTexture3D(const ResourceTexture3DArgs& args, ID3D10Texture3D* const d3dTexture) noexcept
        : DX10ResourceTexture3D(args, d3dTexture)
    { }

    [[nodiscard]] void* map(const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange* const mapReadRange, const ResourceMapRange* const mapWriteRange) noexcept override { return nullptr; }
    void unmap(const uSys mipLevel, const uSys arrayIndex, const ResourceMapRange*) noexcept override { }
};

class TAU_DLL DX10ResourceTransferTexture1D final : public DX10ResourceTexture1D
{
    DELETE_CM(DX10ResourceTransferTexture1D);
private:
    void* _mapping;
public:
    DX10ResourceTransferTexture1D(const ResourceTexture1DArgs& args, ID3D10Texture1D* const d3dTexture) noexcept
        : DX10ResourceTexture1D(args, d3dTexture)
        , _mapping(DX10TransferResource::allocate(_size))
    { }

    ~DX10ResourceTransferTexture1D() noexcept override
    { DX10TransferResource::deallocate(_mapping); }

    void transferMapping(void* ptr) noexcept;
    [[nodiscard]] void* transferMapping() noexcept;

    [[nodiscard]] void* map(uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange, const ResourceMapRange*mapWriteRange) noexcept override;
    void unmap(uSys mipLevel, uSys arrayIndex, const ResourceMapRange*) noexcept override;
};

class TAU_DLL DX10ResourceTransferTexture2D final : public DX10ResourceTexture2D
{
    DELETE_CM(DX10ResourceTransferTexture2D);
private:
    void* _mapping;
public:
    DX10ResourceTransferTexture2D(const ResourceTexture2DArgs& args, ID3D10Texture2D* const d3dTexture) noexcept
        : DX10ResourceTexture2D(args, d3dTexture)
        , _mapping(DX10TransferResource::allocate(_size))
    { }

    ~DX10ResourceTransferTexture2D() noexcept override
    { DX10TransferResource::deallocate(_mapping); }

    void transferMapping(void* ptr) noexcept;
    [[nodiscard]] void* transferMapping() noexcept;

    [[nodiscard]] void* map(uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept override;
    void unmap(uSys mipLevel, uSys arrayIndex, const ResourceMapRange*) noexcept override;
};

class TAU_DLL DX10ResourceTransferTexture3D final : public DX10ResourceTexture3D
{
    DEFAULT_DESTRUCT(DX10ResourceTransferTexture3D);
    DELETE_CM(DX10ResourceTransferTexture3D);
private:
    void* _mapping;
public:
    DX10ResourceTransferTexture3D(const ResourceTexture3DArgs& args, ID3D10Texture3D* const d3dTexture) noexcept
        : DX10ResourceTexture3D(args, d3dTexture)
        , _mapping(DX10TransferResource::allocate(_size))
    { }

    void transferMapping(void* ptr) noexcept;
    [[nodiscard]] void* transferMapping() noexcept;

    [[nodiscard]] void* map(uSys mipLevel, uSys arrayIndex, const ResourceMapRange* mapReadRange, const ResourceMapRange* mapWriteRange) noexcept override;
    void unmap(uSys mipLevel, uSys arrayIndex, const ResourceMapRange*) noexcept override;
};
#endif
