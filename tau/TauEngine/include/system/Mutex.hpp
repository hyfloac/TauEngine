#pragma once

#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

class CSMutex final
{
private:
    CRITICAL_SECTION _criticalSection;
public:
    inline CSMutex() noexcept
        : _criticalSection()
    {
        InitializeCriticalSectionEx(&_criticalSection, 512,
                                #if _DEBUG
                                    CRITICAL_SECTION_NO_DEBUG_INFO
                                #else
                                    0
                                #endif
        );
    }

    inline ~CSMutex() noexcept
    {
        DeleteCriticalSection(&_criticalSection);
    }

    CSMutex(const CSMutex& copy) noexcept = delete;
    CSMutex(CSMutex&& move) noexcept = delete;

    CSMutex& operator =(const CSMutex& copy) noexcept = delete;
    CSMutex& operator =(CSMutex&& move) noexcept = delete;

    inline void lock() noexcept
    {
        EnterCriticalSection(&_criticalSection);
    }

    inline bool try_lock() noexcept
    {
        return TryEnterCriticalSection(&_criticalSection) != 0;
    }

    inline void unlock() noexcept
    {
        LeaveCriticalSection(&_criticalSection);
    }
};

class SRWMutex final
{
private:
    SRWLOCK _srw;
public:
    inline SRWMutex() noexcept
        : _srw()
    {
        InitializeSRWLock(&_srw);
    }

    inline ~SRWMutex() noexcept = default;

    SRWMutex(const SRWMutex& copy) noexcept = delete;
    SRWMutex(SRWMutex&& move) noexcept = delete;

    SRWMutex& operator =(const SRWMutex& copy) noexcept = delete;
    SRWMutex& operator =(SRWMutex&& move) noexcept = delete;

    inline void lock() noexcept
    {
        AcquireSRWLockExclusive(&_srw);
    }

    inline bool try_lock() noexcept
    {
        return TryAcquireSRWLockExclusive(&_srw) != 0;
    }

    inline void unlock() noexcept
    {
        ReleaseSRWLockExclusive(&_srw);
    }
};
