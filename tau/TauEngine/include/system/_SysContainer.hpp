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
#endif
