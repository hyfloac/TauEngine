#pragma once

#ifdef _MSVC_LANG
  #include <malloc.h>
  #define DSA(_TYPE, _NAME, _SIZE) _TYPE* _NAME = static_cast<_TYPE*>(alloca(_SIZE))
#else
  #define DSA(_TYPE, _NAME, _SIZE) _TYPE _NAME[_SIZE]
#endif
