#pragma once

#ifndef TAU_BUFFER_SAFETY
  #if defined(TAU_PRODUCTION)
    #define TAU_BUFFER_SAFETY 0
  #else
    #define TAU_BUFFER_SAFETY 1
  #endif
#endif

#ifndef TAU_RTTI_CHECK
  #if defined(TAU_PRODUCTION)
    #define TAU_RTTI_CHECK 0
  #else
    #define TAU_RTTI_CHECK 1
  #endif
#endif

#ifdef TAU_USER_CONFIG
  #include TAU_USER_CONFIG
#endif

