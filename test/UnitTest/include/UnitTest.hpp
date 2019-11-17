#pragma once

#include <cstdio>

class UnitTest final
{
private:
    const char* _name;
public:
    inline UnitTest(const char* name) noexcept
        : _name(name)
    {
        printf("Starting Test: %s\n", name);
    }

    inline ~UnitTest()
    {
        printf("Finishing Test: %s\n", _name);
    }
};

#define UNIT_TEST() UnitTest _ut_##__LINE__(__FUNCSIG__)

#define _x_STR0(_X) #_X
#define _x_STR(_X) _x_STR0(_X)

#define Assert(_EXPR) if(!(_EXPR)) { fprintf(stderr, "Assert Failed. Expression: %s\n", _x_STR(_EXPR)); }
