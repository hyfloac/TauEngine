#pragma once

#include <CVersion.hpp>

#ifdef __CPP_14
  #define DEPRECATED [[deprecated]]
#elif defined(__GNUC__)
  #ifdef UTILS_DISABLE_PRAGMA
    #define DEPRECATED __attribute__((deprecated))
  #else
    #define DEPRECATED INLINE_PRAGMA(deprecated)
  #endif
#elif defined(_MSC_VER)
  #ifdef UTILS_DISABLE_PRAGMA
    #define DEPRECATED __declspec(deprecated)
  #else
    #define DEPRECATED INLINE_PRAGMA(deprecated)
  #endif
#else
  #warning "Your compiler needs to have to ability to use DEPRECATED"
  #define DEPRECATED
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
  #define DEBUG_BREAK asm volatile ("int 3;")
#endif
