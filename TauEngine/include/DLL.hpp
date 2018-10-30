#pragma once

#ifdef TAU_DLL_BUILD
  #define TAU_DLL __declspec(dllexport)
#elif defined(TAU_DLL_LINK)
  #define TAU_DLL __declspec(dllimport)
#else
  #define TAU_DLL
#endif
