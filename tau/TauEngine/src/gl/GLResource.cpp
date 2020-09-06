#include "gl/GLResource.hpp"
#include "gl/GLResourceBuffer.hpp"
#include "gl/GLResourceTexture.hpp"

NullableRef<IResource> GLResourceBuilder::buildTauRef(const ResourceBufferArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept
{
    NullableRef<GLResourceBuffer> resource(allocator);
    ERROR_CODE_COND_N(!resource, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, resource);
}

NullableRef<IResource> GLResourceBuilder::buildTauRef(const ResourceTexture1DArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept
{
    NullableRef<GLResourceTexture1D> resource(allocator);
    ERROR_CODE_COND_N(!resource, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, resource);
}

NullableRef<IResource> GLResourceBuilder::buildTauRef(const ResourceTexture2DArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept
{
    NullableRef<GLResourceTexture2D> resource(allocator);
    ERROR_CODE_COND_N(!resource, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, resource);
}

NullableRef<IResource> GLResourceBuilder::buildTauRef(const ResourceTexture3DArgs& args, ResourceHeap heap, Error* error, TauAllocator& allocator) const noexcept
{
    NullableRef<GLResourceTexture3D> resource(allocator);
    ERROR_CODE_COND_N(!resource, Error::SystemMemoryAllocationFailure);

    ERROR_CODE_V(Error::NoError, resource);
}

uSys GLResourceBuilder::_allocSize(const uSys type) const noexcept
{
    switch(type)
    {
        case RB_AS_BUFFER:     return sizeof(GLResourceBuffer);
        case RB_AS_TEXTURE_1D: return sizeof(GLResourceTexture1D);
        case RB_AS_TEXTURE_2D: return sizeof(GLResourceTexture2D);
        case RB_AS_TEXTURE_3D: return sizeof(GLResourceTexture3D);
        default:               return 0;
    }
}
