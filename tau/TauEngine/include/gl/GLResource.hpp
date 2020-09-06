#pragma once

#include "graphics/Resource.hpp"

class TAU_DLL TAU_NOVTABLE GLResource : public IResource
{
    DEFAULT_DESTRUCT_VI(GLResource);
    DEFAULT_CM_PO(GLResource);
    RESOURCE_IMPL(GLResource);
protected:
    GLResource(const uSys size) noexcept
        : IResource(size)
    { }
};

class GLResourceBuffer;
class GLResourceTexture1D;
class GLResourceTexture2D;
class GLResourceTexture3D;

class TAU_DLL GLResourceBuilder : public IResourceBuilder
{
public:
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceBufferArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceTexture1DArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceTexture2DArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept override;
    [[nodiscard]] NullableRef<IResource> buildTauRef(const ResourceTexture3DArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept override;
protected:
    [[nodiscard]] uSys _allocSize(uSys type) const noexcept override;
};
