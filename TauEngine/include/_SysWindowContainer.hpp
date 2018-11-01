#pragma once

#ifdef _WIN32
#include <Windows.h>

struct _SysWindowContainer
{
    WNDCLASS windowClass;
    HWND     windowHandle;
    HDC      hdc;
    HGLRC    renderingContext;
};
#endif
