#pragma once

#ifdef null
#define _nullStore
#undef null
#endif
#include <fmt/format.h>
#include <fmt/color.h>
#ifdef _nullStore
#define null nullptr
#undef _nullStore
#endif
#include <cmath>
#include <cstdio>
#include <Objects.hpp>
#include <ConPrinter.hpp>
#include <NumTypes.hpp>
#include <Windows.h>
#include <list>

class UnitTests final
{
    DELETE_CONSTRUCT(UnitTests);
    DELETE_DESTRUCT(UnitTests);
    DELETE_CM(UnitTests);
private:
    static u32 _testsPerformed;
    static u32 _testsPassed;
    static u32 _testsFailed;
    static u32 _currentCycleFails;
public:
    static u32 testsPerformed() noexcept { return _testsPerformed; }
    static u32 testsPassed() noexcept { return _testsPassed; }
    static u32 testsFailed() noexcept { return _testsFailed; }
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
    bool _shouldWrite;
private:
    Console(bool shouldWrite = true) noexcept;
public:
    [[nodiscard]] bool& shouldWrite()       noexcept { return _shouldWrite; }
    [[nodiscard]] bool  shouldWrite() const noexcept { return _shouldWrite; }

    void setOutColor(Color foreground, Color background) const noexcept;
    void setErrColor(Color foreground, Color background) const noexcept;

    void resetOutColor() const noexcept
    { setOutColor(Color::LightGray, Color::Black); }
    void resetErrColor() const noexcept
    { setErrColor(Color::LightGray, Color::Black); }
    
    template<typename _T>
    Console& print(const _T& t) noexcept
    {
        if(_shouldWrite)
        { ConPrinter::print(stderr, t); }
        return *this;
    }

    template<typename _T>
    Console& printW(const _T& t) noexcept
    {
        if(_shouldWrite)
        { ConPrinter::printW(stderr, t); }
        return *this;
    }

    template<typename... _Args>
    Console& print(const char* fmt, _Args... args) noexcept
    {
        if(_shouldWrite)
        { ConPrinter::print(stderr, fmt, args...); }
        return *this;
    }

    template<typename... _Args>
    Console& printW(const wchar_t* fmt, _Args... args) noexcept
    {
        if(_shouldWrite)
        { ConPrinter::printW(stderr, fmt, args...); }
        return *this;
    }

    template<typename... _Args>
    Console& print(const wchar_t* fmt, _Args... args) noexcept
    { return printW(fmt, args...); }
};

class UnitTest final
{
    DELETE_CM(UnitTest);
private:
    const char* _name;
public:
    UnitTest(const char* name) noexcept
        : _name(name)
    {
        Console::Instance().setOutColor(Console::White, Console::Black);
        ConPrinter::printW(stdout, L"Starting Test: %\n", name);
        Console::Instance().resetOutColor();
        UnitTests::reset();
    }

    ~UnitTest()
    {
        Console::Instance().setOutColor(Console::White, Console::Black);
        ConPrinter::printW(stdout, L"Finishing Test: %\n", _name);
        if(UnitTests::passedPrevious())
        {
            ConPrinter::printW(stdout, L"Passed Tests: %\n\n", _name);
        }
        else
        {
            ConPrinter::printW(stdout, L"Failed Tests: %\n\n", _name);
        }
        Console::Instance().resetOutColor();
    }
};

#define UNIT_TEST() UnitTest _ut_##__LINE__(__FUNCSIG__)

#define _tau_STRINGIFY0(_X) L ## #_X
#define _tau_STRINGIFY(_X) _tau_STRINGIFY0(_X)

#define _tau_TOKEN_PASTE0(_X, _Y) _X ## _Y
#define _tau_TOKEN_PASTE(_X, _Y) _tau_TOKEN_PASTE0(_X, _Y)

#define UTAssert(_EXPR) do { \
        if(!(_EXPR)) { \
            Console::Instance().setErrColor(Console::BrightRed, Console::Black); \
            ConPrinter::printW(stderr, L"Assert Failed. Expression: %\n", _tau_STRINGIFY(_EXPR)); UnitTests::fail(); \
            Console::Instance().resetErrColor(); \
        } else { \
            Console::Instance().setOutColor(Console::Green, Console::Black); \
            ConPrinter::printW(stdout, L"Assert Passed. Expression: %\n", _tau_STRINGIFY(_EXPR)); UnitTests::pass(); \
            Console::Instance().resetOutColor(); \
        } \
    } while(0)

