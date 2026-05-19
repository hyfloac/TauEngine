#pragma once

#ifdef _WIN32
  #pragma warning(push, 0)
  #include <Windows.h>
  #pragma warning(pop)
#else
  #include <pthread.h>
#endif

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
#ifdef _WIN32
    CRITICAL_SECTION _criticalSection;
#else
    pthread_mutex_t _mutex;
#endif
public:
    CSMutex() noexcept
    {
#ifdef _WIN32
        const DWORD flags = TAU_MUTEX_USE_DBG ? 0 : CRITICAL_SECTION_NO_DEBUG_INFO;
        InitializeCriticalSectionEx(&_criticalSection, 512, flags);
#else
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&_mutex, &attr);
        pthread_mutexattr_destroy(&attr);
#endif
    }

    ~CSMutex() noexcept
    {
#ifdef _WIN32
        DeleteCriticalSection(&_criticalSection);
#else
        pthread_mutex_destroy(&_mutex);
#endif
    }

    void lock() noexcept
    {
#ifdef _WIN32
        EnterCriticalSection(&_criticalSection);
#else
        pthread_mutex_lock(&_mutex);
#endif
    }

    bool try_lock() noexcept
    {
#ifdef _WIN32
        return TryEnterCriticalSection(&_criticalSection) != 0;
#else
        return pthread_mutex_trylock(&_mutex) == 0;
#endif
    }

    void unlock() noexcept
    {
#ifdef _WIN32
        LeaveCriticalSection(&_criticalSection);
#else
        pthread_mutex_unlock(&_mutex);
#endif
    }
};

class SRWMutex final
{
    DEFAULT_DESTRUCT(SRWMutex);
    DELETE_CM(SRWMutex);
private:
#ifdef _WIN32
    SRWLOCK _srw;
#else
    pthread_rwlock_t _rw;
#endif
public:
    SRWMutex() noexcept
#ifdef _WIN32
        : _srw(SRWLOCK_INIT)
    { }
#else
    {
        pthread_rwlock_init(&_rw, nullptr);
    }
#endif

    void lockRead() noexcept
    {
#ifdef _WIN32
        AcquireSRWLockShared(&_srw);
#else
        pthread_rwlock_rdlock(&_rw);
#endif
    }

    bool tryLockRead() noexcept
    {
#ifdef _WIN32
        return TryAcquireSRWLockShared(&_srw) != 0;
#else
        return pthread_rwlock_tryrdlock(&_rw) == 0;
#endif
    }

    void unlockRead() noexcept
    {
#ifdef _WIN32
        ReleaseSRWLockShared(&_srw);
#else
        pthread_rwlock_unlock(&_rw);
#endif
    }

    void lockWrite() noexcept
    {
#ifdef _WIN32
        AcquireSRWLockExclusive(&_srw);
#else
        pthread_rwlock_wrlock(&_rw);
#endif
    }

    bool tryLockWrite() noexcept
    {
#ifdef _WIN32
        return TryAcquireSRWLockExclusive(&_srw) != 0;
#else
        return pthread_rwlock_trywrlock(&_rw) == 0;
#endif
    }

    void unlockWrite() noexcept
    {
#ifdef _WIN32
        ReleaseSRWLockExclusive(&_srw);
#else
        pthread_rwlock_unlock(&_rw);
#endif
    }

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
