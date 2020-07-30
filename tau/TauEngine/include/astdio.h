#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

#define ASPF_ERR_NULL_STR ((int) (0x88A59A78))
#define ASPF_ERR_SIZE_INV ((int) (0x88A59A79))
#define ASPF_ERR_NULL_BUF ((int) (0x88A59A7A))

/**
 *    An implementation of `vsprintf` that dynamically
 *  allocates a string to store the result in.
 *
 *    Memory is allocated with `malloc`. For safety a
 *  `asfreepf` function is provided.
 *
 *  @return   An error code identical to the one produced
 *          by `vsnprintf`.
 */
int avsprintf(char** store, const char* fmt, va_list args);

/**
 *    An implementation of `sprintf` that dynamically
 *  allocates a string to store the result in.
 *
 *    Memory is allocated with `malloc`. For safety a
 *  `asfreepf` function is provided.
 *
 *  @return   An error code identical to the one produced
 *          by `vsnprintf`.
 */
int asprintf(char** store, const char* fmt, ...);

/**
 *    A `free` function designed specifically for
 *  `avsprintf` and `asprintf`.
 *
 *    This was created due to the issues with cross DLL
 *  boundaries. It is *highly* advised to use this function
 *  over the conventional `free` function declared in
 *  the C standard file `stdlib.h`.
 */
void asfreepf(char* str);

#ifdef __cplusplus
}
#endif
