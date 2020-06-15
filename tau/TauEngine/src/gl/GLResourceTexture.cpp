#include "gl/GLResourceTexture.hpp"
#include <AtomicIntrinsics.hpp>
#include "gl/GLTexture.hpp"

void* GLResourceTexture1D::map(IRenderingContext&, const EResource::MapType mapType, const uSys mipLevel, uSys, const ResourceMapRange* const mapReadRange) noexcept
{
    if(mapType != EResource::MapType::Discard || !mapReadRange || mapReadRange->begin < mapReadRange->end)
    {
        /*
         *   Textures are opaque objects and don't support the same
         * mapping modes as buffers.
         */

        return null;
    }

    const iSys currAtomicLockCount = atomicIncrement(_atomicMapCount);

    if(currAtomicLockCount == 1)
    {
        _currentMapping = new(::std::align_val_t{ 64 }, ::std::nothrow) u8[computeSubResourceSizeMip(_args.dataFormat, _args.width, mipLevel)];
        if(!_currentMapping)
        {
            atomicDecrement(_atomicMapCount);
        }

        _mappingEvent.signal();
    }
    else
    {
        /*
         * The buffer is currently mapped, or being mapped.
         *
         *   We wait until signaled in case the buffer is currently
         * being mapped. This is a manual reset event, thus if the
         * buffer is already mapped it should return immediately.
         */
        (void) _mappingEvent.waitUntilSignaled();

        if(!_currentMapping)
        {
            /*
             * The original mapping has failed.
             *
             * Decrement the map count and return null.
             */

            atomicDecrement(_atomicMapCount);
            return null;
        }
    }

    return _currentMapping;
}

void GLResourceTexture1D::unmap(IRenderingContext&, const uSys mipLevel, uSys) noexcept
{
    const iSys currAtomicLockCount = atomicDecrement(_atomicMapCount);

    if(currAtomicLockCount == 0)
    {
        _mappingEvent.reset();

        glBindTexture(GL_TEXTURE_1D, _texture);
        glTexImage1D(GL_TEXTURE_1D, mipLevel, GLTexture2D::glInternalFormat(_args.dataFormat), ETexture::computeMipSide(_args.width, mipLevel), 0, GLTexture2D::glInputFormat(_args.dataFormat), GLTexture2D::glInputDataType(_args.dataFormat), _currentMapping);
        operator delete[](_currentMapping, ::std::align_val_t{ 64 }, ::std::nothrow);
        _currentMapping = null;
    }
}
