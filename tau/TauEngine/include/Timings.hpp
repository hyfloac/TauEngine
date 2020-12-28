/**
 * @file
 * 
 * Some functions for computing timings.
 */
#pragma once

#include <NumTypes.hpp>
#include <Safeties.hpp>
#include <Objects.hpp>
#include <String.hpp>
#include "DLL.hpp"
#include "system/Mutex.hpp"

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
struct ClockCyclesTimeFrame final
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
 * {@link computeClockCyclesPerTime(const u64) @endlink} which
 * computes a current clock speed.
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

class IFile;

class TAU_DLL TimingsWriter final
{
    DELETE_CONSTRUCT(TimingsWriter);
    DELETE_DESTRUCT(TimingsWriter);
    DELETE_CM(TimingsWriter);
public:
    struct ProfileResult final
    {
        DEFAULT_DESTRUCT(ProfileResult);
        DEFAULT_CM_PU(ProfileResult);
    public:
        const char* name;
        u32 threadID;
        u64 start;
        u64 end;
    public:
        ProfileResult(const char* const _name, const u32 _threadId, const u64 _start, const u64 _end) noexcept
            : name(_name)
            , threadID(_threadId)
            , start(_start)
            , end(_end)
        { }
    };
private:
    static CPPRef<IFile> _profileFile;
    static u32 _profileCount;
    static SRWMutex _mutex;
public:
    static void begin(const char* name, const WDynString& fileName = L"results.json") noexcept;

    static void end() noexcept;

    static void write(const ProfileResult& pr) noexcept;
private:
    static void writeHeader(const char* name) noexcept;
    static void writeFooter() noexcept;
};

class TAU_DLL PerfTimer final
{
    DEFAULT_CM_PU(PerfTimer);
private:
    const char* _name;
    u64 _start;
    bool _stopped;
public:
    PerfTimer(const char* const name) noexcept
        : _name(name)
        , _start(microTime())
        , _stopped(false)
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

class TAU_DLL DeltaTime final
{
    DEFAULT_DESTRUCT(DeltaTime);
    DEFAULT_CM_PU(DeltaTime);
private:
    float _us;
    float _ms;
    float _s;
    float _usSinceLastUpdate;
    float _msSinceLastUpdate;
    float _sSinceLastUpdate;
    float _usSinceLaunch;
    float _msSinceLaunch;
    float _sSinceLaunch;
public:
    DeltaTime() noexcept
        : _us(0.0f)
        , _ms(0.0f)
        , _s(0.0f)
        , _usSinceLastUpdate(0.0f)
        , _msSinceLastUpdate(0.0f)
        , _sSinceLastUpdate(0.0f)
        , _usSinceLaunch(0.0f)
        , _msSinceLaunch(0.0f)
        , _sSinceLaunch(0.0f)
    { }

    [[nodiscard]] float us() const noexcept { return _us; }
    [[nodiscard]] float ms() const noexcept { return _ms; }
    [[nodiscard]] float s() const noexcept { return _s; }
    [[nodiscard]] float uSeconds() const noexcept { return _us; }
    [[nodiscard]] float mSeconds() const noexcept { return _ms; }
    [[nodiscard]] float seconds() const noexcept { return _s; }
    [[nodiscard]] float microseconds() const noexcept { return _us; }
    [[nodiscard]] float milliseconds() const noexcept { return _ms; }

    [[nodiscard]] float usSinceLastUpdate() const noexcept { return _usSinceLastUpdate; }
    [[nodiscard]] float msSinceLastUpdate() const noexcept { return _msSinceLastUpdate; }
    [[nodiscard]] float sSinceLastUpdate() const noexcept { return _sSinceLastUpdate; }
    [[nodiscard]] float uSecondsSinceLastUpdate() const noexcept { return _usSinceLastUpdate; }
    [[nodiscard]] float mSecondsSinceLastUpdate() const noexcept { return _msSinceLastUpdate; }
    [[nodiscard]] float secondsSinceLastUpdate() const noexcept { return _sSinceLastUpdate; }
    [[nodiscard]] float microsecondsSinceLastUpdate() const noexcept { return _usSinceLastUpdate; }
    [[nodiscard]] float millisecondsSinceLastUpdate() const noexcept { return _msSinceLastUpdate; }

    [[nodiscard]] float usSinceLaunch() const noexcept { return _usSinceLaunch; }
    [[nodiscard]] float msSinceLaunch() const noexcept { return _msSinceLaunch; }
    [[nodiscard]] float sSinceLaunch() const noexcept { return _sSinceLaunch; }
    [[nodiscard]] float uSecondsSinceLaunch() const noexcept { return _usSinceLaunch; }
    [[nodiscard]] float mSecondsSinceLaunch() const noexcept { return _msSinceLaunch; }
    [[nodiscard]] float secondsSinceLaunch() const noexcept { return _sSinceLaunch; }
    [[nodiscard]] float microsecondsSinceLaunch() const noexcept { return _usSinceLaunch; }
    [[nodiscard]] float millisecondsSinceLaunch() const noexcept { return _msSinceLaunch; }

    void setDeltaMicro(float us) noexcept;

    void onUpdate() noexcept;
};
