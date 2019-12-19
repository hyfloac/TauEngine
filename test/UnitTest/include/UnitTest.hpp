#pragma once

#include <cstdio>
#include <Objects.hpp>
#include <NumTypes.hpp>

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

class UnitTest final
{
    DELETE_COPY(UnitTest);
private:
    const char* _name;
public:
    inline UnitTest(const char* name) noexcept
        : _name(name)
    {
        printf("Starting Test: %s\n", name);
        UnitTests::reset();
    }

    inline ~UnitTest()
    {
        printf("Finishing Test: %s\n", _name);
        if(UnitTests::passedPrevious())
        {
            printf("Passed Tests: %s\n\n", _name);
        }
        else
        {
            printf("Failed Tests: %s\n\n", _name);
        }
    }
};

#define UNIT_TEST() UnitTest _ut_##__LINE__(__FUNCSIG__)

#define _x_STR0(_X) #_X
#define _x_STR(_X) _x_STR0(_X)

#define Assert(_EXPR) \
    if(!(_EXPR)) { fprintf(stderr, "Assert Failed. Expression: %s\n", _x_STR(_EXPR)); UnitTests::fail(); }\
    else { fprintf(stdout, "Assert Passed. Expression: %s\n", _x_STR(_EXPR)); UnitTests::pass(); }
