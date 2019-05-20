#pragma once

#pragma warning(push, 0)
#include <cstdio>
#pragma warning(pop)

#include <Utils.hpp>

#define ____str(__X) #__X
#define ___str(__X) ____str(__X)

#define Assert(__STATE) if(!(__STATE)) { fprintf(stderr, "`" #__STATE "` Evaluated to false at line " ___str(__LINE__) " in file `" ___str(__FILE__) ".\n"); DEBUG_BREAK; }

#define RUNTIME_ERROR(__TEXT) do { fprintf(stderr, __TEXT "\nOccured at line " ___str(__LINE__) " in file `" ___str(__FILE__) ".\n"); DEBUG_BREAK; } while(0)

#ifdef _DEBUG
  #define Ensure(__STATE) Assert(__STATE)
#else
  #define Ensure(__STATE)
#endif

#define NonNull  /* Guaranteed to not be null. */
#define Nullable /* Potentially could be null. */

/**
 *   A wrapper to print an error and trigger a breakpoint if
 * a value is null.
 */
template<typename _T>
class NotNull final
{
public:
    _T* _t;
public:
    constexpr inline NotNull(_T* t) noexcept : _t(t)
    {
        Ensure(t != nullptr);
    }
    constexpr inline NotNull(const NotNull& copy) noexcept : _t(copy._t) { }
    constexpr inline NotNull(NotNull&& move) noexcept : _t(move._t) { }

    inline ~NotNull() noexcept = default;

    constexpr inline NotNull& operator =(const NotNull& copy) noexcept
    {
        this->_t = copy._t;
        return *this;
    }

    constexpr inline NotNull& operator =(NotNull&& move) noexcept
    {
        this->_t = move._t;
        return *this;
    }

    constexpr inline _T& operator* ()       noexcept { return *_t; }
    constexpr inline _T& operator* () const noexcept { return *_t; }
    constexpr inline _T* operator->()       noexcept { return  _t; }
    constexpr inline _T* operator->() const noexcept { return  _t; }

    constexpr inline _T* operator()()       noexcept { return  _t; }
    constexpr inline _T* operator()() const noexcept { return  _t; }

    constexpr inline operator _T*()         noexcept { return  _t; }
    constexpr inline operator _T*()   const noexcept { return  _t; }
};
