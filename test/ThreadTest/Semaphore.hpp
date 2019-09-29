#pragma once

#include <cstddef>
#include <mutex>
#include <condition_variable>
#include "Mutex.hpp"

template<typename _Mutex>
class Semaphore final
{
private:
    _Mutex _mutex;
    std::condition_variable _cv;
    std::size_t _count;
public:
    inline Semaphore() noexcept
        : _mutex(), _cv(), _count(0)
    { }

    inline ~Semaphore() noexcept = default;

    Semaphore(const Semaphore& copy) noexcept = delete;
    Semaphore(Semaphore&& move) noexcept = delete;

    Semaphore& operator=(const Semaphore& copy) noexcept = delete;
    Semaphore& operator=(Semaphore&& move) noexcept = delete;

    void notify() noexcept
    {
        std::lock_guard<decltype(_mutex)> lock(_mutex);
        ++_count;
        _cv.notify_one();
    }

    void notify_all() noexcept
    {
        std::lock_guard<decltype(_mutex)> lock(_mutex);
        ++_count;
        _cv.notify_all();
    }

    void wait() noexcept
    {
        std::unique_lock<decltype(_mutex)> lock(_mutex);
        while(!_count)
        { _cv.wait(lock); }
        --_count;
    }

    bool try_wait() noexcept
    {
        std::lock_guard<decltype(_mutex)> lock(_mutex);
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
