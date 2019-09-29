#pragma once

#ifdef _WIN32

#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

class Win32Event final
{
private:
    HANDLE _handle;
public:
    inline Win32Event() noexcept
        : _handle(CreateEvent(NULL, FALSE, FALSE, NULL))
    { }

    inline ~Win32Event() noexcept
    {
        CloseHandle(_handle);
    }

    Win32Event(const Win32Event& copy) noexcept = delete;
    Win32Event(Win32Event&& move) noexcept = delete;

    Win32Event& operator=(const Win32Event& copy) noexcept = delete;
    Win32Event& operator=(Win32Event&& move) noexcept = delete;

    inline void signal() const noexcept
    {
        SetEvent(_handle);
    }

    inline bool waitUntilSignaled(unsigned timeoutMS = INFINITE) const noexcept
    {
        return WaitForSingleObject(_handle, timeoutMS) == WAIT_OBJECT_0;
    }

    inline bool checkIfSignaled() const noexcept { return waitUntilSignaled(0); }
};
#endif
