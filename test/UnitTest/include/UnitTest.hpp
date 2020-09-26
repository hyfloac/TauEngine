#pragma once

#include <cmath>
#include <cstdio>
#include <Objects.hpp>
#include <ConPrinter.hpp>
#include <NumTypes.hpp>
#include <Windows.h>

class UnitTests final
{
    DELETE_CONSTRUCT(UnitTests);
    DELETE_DESTRUCT(UnitTests);
    DELETE_CM(UnitTests);
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

    void resetOutColor() const noexcept
    { setOutColor(Color::LightGray, Color::Black); }
    void resetErrColor() const noexcept
    { setErrColor(Color::LightGray, Color::Black); }
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

class UnitTestConPrinter final
{
    DEFAULT_DESTRUCT(UnitTestConPrinter);
    DEFAULT_CM_PU(UnitTestConPrinter);
private:
    bool _shouldWrite;
public:
    UnitTestConPrinter(const bool shouldWrite = true) noexcept
        : _shouldWrite(shouldWrite)
    { }
    
    [[nodiscard]] bool& shouldWrite()       noexcept { return _shouldWrite; }
    [[nodiscard]] bool  shouldWrite() const noexcept { return _shouldWrite; }

    template<typename _T>
    UnitTestConPrinter& print(const _T& t) noexcept
    {
        if(_shouldWrite)
        { ConPrinter::print(stderr, t); }
        return *this;
    }

    template<typename _T>
    UnitTestConPrinter& printW(const _T& t) noexcept
    {
        if(_shouldWrite)
        { ConPrinter::printW(stderr, t); }
        return *this;
    }

    template<typename... _Args>
    UnitTestConPrinter& print(const char* fmt, _Args... args) noexcept
    {
        if(_shouldWrite)
        { ConPrinter::print(stderr, fmt, args...); }
        return *this;
    }

    template<typename... _Args>
    UnitTestConPrinter& printW(const wchar_t* fmt, _Args... args) noexcept
    {
        if(_shouldWrite)
        { ConPrinter::printW(stderr, fmt, args...); }
        return *this;
    }

