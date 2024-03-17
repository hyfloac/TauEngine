#pragma once

#if !defined(_WIN32) && (defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__))
  #define _WIN32
#endif

#ifdef __cplusplus
  #ifdef _MSVC_LANG
    #define __CPP_REAL _MSVC_LANG
  #else 
    #define __CPP_REAL __cplusplus
  #endif
#elif defined(_MSVC_LANG)
  #define __CPP_REAL _MSVC_LANG
#else
  #define __CPP_REAL 1
  #warning("__cplusplus is not defined.")
#endif

#if __CPP_REAL > 201704L
  #define __CPP_20
  #define __CPP_17
  #define __CPP_14
  #define __CPP_11
  #define __CPP_98
  #define __CPP_1
  
  #define __CPP 20
#elif __CPP_REAL >= 201703L || (defined(_MSC_VER) && _MSC_VER > 1900)
  #define __CPP_17
  #define __CPP_14
  #define __CPP_11
  #define __CPP_98
  #define __CPP_1
  
  #define __CPP 17
#elif __CPP_REAL >= 201402L
  #define __CPP_14
  #define __CPP_11
  #define __CPP_98
  #define __CPP_1
  
  #define __CPP 14
#elif __CPP_REAL >= 201103L
  #define __CPP_11
  #define __CPP_98
  #define __CPP_1
  
  #define __CPP 11
#elif __CPP_REAL >= 199711L
 #define __CPP_98
 #define __CPP_1
 
 #define __CPP -98
#elif __CPP_REAL == 1
 #define __CPP_1
 
 #define __CPP 1
#endif
