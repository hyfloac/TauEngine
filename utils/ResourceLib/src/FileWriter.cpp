#include "FileWriter.hpp"
#include "IFile.hpp"
#include <allocator/PageAllocator.hpp>

FileWriter::FileWriter(const CPPRef<IFile>& file) noexcept
    : _file(file)
    , _fileIndex(0)
    , _bufferIndex(0)
    , _bufferSize(TAU_FW_BUFFER_PAGE_CNT * PageAllocator::pageSize())
    , _buffer(PageAllocator::alloc(TAU_FW_BUFFER_PAGE_CNT))
{ file->setPos(0); }

FileWriter::~FileWriter() noexcept
{
    flush();
    PageAllocator::free(_buffer);
}

i64 FileWriter::flush() noexcept
{
    // Is there any data in the buffer?
    if(_bufferIndex == 0)
    { return 0; }

    const i64 writeCount = _file->write(_buffer, _bufferIndex - 1);

    // Did we fail to write?
    if(writeCount <= 0)
    { return 0; }

    _fileIndex += writeCount;
    _bufferIndex = 0;

    return 0;
}

i64 FileWriter::write(const void* buffer, const uSys bufferSize) noexcept
{
    if(!buffer || bufferSize == 0)
    { return 0; }

    // Will we overflow the buffer?
    if(_bufferIndex + bufferSize > _bufferSize)
    {
        uSys writeOffset = 0;
        // Loop through until we've written all the bytes.
        do
        {
            const uSys writeCount = _bufferSize - _bufferIndex;
            (void) ::std::memcpy(reinterpret_cast<u8*>(_buffer) + _bufferIndex, reinterpret_cast<const u8*>(buffer) + writeOffset, writeCount);

            writeOffset += writeCount;
            // Flush on each iteration
            flush();
        } while(bufferSize - writeOffset > _bufferSize);

        // Write the remaining bytes that would not overflow the buffer.
        (void) ::std::memcpy(_buffer, reinterpret_cast<const u8*>(buffer) + writeOffset, bufferSize - writeOffset);
        _bufferIndex = bufferSize - writeOffset;
        return bufferSize;
    }
    else
    {
        (void) ::std::memcpy(reinterpret_cast<u8*>(_buffer) + _bufferIndex, buffer, bufferSize);
        _bufferIndex += bufferSize;
        return bufferSize;
    }
}

i64 FileWriter::write1(const void* buffer) noexcept
{
    if(_bufferIndex == _bufferSize)
    { flush(); }

    if(!buffer)
    { return 0; }

    (void) ::std::memcpy(reinterpret_cast<u8*>(_buffer) + _bufferIndex, buffer, 1);
    ++_bufferIndex;

    return 1;
}

i64 FileWriter::write2(const void* buffer) noexcept
{
    if(_bufferIndex == _bufferSize)
    { flush(); }

    if(!buffer)
    { return 0; }

    // Do we only have 1 byte left in the buffer.
    if(_bufferIndex + 2 > _bufferSize)
    {
        // Write the first byte.
        (void) ::std::memcpy(reinterpret_cast<u8*>(_buffer) + _bufferIndex, buffer, 1);

        // Flush the buffer.
        const i64 writeCount = _file->write(_buffer, _bufferSize);

        // Did we fail to write?
        if(writeCount <= 0)
        { return 0; }

        _fileIndex += writeCount;

        // Set index to 1 because we are in a new buffer.
        _bufferIndex = 1;

        // Write the second byte.
        (void) ::std::memcpy(_buffer, reinterpret_cast<const u8*>(buffer), 1);
    }
    else
    {
        (void) ::std::memcpy(reinterpret_cast<u8*>(_buffer) + _bufferIndex, buffer, 2);
        _bufferIndex += 2;
    }

    return 2;
}

i64 FileWriter::writeCountSafe(const void* buffer, uSys bufferSize) noexcept
{
    if(!buffer || bufferSize == 0)
    { return 0; }

    // Will we overflow the buffer?
    if(_bufferIndex + bufferSize > _bufferSize)
    {
        const uSys writeCount = _bufferSize - _bufferIndex;
        (void) ::std::memcpy(reinterpret_cast<u8*>(_buffer) + _bufferIndex, buffer, writeCount);

        if(flush() <= 0)
        { return writeCount; }

        (void) ::std::memcpy(_buffer, reinterpret_cast<const u8*>(buffer) + writeCount, bufferSize - writeCount);
        _bufferIndex = bufferSize - writeCount;
    }
    else
    {
        (void) ::std::memcpy(reinterpret_cast<u8*>(_buffer) + _bufferIndex, buffer, bufferSize);
        _bufferIndex += bufferSize;
    }

    return bufferSize;
}
