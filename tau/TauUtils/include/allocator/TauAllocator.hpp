#pragma once

#pragma warning(push, 0)
#include <cstdlib>
#pragma warning(pop)

#include "Objects.hpp"
#include "NumTypes.hpp"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

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
[[nodiscard]] constexpr inline _T _alignTo(const _T val, const _T alignment) noexcept
{
    if(alignment == 1)
    { return val; }
    return (val + alignment) & ~(alignment - 1);
}

template<typename _T, _T _Alignment>
[[nodiscard]] constexpr inline _T _alignTo(const _T val) noexcept
{
    if(_Alignment == 1)
    { return val; }
    return (val + _Alignment) & ~(_Alignment - 1);
}

#ifdef _WIN32
[[nodiscard]] inline u32 _ctz(u32 v)
{
    unsigned long trailingZero = 0;
    _BitScanForward(&trailingZero, v);
    return trailingZero;
}

[[nodiscard]] inline u32 _clz(u32 v)
{
    unsigned long leadingZero = 0;
    _BitScanReverse(&leadingZero, v);
    return 31 - leadingZero;
}

[[nodiscard]] inline u64 _ctz(u64 v)
{
    unsigned long trailingZero = 0;
    _BitScanForward64(&trailingZero, v);
    return trailingZero;
}

[[nodiscard]] inline u64 _clz(u64 v)
{
    unsigned long leadingZero = 0;
    _BitScanReverse64(&leadingZero, v);
    return 63 - leadingZero;
}
#else
[[nodiscard]] inline u32 _ctz(u32 v)
{ return __builtin_ctz(v); }

[[nodiscard]] inline u32 _clz(u32 v)
{ return __builtin_clz(v); }

[[nodiscard]] inline u64 _ctz(u64 v)
{ return __builtin_ctzll(v); }

[[nodiscard]] inline u64 _clz(u64 v)
{ return __builtin_clzll(v); }
#endif

#if defined(TAU_CROSS_PLATFORM)
[[nodiscard]] constexpr inline u32 _nextPowerOf2(u32 v) noexcept
{
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return v + 1;
}

[[nodiscard]] constexpr inline u64 _nextPowerOf2(u64 v) noexcept
{
    --v;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v |= v >> 32;
    return v + 1;
}
#else
[[nodiscard]] inline u32 _nextPowerOf2(const u32 v) noexcept
{
    if(v == 1)
    { return 1; }
    return 1 << (32 - _clz(v - 1));
}

[[nodiscard]] inline u64 _nextPowerOf2(const u64 v) noexcept
{
    if(v == 1)
    { return 1; }
    return 1 << (64 - _clz(v - 1)); }
#endif
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
    [[nodiscard]] virtual void* allocate(uSys size) = 0;

    virtual void deallocate(void* obj) = 0;

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
    { return malloc(size); }

    void deallocate(void* const obj) noexcept override
    { free(obj); }
};
