#pragma once

#ifdef _WIN32
#pragma warning(push, 0)
#include <Windows.h>
#pragma warning(pop)

struct _SysWindowContainer final
{
    WNDCLASSA windowClass;
    HWND      windowHandle;
    HDC       hdc;
};

struct _SysContainer final
{
    HANDLE programHandle;
};
#endif
