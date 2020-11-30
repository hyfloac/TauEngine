#pragma once

#include "graphics/Resource.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10GraphicsInterface;

class TAU_DLL TAU_NOVTABLE DX10Resource : public IResource
{
    DEFAULT_DESTRUCT_VI(DX10Resource);
    DELETE_CM(DX10Resource);
    RESOURCE_IMPL(DX10Resource);
protected:
    DX10Resource(const uSys size, const EResource::Type resourceType) noexcept
        : IResource(size, resourceType)
    { }
};

class DX10ResourceBuffer;
class DX10ResourceTexture1D;
class DX10ResourceTexture2D;
class DX10ResourceTexture3D;

class TAU_DLL DX10ResourceBuilder final : public IResourceBuilder
{
    DEFAULT_DESTRUCT_VI(DX10ResourceBuilder);
    DEFAULT_CM_PU(DX10ResourceBuilder);
public:
    struct DXResourceBufferArgs final
    {
        ID3D10Buffer* d3dBuffer;
    };

    struct DXResourceTexture1DArgs final
    {
        ID3D10Texture1D* d3dTexture;
    };

    struct DXResourceTexture2DArgs final
    {
        ID3D10Texture2D* d3dTexture;
    };

    struct DXResourceTexture3DArgs final
    {
        ID3D10Texture3D* d3dTexture;
    };
public:
    [[nodiscard]] static D3D10_USAGE dxUsage(EResource::UsageType usage) noexcept;
    [[nodiscard]] static D3D10_CPU_ACCESS_FLAG dxAccess(EResource::UsageType usage) noexcept;
    [[nodiscard]] static D3D10_BIND_FLAG dxBind(EBuffer::Type type) noexcept;
private:
    DX10GraphicsInterface& _gi;
public:
    DX10ResourceBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceBufferArgs&    args, ResourceHeap heap, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceTexture1DArgs& args, ResourceHeap heap, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceTexture2DArgs& args, ResourceHeap heap, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceTexture3DArgs& args, ResourceHeap heap, [[tau::out]] Error* error, TauAllocator& allocator) const noexcept override;
protected:
    [[nodiscard]] uSys _allocSize(uSys type) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const ResourceBufferArgs&    args, [[tau::out]] DXResourceBufferArgs*    dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const ResourceTexture1DArgs& args, [[tau::out]] DXResourceTexture1DArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const ResourceTexture2DArgs& args, [[tau::out]] DXResourceTexture2DArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const ResourceTexture3DArgs& args, [[tau::out]] DXResourceTexture3DArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
