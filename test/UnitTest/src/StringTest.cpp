#include "UnitTest.hpp"
#include "StringTest.hpp"
#include <String.hpp>

namespace StringTest {

void stringWrapTest() noexcept
{
    UNIT_TEST();
    const String str("Hello World!");
    Assert(strcmp("Hello World!", str.c_str()) == 0);
    Assert(equalsIgnoreCase("heLLo World!", str.c_str()));
}

void stringViewTest() noexcept
{
    UNIT_TEST();
    {
        const String str("Hello World!");
        const StringView strView(str, 0, 5);
        Assert(strView == "Hello");
        Assert(strView != str);
    }

    {
        const String str("Hello World!");
        const StringView strView = StringView::create(str, 6, 5);
        Assert(strView == "World");
        Assert(strView != str);
    }
}

void dynStringTest() noexcept
{
    UNIT_TEST();
    {
        DynString str0("Hello");
        const DynString str1 = str0 + " World";
        str0 = str1 + "!";
        Assert(str1 == "Hello World");
        Assert(str0 == "Hello World!");
        Assert(str0 != str1);
    }
}

}
