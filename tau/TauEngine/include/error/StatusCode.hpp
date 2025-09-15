#pragma once

#include <NumTypes.hpp>

namespace tau {

using StatusCode = i32;

inline constexpr bool IsSuccess(const StatusCode statusCode) noexcept
{
    return statusCode >= 0;
}

inline constexpr bool IsError(const StatusCode statusCode) noexcept
{
    return !IsSuccess(statusCode);
}

inline constexpr StatusCode TauSOk = 0;
inline constexpr StatusCode TauSPartial = 0;

inline constexpr StatusCode TauEFail = -1;
inline constexpr StatusCode TauEInternalError = -2;
inline constexpr StatusCode TauENotImplemented = -3;
inline constexpr StatusCode TauEOutOfSystemMemory = -4;
inline constexpr StatusCode TauEOutOfDeviceMemory = -5;
inline constexpr StatusCode TauEInvalidArg = -8;
inline constexpr StatusCode TauEInvalidArg0 = -800000;
inline constexpr StatusCode TauEInvalidArg1 = -800001;
inline constexpr StatusCode TauEInvalidArg2 = -800002;
inline constexpr StatusCode TauEInvalidArg3 = -800003;
inline constexpr StatusCode TauEInvalidArg4 = -800004;
inline constexpr StatusCode TauEInvalidArg5 = -800005;
inline constexpr StatusCode TauEInvalidArg7 = -800007;
inline constexpr StatusCode TauEInvalidArg8 = -800008;
inline constexpr StatusCode TauEInvalidArg9 = -800009;
inline constexpr StatusCode TauENullPointer = -9;
inline constexpr StatusCode TauEOutOfBounds = -10;
inline constexpr StatusCode TauEBufferTooSmall = -11;


}
