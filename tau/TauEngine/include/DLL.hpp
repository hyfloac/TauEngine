#pragma once

#include "Utils.hpp"

#ifdef TAU_DLL_BUILD
  #define TAU_DLL DYNAMIC_EXPORT
#elif defined(TAU_DLL_LINK)
  #define TAU_DLL DYNAMIC_IMPORT
#else
  #define TAU_DLL
#endif
