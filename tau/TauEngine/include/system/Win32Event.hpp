#pragma once

#ifdef _WIN32

#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

#include <Objects.hpp>

class Win32Event final
{
    DELETE_CM(Win32Event);
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

class Win32ManualEvent final
{
    DELETE_CM(Win32ManualEvent);
private:
    HANDLE _handle;
public:
    inline Win32ManualEvent() noexcept
        : _handle(CreateEvent(NULL, TRUE, FALSE, NULL))
    { }

    inline ~Win32ManualEvent() noexcept
    {
        CloseHandle(_handle);
    }

    inline void signal() const noexcept
    {
        SetEvent(_handle);
    }

    inline void reset() const noexcept
    {
        ResetEvent(_handle);
    }

    inline bool waitUntilSignaled(unsigned timeoutMS = INFINITE) const noexcept
    {
        return WaitForSingleObject(_handle, timeoutMS) == WAIT_OBJECT_0;
    }

    inline bool checkIfSignaled() const noexcept { return waitUntilSignaled(0); }
};
#endif
