#pragma once

#ifdef _MSC_VER
  #define ALIGN_MSVC(_ALIGNMENT) __declspec(align(_ALIGNMENT))
  #define ALIGN_GCC(_ALIGNMENT)
#else
  #define ALIGN_MSVC(_ALIGNMENT) 
  #define ALIGN_GCC(_ALIGNMENT) __attribute__((aligned(_ALIGNMENT)))
#endif
