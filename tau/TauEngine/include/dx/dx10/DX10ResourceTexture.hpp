#pragma once

#ifdef _WIN32
#include <d3d10.h>
#include "dx/DXUtils.hpp"
#include "DX10Resource.hpp"

class TAU_DLL DX10ResourceTexture1D final : public DX10Resource
{
    DELETE_CM(DX10ResourceTexture1D);
private:
    ResourceTexture1DArgs _args;
    ID3D10Texture1D* _d3dTexture;
public:
    DX10ResourceTexture1D(const ResourceTexture1DArgs& args, ID3D10Texture1D* const d3dTexture) noexcept
        : _args(args)
        , _d3dTexture(d3dTexture)
    { }

    ~DX10ResourceTexture1D() noexcept
    { RELEASE_DX(_d3dTexture); }

    [[nodiscard]]       ID3D10Texture1D* d3dTexture()       noexcept { return _d3dTexture; }
    [[nodiscard]] const ID3D10Texture1D* d3dTexture() const noexcept { return _d3dTexture; }

    [[nodiscard]] EResource::Type resourceType() const noexcept override { return EResource::Type::Texture1D; }

    [[nodiscard]] void* map(IRenderingContext& context, EResource::MapType mapType, uSys subResource, const ResourceMapRange* mapReadRange) noexcept override;
    void unmap(IRenderingContext& context, uSys subResource) noexcept override;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};

class TAU_DLL DX10ResourceTexture2D final : public DX10Resource
{
    DELETE_CM(DX10ResourceTexture2D);
private:
    ResourceTexture2DArgs _args;
    ID3D10Texture2D* _d3dTexture;
public:
    DX10ResourceTexture2D(const ResourceTexture2DArgs& args, ID3D10Texture2D* const d3dTexture) noexcept
        : _args(args)
        , _d3dTexture(d3dTexture)
    { }

    ~DX10ResourceTexture2D() noexcept
    { RELEASE_DX(_d3dTexture); }

    [[nodiscard]]       ID3D10Texture2D* d3dTexture()       noexcept { return _d3dTexture; }
    [[nodiscard]] const ID3D10Texture2D* d3dTexture() const noexcept { return _d3dTexture; }

    [[nodiscard]] EResource::Type resourceType() const noexcept override { return EResource::Type::Texture2D; }

    [[nodiscard]] void* map(IRenderingContext& context, EResource::MapType mapType, uSys subResource, const ResourceMapRange* mapReadRange) noexcept override;
    void unmap(IRenderingContext& context, uSys subResource) noexcept override;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};

class TAU_DLL DX10ResourceTexture3D final : public DX10Resource
{
    DELETE_CM(DX10ResourceTexture3D);
private:
    ResourceTexture3DArgs _args;
    ID3D10Texture3D* _d3dTexture;
public:
    DX10ResourceTexture3D(const ResourceTexture3DArgs& args, ID3D10Texture3D* const d3dTexture) noexcept
        : _args(args)
        , _d3dTexture(d3dTexture)
    { }

    ~DX10ResourceTexture3D() noexcept
    { RELEASE_DX(_d3dTexture); }

    [[nodiscard]]       ID3D10Texture3D* d3dTexture()       noexcept { return _d3dTexture; }
    [[nodiscard]] const ID3D10Texture3D* d3dTexture() const noexcept { return _d3dTexture; }

    [[nodiscard]] EResource::Type resourceType() const noexcept override { return EResource::Type::Texture3D; }

    [[nodiscard]] void* map(IRenderingContext& context, EResource::MapType mapType, uSys subResource, const ResourceMapRange* mapReadRange) noexcept override;
    void unmap(IRenderingContext& context, uSys subResource) noexcept override;
protected:
    [[nodiscard]] const void* _getArgs() const noexcept override { return &_args; }
};
#endif
