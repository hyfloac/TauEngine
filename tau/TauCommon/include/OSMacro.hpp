/**
 * @file
 */
#pragma once

#if defined(__has_include) && __has_include(<dlfcn.h>)
    #define HAS_DLFCN
#endif

#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__) || defined(__MACH__)
    #define IS_UNIX
#endif

#if defined(__APPLE__) || defined(__MACH__)
    #define IS_APPLE
#endif

#if defined(__linux__) || defined(__linux) || defined(linux)
    #define IS_LINUX
#endif

#if defined(__FreeBSD__)
    #define IS_FREEBSD
#endif

#if defined(__ANDROID__)
    #define IS_ANDROID
#endif

#if defined(_POSIX_VERSION)
    #define IS_POSIX
#endif

#if defined(IS_UNIX) || defined(IS_APPLE) || defined(IS_LINUX) || defined(IS_FREEBSD) || defined(IS_ANDROID) || defined(IS_POSIX)
    #define IS_NIX
#endif
