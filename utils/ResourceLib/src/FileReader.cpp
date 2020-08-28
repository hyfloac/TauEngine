#include "FileReader.hpp"
#include "IFile.hpp"
#include <allocator/PageAllocator.hpp>

FileReader::FileReader(const CPPRef<IFile>& file) noexcept
    : _file(file)
    , _fileIndex(0)
    , _bufferIndex(0)
    , _bufferSize(TAU_FR_BUFFER_PAGE_CNT * PageAllocator::pageSize())
    , _bufferFillCount(0)
    , _buffer(PageAllocator::alloc(TAU_FR_BUFFER_PAGE_CNT))
{ file->setPos(0); }

FileReader::~FileReader() noexcept
{
    PageAllocator::free(_buffer);
}

i64 FileReader::read(void* const buffer, const uSys bufferSize) noexcept
{
    // Are we at the end of the buffer.
    if(_bufferIndex == _bufferFillCount)
    {
        // Refill the buffer.
        const iSys fillCount = bufferData();
        if(fillCount <= 0)
        { return fillCount; }
    }

    if(bufferSize == 0 || !buffer)
    { return 0; }

    uSys fillOffset = 0;

    /* Continue looping if the user is asking for more bytes
     than are buffered in at any given type. */
    while(fillOffset < bufferSize)
    {
        // Will this read past the buffer.
        if(_bufferIndex + bufferSize> _bufferFillCount)
        {
            // Copy the maximum number of bytes that can be read.
            const uSys readSize = _bufferFillCount - _bufferIndex;
            (void) ::std::memcpy(reinterpret_cast<u8*>(buffer) + fillOffset, reinterpret_cast<u8*>(_buffer) + _bufferIndex, readSize);
            fillOffset += readSize;

            // Refill the buffer.            
            const iSys fillCount = bufferData();
            if(fillCount <= 0)
            { return fillOffset; }            
        }
        else
        {
            // Copy the data.
            (void) ::std::memcpy(reinterpret_cast<u8*>(buffer) + fillOffset, reinterpret_cast<u8*>(_buffer) + _bufferIndex, bufferSize);
            _bufferIndex += bufferSize;
            fillOffset += bufferSize;
        }
    }

    return fillOffset;
}

i64 FileReader::read1(void* buffer) noexcept
{
    // Are we at the end of the buffer.
    if(_bufferIndex == _bufferFillCount)
    {
        const iSys fillCount = bufferData();
        if(fillCount <= 0)
        { return fillCount; }
    }

    if(!buffer)
    { return 0; }

    // Copy the data.    
    (void) ::std::memcpy(buffer, reinterpret_cast<u8*>(_buffer) + _bufferIndex, 1);
    ++_bufferIndex;

    return 1;
}

i64 FileReader::read2(void* buffer) noexcept
{
    // Are we at the end of the buffer.
    if(_bufferIndex == _bufferFillCount)
    {
        const iSys fillCount = bufferData();
        if(fillCount <= 0)
        { return fillCount; }
    }

    if(!buffer)
    { return 0; }

    // Is there only 1 byte left in the buffer.
    if(_bufferIndex + 2 > _bufferFillCount)
    {
        // Copy the byte.
        (void) ::std::memcpy(buffer, reinterpret_cast<u8*>(_buffer) + _bufferIndex, 1);

        // Refill the buffer.        
        const iSys fillCount = bufferData();
        if(fillCount <= 0)
        { return 0; }

        // Copy the remaining byte.        
        (void) ::std::memcpy(reinterpret_cast<u8*>(buffer) + 1, _buffer, 1);

        // Only increment by 1 because we are in a new buffer.
        ++_bufferIndex;
    }
    else
    {
        // Copy the data.
        (void) ::std::memcpy(buffer, reinterpret_cast<u8*>(_buffer) + _bufferIndex, 2);
        _bufferIndex += 2;
    }

    return 2;
}

i64 FileReader::bufferData() noexcept
{
    const iSys fillCount = _file->read(_buffer, _bufferSize);
    if(fillCount <= 0)
    { return fillCount; }

    _fileIndex += fillCount;
    _bufferFillCount = fillCount;
    _bufferIndex = 0;
    return fillCount;
}
