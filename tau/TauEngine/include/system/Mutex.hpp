#pragma once

#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

#include <Objects.hpp>

class CSMutex final
{
    DELETE_CM(CSMutex);
private:
    CRITICAL_SECTION _criticalSection;
public:
    inline CSMutex() noexcept
    {
        InitializeCriticalSectionEx(&_criticalSection, 512,
                                #ifdef TAU_PRODUCTION
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
    DEFAULT_DESTRUCT(SRWMutex);
    DELETE_CM(SRWMutex);
private:
    SRWLOCK _srw;
public:
    inline SRWMutex() noexcept
        : _srw(SRWLOCK_INIT)
    { }

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
