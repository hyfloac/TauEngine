#pragma once

#include "NumTypes.hpp"
#include "Objects.hpp"

class PageAllocator final
{
    DELETE_CONSTRUCT(PageAllocator);
    DELETE_CM(PageAllocator);
    DELETE_DESTRUCT(PageAllocator);
private:
    static uSys _pageSize;
    static bool _initialized;
public:
    static void init() noexcept;

    static void* reserve(uSys numPages) noexcept;

    /**
     * Reserves and commits the pages.
     */
    static void* alloc(uSys numPages) noexcept;

    static void* commitPage(void* page) noexcept;
    static void* commitPages(void* page, uSys pageCount) noexcept;

    static void decommitPage(void* page) noexcept;
    static void decommitPages(void* page, uSys pageCount) noexcept;

    static void free(void* page) noexcept;

    static void setReadWrite(void* page, uSys pageCount = 1) noexcept;
    static void setReadOnly(void* page, uSys pageCount = 1) noexcept;
    static void setExecute(void* page, uSys pageCount = 1) noexcept;

    static uSys pageSize() noexcept;
};
