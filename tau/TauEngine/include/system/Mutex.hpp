#pragma once

#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

#include <Objects.hpp>

#ifndef TAU_MUTEX_USE_DBG
  #ifdef TAU_PRODUCTION
    #define TAU_MUTEX_USE_DBG 0
  #else
    #define TAU_MUTEX_USE_DBG 1
  #endif
#endif

class CSMutex final
{
    DELETE_CM(CSMutex);
private:
    CRITICAL_SECTION _criticalSection;
public:
    CSMutex() noexcept
    {
        const DWORD flags = TAU_MUTEX_USE_DBG ? 0 : CRITICAL_SECTION_NO_DEBUG_INFO;
        InitializeCriticalSectionEx(&_criticalSection, 512, flags);
    }

    ~CSMutex() noexcept
    { DeleteCriticalSection(&_criticalSection); }

    void lock() noexcept
    { EnterCriticalSection(&_criticalSection); }

    bool try_lock() noexcept
    { return TryEnterCriticalSection(&_criticalSection) != 0; }

    void unlock() noexcept
    { LeaveCriticalSection(&_criticalSection); }
};

class SRWMutex final
{
    DEFAULT_DESTRUCT(SRWMutex);
    DELETE_CM(SRWMutex);
private:
    SRWLOCK _srw;
public:
    SRWMutex() noexcept
        : _srw(SRWLOCK_INIT)
    { }

    void lockRead() noexcept
    { AcquireSRWLockShared(&_srw); }

    bool tryLockRead() noexcept
    { return TryAcquireSRWLockShared(&_srw) != 0; }

    void unlockRead() noexcept
    { ReleaseSRWLockShared(&_srw); }

    void lockWrite() noexcept
    { AcquireSRWLockExclusive(&_srw); }

    bool tryLockWrite() noexcept
    { return TryAcquireSRWLockExclusive(&_srw) != 0; }

    void unlockWrite() noexcept
    { ReleaseSRWLockExclusive(&_srw); }

    void lock() noexcept
    { lockWrite(); }

    bool try_lock() noexcept
    { return tryLockWrite(); }

    void unlock() noexcept
    { unlockWrite(); }
};
