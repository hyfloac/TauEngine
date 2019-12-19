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
