#pragma once

#include <spdlog/spdlog.h>
#include <Application.hpp>
#include <system/Window.hpp>
#include <NumTypes.hpp>
#include "TERenderer.hpp"
#include <events/WindowEvent.hpp>
#include "Config.h"
#include <system/RenderingContext.hpp>

static void onWindowEvent(void* param, WindowEvent& e) noexcept;

class TauEditorApplication final : public Application
{
private:
    static constexpr const char* CONFIG_PATH = "|game/config.bin";
    Window* _window;
    Ref<spdlog::logger> _logger;
    TERenderer* _renderer;
    Config _config;
public:
    TauEditorApplication() noexcept;

    TauEditorApplication(const TauEditorApplication& copy) noexcept = delete;
    TauEditorApplication(TauEditorApplication&& move) noexcept = delete;

    ~TauEditorApplication() noexcept override final;

    TauEditorApplication& operator =(const TauEditorApplication& copy) noexcept = delete;
    TauEditorApplication& operator =(TauEditorApplication&& move) noexcept = delete;

    bool init(int argCount, char* args[]) noexcept override final;

    void finalize() noexcept override final;

    void onException(Exception& ex) noexcept override;
protected:
    void update(const float fixedDelta) noexcept override final;

    void render(const float delta) noexcept override final;

    void renderFPS(const u32 ups, const u32 fps) noexcept override final;

    void runMessageLoop() noexcept override final;
private:
    void setupConfig() noexcept;

    void writeConfig() noexcept;

    void onWindowEvent(WindowEvent& e) noexcept;

    bool onCharPress(WindowAsciiKeyEvent& e) noexcept;

    bool onKeyPress(WindowKeyEvent& e) noexcept;

    void onIncorrectContext(IncorrectContextException& ex);
public:
    friend void onWindowEvent(void* param, WindowEvent& e) noexcept;
};