#define Assert(_EXPR) UTAssert(_EXPR)

enum class TauComparisonType
{
    Equal,
    NotEqual,
    Greater,
    Lesser,
    GreaterEqual,
    LesserEqual
};

class ITestCase
{
    DEFAULT_DESTRUCT_VI(ITestCase);
    DELETE_CM(ITestCase);
public:
    ITestCase* _next;
    const wchar_t* _testSuite;
    const wchar_t* _testCase;
    ::std::list<::std::wstring> _messageList;
public:
    ITestCase(const wchar_t* const testSuite, const wchar_t* const testCase) noexcept
        : _next(nullptr)
        , _testSuite(testSuite)
        , _testCase(testCase)
    { }

    void test()
    {
        // Console::Instance().setOutColor(Console::White, Console::Black);
        _messageList.push_back(fmt::format(fg(fmt::color::white) | bg(fmt::color::black), L"Starting Test: {}\n", _testCase));
        Console::Instance().resetOutColor();
        UnitTests::reset();

        _test();

        // Console::Instance().setOutColor(Console::White, Console::Black);
        _messageList.push_back(fmt::format(fg(fmt::color::white) | bg(fmt::color::black), L"Finishing Test: {}\n", _testCase));

        if(UnitTests::passedPrevious())
        { _messageList.push_back(fmt::format(fg(fmt::color::white) | bg(fmt::color::black), L"Passed Tests: {}\n\n", _testCase)); }
        else
        { _messageList.push_back(fmt::format(fg(fmt::color::white) | bg(fmt::color::black), L"Failed Tests: {}\n\n", _testCase)); }

        Console::Instance().resetOutColor();
    }

    void print() const noexcept
    {
        for(const auto& msg : _messageList)
        {
            fmt::print(msg);
        }
    }
protected:
    virtual void _test() = 0;
};

template<typename _Actual, typename _Expected, TauComparisonType _Comparison>
struct TauUnitAssert final
{
    [[nodiscard]] static bool tauAssert(ITestCase& test, const _Actual& a, const _Expected& b, const wchar_t* aStr, const wchar_t* bStr)
    { return false; }
};

template<typename _Actual, typename _Expected>
struct TauUnitAssert<_Actual, _Expected, TauComparisonType::Equal> final
{
    [[nodiscard]] static bool tauAssert(ITestCase& test, const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr)
    {
        if(a == b)
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::green) | bg(fmt::color::black), L"Assert Passed. Expression: {} == {} ({} == {})\n", aStr, bStr, a, b));
            UnitTests::pass();
            return true;
        }
        else
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::red) | bg(fmt::color::black), L"Assert Failed. Expression: {} == {} ({} == {})\n", aStr, bStr, a, b));
            UnitTests::fail();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected>
struct TauUnitAssert<_Actual, _Expected, TauComparisonType::NotEqual> final
{
    [[nodiscard]] bool static tauAssert(ITestCase& test, const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr)
    {
        if(a != b)
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::green) | bg(fmt::color::black), L"Assert Passed. Expression: {} != {} ({} != {})\n", aStr, bStr, a, b));
            UnitTests::pass();
            return true;
        }
        else
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::red) | bg(fmt::color::black), L"Assert Failed. Expression: {} != {} ({} != {})\n", aStr, bStr, a, b));
            UnitTests::fail();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected>
struct TauUnitAssert<_Actual, _Expected, TauComparisonType::Greater> final
{
    [[nodiscard]] bool static tauAssert(ITestCase& test, const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr)
    {
        if(a > b)
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::green) | bg(fmt::color::black), L"Assert Passed. Expression: {} > {} ({} > {})\n", aStr, bStr, a, b));
            UnitTests::pass();
            return true;
        }
        else
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::red) | bg(fmt::color::black), L"Assert Failed. Expression: {} > {} ({} > {})\n", aStr, bStr, a, b));
            UnitTests::fail();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected>
struct TauUnitAssert<_Actual, _Expected, TauComparisonType::Lesser> final
{
    [[nodiscard]] static bool tauAssert(ITestCase& test, const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr)
    {
        if(a < b)
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::green) | bg(fmt::color::black), L"Assert Passed. Expression: {} < {} ({} < {})\n", aStr, bStr, a, b));
            UnitTests::pass();
            return true;
        }
        else
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::red) | bg(fmt::color::black), L"Assert Failed. Expression: {} < {} ({} < {})\n", aStr, bStr, a, b));
            UnitTests::fail();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected>
