#pragma once

#ifdef TAU_PRODUCTION
  #define RELEASE_DX(_VAR) do \
    if(_VAR) { \
        (_VAR)->Release(); \
    } \
    while(false) 
#else
  #define RELEASE_DX(_VAR) do \
    if(_VAR) { \
        (_VAR)->Release(); \
        (_VAR) = nullptr; \
    } \
    while(false) 
#endif
