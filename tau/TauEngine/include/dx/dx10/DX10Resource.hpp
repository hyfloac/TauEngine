#pragma once

#include "graphics/Resource.hpp"

#ifdef _WIN32
#include <d3d10.h>

class DX10GraphicsInterface;

class TAU_DLL TAU_NOVTABLE DX10Resource : public IResource
{
    DEFAULT_CONSTRUCT_PO(DX10Resource);
    DEFAULT_DESTRUCT_VI(DX10Resource);
    DEFAULT_CM_PO(DX10Resource);
    RESOURCE_IMPL(DX10Resource);
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

    [[nodiscard]] DX10Resource* build(const ResourceBufferArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10Resource* build(const ResourceBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IResource> buildCPPRef(const ResourceBufferArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IResource> buildTauSRef(const ResourceBufferArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX10Resource* build(const ResourceTexture1DArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10Resource* build(const ResourceTexture1DArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IResource> buildCPPRef(const ResourceTexture1DArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceTexture1DArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IResource> buildTauSRef(const ResourceTexture1DArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX10Resource* build(const ResourceTexture2DArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10Resource* build(const ResourceTexture2DArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IResource> buildCPPRef(const ResourceTexture2DArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceTexture2DArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IResource> buildTauSRef(const ResourceTexture2DArgs& args, Error* error, TauAllocator& allocator) const noexcept override;

    [[nodiscard]] DX10Resource* build(const ResourceTexture3DArgs& args, Error* error) const noexcept override;
    [[nodiscard]] DX10Resource* build(const ResourceTexture3DArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] CPPRef<IResource> buildCPPRef(const ResourceTexture3DArgs& args, Error* error) const noexcept override;
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceTexture3DArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableStrongRef<IResource> buildTauSRef(const ResourceTexture3DArgs& args, Error* error, TauAllocator& allocator) const noexcept override;
private:
    [[nodiscard]] bool processArgs(const ResourceBufferArgs& args, [[tau::out]] DXResourceBufferArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const ResourceTexture1DArgs& args, [[tau::out]] DXResourceTexture1DArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const ResourceTexture2DArgs& args, [[tau::out]] DXResourceTexture2DArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
    [[nodiscard]] bool processArgs(const ResourceTexture3DArgs& args, [[tau::out]] DXResourceTexture3DArgs* dxArgs, [[tau::out]] Error* error) const noexcept;
};
#endif
