#pragma once

#ifdef _WIN32

#include <Windows.h>

class Event final
{
private:
    HANDLE _handle;
public:
    inline Event() noexcept
        : _handle(CreateEvent(NULL, FALSE, FALSE, NULL))
    { }

    inline ~Event() noexcept
    { CloseHandle(_handle); }

    Event(const Event& copy) noexcept = delete;
    Event(Event&& move) noexcept = delete;

    Event& operator=(const Event& copy) noexcept = delete;
    Event& operator=(Event&& move) noexcept = delete;

    inline void signal() noexcept
    { SetEvent(_handle); }

    inline bool waitUntilSignaled(unsigned timeoutMS = INFINITE) noexcept
    { return WaitForSingleObject(_handle, timeoutMS) == WAIT_OBJECT_0; }

    inline bool checkIfSignaled() noexcept { return waitUntilSignaled(0); }
};
#endif
