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

template<typename _Mutex>
class Lock final
{
    DELETE_CM(Lock);
private:
    _Mutex& _mutex;
public:
    Lock(_Mutex& mutex) noexcept
        : _mutex(mutex)
    { _mutex.lock(); }

    ~Lock() noexcept
    { _mutex.unlock(); }
};

template<typename _Mutex>
class UniqueLock final
{
    DELETE_COPY(UniqueLock);
private:
    _Mutex* _mutex;
public:
    UniqueLock(_Mutex& mutex) noexcept
        : _mutex(&mutex)
    { _mutex->lock(); }

    ~UniqueLock() noexcept
    {
        if(_mutex)
        { _mutex->unlock(); }
    }

    UniqueLock(UniqueLock<_Mutex>&& move) noexcept
        : _mutex(move._mutex)
    { move._mutex = nullptr; }

    UniqueLock& operator=(UniqueLock<_Mutex>&& move) noexcept = delete;

    void lock() noexcept
    { _mutex->lock(); }

    bool try_lock() noexcept
    { return _mutex->try_lock(); }

    void unlock() noexcept
    { _mutex->unlock(); }
};

class ReadLock final
{
    DELETE_CM(ReadLock);
private:
    SRWMutex& _mutex;
public:
    ReadLock(SRWMutex& mutex) noexcept
        : _mutex(mutex)
    { _mutex.lockRead(); }

    ~ReadLock() noexcept
    { _mutex.unlockRead(); }
};