    template<typename... _Args>
    UnitTestConPrinter& print(const wchar_t* fmt, _Args... args) noexcept
    { return printW(fmt, args...); }
};


#define _tau_STRINGIFY0(_X) L ## #_X
#define _tau_STRINGIFY(_X) _tau_STRINGIFY0(_X)

#define UNIT_TEST() UnitTest _ut_##__LINE__(__FUNCSIG__)

#define _x_STR0(_X) #_X
#define _x_STR(_X) _x_STR0(_X)

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

template<typename _Actual, typename _Expected, TauComparisonType _Comparison>
struct TauUnitAssert final
{
    [[nodiscard]] static bool tauAssert(const _Actual& a, const _Expected& b, const wchar_t* aStr, const wchar_t* bStr)
    { return false; }
};

template<typename _Actual, typename _Expected>
struct TauUnitAssert<_Actual, _Expected, TauComparisonType::Equal> final
{
    [[nodiscard]] static bool tauAssert(const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr)
    {
        if(a == b)
        {
            Console::Instance().setOutColor(Console::Green, Console::Black);
            ConPrinter::printW(stdout, L"Assert Passed. Expression: % == % (% == %)\n", aStr, bStr, a, b);
            UnitTests::pass();
            Console::Instance().resetOutColor();
            return true;
        }
        else
        {
            
            Console::Instance().setErrColor(Console::Green, Console::Black);
            ConPrinter::printW(stderr, L"Assert Failed. Expression: % == % (% == %)\n", aStr, bStr, a, b);
            UnitTests::fail();
            Console::Instance().resetErrColor();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected>
struct TauUnitAssert<_Actual, _Expected, TauComparisonType::NotEqual> final
{
    [[nodiscard]] bool static tauAssert(const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr)
    {
        if(a != b)
        {
            Console::Instance().setOutColor(Console::Green, Console::Black);
            ConPrinter::printW(stdout, L"Assert Passed. Expression: % != % (% != %)\n", aStr, bStr, a, b);
            UnitTests::pass();
            Console::Instance().resetOutColor();
            return true;
        }
        else
        {
            
            Console::Instance().setErrColor(Console::Green, Console::Black);
            ConPrinter::printW(stderr, L"Assert Failed. Expression: % != % (% != %)\n", aStr, bStr, a, b);
            UnitTests::fail();
            Console::Instance().resetErrColor();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected>
struct TauUnitAssert<_Actual, _Expected, TauComparisonType::Greater> final
{
    [[nodiscard]] bool static tauAssert(const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr)
    {
        if(a > b)
        {
            Console::Instance().setOutColor(Console::Green, Console::Black);
            ConPrinter::printW(stdout, L"Assert Passed. Expression: % > % (% > %)\n", aStr, bStr, a, b);
            UnitTests::pass();
            Console::Instance().resetOutColor();
            return true;
        }
        else
        {
            
            Console::Instance().setErrColor(Console::Green, Console::Black);
            ConPrinter::printW(stderr, L"Assert Failed. Expression: % > % (% > %)\n", aStr, bStr, a, b);
            UnitTests::fail();
            Console::Instance().resetErrColor();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected>
struct TauUnitAssert<_Actual, _Expected, TauComparisonType::Lesser> final
{
    [[nodiscard]] static bool tauAssert(const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr)
    {
        if(a < b)
        {
            Console::Instance().setOutColor(Console::Green, Console::Black);
            ConPrinter::printW(stdout, L"Assert Passed. Expression: % < % (% < %)\n", aStr, bStr, a, b);
            UnitTests::pass();
            Console::Instance().resetOutColor();
            return true;
        }
        else
        {
            
            Console::Instance().setErrColor(Console::Green, Console::Black);
            ConPrinter::printW(stderr, L"Assert Failed. Expression: % < % (% < %)\n", aStr, bStr, a, b);
            UnitTests::fail();
            Console::Instance().resetErrColor();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected>
struct TauUnitAssert<_Actual, _Expected, TauComparisonType::GreaterEqual> final
{
    [[nodiscard]] static bool tauAssert(const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr)
    {
        if(a >= b)
        {
            Console::Instance().setOutColor(Console::Green, Console::Black);
            ConPrinter::printW(stdout, L"Assert Passed. Expression: % >= % (% >= %)\n", aStr, bStr, a, b);
            UnitTests::pass();
            Console::Instance().resetOutColor();
            return true;
        }
        else
        {
            
            Console::Instance().setErrColor(Console::Green, Console::Black);
            ConPrinter::printW(stderr, L"Assert Failed. Expression: % >= % (% >= %)\n", aStr, bStr, a, b);
            UnitTests::fail();
            Console::Instance().resetErrColor();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected>
struct TauUnitAssert<_Actual, _Expected, TauComparisonType::LesserEqual> final
{
    [[nodiscard]] static bool tauAssert(const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr)
    {
        if(a <= b)
        {
            Console::Instance().setOutColor(Console::Green, Console::Black);
            ConPrinter::printW(stdout, L"Assert Passed. Expression: % <= % (% <= %)\n", aStr, bStr, a, b);
            UnitTests::pass();
            Console::Instance().resetOutColor();
            return true;
        }
        else
        {
            
            Console::Instance().setErrColor(Console::Green, Console::Black);
            ConPrinter::printW(stderr, L"Assert Failed. Expression: % <= % (% <= %)\n", aStr, bStr, a, b);
            UnitTests::fail();
            Console::Instance().resetErrColor();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected, TauComparisonType _Comparison, bool _Relative, typename _Epsilon = _Actual>
struct TauUnitAssertFloat final
{
    [[nodiscard]] static bool tauAssert(const _Actual& a, const _Expected& b, const wchar_t* aStr, const wchar_t* bStr, _Epsilon epsilon = 1E-5f)
    { return false; }
};

template<typename _Actual, typename _Expected, typename _Epsilon>
struct TauUnitAssertFloat<_Actual, _Expected, TauComparisonType::Equal, true, _Epsilon>  final
{
    [[nodiscard]] static bool tauAssert(const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr, const _Epsilon epsilon = 1E-5f)
    {
        if(rEpsilonEquals(a, b, epsilon))
        {
            Console::Instance().setOutColor(Console::Green, Console::Black);
            ConPrinter::printW(stdout, L"Assert Passed. Expression: % ~~= % (% ~~= %)\n", aStr, bStr, a, b);
            UnitTests::pass();
            Console::Instance().resetOutColor();
            return true;
        }
        else
        {
            
            Console::Instance().setErrColor(Console::Green, Console::Black);
            ConPrinter::printW(stderr, L"Assert Failed. Expression: % ~~= % (% ~~= %)\n", aStr, bStr, a, b);
            UnitTests::fail();
            Console::Instance().resetErrColor();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected, typename _Epsilon>
struct TauUnitAssertFloat<_Actual, _Expected, TauComparisonType::Equal, false, _Epsilon>  final
{
    [[nodiscard]] static bool tauAssert(const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr, const _Epsilon epsilon = 1E-5f)
    {
        if(aEpsilonEquals(a, b, epsilon))
        {
            Console::Instance().setOutColor(Console::Green, Console::Black);
            ConPrinter::printW(stdout, L"Assert Passed. Expression: % ~~= % (% ~~= %)\n", aStr, bStr, a, b);
            UnitTests::pass();
            Console::Instance().resetOutColor();
            return true;
        }
        else
        {
            
            Console::Instance().setErrColor(Console::Green, Console::Black);
            ConPrinter::printW(stderr, L"Assert Failed. Expression: % ~~= % (% ~~= %)\n", aStr, bStr, a, b);
            UnitTests::fail();
            Console::Instance().resetErrColor();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected, typename _Epsilon>
struct TauUnitAssertFloat<_Actual, _Expected, TauComparisonType::NotEqual, true, _Epsilon>  final
{
    [[nodiscard]] static bool tauAssert(const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr, const _Epsilon epsilon = 1E-5f)
    {
        if(!rEpsilonEquals(a, b, epsilon))
        {
            Console::Instance().setOutColor(Console::Green, Console::Black);
            ConPrinter::printW(stdout, L"Assert Passed. Expression: % ~~= % (% ~~= %)\n", aStr, bStr, a, b);
            UnitTests::pass();
            Console::Instance().resetOutColor();
            return true;
        }
        else
        {
            
            Console::Instance().setErrColor(Console::Green, Console::Black);
            ConPrinter::printW(stderr, L"Assert Failed. Expression: % ~~= % (% ~~= %)\n", aStr, bStr, a, b);
            UnitTests::fail();
            Console::Instance().resetErrColor();
            return false;
        }
    }
};

template<typename _Actual, typename _Expected, typename _Epsilon>
struct TauUnitAssertFloat<_Actual, _Expected, TauComparisonType::NotEqual, false, _Epsilon>  final
{
    [[nodiscard]] static bool tauAssert(const _Actual& a, const _Expected& b, const wchar_t* const aStr, const wchar_t* const bStr, const _Epsilon epsilon = 1E-5f)
    {
        if(!aEpsilonEquals(a, b, epsilon))
        {
            Console::Instance().setOutColor(Console::Green, Console::Black);
            ConPrinter::printW(stdout, L"Assert Passed. Expression: % ~~= % (% ~~= %)\n", aStr, bStr, a, b);
            UnitTests::pass();
            Console::Instance().resetOutColor();
            return true;
        }
        else
        {
            
            Console::Instance().setErrColor(Console::Green, Console::Black);
            ConPrinter::printW(stderr, L"Assert Failed. Expression: % ~~= % (% ~~= %)\n", aStr, bStr, a, b);
            UnitTests::fail();
            Console::Instance().resetErrColor();
            return false;
        }
    }
};

#define TAU_ASSERT_OPERATOR_(_A, _B, _OP, _SHOULD_RET) \
    if(!TauUnitAssert<decltype(_A), decltype(_B), _OP>::tauAssert(_A, _B, _tau_STRINGIFY(_A), _tau_STRINGIFY(_B))) { \
        _printer.shouldWrite() = true;                  \
        if constexpr(_SHOULD_RET)                       \
        { return; }                                     \
    } else {                                            \
        _printer.shouldWrite() = false;                 \
    }                                                   \
    UnitTestConPrinter& _tau_TOKEN_PASTE(_p_, __LINE__) = _printer; (void) _tau_TOKEN_PASTE(_p_, __LINE__); _tau_TOKEN_PASTE(_p_, __LINE__) = _printer

#define TAU_ASSERT_OPERATOR_FP_(_A, _B, _OP, _IS_RELATIVE, _SHOULD_RET) \
    if(!TauUnitAssertFloat<decltype(_A), decltype(_B), _OP, _IS_RELATIVE>::tauAssert(_A, _B, _tau_STRINGIFY(_A), _tau_STRINGIFY(_B))) { \
        _printer.shouldWrite() = true;                  \
        if constexpr(_SHOULD_RET)                       \
        { return; }                                     \
    } else {                                            \
        _printer.shouldWrite() = false;                 \
    }                                                   \
    UnitTestConPrinter& _tau_TOKEN_PASTE(_p_, __LINE__) = _printer; (void) _tau_TOKEN_PASTE(_p_, __LINE__); _tau_TOKEN_PASTE(_p_, __LINE__) = _printer

#define TAU_ASSERT_OPERATOR_FPE_(_A, _B, _OP, _IS_RELATIVE, _EPSILON, _SHOULD_RET) \
    if(!TauUnitAssertFloat<decltype(_A), decltype(_B), _OP, _IS_RELATIVE, decltype(_EPSILON)>::tauAssert(_A, _B, _tau_STRINGIFY(_A), _tau_STRINGIFY(_B)), _EPSILON) { \
        _printer.shouldWrite() = true;                  \
        if constexpr(_SHOULD_RET)                       \
        { return; }                                     \
    } else {                                            \
        _printer.shouldWrite() = false;                 \
    }                                                   \
    UnitTestConPrinter& _tau_TOKEN_PASTE(_p_, __LINE__) = _printer; (void) _tau_TOKEN_PASTE(_p_, __LINE__); _tau_TOKEN_PASTE(_p_, __LINE__) = _printer   

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

#define TAU_ASSERT_FPE_EQ_REL(_A, _B, _EPSILON)  TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::Equal,    true, _EPSILON,  true)
#define TAU_ASSERT_FPE_NEQ_REL(_A, _B, _EPSILON) TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::NotEqual, true, _EPSILON,  true)
#define TAU_ASSERT_FPE_EQ_ABS(_A, _B, _EPSILON)  TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::Equal,    false, _EPSILON, true)
#define TAU_ASSERT_FPE_NEQ_ABS(_A, _B, _EPSILON) TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::NotEqual, false, _EPSILON, true)
#define TAU_ASSERT_FPE_EQ(_A, _B, _IS_RELATIVE, _EPSILON)  TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::Equal,    _IS_RELATIVE, _EPSILON, true)
#define TAU_ASSERT_FPE_NEQ(_A, _B, _IS_RELATIVE, _EPSILON) TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::NotEqual, _IS_RELATIVE, _EPSILON, true)
                     
#define TAU_EXPECT_FPE_EQ_REL(_A, _B, _EPSILON)  TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::Equal,    true, _EPSILON,  false)
#define TAU_EXPECT_FPE_NEQ_REL(_A, _B, _EPSILON) TAU_ASSERT_OPERATOR_FPE_(_A, _B, TauComparisonType::NotEqual, true, _EPSILON,  false)
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

class ITestCase
{
    DEFAULT_DESTRUCT_VI(ITestCase);
    DELETE_CM(ITestCase);
public:
    ITestCase* _next;
    const wchar_t* _testSuite;
    const wchar_t* _testCase;
public:
    ITestCase(const wchar_t* const testSuite, const wchar_t* const testCase) noexcept
        : _next(nullptr)
        , _testSuite(testSuite)
        , _testCase(testCase)
    { }

    void test()
    {
        Console::Instance().setOutColor(Console::White, Console::Black);
        ConPrinter::printW(stdout, L"Starting Test: %\n", _testCase);
        Console::Instance().resetOutColor();
        UnitTests::reset();

        _test();

        int x = 3, y = 4;
        (void) TauUnitAssert<decltype(x), decltype(y), TauComparisonType::Equal>::tauAssert(x, y, L"x", L"y");

        Console::Instance().setOutColor(Console::White, Console::Black);
        ConPrinter::printW(stdout, L"Finishing Test: %\n", _testCase);
        if(UnitTests::passedPrevious())
        { ConPrinter::printW(stdout, L"Passed Tests: %\n\n", _testCase); }
        else
        { ConPrinter::printW(stdout, L"Failed Tests: %\n\n", _testCase); }
        Console::Instance().resetOutColor();
    }
protected:
    virtual void _test() = 0;
};

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

#define TEST_FILE() \
    static TestFileContainer _unitTest_testFileContainer

#define TAU_TEST(_TestSuite, _TestCase)                             \
    namespace _TestSuite##Suite {                                   \
        class _TestCase##Case final : public ITestCase {            \
            DEFAULT_DESTRUCT_VI(_TestCase##Case);                   \
            DELETE_CM(_TestCase##Case);                             \
        private:                                                    \
            UnitTestConPrinter _printer;                            \
        public:                                                     \
            _TestCase##Case() noexcept                              \
                : ITestCase(L ## #_TestSuite, L ## #_TestCase)      \
            { _unitTest_testFileContainer.registerTestCase(this); } \
        protected:                                                  \
            void _test();                                           \
        };                                                          \
        static _TestCase##Case _test_##_TestCase;                   \
    }                                                               \
    void _TestSuite##Suite::_TestCase##Case::_test()

#define RUN_ALL_TESTS()                                               \
    do {                                                              \
        ITestCase* testCase = _unitTest_testFileContainer.head();     \
        const wchar_t* currentSuite = nullptr;                        \
        while(testCase) {                                             \
            if(currentSuite != testCase->_testSuite) {                \
                currentSuite = testCase->_testSuite;                  \
                ConPrinter::printW(L"Test Suite: %\n", currentSuite); \
            }                                                         \
            testCase->test();                                         \
            testCase = testCase->_next;                               \
        }                                                             \
    } while(0) 
