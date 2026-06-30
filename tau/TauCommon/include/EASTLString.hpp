/**
 * @file
 */
#pragma once

#if __has_include(<EASTL/functional.h>)
#include <EASTL/functional.h>
#include <String.hpp>

namespace eastl
{

template<typename Char>
struct hash<StringBaseT<Char>>
{
    inline uSys operator()(const ConstExprStringT<Char>& str) const noexcept
    { return str.HashCode(); }
};

template<typename Char>
struct hash<ConstExprStringT<Char>>
{
    inline uSys operator()(const ConstExprStringT<Char>& str) const noexcept
    { return str.HashCode(); }
};

template<typename Char>
struct hash<DynStringT<Char>>
{
    inline uSys operator()(const DynStringT<Char>& str) const noexcept
    { return str.HashCode(); }
};

template<typename Char>
struct hash<DynStringViewT<Char>>
{
    inline uSys operator()(const DynStringViewT<Char>& str) const noexcept
    { return str.HashCode(); }
};

}

#endif
