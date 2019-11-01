#pragma once

#include <layer/ILayer.hpp>
#include <Objects.hpp>
#include <RenderingPipeline.hpp>
#include <vector>
#include <String.hpp>
#include <console/ConsoleHandler.hpp>
#include <events/WindowEvent.hpp>
#include "State.hpp"

class Camera2DController;
class ConsoleLayer;

class ConsoleLayer final : public ILayer
{
    DEFAULT_DESTRUCT(ConsoleLayer);
    LAYER_IMPL(ConsoleLayer);
private:
    Window& _window;
    TextHandler& _th;
    const glm::mat4& _ortho;
    RenderingPipeline& _rp;
    State& _state;
    Camera2DController& _camera;
    float _textScale;

    ConsoleHandler _ch;
    std::vector<DynString> _strings;
    StringBuilder _lineBuilder;
    StringBuilder _inputBuilder;
    bool _columnMarker;
public:
    ConsoleLayer(Window& window, TextHandler& th, const glm::mat4& ortho, RenderingPipeline& rp, State& state, Camera2DController& camera, float textScale = 1.0f) noexcept;

    void print(const DynString& str) noexcept;

    void onRender(float delta) noexcept override;
    void onUpdate(float fixedDelta) noexcept override;
    void onEvent(Event& e) noexcept override;
private:
    bool onKeyPress(WindowKeyEvent& e) noexcept;
    bool onCharPress(WindowAsciiKeyEvent& e) noexcept;
private:
    static void cPrint(ConsoleLayer* cLayer, const char* str) noexcept;
    static void cPrintLn(ConsoleLayer* cLayer, const char* str) noexcept;
    static void cPrintNL(ConsoleLayer* cLayer) noexcept;
    static void cPrintF(ConsoleLayer* cLayer, const char* fmt, va_list args) noexcept;

    static inline void __cdecl ccPrint(void* cLayer, const char* str) noexcept
    { cPrint(reinterpret_cast<ConsoleLayer*>(cLayer), str); }

    static inline void __cdecl ccPrintLn(void* cLayer, const char* str) noexcept
    { cPrintLn(reinterpret_cast<ConsoleLayer*>(cLayer), str); }

    static inline void __cdecl ccPrintNL(void* cLayer) noexcept
    { cPrintNL(reinterpret_cast<ConsoleLayer*>(cLayer)); }

    static inline void __cdecl ccPrintF(void* cLayer, const char* fmt, va_list args) noexcept
    { cPrintF(reinterpret_cast<ConsoleLayer*>(cLayer), fmt, args); }
private:
    friend class SetTextScaleCommand;
    friend class SetCameraCommand;
};

class SetTextScaleCommand final : public ConsoleHandler::Command
{
private:
    ConsoleLayer* _cl;
public:
    SetTextScaleCommand(ConsoleLayer* _cl) noexcept
        : _cl(_cl)
    { }

    [[nodiscard]] const char* name() const noexcept override { return "setTextScale"; }
    [[nodiscard]] const char* usage() const noexcept override { return "setTextScale <scale{f32}>"; }
    [[nodiscard]] const char* info() const noexcept override { return "Sets the text scale for the console."; }
    [[nodiscard]] i32 execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler * consoleHandler) noexcept override;
};

class SetExclusiveCommand final : public ConsoleHandler::Command
{
private:
    State& _state;
public:
    SetExclusiveCommand(State& state) noexcept
        : _state(state)
    { }

    [[nodiscard]] const char* name() const noexcept override { return "setExclusive"; }
    [[nodiscard]] const char* usage() const noexcept override { return "setExclusive <exclusive{boolean}>"; }
    [[nodiscard]] const char* info() const noexcept override { return "Sets the console as the exclusive renderer."; }
    [[nodiscard]] i32 execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept override;
};

class SetCameraCommand final : public ConsoleHandler::Command
{
private:
    ConsoleLayer* _cl;
public:
    SetCameraCommand(ConsoleLayer* _cl) noexcept
        : _cl(_cl)
    { }

    [[nodiscard]] const char* name() const noexcept override { return "setCamera"; }
    [[nodiscard]] const char* usage() const noexcept override { return "setCamera <cmd{enum{reset|pos|rot}}> <(pos)<x{f32}> <y{f32}> <z{f32}>> <(rot)degrees{f32}>"; }
    [[nodiscard]] const char* info() const noexcept override { return "Sets various camera properties."; }
    [[nodiscard]] i32 execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept override;
};