struct TauUnitAssert<_Actual, _Expected, TauComparisonType::GreaterEqual> final
{
    [[nodiscard]] static bool tauAssert(ITestCase& test, const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr)
    {
        if(a >= b)
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::green) | bg(fmt::color::black), L"Assert Passed. Expression: {} >= {} ({} >= {})\n", aStr, bStr, a, b));
            UnitTests::pass();
            return true;
        }
        else
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::red) | bg(fmt::color::black), L"Assert Failed. Expression: {} >= {} ({} >= {})\n", aStr, bStr, a, b));
            UnitTests::fail();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected>
struct TauUnitAssert<_Actual, _Expected, TauComparisonType::LesserEqual> final
{
    [[nodiscard]] static bool tauAssert(ITestCase& test, const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr)
    {
        if(a <= b)
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::green) | bg(fmt::color::black), L"Assert Passed. Expression: {} <= {} ({} <= {})\n", aStr, bStr, a, b));
            UnitTests::pass();
            return true;
        }
        else
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::red) | bg(fmt::color::black), L"Assert Failed. Expression: {} <= {} ({} <= {})\n", aStr, bStr, a, b));
            UnitTests::fail();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected, TauComparisonType _Comparison, bool _Relative, typename _Epsilon = _Actual>
struct TauUnitAssertFloat final
{
    [[nodiscard]] static bool tauAssert(ITestCase& test, const _Actual& a, const _Expected& b, const wchar_t* aStr, const wchar_t* bStr, _Epsilon epsilon = 1E-5f)
    { return false; }
};

template<typename _Actual, typename _Expected, typename _Epsilon>
struct TauUnitAssertFloat<_Actual, _Expected, TauComparisonType::Equal, true, _Epsilon>  final
{
    [[nodiscard]] static bool tauAssert(ITestCase& test, const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr, const _Epsilon epsilon = 1E-5f)
    {
        if(rEpsilonEquals(a, b, epsilon))
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::green) | bg(fmt::color::black), L"Assert Passed. Expression: {} ~== {} ({} ~== {})\n", aStr, bStr, a, b));
            UnitTests::pass();
            return true;
        }
        else
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::red) | bg(fmt::color::black), L"Assert Failed. Expression: {} ~== {} ({} ~== {})\n", aStr, bStr, a, b));
            UnitTests::fail();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected, typename _Epsilon>
struct TauUnitAssertFloat<_Actual, _Expected, TauComparisonType::Equal, false, _Epsilon>  final
{
    [[nodiscard]] static bool tauAssert(ITestCase& test, const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr, const _Epsilon epsilon = 1E-5f)
    {
        if(aEpsilonEquals(a, b, epsilon))
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::green) | bg(fmt::color::black), L"Assert Passed. Expression: {} ~== {} ({} ~== {})\n", aStr, bStr, a, b));
            UnitTests::pass();
            return true;
        }
        else
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::red) | bg(fmt::color::black), L"Assert Failed. Expression: {} ~== {} ({} ~== {})\n", aStr, bStr, a, b));
            UnitTests::fail();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected, typename _Epsilon>
struct TauUnitAssertFloat<_Actual, _Expected, TauComparisonType::NotEqual, true, _Epsilon>  final
{
    [[nodiscard]] static bool tauAssert(ITestCase& test, const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr, const _Epsilon epsilon = 1E-5f)
    {
        if(!rEpsilonEquals(a, b, epsilon))
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::green) | bg(fmt::color::black), L"Assert Passed. Expression: {} ~!= {} ({} ~!= {})\n", aStr, bStr, a, b));
            UnitTests::pass();
            return true;
        }
        else
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::red) | bg(fmt::color::black), L"Assert Failed. Expression: {} ~!= {} ({} ~!= {})\n", aStr, bStr, a, b));
            UnitTests::fail();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected, typename _Epsilon>
struct TauUnitAssertFloat<_Actual, _Expected, TauComparisonType::NotEqual, false, _Epsilon>  final
{
    [[nodiscard]] static bool tauAssert(ITestCase& test, const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr, const _Epsilon epsilon = 1E-5f)
    {
        if(!aEpsilonEquals(a, b, epsilon))
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::green) | bg(fmt::color::black), L"Assert Passed. Expression: {} ~!= {} ({} ~!= {})\n", aStr, bStr, a, b));
            UnitTests::pass();
            return true;
        }
        else
        {
            test._messageList.push_back(fmt::format(fg(fmt::color::red) | bg(fmt::color::black), L"Assert Failed. Expression: {} ~!= {} ({} ~!= {})\n", aStr, bStr, a, b));
            UnitTests::fail();
            return false;
        }
    }
};

