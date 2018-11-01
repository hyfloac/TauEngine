#pragma once
#include <NumTypes.hpp>
#include <DLL.hpp>

TAU_DLL u64 rdtsc() noexcept;

TAU_DLL u64 currentTimeMillis() noexcept;

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
 * period of time. The time frame overwhich this calculation 
 * is taken is passed by the parameter `timeoutMS`. If this is
 * taking too long to compute lower the value. If lowering the 
 * value causes it to return odd results, increase the value.
 */
TAU_DLL void computeClockCyclesPerTime(const u64 timeoutMS = 50) noexcept;

TAU_DLL const ClockCyclesTimeFrame* getClockCyclesPerTimeFrame() noexcept;
