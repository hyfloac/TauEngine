#pragma once

#pragma warning(push, 0)
#include <cstdio>
#pragma warning(pop)
#include <Utils.hpp>

#ifdef _DEBUG
  #define Ensure(__STATE) if(!(__STATE)) { fprintf(stderr, "`" #__STATE "` Evaluated to false.\n"); DEBUG_BREAK; }
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
class NotNull
{
public:
    _T* _t;
public:
    inline NotNull(_T* t) noexcept : _t(t)
    {
        Ensure(t != nullptr);
    }
    inline NotNull(const NotNull& copy) noexcept : _t(copy._t) { }
    inline NotNull(NotNull&& move) noexcept : _t(move._t) { }

    inline NotNull& operator =(const NotNull& copy) noexcept
    {
        this->_t = copy._t;
        return *this;
    }

    inline NotNull& operator =(NotNull&& move) noexcept
    {
        this->_t = move._t;
        return *this;
    }

    inline _T& operator* ()       noexcept { return *_t; }
    inline _T& operator* () const noexcept { return *_t; }
    inline _T* operator->()       noexcept { return  _t; }
    inline _T* operator->() const noexcept { return  _t; }

    inline _T* operator()()       noexcept { return  _t; }
    inline _T* operator()() const noexcept { return  _t; }

    inline operator _T*()         noexcept { return  _t; }
    inline operator _T*()   const noexcept { return  _t; }
};
