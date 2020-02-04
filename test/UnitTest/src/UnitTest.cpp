#include "UnitTest.hpp"

static u32 _testsPerformed = 0;
static u32 _testsPassed = 0;
static u32 _testsFailed = 0;
static u32 _currentCycleFails = 0;

u32 UnitTests::testsPerformed() noexcept
{ return _testsPerformed; }

u32 UnitTests::testsPassed() noexcept
{ return _testsPassed; }

u32 UnitTests::testsFailed() noexcept
{ return _testsFailed; }

void UnitTests::reset() noexcept
{
    _currentCycleFails = 0;
}

void UnitTests::pass() noexcept
{
    ++_testsPerformed;
    ++_testsPassed;
}

void UnitTests::fail() noexcept
{
    ++_testsPerformed;
    ++_testsFailed;
    ++_currentCycleFails;
}

bool UnitTests::passedPrevious() noexcept
{
    return _currentCycleFails == 0;
}


Console& Console::Instance() noexcept
{
    static Console _instance;
    return _instance;
}

WORD Console::transformColor(const Color foreground, const Color background) noexcept
{
    WORD ret = 0;

    switch(foreground)
    {
        case Red:           ret |= FOREGROUND_RED;  break;
        case Green:         ret |= FOREGROUND_GREEN; break;
        case Blue:          ret |= FOREGROUND_BLUE;  break;
        case Yellow:        ret |= FOREGROUND_RED   | FOREGROUND_GREEN; break;
        case Cyan:          ret |= FOREGROUND_GREEN | FOREGROUND_BLUE; break;
        case Magenta:       ret |= FOREGROUND_BLUE  | FOREGROUND_RED; break;
        case BrightRed:     ret |= FOREGROUND_RED   | FOREGROUND_INTENSITY;  break;
        case BrightGreen:   ret |= FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case BrightBlue:    ret |= FOREGROUND_BLUE  | FOREGROUND_INTENSITY;  break;
        case BrightYellow:  ret |= FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_INTENSITY; break;
        case BrightCyan:    ret |= FOREGROUND_GREEN | FOREGROUND_BLUE  | FOREGROUND_INTENSITY; break;
        case BrightMagenta: ret |= FOREGROUND_BLUE  | FOREGROUND_RED   | FOREGROUND_INTENSITY; break;
        case White:         ret |= FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
        case Black:         ret |= 0; break;
        case Gray:          ret |= FOREGROUND_INTENSITY; break;
        case LightGray:     ret |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
        default: break;     
    }

    switch(background)
    {
        case Red:           ret |= BACKGROUND_RED;  break;
        case Green:         ret |= BACKGROUND_GREEN; break;
        case Blue:          ret |= BACKGROUND_BLUE;  break;
        case Yellow:        ret |= BACKGROUND_RED   | BACKGROUND_GREEN; break;
        case Cyan:          ret |= BACKGROUND_GREEN | BACKGROUND_BLUE; break;
        case Magenta:       ret |= BACKGROUND_BLUE  | BACKGROUND_RED; break;
        case BrightRed:     ret |= BACKGROUND_RED   | BACKGROUND_INTENSITY;  break;
        case BrightGreen:   ret |= BACKGROUND_GREEN | BACKGROUND_INTENSITY; break;
        case BrightBlue:    ret |= BACKGROUND_BLUE  | BACKGROUND_INTENSITY;  break;
        case BrightYellow:  ret |= BACKGROUND_RED   | BACKGROUND_GREEN | BACKGROUND_INTENSITY; break;
        case BrightCyan:    ret |= BACKGROUND_GREEN | BACKGROUND_BLUE  | BACKGROUND_INTENSITY; break;
        case BrightMagenta: ret |= BACKGROUND_BLUE  | BACKGROUND_RED   | BACKGROUND_INTENSITY; break;
        case White:         ret |= BACKGROUND_RED   | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY; break;
        case Black:         ret |= 0; break;
        case Gray:          ret |= BACKGROUND_INTENSITY; break;
        case LightGray:     ret |= BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE; break;
        default: break;     
    }

    return ret;
}

Console::Console() noexcept
    : _outConsole(GetStdHandle(STD_OUTPUT_HANDLE)),
      _errConsole(GetStdHandle(STD_ERROR_HANDLE))
{ }

void Console::setOutColor(const Color foreground, const Color background) const noexcept
{
    SetConsoleTextAttribute(_outConsole, transformColor(foreground, background));
}

void Console::setErrColor(const Color foreground, const Color background) const noexcept
{
    SetConsoleTextAttribute(_errConsole, transformColor(foreground, background));
}
