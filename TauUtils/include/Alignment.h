#pragma once

#ifdef _MSC_VER
  #define ALIGN(_ALIGNMENT) __declspec(align(_ALIGNMENT))
#else
  #define ALIGN(_ALIGNMENT) __attribute__((aligned(_ALIGNMENT)))
#endif
