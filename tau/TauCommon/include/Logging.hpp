/**
 * @file
 */
#pragma once

#include <ConPrinter.hpp>

#ifndef TAU_ENABLE_DEBUG_LOGGING
    #if defined(TAU_PRODUCTION)
        #define TAU_ENABLE_DEBUG_LOGGING 0
    #else
        #define TAU_ENABLE_DEBUG_LOGGING 1
    #endif
#endif

#ifndef TAU_ENABLE_ENTRYPOINT_TRACING
    #if defined(TAU_PRODUCTION)
        #define TAU_ENABLE_ENTRYPOINT_TRACING 0
    #else
        #define TAU_ENABLE_ENTRYPOINT_TRACING 1
    #endif
#endif

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4505)
static void __declspec(noinline) TauGetCallAddress(void** address)
{
    *address = _ReturnAddress();
}
#pragma warning(pop)

#else
__attribute__((noinline)) static void TauGetCallAddress(void** address)
{
    *address = __builtin_extract_return_addr(__builtin_return_address(0));
}
#endif

#if TAU_ENABLE_DEBUG_LOGGING
#include <ConPrinter.hpp>
#endif

#if TAU_ENABLE_DEBUG_LOGGING
template<typename... Args>
static void TauLog(
    const c8* const level,
    const void* const address,
    const char* const functionName,
    const size_t line,
    const Args&... args
) noexcept
{
    ConPrinter::Print(u8"[{}](0x{XP0}) {}:{}: ", level, address, functionName, line);
    ConPrinter::PrintLn(args...);
}

#define DECL_LOG(NAME) \
    template<typename... Args> \
    inline void TauLog##NAME(const void* address, const char* const functionName, const size_t line, const Args&... args) \
    {                                                         \
        TauLog(u8###NAME, address, functionName, line, args...); \
    }

DECL_LOG(Debug);
DECL_LOG(Info);
DECL_LOG(Warn);
DECL_LOG(Error);

#define internal_LOG(LEVEL, FUNCTION, FILE, LINE, ...) \
    do {                                                    \
        void* logFuncAddress;                               \
        TauGetCallAddress(&logFuncAddress);                  \
        TauLog##LEVEL(logFuncAddress, FUNCTION, LINE , ## __VA_ARGS__); \
    } while(false)
#else
#define internal_LOG(LEVEL, FUNCTION, FILE, LINE, ...)
#endif

#if TAU_ENABLE_ENTRYPOINT_TRACING
  #define TRACE_ENTRYPOINT() internal_LOG(Debug, __FUNCTION__, __FILE__, __LINE__, "")
  #define TRACE_ENTRYPOINT_ARG(...) internal_LOG(Debug, __FUNCTION__, __FILE__, __LINE__ , ## __VA_ARGS__)
#else
  #define TRACE_ENTRYPOINT() do { } while(false)
  #define TRACE_ENTRYPOINT_ARG(...) do { } while(false)
#endif


#if GS_ENABLE_DEBUG_LOGGING
  #define LOG_DEBUG(...) internal_LOG(Debug, __FUNCTION__, __FILE__, __LINE__ , ## __VA_ARGS__)
#else
  #define LOG_DEBUG(...) do { } while(false)
#endif

#define LOG_INFO(...) internal_LOG(Info, __FUNCTION__, __FILE__, __LINE__ , ## __VA_ARGS__)
#define LOG_WARN(...) internal_LOG(Warn, __FUNCTION__, __FILE__, __LINE__ , ## __VA_ARGS__)
#define LOG_ERROR(...) internal_LOG(Error, __FUNCTION__, __FILE__, __LINE__ , ## __VA_ARGS__)
