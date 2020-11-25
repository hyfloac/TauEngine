#pragma once

#pragma warning(push, 0)
#include <cstddef>
#include <mutex>
#include <condition_variable>
#pragma warning(pop)
#include "Mutex.hpp"

template<typename _Mutex>
class Semaphore final
{
    DEFAULT_DESTRUCT(Semaphore);
    DELETE_CM(Semaphore);
private:
    _Mutex _mutex;
    ::std::condition_variable _cv;
    ::std::size_t _count;
public:
    Semaphore() noexcept
        : _mutex()
        , _count(0)
    { }

    void notify() noexcept
    {
        ::std::lock_guard<_Mutex> lock(_mutex);
        ++_count;
        _cv.notify_one();
    }

    void notify_all() noexcept
    {
        ::std::lock_guard<_Mutex> lock(_mutex);
        ++_count;
        _cv.notify_all();
    }

    void wait() noexcept
    {
        ::std::unique_lock<_Mutex> lock(_mutex);
        while(!_count)
        {
            _cv.wait(lock);
        }
        --_count;
    }

    bool try_wait() noexcept
    {
        ::std::lock_guard<_Mutex> lock(_mutex);
        if(_count)
        {
            --_count;
            return true;
        }
        return false;
    }
};

using CPPSemaphore = Semaphore<std::mutex>;
using CSSemaphore = Semaphore<CSMutex>;
using SRWSemaphore = Semaphore<SRWMutex>;
