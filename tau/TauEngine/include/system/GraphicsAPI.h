#pragma once

#include <TauMacros.hpp>

#ifdef TAU_ENGINE_BUILD
  #define TAU_GAPI_DLL DYNAMIC_IMPORT
#else
  #define TAU_GAPI_DLL DYNAMIC_EXPORT
#endif

#if __cplusplus
extern "C" {
#endif

typedef const char*(* tauGetGraphicsAPIName_f)();
typedef bool(* tauLoadGraphicsAPI_f)();

static constexpr const char* tauGetGraphicsAPIName_name = "tauGetGraphicsAPIName";
static constexpr const char* tauLoadGraphicsAPI_name = "tauLoadGraphicsAPI";

TAU_GAPI_DLL const char* tauGetGraphicsAPIName();

TAU_GAPI_DLL bool tauLoadGraphicsAPI();

#if __cplusplus
}
#endif
