#pragma once

#include <Objects.hpp>
#include "imgui.h"

class Window;
class IBuffer;
class IIndexBuffer;
class IVertexArray;
class IRenderingContext;
class IShader;
class IShaderProgram;
class Event;
class WindowKeyEvent;
class WindowAsciiKeyEvent;
class WindowMouseClickEvent;

class IMGUI_IMPL_API ImGuiTau final
{
    DEFAULT_COPY(ImGuiTau);
private:
    Window& _window;
    CPPRef<IBuffer> _vertexBuffer;
    CPPRef<IIndexBuffer> _indexBuffer;
    CPPRef<IVertexArray> _va;
    CPPRef<IShader> _vertexShader;
    CPPRef<IShaderProgram> _shader;
public:
    ImGuiTau(Window& window, const CPPRef<IShader>& vertexShader, const CPPRef<IShaderProgram>& shaderProgram) noexcept
        : _window(window),
          _vertexBuffer(null), _indexBuffer(null),
          _va(null),
          _vertexShader(vertexShader), _shader(shaderProgram)
    { }

    ~ImGuiTau() noexcept = default;

    void render(IRenderingContext& ctx, ImDrawData* drawData) noexcept;

    void update() noexcept;

    void onEvent(Event& e) noexcept;
private:
    void initBuffers(IRenderingContext& ctx, ImDrawData* drawData) noexcept;

    void initKeys() noexcept;

    bool onKeyPress(WindowKeyEvent& e) noexcept;
    bool onAsciiKeyPress(WindowAsciiKeyEvent& e) noexcept;
    bool onMouseClick(WindowMouseClickEvent& e) noexcept;
};
