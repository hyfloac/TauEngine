#pragma once

#include "NumTypes.hpp"
#include "Objects.hpp"

class PageAllocator final
{
    DELETE_CONSTRUCT(PageAllocator);
    DELETE_COPY(PageAllocator);
    DELETE_DESTRUCT(PageAllocator);
public:
    static void init() noexcept;

    static void* reserve(uSys numPages) noexcept;

    static void* commitPage(void* page) noexcept;

    static void decommitPage(void* page) noexcept;

    static void decommitPages(void* page, uSys pageCount) noexcept;

    static void free(void* page) noexcept;

    static uSys pageSize() noexcept;
};
