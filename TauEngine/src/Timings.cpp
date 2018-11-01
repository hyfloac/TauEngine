#include <Timings.hpp>

#ifdef _WIN32
  #include <intrin.h>
  #include <Windows.h>
#else
  #include <x86intrin.h>
  #include <sys/time.h>
  #include <ctime>
#endif

#include <chrono>
#include <thread>

u64 rdtsc() noexcept
{
    return __rdtsc();
}

u64 currentTimeMillis() noexcept
{
#ifdef _WIN32
    /* Windows */
    FILETIME fileTime;
    LARGE_INTEGER fileTimeInt;

    /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
     * to a LARGE_INTEGER structure. */
    GetSystemTimeAsFileTime(&fileTime);
    fileTimeInt.LowPart = fileTime.dwLowDateTime;
    fileTimeInt.HighPart = (LONG) fileTime.dwHighDateTime;

    u64 ret = (u64) fileTimeInt.QuadPart;
    ret -= 116444736000000000uLL; /* Convert from file time to UNIX epoch time. */
    ret /= 10000; /* From 100 nano seconds (10^-7) to 1 millisecond (10^-3) intervals */

    return ret;
#else
    /* Linux */
    struct timeval timeVal;

    gettimeofday(&timeVal, null);

    u64 ret = timeVal.tv_usec;
    /* Convert from micro seconds (10*-6) to milliseconds (10**-3) */
    ret /= 1000;

    /* Adds the seconds (10**0) after converting them to milliseconds (10**-3) */
    ret += (timeVal.tv_sec * 1000);

    return ret;
#endif
}

static ClockCyclesTimeFrame clockCycles;

void computeClockCyclesPerTime(const u64 timeoutMS) noexcept
{
    const u64 timeBegin = currentTimeMillis();
    const u64 clockBegin = rdtsc();
    std::this_thread::sleep_for(std::chrono::milliseconds(timeoutMS));
    const u64 timeEnd = currentTimeMillis();
    const u64 clockEnd = rdtsc();

    const u64 time = timeEnd - timeBegin;
    const u64 clock = clockEnd - clockBegin;

    clockCycles = {};

    clockCycles.clockCyclesPerMillisecondF = (f64) clock / (f64) time;
    clockCycles.clockCyclesPerMillisecond = (u64) clockCycles.clockCyclesPerMillisecondF;

    clockCycles.clockCyclesPerSecondF = clockCycles.clockCyclesPerMillisecondF / 1000.0;
    clockCycles.clockCyclesPerSecond = (u64) clockCycles.clockCyclesPerSecondF;

    clockCycles.clockCyclesPerMicrosecondF = (clock * 1000.0) / (f64) time;
    clockCycles.clockCyclesPerMicrosecond = (u64) clockCycles.clockCyclesPerMicrosecondF;

    clockCycles.clockCyclesPerNanosecondF = (clock * 1000000.0) / (f64) time;
    clockCycles.clockCyclesPerNanosecond = (u64) clockCycles.clockCyclesPerNanosecondF;
}

const ClockCyclesTimeFrame* getClockCyclesPerTimeFrame() noexcept
{
    return &clockCycles;
}
