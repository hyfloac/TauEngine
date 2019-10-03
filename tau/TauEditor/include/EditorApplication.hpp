#pragma once

#include <spdlog/spdlog.h>
#include <Application.hpp>
#include <system/Window.hpp>
#include <NumTypes.hpp>
#include "TERenderer.hpp"
#include <events/Event.hpp>
#include <events/WindowEvent.hpp>

static void onWindowEvent(void* param, WindowEvent& e) noexcept;

class TauEditorApplication final : public Application
{
private:
    Window* _window;
    Ref<spdlog::logger> _logger;
    TERenderer* _renderer;
public:
    TauEditorApplication() noexcept;

    TauEditorApplication(const TauEditorApplication& copy) noexcept = delete;
    TauEditorApplication(TauEditorApplication&& move) noexcept = delete;

    ~TauEditorApplication() noexcept override final;

    TauEditorApplication& operator =(const TauEditorApplication& copy) noexcept = delete;
    TauEditorApplication& operator =(TauEditorApplication&& move) noexcept = delete;

    bool init(int argCount, char* args[]) noexcept override final;

    void finalize() noexcept override final;
protected:
    void update(const float fixedDelta) noexcept override final;

    void render(const float delta) noexcept override final;

    void renderFPS(const u32 ups, const u32 fps) noexcept override final;

    void runMessageLoop() noexcept override final;
private:
    void onWindowEvent(WindowEvent& e) noexcept;

    bool onCharPress(WindowAsciiKeyEvent& e) noexcept;

    bool onKeyPress(WindowKeyEvent& e) noexcept;
public:
    friend void onWindowEvent(void* param, WindowEvent& e) noexcept;
};
