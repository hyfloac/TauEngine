#pragma once

#include <CVersion.hpp>

#ifdef _MSC_VER
  #define INLINE_PRAGMA(__X) __pragma(__X)
#elif defined(__GNUC__)
  #define INLINE_PRAGMA(__X) _Pragma(__X)
#else
  #define INLINE_PRAGMA(__X)
#endif

#ifdef __CPP_14
  #define DEPRECATED [[deprecated]]
  #define DEPRECATED_R(__REASON) [[deprecated(__REASON)]]
#elif defined(__GNUC__)
  #ifdef UTILS_DISABLE_PRAGMA
    #define DEPRECATED __attribute__((deprecated))
    #define DEPRECATED_R(__REASON) __attribute__((deprecated(__REASON))))
  #else
    #define DEPRECATED INLINE_PRAGMA(deprecated)
    #define DEPRECATED_R(__REASON) INLINE_PRAGMA(deprecated)
  #endif
#elif defined(_MSC_VER)
  #ifdef UTILS_DISABLE_PRAGMA
    #define DEPRECATED __declspec(deprecated)
    #define DEPRECATED_R(__REASON) __declspec(deprecated(__REASON)))
  #else
    #define DEPRECATED INLINE_PRAGMA(deprecated)
    #define DEPRECATED_R(__REASON) INLINE_PRAGMA(deprecated)
  #endif
#else
  #warning "Your compiler needs to have to ability to use DEPRECATED"
  #define DEPRECATED
  #define DEPRECATED_R(__REASON)
#endif

#ifdef __CPP_11
  #define null nullptr
#else
  #ifdef USE_C_NULL
    #define null ((void*) 0)
  #else
    #define null NULL
  #endif
#endif

#ifdef _WIN32
  #define DEBUG_BREAK __debugbreak()
#else
  #define DEBUG_BREAK asm volatile ("int $3;")
#endif

#if defined(__GNUC__) || defined(__CLANG__) || defined(_MSC_VER)
  #define RESTRICT __restrict
#else
  #define RESTRICT
#endif
