#pragma once

#ifdef _MSVC_LANG
#pragma warning(push, 0)
  #include <malloc.h>
#pragma warning(pop)
  #define DSA(_TYPE, _NAME, _SIZE) _TYPE* _NAME = static_cast<_TYPE*>(alloca(_SIZE))
#else
  #define DSA(_TYPE, _NAME, _SIZE) _TYPE _NAME[_SIZE]
#endif
