#pragma once

#include "NumTypes.hpp"
#include "_SysWindowContainer.hpp"
#include "DLL.hpp"

class TAU_DLL Window
{
private:
    u32 _width;
    u32 _height;
    const char* _title;
    _SysWindowContainer _windowContainer;
public:
    Window(u32 width, u32 height, const char* title) noexcept;
    ~Window() noexcept;

    inline u32 width()  const noexcept { return _width;  }
    inline u32 height() const noexcept { return _height; }
    inline const char* title() const noexcept { return _title; }
    inline const _SysWindowContainer* __windowContainer() const noexcept { return &_windowContainer; }

    void setSize(u32 width, u32 height) noexcept;
    void setTitle(const char* title) noexcept;

    void createWindow() noexcept;
    void closeWindow() noexcept;
    void showWindow() const noexcept;


};
