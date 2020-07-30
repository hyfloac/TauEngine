#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

#include "astdio.h"

int avsprintf(char** const store, const char* const fmt, va_list args)
{
    if(!store) 
    { return ASPF_ERR_NULL_STR; }

    va_list argsCopy;
    va_copy(argsCopy, args);
    const int size = vsnprintf(NULL, 0, fmt, argsCopy) + 1;
    va_end(argsCopy);

    if(size <= 0)
    { return ASPF_ERR_SIZE_INV; }

    char* buffer = (char*) malloc(sizeof(char) * size);

    if(!buffer)
    { return ASPF_ERR_NULL_BUF; }

    buffer[size - 1] = '\0';

    *store = buffer;

    return vsnprintf(buffer, (size_t) size, fmt, args);
}

int asprintf(char** store, const char* const fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    const int error = avsprintf(store, fmt, args);
    va_end(args);

    return error;
}

void asfreepf(char* str)
{
    free((void*) str);
}

#ifdef __cplusplus
}
#endif