[[nodiscard]] inline bool tauAssert(ITestCase& test, const bool expr, const wchar_t* const exprStr)
{
    if(expr)
    {
        test._messageList.push_back(fmt::format(fg(fmt::color::green) | bg(fmt::color::black), L"Assert Passed. Expression: {}\n", exprStr));
        UnitTests::pass();
        return true;
    }
    else
    {
        
        test._messageList.push_back(fmt::format(fg(fmt::color::red) | bg(fmt::color::black), L"Assert Failed. Expression: {}\n", exprStr));
        UnitTests::fail();
        return false;
    }
}

#define TAU_ASSERT_(_EXPR, _SHOULD_RET) \
    if(!tauAssert(*this, _EXPR, _tau_STRINGIFY(_EXPR))) { \
        Console::Instance().shouldWrite() = true;  \
        if constexpr(_SHOULD_RET)                  \
        { return; }                                \
    } else {                                       \
        Console::Instance().shouldWrite() = false; \
    }                                              \
    (void) Console::Instance()

#define TAU_ASSERT_OPERATOR_(_A, _B, _OP, _SHOULD_RET) \
    if(!TauUnitAssert<decltype(_A), decltype(_B), _OP>::tauAssert(*this, _A, _B, _tau_STRINGIFY(_A), _tau_STRINGIFY(_B))) { \
        Console::Instance().shouldWrite() = true;             \
        if constexpr(_SHOULD_RET)                  \
        { return; }                                \
    } else {                                       \
        Console::Instance().shouldWrite() = false; \
    }                                              \
    (void) Console::Instance()

#define TAU_ASSERT_OPERATOR_FP_(_A, _B, _OP, _IS_RELATIVE, _SHOULD_RET) \
    if(!TauUnitAssertFloat<decltype(_A), decltype(_B), _OP, _IS_RELATIVE>::tauAssert(*this, _A, _B, _tau_STRINGIFY(_A), _tau_STRINGIFY(_B))) { \
        Console::Instance().shouldWrite() = true;  \
        if constexpr(_SHOULD_RET)                  \
        { return; }                                \
    } else {                                       \
        Console::Instance().shouldWrite() = false; \
    }                                              \
    (void) Console::Instance()

#define TAU_ASSERT_OPERATOR_FPE_(_A, _B, _OP, _IS_RELATIVE, _EPSILON, _SHOULD_RET) \
    if(!TauUnitAssertFloat<decltype(_A), decltype(_B), _OP, _IS_RELATIVE, decltype(_EPSILON)>::tauAssert(*this, _A, _B, _tau_STRINGIFY(_A), _tau_STRINGIFY(_B), _EPSILON)) { \
        Console::Instance().shouldWrite() = true;  \
        if constexpr(_SHOULD_RET)                  \
        { return; }                                \
    } else {                                       \
        Console::Instance().shouldWrite() = false; \
    }                                              \
    (void) Console::Instance()


#define TAU_ASSERT(_EXPR) TAU_ASSERT_(_EXPR, true)
#define TAU_EXPECT(_EXPR) TAU_ASSERT_(_EXPR, false)

#define TAU_ASSERT_EQ(_A, _B)  TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::Equal,        true)
#define TAU_ASSERT_NEQ(_A, _B) TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::NotEqual,     true)
#define TAU_ASSERT_GR(_A, _B)  TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::Greater,      true)
#define TAU_ASSERT_LS(_A, _B)  TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::Lesser,       true)
#define TAU_ASSERT_GEQ(_A, _B) TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::GreaterEqual, true)
#define TAU_ASSERT_LEQ(_A, _B) TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::LesserEqual,  true)

#define TAU_EXPECT_EQ(_A, _B)  TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::Equal,        false)
#define TAU_EXPECT_NEQ(_A, _B) TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::NotEqual,     false)
#define TAU_EXPECT_GR(_A, _B)  TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::Greater,      false)
#define TAU_EXPECT_LS(_A, _B)  TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::Lesser,       false)
#define TAU_EXPECT_GEQ(_A, _B) TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::GreaterEqual, false)
#define TAU_EXPECT_LEQ(_A, _B) TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::LesserEqual,  false)


