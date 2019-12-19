/** @file */
#include <Timings.hpp>
#include "system/Mutex.hpp"

#include <Win32File.hpp>
#include <VFS.hpp>

#pragma warning(push, 0)
#include <thread>
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
#include <string>
#pragma warning(pop)

u64 rdtsc() noexcept
{
    return __rdtsc();
}

u64 currentTimeMillis() noexcept
{
#ifdef _WIN32
    /* Windows */
    FILETIME fileTime;
    ULARGE_INTEGER fileTimeInt;

    /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
     * to a LARGE_INTEGER structure. */
    GetSystemTimeAsFileTime(&fileTime);
    fileTimeInt.LowPart = fileTime.dwLowDateTime;
    fileTimeInt.HighPart = fileTime.dwHighDateTime;

    u64 ret = static_cast<u64>(fileTimeInt.QuadPart);
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

    /* Subtracts the seconds (10**0) after converting them to milliseconds (10**-3) */
    ret += (timeVal.tv_sec * 1000);

    return ret;
#endif
}

u64 microTime() noexcept
{
#ifdef _WIN32
    /* Windows */
    FILETIME fileTime;
    ULARGE_INTEGER fileTimeInt;

    /* Get the amount of 100 nano seconds intervals elapsed since January 1, 1601 (UTC) and copy it
     * to a LARGE_INTEGER structure. */
    GetSystemTimeAsFileTime(&fileTime);
    fileTimeInt.LowPart = fileTime.dwLowDateTime;
    fileTimeInt.HighPart = fileTime.dwHighDateTime;

    u64 ret = static_cast<u64>(fileTimeInt.QuadPart);
    ret /= 10; /* From 100 nano seconds (10^-7) to microseconds (10^-6) intervals */

    return ret;
#else
    /* Linux */
    struct timeval timeVal;

    gettimeofday(&timeVal, null);

    return timeVal.tv_usec;
#endif
}

/**
 * The current clock speed information.
 */
static ClockCyclesTimeFrame clockCycles;

void computeClockCyclesPerTime(const u64 timeoutMS) noexcept
{
    Ensure(timeoutMS > 0);

    const u64 timeBegin = currentTimeMillis();
    const u64 clockBegin = rdtsc();
    std::this_thread::sleep_for(std::chrono::milliseconds(timeoutMS));
    const u64 timeEnd = currentTimeMillis();
    const u64 clockEnd = rdtsc();

    const u64 time = timeEnd - timeBegin;
    const u64 clock = clockEnd - clockBegin;

    clockCycles = {};

    clockCycles.clockCyclesPerMillisecondF = static_cast<f64>(clock) / static_cast<f64>(time);
    clockCycles.clockCyclesPerMillisecond = static_cast<u64>(clockCycles.clockCyclesPerMillisecondF);

    clockCycles.clockCyclesPerSecondF = clockCycles.clockCyclesPerMillisecondF / 1000.0;
    clockCycles.clockCyclesPerSecond = static_cast<u64>(clockCycles.clockCyclesPerSecondF);

    clockCycles.clockCyclesPerMicrosecondF = (clock * 1000.0) / static_cast<f64>(time);
    clockCycles.clockCyclesPerMicrosecond = static_cast<u64>(clockCycles.clockCyclesPerMicrosecondF);

    clockCycles.clockCyclesPerNanosecondF = (clock * 1000000.0) / static_cast<f64>(time);
    clockCycles.clockCyclesPerNanosecond = static_cast<u64>(clockCycles.clockCyclesPerNanosecondF);
}

/**
 *   The unix epoch for which the program started. This is 
 * initialized in {@link initProgramStartTimes(void)} which is 
 * called from {@link DLLMain(HMODULE,DWORD,LPVOID)}, so it
 * is really initialized at the time the DLL is first loaded.
 */
static u64 programStartTime = 0;

/**
 *   The number of clock cycles that have occured when the 
 * program started. This is initialized in 
 * {@link initProgramStartTimes(void)} which is called from 
 * {@link DLLMain(HMODULE,DWORD,LPVOID)}, so it is really 
 * initialized at the time the DLL is first loaded.
 */
static u64 programStartClockCycles = 0;

/**
 *   This is a mostly hidden function used to initialize the 
 * variables {@link programStartTime} and 
 * {@link programStartClockCycles}. It is called from 
 * {@link DLLMain(HMODULE,DWORD,LPVOID)}.
 */
void initProgramStartTimes() noexcept
{
    if(!programStartTime || !programStartClockCycles)
    {
        programStartTime = currentTimeMillis();
        programStartClockCycles = rdtsc();
    }
}

