#pragma once

#include <spdlog/spdlog.h>
#include <Application.hpp>
#include <system/Window.hpp>
#include <NumTypes.hpp>
#include <events/WindowEvent.hpp>
#include <system/RenderingContext.hpp>
#include <Objects.hpp>
#include "Config.h"

static void onWindowEvent(void* param, WindowEvent& e) noexcept;

class DX10Application final : public Application
{
    DELETE_COPY(DX10Application);
private:
    static constexpr const char* CONFIG_PATH = "|game/config.bin";
    Config _config;
    Window* _window;
    Ref<spdlog::logger> _logger;
    u8 _r, _g, _b;
    u8 _rr, _gg, _bb;
    Ref<IShader> _vertexShader;
public:
    DX10Application() noexcept;

    ~DX10Application() noexcept override;

    [[nodiscard]] Ref<spdlog::logger> logger() const noexcept { return _logger; }

    bool init(int argCount, char* args[]) noexcept override;

    void finalize() noexcept override;

    void onException(Exception& ex) noexcept override;
protected:
    void update(float fixedDelta) noexcept override;

    void render(const DeltaTime& delta) noexcept override;

    void renderFPS(u32 ups, u32 fps) noexcept override;

    void runMessageLoop() noexcept override;
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