#define TAU_ASSERT_FP_EQ_REL(_A, _B)  TAU_ASSERT_OPERATOR_FP_(_A, _B, TauComparisonType::Equal,    true,  true)
#define TAU_ASSERT_FP_NEQ_REL(_A, _B) TAU_ASSERT_OPERATOR_FP_(_A, _B, TauComparisonType::NotEqual, true,  true)
#define TAU_ASSERT_FP_EQ_ABS(_A, _B)  TAU_ASSERT_OPERATOR_FP_(_A, _B, TauComparisonType::Equal,    false, true)
#define TAU_ASSERT_FP_NEQ_ABS(_A, _B) TAU_ASSERT_OPERATOR_FP_(_A, _B, TauComparisonType::NotEqual, false, true)
#define TAU_ASSERT_FP_EQ(_A, _B, _IS_RELATIVE)  TAU_ASSERT_OPERATOR_FP_(_A, _B, TauComparisonType::Equal,    _IS_RELATIVE, true)
#define TAU_ASSERT_FP_NEQ(_A, _B, _IS_RELATIVE) TAU_ASSERT_OPERATOR_FP_(_A, _B, TauComparisonType::NotEqual, _IS_RELATIVE, true)

#define TAU_EXPECT_FP_EQ_REL(_A, _B)  TAU_ASSERT_OPERATOR_FP_(_A, _B, TauComparisonType::Equal,    true,  false)
#define TAU_EXPECT_FP_NEQ_REL(_A, _B) TAU_ASSERT_OPERATOR_FP_(_A, _B, TauComparisonType::NotEqual, true,  false)
#define TAU_EXPECT_FP_EQ_ABS(_A, _B)  TAU_ASSERT_OPERATOR_FP_(_A, _B, TauComparisonType::Equal,    false, false)
#define TAU_EXPECT_FP_NEQ_ABS(_A, _B) TAU_ASSERT_OPERATOR_FP_(_A, _B, TauComparisonType::NotEqual, false, false)
#define TAU_EXPECT_FP_EQ(_A, _B, _IS_RELATIVE)  TAU_ASSERT_OPERATOR_FP_(_A, _B, TauComparisonType::Equal,    _IS_RELATIVE, false)
#define TAU_EXPECT_FP_NEQ(_A, _B, _IS_RELATIVE) TAU_ASSERT_OPERATOR_FP_(_A, _B, TauComparisonType::NotEqual, _IS_RELATIVE, false)


#define TAU_ASSERT_FPE_EQ_REL(_A, _B, _EPSILON)  TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::Equal,    true,  _EPSILON, true)
#define TAU_ASSERT_FPE_NEQ_REL(_A, _B, _EPSILON) TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::NotEqual, true,  _EPSILON, true)
#define TAU_ASSERT_FPE_EQ_ABS(_A, _B, _EPSILON)  TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::Equal,    false, _EPSILON, true)
#define TAU_ASSERT_FPE_NEQ_ABS(_A, _B, _EPSILON) TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::NotEqual, false, _EPSILON, true)
#define TAU_ASSERT_FPE_EQ(_A, _B, _IS_RELATIVE, _EPSILON)  TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::Equal,    _IS_RELATIVE, _EPSILON, true)
#define TAU_ASSERT_FPE_NEQ(_A, _B, _IS_RELATIVE, _EPSILON) TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::NotEqual, _IS_RELATIVE, _EPSILON, true)
                     
#define TAU_EXPECT_FPE_EQ_REL(_A, _B, _EPSILON)  TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::Equal,    true,  _EPSILON, false)
#define TAU_EXPECT_FPE_NEQ_REL(_A, _B, _EPSILON) TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::NotEqual, true,  _EPSILON, false)
#define TAU_EXPECT_FPE_EQ_ABS(_A, _B, _EPSILON)  TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::Equal,    false, _EPSILON, false)
#define TAU_EXPECT_FPE_NEQ_ABS(_A, _B, _EPSILON) TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::NotEqual, false, _EPSILON, false)
#define TAU_EXPECT_FPE_EQ(_A, _B, _IS_RELATIVE, _EPSILON)  TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::Equal,    _IS_RELATIVE, _EPSILON, false)
#define TAU_EXPECT_FPE_NEQ(_A, _B, _IS_RELATIVE, _EPSILON) TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::NotEqual, _IS_RELATIVE, _EPSILON, false)


