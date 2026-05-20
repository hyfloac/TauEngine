#pragma once

#ifdef _WIN32
#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

struct _SysWindowContainer final
{
    WNDCLASSEXW windowClass;
    HWND        windowHandle;
    HDC         hdc;
};

struct _SysContainer final
{
    HMODULE programHandle;
};
#else
// Stub containers for non-Windows builds. A real X11/Wayland backend would
// store its native handles here.
struct _SysWindowContainer final
{
    void* windowHandle;
    void* displayHandle;
};

struct _SysContainer final
{
    void* programHandle;
};
#endif
