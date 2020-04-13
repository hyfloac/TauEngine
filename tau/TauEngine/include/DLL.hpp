#pragma once

#include "Utils.hpp"

#ifdef TAU_DLL_BUILD
  #define TAU_DLL DYNAMIC_EXPORT
#elif defined(TAU_DLL_LINK)
  #define TAU_DLL DYNAMIC_IMPORT
#else
  #define TAU_DLL
#endif

/*
 * Don't use NOVTABLE in debug modes as it breaks NatVis.
 */
#if defined(TAU_PRODUCTION)
  #define TAU_NOVTABLE NOVTABLE
#else
  #define TAU_NOVTABLE
#endif
