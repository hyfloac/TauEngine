#pragma once

#include <Windows.h>

static LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

typedef void(*__cdecl onResize_f)(UINT, UINT, void*);

class Window final
{
private:
    WNDCLASSEXW _windowClass;
    HWND _hWnd;
    HDC _hDC;
    LONG _x;
    LONG _y;
    LONG _width;
    LONG _height;
    LONG _cWidth;
    LONG _cHeight;
    bool _shouldClose;
    onResize_f _onResize;
    void* _userParam;
public:
    Window(const LONG cWidth, const LONG cHeight, void* const userParam = nullptr) noexcept
        : _windowClass{}
        , _hWnd(NULL)
        , _hDC(NULL)
        , _x(0)
        , _y(0)
        , _width(0)
        , _height(0)
        , _cWidth(cWidth)
        , _cHeight(cHeight)
        , _shouldClose(false)
        , _onResize(nullptr)
        , _userParam(userParam)
    { }

    [[nodiscard]] HWND hWnd() const noexcept { return _hWnd; }
    [[nodiscard]] HDC hDC() const noexcept { return _hDC; }
    [[nodiscard]] LONG x() const noexcept { return _x; }
    [[nodiscard]] LONG y() const noexcept { return _y; }
    [[nodiscard]] LONG width() const noexcept { return _width; }
    [[nodiscard]] LONG height() const noexcept { return _height; }
    [[nodiscard]] LONG cWidth() const noexcept { return _cWidth; }
    [[nodiscard]] LONG cHeight() const noexcept { return _cHeight; }

    [[nodiscard]] bool  shouldClose() const noexcept { return _shouldClose; }
    [[nodiscard]] bool& shouldClose()       noexcept { return _shouldClose; }

    void setOnResize(onResize_f onResize) noexcept { _onResize = onResize; }

    void create() noexcept;
    void close() noexcept;

    void show() noexcept;
    void hide() noexcept;
private:
    friend LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
};
