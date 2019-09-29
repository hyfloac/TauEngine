#pragma once

#define APP_IMPL
#include <Application.hpp>
#include <system/Window.hpp>

class TauEditorApplication final : public Application
{
private:
    Window* _window;
public:
    TauEditorApplication() noexcept;

    TauEditorApplication(const TauEditorApplication& copy) noexcept = delete;
    TauEditorApplication(TauEditorApplication&& move) noexcept = delete;

    ~TauEditorApplication() noexcept override final;

    TauEditorApplication& operator =(const TauEditorApplication& copy) noexcept = delete;
    TauEditorApplication& operator =(TauEditorApplication&& move) noexcept = delete;

    void init(int argCount, char* args[]) noexcept override final;

    void update(const float fixedDelta) noexcept override final;

    void render(const float delta) noexcept override final;

    void renderFPS(const u32 ups, const u32 fps) noexcept override final;
};
