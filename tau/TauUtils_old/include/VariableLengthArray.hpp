#pragma once

#ifdef _MSVC_LANG
#pragma warning(push, 0)
  #include <malloc.h>
#pragma warning(pop)
  #define VLA(_TYPE, _NAME, _SIZE) _TYPE* _NAME = static_cast<_TYPE*>(_alloca(_SIZE * sizeof(_TYPE)))
#else
  #define VLA(_TYPE, _NAME, _SIZE) _TYPE _NAME[_SIZE]
#endif

#define VLA_MAX_LEN (512)
