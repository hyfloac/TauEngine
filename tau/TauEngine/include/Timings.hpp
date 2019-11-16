/**
 * @file
 * 
 * Some functions for computing timings.
 */
#pragma once

#include <NumTypes.hpp>
#include <Safeties.hpp>
#include <Objects.hpp>
#include <IFile.hpp>
#include "DLL.hpp"

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

TAU_DLL u64 microTime() noexcept;

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

class TAU_DLL TimingsWriter final
{
    DELETE_CONSTRUCT(TimingsWriter);
    DELETE_DESTRUCT(TimingsWriter);
    DELETE_COPY(TimingsWriter);
public:
    struct ProfileResult final
    {
        DEFAULT_DESTRUCT(ProfileResult);
        DEFAULT_COPY(ProfileResult);
    public:
        const char* name;
        u32 threadID;
        u64 start;
        u64 end;
    public:
        ProfileResult(const char* const name, const u32 threadId, const u64 start, const u64 end) noexcept
            : name(name), threadID(threadId), start(start), end(end)
        { }
    };
public:
    static void begin(const char* name, const char* fileName = "results.json") noexcept;

    static void end() noexcept;

    static void write(const ProfileResult& pr) noexcept;
private:
    static void writeHeader(const char* name) noexcept;
    static void writeFooter() noexcept;
};

class TAU_DLL PerfTimer final
{
    DEFAULT_COPY(PerfTimer);
private:
    const char* _name;
    u64 _start;
    bool _stopped;
public:
    PerfTimer(const char* const name) noexcept
        : _name(name), _start(microTime()), _stopped(false)
    { }

    ~PerfTimer() noexcept
    {
        if(!_stopped)
        { stop(); }
    }

    void stop() noexcept;
};

#ifndef TAU_PERF_MONITOR
  #define TAU_PERF_MONITOR 0
#endif

#if TAU_PERF_MONITOR
  #define PERF_NAMED(_NAME) PerfTimer _x_timer##__LINE__(_NAME)
  #define PERF() PERF_NAMED(__FUNCSIG__)
#else
  #define PERF_NAMED(_NAME)
  #define PERF()
#endif
