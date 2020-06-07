#pragma once

#if defined(_WIN32)
  #define DYNAMIC_EXPORT __declspec(dllexport)
  #define DYNAMIC_IMPORT __declspec(dllimport)
#elif defined(__GNUC__) || defined(__CLANG__)
  #define DYNAMIC_EXPORT __attribute__((visibility("default")))
  #define DYNAMIC_IMPORT 
#else
  #define DYNAMIC_EXPORT
  #define DYNAMIC_IMPORT
#endif

#ifdef TAU_DLL_BUILD
  #define TAU_DLL DYNAMIC_EXPORT
#elif defined(TAU_DLL_LINK)
  #define TAU_DLL DYNAMIC_IMPORT
#else
  #define TAU_DLL
#endif

#ifdef _MSC_VER
  #define ALIGN_MSVC(_ALIGNMENT) __declspec(align(_ALIGNMENT))
  #define ALIGN_GCC(_ALIGNMENT)
#else
  #define ALIGN_MSVC(_ALIGNMENT) 
  #define ALIGN_GCC(_ALIGNMENT) __attribute__((aligned(_ALIGNMENT)))
#endif

#if __cplusplus
  #define NOEXCEPT noexcept
  #define EXTERN extern "C"
#else
  #define NOEXCEPT
  #define EXTERN extern
#endif
