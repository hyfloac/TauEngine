#pragma once

#include <type_traits>
#include <typeinfo>

template<typename _T>
struct TypeName
{
    static const char* Name() { return typeid(_T).name(); }
};

#define TYPE_NAME(__TYPE, __NAME) template<> struct TypeName<__TYPE> { static const char* Name() { return __NAME; } };
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

TYPE_NAME_T(float);
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
  #else
    TYPE_NAME_T(char*, "char pointer");
  #endif

  TYPE_NAME(short*, "short pointer");
  TYPE_NAME(int*, "int pointer");

  #ifdef LONG_IS_INT
    TYPE_NAME(long*, "int pointer");
    TYPE_NAME(long long*, "long pointer");
  #else
    TYPE_NAME(long*, "long pointer");
    TYPE_NAME(long long*, "long long pointer");
  #endif

  TYPE_NAME(float*, "float pointer");
  TYPE_NAME(double*, "double pointer");
  TYPE_NAME(bool*, "boolean pointer");
  TYPE_NAME(void*, "void pointer");
#else
  #ifdef CHAR_PTR_IS_STRING
    TYPE_NAME(char*, "string");
  #else
    TYPE_NAME_T(char*);
  #endif

  TYPE_NAME_T(short*);
  TYPE_NAME_T(int*);

  #ifdef LONG_IS_INT
    TYPE_NAME(long*, "int*");
    TYPE_NAME(long long*, "long*");
  #else
    TYPE_NAME_T(long*);
    TYPE_NAME_T(long long*);
  #endif

  TYPE_NAME_T(float*);
  TYPE_NAME_T(double*);
  TYPE_NAME_T(bool*);
  TYPE_NAME_T(void*);
#endif
