#pragma once

#include "Utils.hpp"

#ifdef TAU_ENGINE_BUILD
  #define TAU_GAPI_DLL DYNAMIC_IMPORT
#else
  #define TAU_GAPI_DLL DYNAMIC_EXPORT
#endif

#if __cplusplus
extern "C" {
#endif

typedef const char*(* __cdecl tauGetGraphicsAPIName_f)();
typedef bool(* __cdecl tauLoadGraphicsAPI_f)();

static constexpr const char* tauGetGraphicsAPIName_name = "tauGetGraphicsAPIName";
static constexpr const char* tauLoadGraphicsAPI_name = "tauLoadGraphicsAPI";

TAU_GAPI_DLL const char* __cdecl tauGetGraphicsAPIName();

TAU_GAPI_DLL bool __cdecl tauLoadGraphicsAPI();

#if __cplusplus
}
#endif
