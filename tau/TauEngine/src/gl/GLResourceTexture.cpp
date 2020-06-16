#include "gl/GLResourceTexture.hpp"
#include <AtomicIntrinsics.hpp>
#include "gl/GLTexture.hpp"

void* GLResourceTexture::map(IRenderingContext&, const EResource::MapType mapType, const uSys mipLevel, uSys, const ResourceMapRange* const mapReadRange) noexcept
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
        _currentMapping = new(::std::align_val_t{ 64 }, ::std::nothrow) u8[computeSize(mipLevel)];
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
        (void)_mappingEvent.waitUntilSignaled();

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

void GLResourceTexture::unmap(IRenderingContext&, const uSys mipLevel, const uSys arrayIndex) noexcept
{
    const iSys currAtomicLockCount = atomicDecrement(_atomicMapCount);

    if(currAtomicLockCount == 0)
    {
        _mappingEvent.reset();

        uploadTexture(mipLevel, arrayIndex, _currentMapping);
        operator delete[](_currentMapping, ::std::align_val_t{ 64 }, ::std::nothrow);
        _currentMapping = null;
    }
}

uSys GLResourceTexture1D::computeSize(uSys mipLevel) const noexcept
{ return computeSubResourceSizeMip(_args.dataFormat, _args.width, mipLevel); }

void GLResourceTexture1D::uploadTexture(uSys mipLevel, uSys, const void* const data) const noexcept
{
    glBindTexture(GL_TEXTURE_1D, _texture);
    glTexImage1D(
        GL_TEXTURE_1D, 
        mipLevel, 
        GLTexture2D::glInternalFormat(_args.dataFormat), 
        ETexture::computeMipSide(_args.width, mipLevel), 
        0, GLTexture2D::glInputFormat(_args.dataFormat), 
        GLTexture2D::glInputDataType(_args.dataFormat), 
        data);
}

uSys GLResourceTexture2D::computeSize(uSys mipLevel) const noexcept
{ return computeSubResourceSizeMip(_args.dataFormat, _args.width, _args.height, mipLevel); }

void GLResourceTexture2D::uploadTexture(uSys mipLevel, uSys, const void* const data) const noexcept
{
    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexImage2D(
        GL_TEXTURE_2D, 
        mipLevel, 
        GLTexture2D::glInternalFormat(_args.dataFormat), 
        ETexture::computeMipSide(_args.width, mipLevel), 
        ETexture::computeMipSide(_args.height, mipLevel), 
        0, 
        GLTexture2D::glInputFormat(_args.dataFormat), 
        GLTexture2D::glInputDataType(_args.dataFormat), 
        data);
}

uSys GLResourceTexture3D::computeSize(uSys mipLevel) const noexcept
{ return computeSubResourceSizeMip(_args.dataFormat, _args.width, _args.height, _args.depth, mipLevel); }

void GLResourceTexture3D::uploadTexture(uSys mipLevel, uSys, const void* const data) const noexcept
{
    glBindTexture(GL_TEXTURE_1D, _texture);
    glTexImage3D(
        GL_TEXTURE_1D,
        mipLevel,
        GLTexture2D::glInternalFormat(_args.dataFormat),
        ETexture::computeMipSide(_args.width, mipLevel),
        ETexture::computeMipSide(_args.height, mipLevel),
        ETexture::computeMipSide(_args.depth, mipLevel),
        0,
        GLTexture2D::glInputFormat(_args.dataFormat),
        GLTexture2D::glInputDataType(_args.dataFormat),
        data);
}