#define TAU_ASSERT_FP_GR(_A, _B)  TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::Greater,      true)
#define TAU_ASSERT_FP_LS(_A, _B)  TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::Lesser,       true)
#define TAU_ASSERT_FP_GEQ(_A, _B) TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::GreaterEqual, true)
#define TAU_ASSERT_FP_LEQ(_A, _B) TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::LesserEqual,  true)

#define TAU_EXPECT_FP_GR(_A, _B)  TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::Greater,      false)
#define TAU_EXPECT_FP_LS(_A, _B)  TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::Lesser,       false)
#define TAU_EXPECT_FP_GEQ(_A, _B) TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::GreaterEqual, false)
#define TAU_EXPECT_FP_LEQ(_A, _B) TAU_ASSERT_OPERATOR_(_A, _B, TauComparisonType::LesserEqual,  false)


#define TAU_ASSERT_INF(_F) TAU_ASSERT_(::std::isinf(_F), true)
#define TAU_EXPECT_INF(_F) TAU_ASSERT_(::std::isinf(_F), false)

#define TAU_ASSERT_FINITE(_F) TAU_ASSERT_(::std::isfinite(_F), true)
#define TAU_EXPECT_FINITE(_F) TAU_ASSERT_(::std::isfinite(_F), false)

#define TAU_ASSERT_NAN(_F) TAU_ASSERT_(::std::isnan(_F), true)
#define TAU_EXPECT_NAN(_F) TAU_ASSERT_(::std::isnan(_F), false)

class TestFileContainer final
{
    DEFAULT_DESTRUCT_VI(TestFileContainer);
    DELETE_CM(TestFileContainer);
private:
    ITestCase* _testCaseHead;
    ITestCase* _testCaseTail;
public:
    TestFileContainer() noexcept
        : _testCaseHead(nullptr)
        , _testCaseTail(nullptr)
    { }

    void registerTestCase(ITestCase* testCase) noexcept
    {
        if(!_testCaseHead)
        {
            _testCaseHead = testCase;
            _testCaseTail = testCase;
        }
        else
        {
            ITestCase* curr = _testCaseHead;
            bool suiteFound = false;
            while(curr)
            {
                if(curr->_testSuite == testCase->_testSuite)
                {
                    suiteFound = true;
                    break;
                }
                curr = curr->_next;
            }

            if(!suiteFound)
            {
                _testCaseTail->_next = testCase;
                _testCaseTail = testCase;
            }
            else
            {
                ITestCase* prev = curr;
                while(curr)
                {
                    if(curr->_testSuite != testCase->_testSuite)
                    {
                        prev->_next = testCase;
                        testCase->_next = curr;
                        break;
                    }
                    prev = curr;
                    curr = curr->_next;
                }

                if(!curr)
                {
                    _testCaseTail->_next = testCase;
                    _testCaseTail = testCase;
                }
            }
        }
    }

    [[nodiscard]] ITestCase* head() noexcept { return _testCaseHead; }
};

#define TAU_TEST(_TestSuite, _TestCase)                             \
    namespace _TestSuite##Suite {                                   \
        class _TestCase##Case final : public ITestCase {            \
            DEFAULT_DESTRUCT_VI(_TestCase##Case);                   \
            DELETE_CM(_TestCase##Case);                             \
        private:                                                    \
            static _TestCase##Case _instance;                       \
        public:                                                     \
            _TestCase##Case() noexcept                              \
                : ITestCase(L ## #_TestSuite, L ## #_TestCase)      \
            { _unitTest_testFileContainer.registerTestCase(this); } \
        protected:                                                  \
            void _test();                                           \
        };                                                          \
        _TestCase##Case _TestCase##Case::_instance;                 \
    }                                                               \
    void _TestSuite##Suite::_TestCase##Case::_test()

#define RUN_ALL_TESTS()                                             \
    do {                                                            \
        ITestCase* testCase = _unitTest_testFileContainer.head();   \
        const wchar_t* currentSuite = nullptr;                      \
        while(testCase) {                                           \
            if(currentSuite != testCase->_testSuite) {              \
                currentSuite = testCase->_testSuite;                \
                ConPrinter::printW(L"Starting Test Suite: %\n", currentSuite); \
            }                                                       \
            testCase->test();                                       \
            testCase->print();                                      \
            testCase = testCase->_next;                             \
            if(!testCase || currentSuite != testCase->_testSuite) { \
                ConPrinter::printW(L"Finishing Test Suite: %\n", currentSuite); \
            }                                                       \
        }                                                           \
    } while(0) 

static TestFileContainer _unitTest_testFileContainer;
