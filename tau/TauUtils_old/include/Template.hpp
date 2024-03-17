#pragma once

#pragma warning(push, 0)
#include <type_traits>
#include <typeinfo>
#pragma warning(push, 0)

template<typename _T>
struct TypeName final
{
    constexpr static const char* const Name = typeid(_T).name();
};

#define TYPE_NAME(__TYPE, __NAME) \
    template<> \
    struct TypeName<__TYPE> final { \
        constexpr static const char* const Name = __NAME; \
    };
#define TYPE_NAME_T(__TYPE) TYPE_NAME(__TYPE, #__TYPE)

#ifdef CHAR_IS_BYTE
  TYPE_NAME(char, "byte");
#else
  TYPE_NAME_T(char);
#endif

TYPE_NAME_T(short);
TYPE_NAME_T(int);

#ifdef LONG_IS_INT
  TYPE_NAME(long, "int");
  TYPE_NAME(long long, "long");
#else
  TYPE_NAME_T(long);
  TYPE_NAME_T(long long);
#endif

#ifdef FLOAT_IS_SINGLE
  TYPE_NAME(float, "single");
#else
  TYPE_NAME_T(float);
#endif

TYPE_NAME_T(double);

#ifdef BOOL_IS_BOOLEAN
  TYPE_NAME(bool, "boolean");
#else
  TYPE_NAME_T(bool);
#endif

TYPE_NAME_T(void);

#ifdef PTR_IS_POINTER
  #ifdef CHAR_PTR_IS_STRING
    TYPE_NAME(char*, "string");
    TYPE_NAME(const char*, "string");
  #else
    TYPE_NAME_T(char*, "char pointer");
    TYPE_NAME_T(const char*, "const char pointer");
  #endif

  TYPE_NAME(short*, "short pointer");
  TYPE_NAME(int*, "int pointer");

  TYPE_NAME(const short*, "const short pointer");
  TYPE_NAME(const int*, "const int pointer");

  #ifdef LONG_IS_INT
    TYPE_NAME(long*, "int pointer");
    TYPE_NAME(long long*, "long pointer");
    TYPE_NAME(const long*, "const int pointer");
    TYPE_NAME(const long long*, "const long pointer");
  #else
    TYPE_NAME(long*, "long pointer");
    TYPE_NAME(long long*, "long long pointer");
    TYPE_NAME(const long*, "const long pointer");
    TYPE_NAME(const long long*, "const long long pointer");
  #endif

  #ifdef FLOAT_IS_SINGLE
      TYPE_NAME(float*, "single pointer");
      TYPE_NAME(const float*, "const single pointer");
  #else
      TYPE_NAME(float*, "float pointer");
      TYPE_NAME(const float*, "const float pointer");
  #endif

  TYPE_NAME(double*, "double pointer");
  TYPE_NAME(const double*, "const double pointer");

  #ifdef BOOL_IS_BOOLEAN
    TYPE_NAME(bool*, "boolean pointer");
    TYPE_NAME(const bool*, "const boolean pointer");
  #else
    TYPE_NAME(bool*, "bool pointer");
    TYPE_NAME(const bool*,"const bool pointer");
  #endif

  TYPE_NAME(void*, "void pointer");
  TYPE_NAME(const void*, "const void pointer");
#else
  #ifdef CHAR_PTR_IS_STRING
    TYPE_NAME(char*, "string");
    TYPE_NAME(const char*, "string");
  #else
    TYPE_NAME_T(char*);
    TYPE_NAME_T(const char*);
  #endif

  TYPE_NAME_T(short*);
  TYPE_NAME_T(int*);
  TYPE_NAME_T(const short*);
  TYPE_NAME_T(const int*);

  #ifdef LONG_IS_INT
    TYPE_NAME(long*, "int*");
    TYPE_NAME(long long*, "long*");
    TYPE_NAME(const long*, "const int*");
    TYPE_NAME(const long long*, "const long*");
  #else
    TYPE_NAME_T(long*);
    TYPE_NAME_T(long long*);
    TYPE_NAME_T(const long*);
    TYPE_NAME_T(const long long*);
  #endif

  #ifdef FLOAT_IS_SINGLE
    TYPE_NAME(float*, "single*");
    TYPE_NAME(const float*, "const single*");
  #else
    TYPE_NAME_T(float*);
    TYPE_NAME_T(const float*);
  #endif
  
    TYPE_NAME_T(double*);
    TYPE_NAME_T(const double*);
  
  #ifdef BOOL_IS_BOOLEAN
    TYPE_NAME(bool*, "boolean*");
    TYPE_NAME(const bool*, "const boolean*");
  #else
    TYPE_NAME_T(bool*);
    TYPE_NAME_T(const bool*);
  #endif

  TYPE_NAME_T(void*);
  TYPE_NAME_T(const void*);
#endif
