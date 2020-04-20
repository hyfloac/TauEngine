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
#include "Globals.hpp"
#include <system/GraphicsInterface.hpp>
#include <openvr.h>
#include <texture/FrameBuffer.hpp>

static void onWindowEvent(void* param, WindowEvent& e) noexcept;

class TauEditorApplication final : public Application
{
    DELETE_COPY(TauEditorApplication);
private:
    static constexpr const char* CONFIG_PATH = "|game/config.bin";
    Config _config;
    Window* _window;
    NullableRef<IGraphicsInterface> _graphicsInterface;
    NullableRef<IRenderingContext> _renderingContext;
    CPPRef<spdlog::logger> _logger;
    TERenderer* _renderer;
    vr::IVRSystem* _vr;
    State _gameState;
    Globals* _globals;
    GameRecorder _gr;

    glm::mat4 _vrProjLeft;
    glm::mat4 _vrProjRight;

    IFrameBuffer* _vrLeftFB;
    IFrameBuffer* _vrRightFB;

    u32 _width{}, _height{};
public:
    TauEditorApplication() noexcept;

    ~TauEditorApplication() noexcept override;

    [[nodiscard]] CPPRef<spdlog::logger> logger() const noexcept { return _logger; }

    bool init(int argCount, char* args[]) noexcept override;

    void finalize() noexcept override;

    void onException(ExceptionData& ex) noexcept override;
protected:
    void update(float fixedDelta) noexcept override;

    void render(const DeltaTime& delta) noexcept override;

    void renderFPS(u32 ups, u32 fps) noexcept override;

    void runMessageLoop() noexcept override;
private:
    void setupConfig() noexcept;

    void writeConfig() const noexcept;

    void onWindowEvent(WindowEvent& e) noexcept;

    bool onCharPress(WindowAsciiKeyEvent& e) const noexcept;

    bool onKeyPress(WindowKeyEvent& e) noexcept;

    bool onWindowResize(WindowResizeEvent& e) const noexcept;

    void onIncorrectContext(IncorrectContextException& ex) const noexcept;
    void onBufferSafetyException(BufferSafetyException& ex) const noexcept;

    void initVRControls() const noexcept;
    void initVRFrameBuffers() noexcept;
public:
    friend void onWindowEvent(void* param, WindowEvent& e) noexcept;
};
