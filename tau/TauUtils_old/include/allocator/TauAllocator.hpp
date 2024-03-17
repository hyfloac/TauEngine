#pragma once

#include "Objects.hpp"
#include "NumTypes.hpp"

namespace _TauAllocatorUtils {
template<typename _T>
struct _RemoveReference final
{ using type = _T; };

template<typename _T>
struct _RemoveReference<_T&> final
{ using type = _T; };

template<typename _T>
struct _RemoveReference<_T&&> final
{ using type = _T; };

template<typename _T>
using _RemoveReferenceT = typename _RemoveReference<_T>::type;

template<typename _T>
[[nodiscard]] constexpr inline _T&& _forward(_RemoveReferenceT<_T>& ref) noexcept
{ return static_cast<_T&&>(ref); }

template<typename _T>
[[nodiscard]] constexpr inline _T&& _forward(_RemoveReferenceT<_T>&& ref) noexcept
{ return static_cast<_T&&>(ref); }

template<typename _T>
[[nodiscard]] constexpr _RemoveReferenceT<_T>&& _move(_T&& arg) noexcept
{ return static_cast<_RemoveReferenceT<_T>&&>(arg); }
}

enum class AllocationAlignment : uSys { };
enum class PageCountVal : uSys { };

enum class AllocationTracking
{
    /**
     * Doesn't perform any allocation tracking.
     */
    None,
    /**
     *   Tracks every time an object is allocated and deallocated.
     * This doesn't track the individual allocations of any given
     * object, but instead keeps track of the number of
     * allocations. Performing a double delete is not tracked.
     */
    Count,
    /**
     *   Tracks every time an object is allocated and deallocated.
     * This also tracks any time an object is double deleted. A
     * double deletion doesn't count towards the deallocation
     * count.
     */
    DoubleDeleteCount
};

class TauAllocator
{
    DEFAULT_CONSTRUCT_PO(TauAllocator);
    DEFAULT_DESTRUCT_VI(TauAllocator);
    DELETE_CM(TauAllocator);
public:
    [[nodiscard]] virtual void* allocate(uSys size) noexcept = 0;

    virtual void deallocate(void* obj) noexcept = 0;

    template<typename _T, typename... _Args>
    _T* allocateT(_Args&&... args) noexcept
    {
        void* const allocation = allocate(sizeof(_T));
        if(!allocation) { return nullptr; }
        return new(allocation) _T(_TauAllocatorUtils::_forward<_Args>(args)...);
    }

    template<typename _T>
    void deallocateT(_T* const obj) noexcept
    {
        if(!obj) { return; }
        obj->~_T();
        deallocate(obj);
    }
};

class DefaultTauAllocator final : public TauAllocator
{
    DEFAULT_CONSTRUCT_PI(DefaultTauAllocator);
    DEFAULT_DESTRUCT(DefaultTauAllocator);
    DELETE_CM(DefaultTauAllocator);
public:
    static DefaultTauAllocator& Instance() noexcept;
public:
    [[nodiscard]] void* allocate(const uSys size) noexcept override
    { return operator new(size); }

    void deallocate(void* const obj) noexcept override
    { operator delete(obj); }
};
