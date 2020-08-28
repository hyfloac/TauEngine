#pragma once

#include "graphics/CommandAllocator.hpp"

#include <allocator/FixedBlockAllocator.hpp>
#include <allocator/FreeListAllocator.hpp>

#include "Safeties.hpp"

class TAU_DLL GLCommandAllocator final : public ICommandAllocator
{
    COMMAND_ALLOCATOR_IMPL(GLCommandAllocator);
private:
    FixedBlockArenaAllocator<> _fbAllocator;
    FreeListAllocator _freeList;
public:
    GLCommandAllocator(uSys maxTotalCommands) noexcept;

    [[nodiscard]] const void* head() const noexcept { return _fbAllocator.head(); }
    [[nodiscard]] uSys allocIndex() const noexcept { return _fbAllocator.allocIndex(); }
    [[nodiscard]] uSys blockSize() const noexcept { return _fbAllocator.blockSize(); }

    void reset(const bool releasePages = false) noexcept override
    { _fbAllocator.reset(releasePages); }

    void* allocate() noexcept
    { return _fbAllocator.allocate(); }

    template<typename _T, typename... _Args>
    _T* allocateT(_Args&&... args) noexcept
    {
        void* const placement = allocate();
        if(!placement) 
        { return null; }
        return new(placement) _T(_TauAllocatorUtils::_forward<_Args>(args)...);
    }

    template<typename _T, typename... _Args>
    _T* allocateFreeList(_Args&&... args) noexcept
    {
        return _freeList.allocateT<_T>(_TauAllocatorUtils::_forward<_Args>(args)...);
    }

#if TAU_CA_EXPOSE_MEM_STAT
    [[nodiscard]] iSys  reservedMemory() const noexcept override { return _fbAllocator.reservedPages();  }
    [[nodiscard]] iSys committedMemory() const noexcept override { return _fbAllocator.committedPages(); }
    [[nodiscard]] iSys allocatedMemory() const noexcept override { return _fbAllocator.allocIndex();     }
#endif
};
