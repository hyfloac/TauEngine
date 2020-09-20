#pragma once

#include <cmath>
#include <cstdio>
#include <Objects.hpp>
#include <NumTypes.hpp>
#include <Windows.h>

class UnitTests final
{
    DELETE_CONSTRUCT(UnitTests);
    DELETE_DESTRUCT(UnitTests);
    DELETE_COPY(UnitTests);
public:
    static u32 testsPerformed() noexcept;
    static u32 testsPassed() noexcept;
    static u32 testsFailed() noexcept;
    static void pass() noexcept;
    static void fail() noexcept;
    static void reset() noexcept;
    static bool passedPrevious() noexcept;
};

class Console final
{
public:
    enum Color : u8
    {
        Red = 0,
        Green,
        Blue,
        Yellow,
        Cyan,
        Magenta,
        BrightRed,
        BrightGreen,
        BrightBlue,
        BrightYellow,
        BrightCyan,
        BrightMagenta,
        White,
        Black,
        Gray,
        LightGray
    };
public:
    static Console& Instance() noexcept;
private:
    static WORD transformColor(Color foreground, Color background) noexcept;
private:
    HANDLE _outConsole;
    HANDLE _errConsole;
private:
    Console() noexcept;
public:
    void setOutColor(Color foreground, Color background) const noexcept;
    void setErrColor(Color foreground, Color background) const noexcept;

    inline void resetOutColor() const noexcept
    { setOutColor(Color::LightGray, Color::Black); }
    inline void resetErrColor() const noexcept
    { setErrColor(Color::LightGray, Color::Black); }
};

class UnitTest final
{
    DELETE_COPY(UnitTest);
private:
    const char* _name;
public:
    inline UnitTest(const char* name) noexcept
        : _name(name)
    {
        Console::Instance().setOutColor(Console::White, Console::Black);
        printf("Starting Test: %s\n", name);
        Console::Instance().resetOutColor();
        UnitTests::reset();
    }

    inline ~UnitTest()
    {
        Console::Instance().setOutColor(Console::White, Console::Black);
        printf("Finishing Test: %s\n", _name);
        if(UnitTests::passedPrevious())
        {
            printf("Passed Tests: %s\n\n", _name);
        }
        else
        {
            printf("Failed Tests: %s\n\n", _name);
        }
        Console::Instance().resetOutColor();
    }
};

#define UNIT_TEST() UnitTest _ut_##__LINE__(__FUNCSIG__)

#define _x_STR0(_X) #_X
#define _x_STR(_X) _x_STR0(_X)

#define UTAssert(_EXPR) do { \
        if(!(_EXPR)) { \
            Console::Instance().setErrColor(Console::BrightRed, Console::Black); \
            fprintf(stderr, "Assert Failed. Expression: %s\n", _x_STR(_EXPR)); UnitTests::fail(); \
            Console::Instance().resetErrColor(); \
        } else { \
            Console::Instance().setOutColor(Console::Green, Console::Black); \
            fprintf(stdout, "Assert Passed. Expression: %s\n", _x_STR(_EXPR)); UnitTests::pass(); \
            Console::Instance().resetOutColor(); \
        } \
    } while(0)

#define Assert(_EXPR) UTAssert(_EXPR)

template<typename _T>
static inline bool epsilonEquals(const _T x, const _T y, const _T epsilon = 1E-5)
{
    const _T epsilonTrue = max(fabs(x), fabs(y)) * epsilon;
    return fabs(x - y) <= epsilonTrue;
}