void computeClockCyclesFromRuntime() noexcept
{
    const u64 time = currentTimeMillis() - programStartTime;
    const u64 clock = rdtsc() - programStartClockCycles;

    clockCycles = {};

    clockCycles.clockCyclesPerMillisecondF = static_cast<f64>(clock) / static_cast<f64>(time);
    clockCycles.clockCyclesPerMillisecond = static_cast<u64>(clockCycles.clockCyclesPerMillisecondF);

    clockCycles.clockCyclesPerSecondF = clockCycles.clockCyclesPerMillisecondF / 1000.0;
    clockCycles.clockCyclesPerSecond = static_cast<u64>(clockCycles.clockCyclesPerSecondF);

    clockCycles.clockCyclesPerMicrosecondF = (clock * 1000.0) / static_cast<f64>(time);
    clockCycles.clockCyclesPerMicrosecond = static_cast<u64>(clockCycles.clockCyclesPerMicrosecondF);

    clockCycles.clockCyclesPerNanosecondF = (clock * 1000000.0) / static_cast<f64>(time);
    clockCycles.clockCyclesPerNanosecond = static_cast<u64>(clockCycles.clockCyclesPerNanosecondF);
}

const ClockCyclesTimeFrame* getClockCyclesPerTimeFrame() noexcept
{
    return &clockCycles;
}

static Ref<IFile> _profileFile = null;
static u32 _profileCount = 0;
static SRWMutex _mutex;

void TimingsWriter::begin(const char* name, const char* fileName) noexcept
{
    _mutex.lock();
    if(!_profileFile)
    {
        _profileFile = VFS::Instance().openFile(fileName, FileProps::WriteNew);
        _profileCount = 0;
        TimingsWriter::writeHeader(name);
    }
    _mutex.unlock();
}

void TimingsWriter::end() noexcept
{
    _mutex.lock();
    if(_profileFile)
    {
        writeFooter();
        _profileFile = null;
    }
    _mutex.unlock();
}

void TimingsWriter::write(const ProfileResult& pr) noexcept
{
    _mutex.lock();
    if(_profileFile)
    {
        if(_profileCount++ > 0)
        {
            _profileFile->writeString(R"(,)");
        }

        const u64 duration = pr.end - pr.start;

        _profileFile->writeString(R"({)");
        _profileFile->writeString(R"("cat":"function",)");
        _profileFile->writeString(R"("dur":)");
        {
            const std::string durString = std::to_string(duration);
            _profileFile->writeString(durString.c_str());
        }
        _profileFile->writeString(R"(,)");
        _profileFile->writeString(R"("name":")");
        _profileFile->writeString(pr.name);
        _profileFile->writeString(R"(",)");
        _profileFile->writeString(R"("ph":"X",)");
        _profileFile->writeString(R"("pid":"0",)");
        _profileFile->writeString(R"("tid":)");
        {
            const std::string threadIDString = std::to_string(pr.threadID);
            _profileFile->writeString(threadIDString.c_str());
        }
        _profileFile->writeString(R"(,)");
        _profileFile->writeString(R"("ts":)");
        {
            const std::string startString = std::to_string(pr.start);
            _profileFile->writeString(startString.c_str());
        }
        _profileFile->writeString(R"(})");
    }
    _mutex.unlock();
}

void TimingsWriter::writeHeader(const char* name) noexcept
{
    if(_profileFile)
    {
        _profileFile->writeString(R"({"otherData:":{"name": ")");
        _profileFile->writeString(name);
        _profileFile->writeString(R"("},"traceEvents":[)");
    }
}

void TimingsWriter::writeFooter() noexcept
{
    if(_profileFile)
    {
        _profileFile->writeString(R"(]})");
    }
}

void PerfTimer::stop() noexcept
{
    const u64 end = microTime();
    const u32 threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
    TimingsWriter::write(TimingsWriter::ProfileResult(_name, threadID, _start, end));
    _stopped = true;
}


DeltaTime::DeltaTime() noexcept
    : _us(0.0f), _ms(0.0f), _s(0.0f),
      _usSinceLastUpdate(0.0f), _msSinceLastUpdate(0.0f), _sSinceLastUpdate(0.0f),
      _usSinceLaunch(0.0f), _msSinceLaunch(0.0f), _sSinceLaunch(0.0f)
{ }

void DeltaTime::setDeltaMicro(float us) noexcept
{
    _us = us;
    _ms = _us / 1000.0f;
    _s = _us / 1000000.0f;
    _usSinceLastUpdate += us;
    _msSinceLastUpdate = _usSinceLastUpdate / 1000.0f;
    _sSinceLastUpdate = _usSinceLastUpdate / 1000000.0f;
    _usSinceLaunch += us;
    _msSinceLaunch = _usSinceLaunch / 1000.0f;
    _sSinceLaunch = _usSinceLaunch / 1000000.0f;
}

void DeltaTime::onUpdate() noexcept
{
    _usSinceLastUpdate = 0.0f;
    _msSinceLastUpdate = 0.0f;
    _sSinceLastUpdate = 0.0f;
}

