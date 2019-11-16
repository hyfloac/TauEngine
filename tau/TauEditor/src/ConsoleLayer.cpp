#include "ConsoleLayer.hpp"
#include <astdio.h>
#include <camera/Camera2D.hpp>
#include <TextHandler.hpp>
#include <EnumBitFields.hpp>
#include <Windows.h>

ConsoleLayer::ConsoleLayer(Window& window, GameRecorder& gr, TextHandler& th, const GlyphSetHandle& consolas, const GlyphSetHandle& consolasBold, const GlyphSetHandle& consolasItalic, const GlyphSetHandle& consolasBoldItalic, const glm::mat4& ortho, RenderingPipeline& rp, State& state, Camera2DController& camera, float textScale) noexcept
    : ILayer(false),
      _window(window), _th(th),
      _consolas(consolas), _consolasBold(consolasBold), _consolasItalic(consolasItalic), _consolasBoldItalic(consolasBoldItalic),
      _ortho(ortho), _rp(rp), _state(state), _camera(camera), _textScale(textScale),
      _ch({ ccPrint, ccPrintLn, ccPrintNL, ccPrintF }, this),
      _strings(), _lineBuilder(), _inputBuilder(), _columnMarker(true)
{
    _ch.addCommand(new SetTextScaleCommand(this));
    _ch.addCommand(new SetExclusiveCommand(state));
    _ch.addCommand(new SetCameraCommand(this));
    _ch.addCommand(new GameRecorderCommand(gr));
    // _ch.addCommand(new LoadFontCommand(th, rl));
    _ch.addCommand(new Console::dc::BoolAliasCommand);
    _ch.addCommand(new Console::dc::ExitCommand);
    _ch.addCommand(new Console::dc::ParseNumCommand);
    _ch.addCommand(new Console::dc::AliasCommand);
    _ch.addCommand(new Console::dc::HelpCommand);
    _ch.addCommand(new Console::dc::InfoCommand);
}

void ConsoleLayer::print(const DynString& str) noexcept
{
    _strings.push_back(str);
}

void ConsoleLayer::onRender(const float delta) noexcept
{
    UNUSED(delta);
    if(_visible && _consolas != -1)
    {
        const float textOffset = 50.0f * _textScale;
        const float maxY = 0.0f;
        float y = static_cast<float>(_window.height() - textOffset);

        if(_columnMarker)
        { _inputBuilder.append('|'); }
        else
        { _inputBuilder.append(' '); }
        y += _rp.pushRenderTextLineWrapped(&_th, _consolas, _inputBuilder.c_str(), 0.0f, y, _textScale, 0, 255, 255, &_window, -textOffset, _ortho);
        _inputBuilder.backspace();

        if(_lineBuilder.length() > 0)
        {
            y += _rp.pushRenderTextLineWrapped(&_th, _consolas, _lineBuilder.c_str(), 0.0f, y, _textScale, 0, 120, 255, &_window, -textOffset, _ortho);
        }

        for(auto it = _strings.rbegin(); it != _strings.rend(); ++it)
        {
            y += _rp.pushRenderTextLineWrapped(&_th, _consolas, *it, 0.0f, y, _textScale, 255, 255, 255, &_window, -textOffset, _ortho);
            if(y - textOffset < maxY)
            { break; }
        }
    }
}

void ConsoleLayer::onUpdate(const float fixedDelta) noexcept
{
    UNUSED(fixedDelta);
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

static char* getClipboard() noexcept
{
    if(!OpenClipboard(NULL))
    { }

    HANDLE hData = GetClipboardData(CF_TEXT);
    if(!hData)
    { }

    char* pszText = reinterpret_cast<char*>(GlobalLock(hData));
    if(!pszText)
    { }

    // DynString ret(pszText);

    GlobalUnlock(hData);

    CloseClipboard();

    return pszText;
}

bool ConsoleLayer::onKeyPress(WindowKeyEvent& e) noexcept
{
    if(e.key() == Keyboard::Key::F1 && e.event() == Keyboard::Event::Pressed)
    {
        _visible = !_visible;
        if(_visible)
        {
            Mouse::setVisible(true);
            _state = State::Console;
        }
        else
        {
            Mouse::setVisible(false);
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
                _strings.push_back(command);
                const i32 ret = _ch.runCommand(command.c_str());
                if(ret == NO_COMMAND_FOUND)
                {
                    _ch.println("Command not found.");
                }
                _inputBuilder.reset();
            }
        }
        else if(e.key() == Keyboard::Key::BackSpace && (e.event() == Keyboard::Event::Pressed || e.event() == Keyboard::Event::Held))
        {
            _inputBuilder.backspace();
        }
        else if(e.key() == Keyboard::Key::V && 
               (e.event() == Keyboard::Event::Pressed || e.event() == Keyboard::Event::Held) && 
               (e.flags() & Keyboard::Flags::Ctrl) != static_cast<Keyboard::Flags>(0))
        {
            _inputBuilder.append(getClipboard());
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

i32 SetTextScaleCommand::execute(const char* commandName, const char* args[], u32 argCount, Console::Controller* consoleHandler) noexcept
{
    UNUSED(commandName);
    if(argCount == 1)
    {
        Console::ParseIntError error;
        const float scale = consoleHandler->parseF32(args[0], &error);
        _cl->_textScale = scale;
        return 0;
    }
    return 1;
}

i32 SetExclusiveCommand::execute(const char* commandName, const char* args[], u32 argCount, Console::Controller* consoleHandler) noexcept
{
    UNUSED(commandName);
    if(argCount == 1)
    {
        const Console::BoolFromStr exclusive = consoleHandler->parseBool(args[0]);
        if(exclusive == Console::BoolFromStr::True)
        { _state = State::ConsoleExclusive; }
        else if(exclusive == Console::BoolFromStr::False)
        { _state = State::Console; }
        else { return -1; }
        return 0;
    }
    return 1;
}

i32 SetCameraCommand::execute(const char* commandName, const char* args[], u32 argCount, Console::Controller* consoleHandler) noexcept
{
    UNUSED(commandName);
    if(argCount >= 1)
    {
        if(strcmp(args[0], "reset") == 0)
        {
            _cl->_camera->set({ 0.0f, 0.0f, 0.0f }, 0.0f);
        }
        else if(argCount == 2 && strcmp(args[0], "rot") == 0)
        {
            Console::ParseIntError error;
            const float rot = consoleHandler->parseF32(args[1], &error);
            _cl->_camera->rotation(rot);
        }
        else if(argCount == 4 && strcmp(args[0], "pos") == 0)
        {
            Console::ParseIntError error;
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


i32 GameRecorderCommand::execute(const char* commandName, const char* args[], u32 argCount, Console::Controller* consoleHandler) noexcept
{
    UNUSED(commandName);
    if(argCount != 1)
    {
        consoleHandler->printf("Usage: %s", usage());
        return 1;
    }

    if(strcmp(args[0], "start") == 0)
    {
        _gr.startRecording();
        consoleHandler->println("Started recording.");
    }
    else if(strcmp(args[0], "stop") == 0)
    {
        _gr.stopRecording();
        consoleHandler->println("Stopped recording.");
    }
    else if(strcmp(args[0], "play") == 0)
    {
        _gr.beginPlayBack();
    }

    return 0;
}
