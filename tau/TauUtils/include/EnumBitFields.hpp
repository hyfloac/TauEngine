#pragma once

#include <NumTypes.hpp>

/*
 *   A series of templates for producing binary arithmetic
 * on enum bit fields.
 */

template<typename T> inline T  operator ~ (T  a)      noexcept { return static_cast<T>      (~static_cast<u64>(a)); }
template<typename T> inline T  operator | (T  a, T b) noexcept { return static_cast<T>      (static_cast<u64>      (a) |  static_cast<u64>(b)); }
template<typename T> inline T  operator & (T  a, T b) noexcept { return static_cast<T>      (static_cast<u64>      (a) &  static_cast<u64>(b)); }
template<typename T> inline T  operator ^ (T  a, T b) noexcept { return static_cast<T>      (static_cast<u64>      (a) ^  static_cast<u64>(b)); }
template<typename T> inline T& operator |=(T& a, T b) noexcept { return reinterpret_cast<T&>(reinterpret_cast<u64&>(a) |= static_cast<u64>(b)); }
template<typename T> inline T& operator &=(T& a, T b) noexcept { return reinterpret_cast<T&>(reinterpret_cast<u64&>(a) &= static_cast<u64>(b)); }
template<typename T> inline T& operator ^=(T& a, T b) noexcept { return reinterpret_cast<T&>(reinterpret_cast<u64&>(a) ^= static_cast<u64>(b)); }

template<typename T> inline bool hasFlag(T in, T flag)  noexcept { return (static_cast<u64>(in) & static_cast<u64>(flag)) != 0; }
template<typename T> inline T setFlag(T in, T flag)     noexcept { return in |   flag; }
template<typename T> inline T& setFlag(T& in, T flag)   noexcept { return in |=  flag; }
template<typename T> inline T unsetFlag(T in, T flag)   noexcept { return in &  ~flag; }
template<typename T> inline T& unsetFlag(T& in, T flag) noexcept { return in &= ~flag; }
