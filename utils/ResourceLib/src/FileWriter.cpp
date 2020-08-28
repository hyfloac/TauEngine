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
    PageAllocator::free(_buffer);
}

i64 FileWriter::flush() noexcept
{
    return 0;
}

i64 FileWriter::write(const void* buffer, uSys bufferSize) noexcept
{
    UNUSED2(buffer, bufferSize);
    return 0;
}

i64 FileWriter::write1(const void* buffer) noexcept
{
    UNUSED(buffer);
    return 0;
}

i64 FileWriter::write2(const void* buffer) noexcept
{
    UNUSED(buffer);
    return 0;
}
