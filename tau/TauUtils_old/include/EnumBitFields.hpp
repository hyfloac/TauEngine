#pragma once

/*
 *   A series of templates for producing binary arithmetic
 * on enum bit fields.
 */

template<typename _T> inline _T  operator ~(_T a) noexcept;

template<typename _T> inline _T operator |(_T a, _T b) noexcept;
template<typename _T> inline _T operator &(_T a, _T b) noexcept;
template<typename _T> inline _T operator ^(_T a, _T b) noexcept;

template<typename _T> inline _T& operator |=(_T& a, const _T& b) noexcept;
template<typename _T> inline _T& operator &=(_T& a, const _T& b) noexcept;
template<typename _T> inline _T& operator ^=(_T& a, const _T& b) noexcept;

template<typename _Int, typename _Enum> inline _Int  operator |(_Int a, _Enum b) noexcept;
template<typename _Int, typename _Enum> inline _Int  operator &(_Int a, _Enum b) noexcept;
template<typename _Int, typename _Enum> inline _Int  operator ^(_Int a, _Enum b) noexcept;

template<typename _Int, typename _Enum> inline _Int& operator |=(_Int& a, const _Enum& b) noexcept;
template<typename _Int, typename _Enum> inline _Int& operator &=(_Int& a, const _Enum& b) noexcept;
template<typename _Int, typename _Enum> inline _Int& operator ^=(_Int& a, const _Enum& b) noexcept;

template<typename _T> inline bool  hasFlag(      _T  in, _T flag) noexcept;
template<typename _T> inline _T    setFlag(const _T& in, _T flag) noexcept;
template<typename _T> inline _T&   setFlag(      _T& in, _T flag) noexcept;
template<typename _T> inline _T  unsetFlag(const _T& in, _T flag) noexcept;
template<typename _T> inline _T& unsetFlag(      _T& in, _T flag) noexcept;

template<typename _Int, typename _Enum> inline bool    hasFlag(      _Int  in, _Enum flag) noexcept;
template<typename _Int, typename _Enum> inline _Int    setFlag(const _Int& in, _Enum flag) noexcept;
template<typename _Int, typename _Enum> inline _Int&   setFlag(      _Int& in, _Enum flag) noexcept;
template<typename _Int, typename _Enum> inline _Int  unsetFlag(const _Int& in, _Enum flag) noexcept;
template<typename _Int, typename _Enum> inline _Int& unsetFlag(      _Int& in, _Enum flag) noexcept;

template<typename _T> inline _T setFlag(const _T& in, _T flag, bool enable) noexcept;
template<typename _T> inline _T setFlag(      _T& in, _T flag, bool enable) noexcept;

template<typename _Int, typename _Enum> inline _Int setFlag(const _Int& in, _Enum flag, bool enable) noexcept;
template<typename _Int, typename _Enum> inline _Int setFlag(      _Int& in, _Enum flag, bool enable) noexcept;

#include "EnumBitFields.inl"
