/**
 * @file
 * 
 * Some functions for computing timings.
 */
#pragma once

#include <NumTypes.hpp>
#include <DLL.hpp>
#include <Safeties.hpp>

/**
 *   This retrieves the number of clock cycles that has passed
 * since the start of the system. It is primarily an alias to
 * the `rdtsc` assembly instruction.
 * 
 * @return
 *    Returns the number of clock cycles since launch.
 */
TAU_DLL u64 rdtsc() noexcept;

/**
 * This gets the current unix epoch in milliseconds.
 * 
 * @return
 *    The current unix epoch in milliseconds.
 */
TAU_DLL u64 currentTimeMillis() noexcept;

/**
 *   This structure stores the number of clock cycles that 
 * occur over a set period of time.
 */
struct ClockCyclesTimeFrame
{
    u64 clockCyclesPerSecond;
    f64 clockCyclesPerSecondF;
    u64 clockCyclesPerMillisecond;
    f64 clockCyclesPerMillisecondF;
    u64 clockCyclesPerMicrosecond;
    f64 clockCyclesPerMicrosecondF;
    u64 clockCyclesPerNanosecond;
    f64 clockCyclesPerNanosecondF;
};

/**
 *   Used to recompute the number of clock cycles over a given
 * period of time. The time frame over which this calculation 
 * is taken is passed by the parameter `timeoutMS`. If this is
 * taking too long to compute lower the value. If lowering the 
 * value causes it to return odd results, increase the value.
 * 
 * @param[in] timeoutMS
 *    The number of milliseconds to timeout for.
 */
TAU_DLL void computeClockCyclesPerTime(const u64 timeoutMS = 50) noexcept;

/**
 *   Used to compute the {@link ClockCyclesTimeFrame} from how
 * long the program has been running for. This function 
 * produces an average clock speed unlike 
 * {@link computeClockCyclesPerTime(const u64)} which computes
 * a current clock speed.
 */
TAU_DLL void computeClockCyclesFromRuntime() noexcept;

/**
 *   This function simply retrieves the current clock speed 
 * information.
 * 
 * @return
 *    A pointer to the current clock speed information.
 */
TAU_DLL NonNull const ClockCyclesTimeFrame* getClockCyclesPerTimeFrame() noexcept;
