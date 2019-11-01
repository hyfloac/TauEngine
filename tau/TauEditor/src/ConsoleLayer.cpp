#include "ConsoleLayer.hpp"
#include <astdio.h>
#include <camera/Camera2D.hpp>

ConsoleLayer::ConsoleLayer(Window& window, TextHandler& th, const glm::mat4& ortho, RenderingPipeline& rp, State& state, Camera2DController& camera, float textScale) noexcept
    : ILayer(false),
      _window(window), _th(th), _ortho(ortho), _rp(rp), _state(state), _camera(camera), _textScale(textScale),
      _ch({ ccPrint, ccPrintLn, ccPrintNL, ccPrintF }, this),
      _strings(), _lineBuilder(), _inputBuilder(), _columnMarker(true)
{
    _ch.addCommand(new SetTextScaleCommand(this));
    _ch.addCommand(new SetExclusiveCommand(state));
    _ch.addCommand(new SetCameraCommand(this));
    _ch.addCommand(new dc::BoolAliasCommand);
    _ch.addCommand(new dc::ExitCommand);
    _ch.addCommand(new dc::ParseNumCommand);
    _ch.addCommand(new dc::AliasCommand);
    _ch.addCommand(new dc::HelpCommand);
    _ch.addCommand(new dc::InfoCommand);
}

void ConsoleLayer::print(const DynString& str) noexcept
{
    _strings.push_back(str);
}

void ConsoleLayer::onRender(const float delta) noexcept
{
    if(_visible)
    {
        const float textOffset = 40.0f * _textScale;
        const float maxY = 0.0f;
        float y = static_cast<float>(_window.height() - textOffset);

        if(_columnMarker)
        {
            _inputBuilder.append('|');
        }
        y += _rp.pushRenderTextLineWrapped(&_th, _inputBuilder.c_str(), 0.0f, y, _textScale, 0, 255, 255, &_window, -textOffset, _ortho);
        if(_columnMarker)
        {
            _inputBuilder.backspace();
        }
        if(_lineBuilder.length() > 0)
        {
            y += _rp.pushRenderTextLineWrapped(&_th, _lineBuilder.c_str(), 0.0f, y, _textScale, 0, 120, 255, &_window, -textOffset, _ortho);
        }

        for(auto it = _strings.rbegin(); it != _strings.rend(); ++it)
        {
            y += _rp.pushRenderTextLineWrapped(&_th, *it, 0.0f, y, _textScale, 255, 255, 255, &_window, -textOffset, _ortho);
            if(y - textOffset < maxY)
            {
                break;
            }
        }
    }
}

void ConsoleLayer::onUpdate(const float fixedDelta) noexcept
{
    static u64 _count = 0;
    ++_count;
    if(_count >= 16)
    {
        _count = 0;
        _columnMarker = !_columnMarker;
    }
}

void ConsoleLayer::onEvent(Event& e) noexcept
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowKeyEvent>(this, &ConsoleLayer::onKeyPress);
    dispatcher.dispatch<WindowAsciiKeyEvent>(this, &ConsoleLayer::onCharPress);
}

bool ConsoleLayer::onKeyPress(WindowKeyEvent& e) noexcept
{
    if(e.key() == Keyboard::Key::F1 && e.event() == Keyboard::Event::Pressed)
    {
        _visible = !_visible;
        if(_visible)
        {
            _state = State::Console;
        }
        else
        {
            _state = State::Game;
        }
    }
    else if(_visible)
    {
        if(e.key() == Keyboard::Key::Enter && e.event() == Keyboard::Event::Pressed)
        {
            if(_inputBuilder.length() > 0)
            {
                const DynString command = _inputBuilder.toString();
                _ch.runCommand(command.c_str());
                _strings.push_back(command);
                _inputBuilder.reset();
            }
        }
        else if(e.key() == Keyboard::Key::BackSpace && (e.event() == Keyboard::Event::Pressed || e.event() == Keyboard::Event::Held))
        {
            _inputBuilder.backspace();
        }
        return true;
    }
    return false;
}

bool ConsoleLayer::onCharPress(WindowAsciiKeyEvent& e) noexcept
{
    if(_visible)
    {
        if(e.c() >= ' ' && e.c() <= '~')
        {
            _inputBuilder.append(e.c());
        }
        return true;
    }
    return false;
}

void ConsoleLayer::cPrint(ConsoleLayer* cLayer, const char* str) noexcept
{
    cLayer->print(DynString(str));
}

void ConsoleLayer::cPrintLn(ConsoleLayer* cLayer, const char* str) noexcept
{
    cLayer->print(DynString(str));
}

void ConsoleLayer::cPrintNL(ConsoleLayer* cLayer) noexcept
{
    cLayer->print(DynString(""));
}

void ConsoleLayer::cPrintF(ConsoleLayer* cLayer, const char* fmt, va_list args) noexcept
{
    char* str;
    avsprintf(&str, fmt, args);
    cLayer->print(DynString(str));
    asfreepf(str);
}

i32 SetTextScaleCommand::execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept
{
    if(argCount == 1)
    {
        ParseIntError error;
        const float scale = consoleHandler->parseF32(args[0], &error);
        _cl->_textScale = scale;
        return 0;
    }
    return 1;
}

i32 SetExclusiveCommand::execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept
{
    if(argCount == 1)
    {
        const BoolFromStr exclusive = consoleHandler->parseBool(args[0]);
        if(exclusive == BoolFromStr::BFS_TRUE)
        { _state = State::ConsoleExclusive; }
        else if(exclusive == BoolFromStr::BFS_FALSE)
        { _state = State::Console; }
        else { return -1; }
        return 0;
    }
    return 1;
}


i32 SetCameraCommand::execute(const char* commandName, const char* args[], u32 argCount, ConsoleHandler* consoleHandler) noexcept
{
    if(argCount >= 1)
    {
        if(strcmp(args[0], "reset") == 0)
        {
            _cl->_camera->set({ 0.0f, 0.0f, 0.0f }, 0.0f);
        }
        else if(argCount == 2 && strcmp(args[0], "rot") == 0)
        {
            ParseIntError error;
            const float rot = consoleHandler->parseF32(args[1], &error);
            _cl->_camera->rotation(rot);
        }
        else if(argCount == 4 && strcmp(args[0], "pos") == 0)
        {
            ParseIntError error;
            const float x = consoleHandler->parseF32(args[1], &error);
            const float y = consoleHandler->parseF32(args[2], &error);
            const float z = consoleHandler->parseF32(args[3], &error);
            _cl->_camera->position({ x, y, z });
        }
        else
        {
            consoleHandler->printf("Invalid command `%s`.", args[0]);
            consoleHandler->printf("Usage: %s", usage());
            return -1;
        }
    }
    return 0;
}
