#pragma once

#include <cstdio>

#define _STR_BOOL(_X) ((_X) ? "True" : "False")

static inline void _print(const char* testCase, const char* expression, bool result) noexcept
{
    printf("[%s]: `%s` : %s\n", testCase, expression, _STR_BOOL(result));
}

#define BEGIN_TESTS(_NAME) 

#define TEST_CASE(_NAME, _FUNC) void _FUNC() noexcept { const char* const _test_TestCase = #_NAME;
#define TEST(_EXP) _print(_test_TestCase, #_EXP, _EXP);
