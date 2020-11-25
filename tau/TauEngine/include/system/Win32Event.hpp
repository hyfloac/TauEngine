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
    Win32Event() noexcept
        : _handle(CreateEventW(nullptr, FALSE, FALSE, nullptr))
    { }

    ~Win32Event() noexcept
    { CloseHandle(_handle); }

    void signal() const noexcept
    { SetEvent(_handle); }

    bool waitUntilSignaled(unsigned timeoutMS = INFINITE) const noexcept
    { return WaitForSingleObject(_handle, timeoutMS) == WAIT_OBJECT_0; }

    bool checkIfSignaled() const noexcept { return waitUntilSignaled(0); }
};

class Win32ManualEvent final
{
    DELETE_CM(Win32ManualEvent);
private:
    HANDLE _handle;
public:
    Win32ManualEvent() noexcept
        : _handle(CreateEventW(nullptr, TRUE, FALSE, nullptr))
    { }

    ~Win32ManualEvent() noexcept
    { CloseHandle(_handle); }

    void signal() const noexcept
    { SetEvent(_handle); }

    void reset() const noexcept
    { ResetEvent(_handle); }

    bool waitUntilSignaled(unsigned timeoutMS = INFINITE) const noexcept
    { return WaitForSingleObject(_handle, timeoutMS) == WAIT_OBJECT_0; }

    bool checkIfSignaled() const noexcept { return waitUntilSignaled(0); }
};
#endif
