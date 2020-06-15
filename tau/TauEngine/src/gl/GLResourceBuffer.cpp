#include "gl/GLResourceBuffer.hpp"
#include <AtomicIntrinsics.hpp>

void* GLResourceBuffer::map(IRenderingContext&, const EResource::MapType mapType, uSys, uSys, const ResourceMapRange* const mapReadRange) noexcept
{
    const iSys currAtomicLockCount = atomicIncrement(_atomicMapCount);

    if(currAtomicLockCount == 1)
    {
        // The buffer is not currently mapped.

        if(!mapReadRange || mapReadRange->begin < mapReadRange->end)
        {
            // The user has requested read access to the buffer

            if(mapType == EResource::MapType::Default)
            {
                // The user has requested write access.

                if(mapReadRange)
                {
                    _currentMapping = glMapBufferRange(_glBufferType, mapReadRange->begin, mapReadRange->length(), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
                }
                else
                {
                    _currentMapping = glMapBufferRange(_glBufferType, 0, _size, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
                }
            }
            else if(mapType == EResource::MapType::Discard)
            {
                /*
                 *   The user does not care about the data currently in the
                 * buffer.
                 *
                 * They also require read access for some reason.
                 */

                _currentMapping = new(::std::align_val_t{ 64 }, ::std::nothrow) u8[_size];
            }
            else if(mapType == EResource::MapType::NoOverwrite)
            {
                /*
                 * The user has requested write access.
                 *
                 *   They also promise not to overwrite any data that is
                 * currently in flight.
                 */

                if(mapReadRange)
                {
                    _currentMapping = glMapBufferRange(_glBufferType, mapReadRange->begin, mapReadRange->length(), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
                }
                else
                {
                    _currentMapping = glMapBufferRange(_glBufferType, 0, _size, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
                }
            }
            else if(mapType == EResource::MapType::NoWrite)
            {
                // The user has not requested write access.

                if(mapReadRange)
                {
                    _currentMapping = glMapBufferRange(_glBufferType, mapReadRange->begin, mapReadRange->length(), GL_MAP_READ_BIT);
                }
                else
                {
                    _currentMapping = glMapBufferRange(_glBufferType, 0, _size, GL_MAP_READ_BIT);
                }
            }
        }
        else
        {
            // The user does not need read access.

            switch(mapType)
            {
                case EResource::MapType::Default:
                    // The user wants to only modify portions of the existing buffer.
                    _currentMapping = glMapBufferRange(_glBufferType, 0, _size, GL_MAP_WRITE_BIT);
                    break;
                case EResource::MapType::Discard:
                    // The user doesn't need to retain any of the previous data.
                    _currentMapping = new(::std::align_val_t{ 64 }, ::std::nothrow) u8[_size];
                    break;
                case EResource::MapType::NoOverwrite:
                    // The user promised not to overwrite any data that is currently in flight.
                    _currentMapping = glMapBufferRange(_glBufferType, 0, _size, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
                    break;
                default: break; // The user has requested an invalid mode.
            }
        }

        if(_currentMapping)
        {
            _currentMapType = mapType;
        }
        else
        {
            /*
             * The user has requested an unsupported mode.
             *
             *   Something failed and thus there is not an allocation.
             * Decrement the map count and signal the other threads to
             * continue.
             */
            atomicDecrement(_atomicMapCount);
        }

        /*
         * Signal that the memory has been mapped.
         *
         *   If a thread attempted to map after the current thread, but
         * before this thread has finished mapping, that thread will
         * have halted until this signal is set.
         */

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

        if(_currentMapType != mapType || !_currentMapping)
        {
            /*
             *   This thread is mapped differently from the original
             * mapping or the original mapping has failed.
             *
             * Decrement the map count and return null.
             */

            atomicDecrement(_atomicMapCount);
            return null;
        }
    }
    return _currentMapping;
}

void GLResourceBuffer::unmap(IRenderingContext&, uSys, uSys) noexcept
{
    const iSys currAtomicLockCount = atomicDecrement(_atomicMapCount);

    if(currAtomicLockCount == 0)
    {
        _mappingEvent.reset();

        switch(_currentMapType)
        {
            case EResource::MapType::Default:
                glUnmapBuffer(_glBufferType);
                break;
            case EResource::MapType::Discard:
                glBindBuffer(_glBufferType, _buffer);
                glBufferData(_glBufferType, _size, _currentMapping, _glUsage);
                operator delete[](_currentMapping, ::std::align_val_t{ 64 }, ::std::nothrow);
                break;
            case EResource::MapType::NoOverwrite:
                glUnmapBuffer(_glBufferType);
                break;
            case EResource::MapType::NoWrite:
                glUnmapBuffer(_glBufferType);
                break;
            default: break;
        }

        _currentMapping = null;
        _currentMapType = static_cast<EResource::MapType>(0);
    }
}
