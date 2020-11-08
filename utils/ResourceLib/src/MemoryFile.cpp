#include "MemoryFile.hpp"
#include <allocator/PageAllocator.hpp>
#include <TUMaths.hpp>

#pragma warning(push, 0)
#ifdef _WIN32
  #include <intrin.h>
  #include <Windows.h>
#else
  #include <sys/time.h>
  #include <ctime>
#endif
#pragma warning(pop)


static u64 currentTimeMillis() noexcept
{
#ifdef _WIN32
    /* Windows */
    FILETIME fileTime;
    ULARGE_INTEGER fileTimeInt;

    /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
     * to a LARGE_INTEGER structure. */
    GetSystemTimeAsFileTime(&fileTime);
    fileTimeInt.LowPart = fileTime.dwLowDateTime;
    fileTimeInt.HighPart = fileTime.dwHighDateTime;

    u64 ret = static_cast<u64>(fileTimeInt.QuadPart);
    ret -= 116444736000000000uLL; /* Convert from file time to UNIX epoch time. */
    ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

    return ret;
#else
    /* Linux */
    struct timeval timeVal;

    gettimeofday(&timeVal, null);

    u64 ret = timeVal.tv_usec;
    /* Convert from micro seconds (10*-6) to milliseconds (10**-3) */
    ret /= 1000;

    /* Subtracts the seconds (10**0) after converting them to milliseconds (10**-3) */
    ret += (timeVal.tv_sec * 1000);

    return ret;
#endif
}

MemoryFile::InternalFile::InternalFile(void* const rawPages, const uSys _reservedPages) noexcept
    : view { rawPages }
    , reservedPages(_reservedPages)
    , committedPages(1)
    , fileSize(0)
    , modifyTime(0)
    , creationTime(currentTimeMillis())
{ }

void MemoryFile::setPos(const uSys pos) noexcept
{
    _cursor = pos;
    if(_cursor > _file->fileSize)
    {
        (void) assertSize(_cursor);
    }
}

void MemoryFile::advancePos(const iSys phase) noexcept
{
    // Ensure the phase won't underflow.
    if(phase < 0 && -phase > static_cast<iSys>(_cursor))
    { return; }

    _cursor += phase;
    if(_cursor > _file->fileSize)
    {
        (void) assertSize(_cursor);
    }
}

i64 MemoryFile::readBytes(u8* const buffer, const uSys len) noexcept
{
    if(_props != FileProps::Read && _props != FileProps::ReadWrite)
    { return -1; }

    if(_cursor + len > _file->fileSize)
    {
        const uSys readLen = _file->fileSize - _cursor;
        (void) ::std::memcpy(buffer, _file->view.buf + _cursor, readLen);
        _cursor += readLen;
        return readLen;
    }
    else
    {
        (void) ::std::memcpy(buffer, _file->view.buf + _cursor, len);
        _cursor += len;
        return len;
    }
}

i64 MemoryFile::writeBytes(const u8* const buffer, const uSys len) noexcept
{
    if(_props == FileProps::Read)
    { return -1; }

    if(_cursor + len > _file->committedPages * PageAllocator::pageSize())
    {
        if(!assertSize(_cursor + len))
        { return -1; }
    }
    (void) ::std::memcpy(_file->view.buf + _cursor, buffer, len);
    _cursor += len;
    if(_cursor > _file->fileSize)
    { _file->fileSize = _cursor; }

    _file->modifyTime = currentTimeMillis();

    return len;
}

bool MemoryFile::reserveData(const uSys pages) noexcept
{
    if(_file->committedPages + pages > _file->reservedPages)
    { return false; }
    const uSys pageBytes = (_file->committedPages - 1) * PageAllocator::pageSize();
    (void) PageAllocator::commitPages(_file->view.buf + pageBytes, pages);
    _file->committedPages += pages;
    return true;
}

bool MemoryFile::assertSize(const uSys targetSize) noexcept
{
    const uSys pageBytes = (_file->committedPages - 1) * PageAllocator::pageSize();
    if(pageBytes < targetSize)
    {
        const uSys targetPageBytes = _alignTo(targetSize, PageAllocator::pageSize());
        const uSys diff = targetPageBytes - pageBytes;
        const uSys targetPageCount = diff / PageAllocator::pageSize() + 4;
        return reserveData(targetPageCount);
    }
    return true;
}


CPPRef<MemoryFileLoader>& MemoryFileLoader::Instance() noexcept
{
    static CPPRef<MemoryFileLoader> instance(new MemoryFileLoader);
    return instance;
}

bool MemoryFileLoader::fileExists(const wchar_t* path) const noexcept
{ return _fileMap.count(path) > 0; }

CPPRef<IFile> MemoryFileLoader::load(const wchar_t* path, FileProps props) const noexcept
{
    const WDynString wPath(path);
    if(_fileMap.count(wPath) > 0)
    {
        if(props == FileProps::WriteNew)
        {
            if(!deleteFile(path))
            { return nullptr; }
        }
        else if(props == FileProps::WriteAppend)
        {
            const auto& file = _fileMap.at(wPath);
            return CPPRef<MemoryFile>(new(::std::nothrow) MemoryFile(file, ::std::move(wPath), props, file->fileSize));
        }
        else
        {
            return CPPRef<MemoryFile>(new(::std::nothrow) MemoryFile(_fileMap.at(wPath), ::std::move(wPath), props));
        }
    }
    
    void* pages = PageAllocator::reserve(16384);
    PageAllocator::commitPage(pages);

    const Ref<MemoryFile::InternalFile> internalFile(pages, 16384);
    _fileMap[wPath] = internalFile;
    return CPPRef<MemoryFile>(new(::std::nothrow) MemoryFile(internalFile, ::std::move(wPath), props));
}

bool MemoryFileLoader::deleteFile(const wchar_t* path) const noexcept
{
    const WDynString wPath(path);
    if(_fileMap.count(wPath) > 0)
    {
        auto& file = _fileMap.at(wPath);
        if(file.refCount() > 1)
        { return false; }
        _fileMap.erase(wPath);
        return true;
    }

    return false;
}

u64 MemoryFileLoader::creationTime(const wchar_t* path) const noexcept
{
    const WDynString wPath(path);
    if(_fileMap.count(wPath) > 0)
    {
        return _fileMap.at(wPath)->creationTime;
    }

    return 0;
}

u64 MemoryFileLoader::modifyTime(const wchar_t* path) const noexcept
{
    const WDynString wPath(path);
    if(_fileMap.count(wPath) > 0)
    {
        return _fileMap.at(wPath)->modifyTime;
    }

    return 0;
}
