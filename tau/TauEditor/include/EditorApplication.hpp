#pragma once

#include <spdlog/spdlog.h>
#include <Application.hpp>
#include <system/Window.hpp>
#include <NumTypes.hpp>
#include "TERenderer.hpp"
#include <events/WindowEvent.hpp>
#include "Config.h"
#include <system/RenderingContext.hpp>
#include <Objects.hpp>

static void onWindowEvent(void* param, WindowEvent& e) noexcept;

class TauEditorApplication final : public Application
{
    DELETE_COPY(TauEditorApplication);
private:
    static constexpr const char* CONFIG_PATH = "|game/config.bin";
    Config _config;
    Window* _window;
    Ref<spdlog::logger> _logger;
    TERenderer* _renderer;
    State _gameState;
public:
    TauEditorApplication() noexcept;

    ~TauEditorApplication() noexcept override final;

    [[nodiscard]] Ref<spdlog::logger> logger() const noexcept { return _logger; }

    bool init(int argCount, char* args[]) noexcept override final;

    void finalize() noexcept override final;

    void onException(ExceptionData& ex) noexcept override;
protected:
    void update(float fixedDelta) noexcept override final;

    void render(const DeltaTime& delta) noexcept override final;

    void renderFPS(u32 ups, u32 fps) noexcept override final;

    void runMessageLoop() noexcept override final;
private:
    void setupConfig() noexcept;

    void writeConfig() noexcept;

    void onWindowEvent(WindowEvent& e) noexcept;

    bool onCharPress(WindowAsciiKeyEvent& e) const noexcept;

    bool onKeyPress(WindowKeyEvent& e) noexcept;

    bool onWindowResize(WindowResizeEvent& e) const noexcept;

    void onIncorrectContext(IncorrectContextException& ex) const noexcept;
public:
    friend void onWindowEvent(void* param, WindowEvent& e) noexcept;
};
