#pragma once

#include "graphics/Resource.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10GraphicsInterface;

class TAU_DLL TAU_NOVTABLE DX10Resource : public IResource
{
    DEFAULT_DESTRUCT_VI(DX10Resource);
    DEFAULT_CM_PO(DX10Resource);
    RESOURCE_IMPL(DX10Resource);
protected:
    DX10Resource(const uSys size) noexcept
        : IResource(size)
    { }
};

class DX10ResourceBuffer;

class TAU_DLL DX10ResourceBuilder final : public IResourceBuilder
{
    DEFAULT_DESTRUCT_VI(DX10ResourceBuilder);
    DEFAULT_CM_PU(DX10ResourceBuilder);
public:
    struct DXResourceBufferArgs final
    {
        ID3D10Buffer* d3dBuffer;
    };
public:
    [[nodiscard]] static D3D10_USAGE getDXUsage(EResource::UsageType usage) noexcept;
    [[nodiscard]] static D3D10_CPU_ACCESS_FLAG getDXAccess(EResource::UsageType usage) noexcept;
    [[nodiscard]] static D3D10_BIND_FLAG getDXBind(EBuffer::Type type) noexcept;
private:
    DX10GraphicsInterface& _gi;
public:
    DX10ResourceBuilder(DX10GraphicsInterface& gi) noexcept
        : _gi(gi)
    { }

    [[nodiscard]] DX10Resource* build(const ResourceBufferArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10Resource* build(const ResourceBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IResource> buildCPPRef(const ResourceBufferArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IResource> buildTauSRef(const ResourceBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const ResourceBufferArgs& args, [[tau::out]] DXResourceBufferArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
