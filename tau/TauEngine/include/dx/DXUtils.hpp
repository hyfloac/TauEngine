#pragma once

#include <NumTypes.hpp>

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

class IDXGIAdapter;

namespace DX10Utils {
u64 getRowPitchAlignment(IDXGIAdapter* adapter) noexcept;

u64 getDepthPitchAlignment(IDXGIAdapter* adapter) noexcept;
}
